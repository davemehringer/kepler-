from mayavi.mlab import *
import numpy as np
import os
import random
import sys
import threading
import time

def do_colors():
    global colors
    colors = (np.random.random((nbodies, 4))*255).astype(np.uint16)
    colors[:,-1] = 255 # No transparency
    for c in colors:
        red = c[0]
        green = c[1]
        blue = c[2]
        sum = red + green + blue
        if sum < 384:
            while True:
                red = (np.random.random((1))*255).astype(np.uint16)[0]
                green = (np.random.random((1))*255).astype(np.uint16)[0]
                blue = (np.random.random((1))*255).astype(np.uint16)[0]
                sum = red + green + blue
                if sum >= 384:
                    c[:] = [red, green, blue, 255]
                    break
        norm_colors.append(tuple(c[0:3]/255.0))

def do_labels():
    if texts == []:
        show_labels()
    elif name_toggle or tag_toggle:
        modify_labels()
    else:
        hide_labels()

def do_tags():
    global tag_map
    n = len(tag_chars)
    for i in xrange(nbodies):
        f = tag_chars[i/n]
        z = tag_chars[i % n]
        tag = str(z)
        if f != tag_chars[0]:
            tag = str(f) + str(z)
        tag_map.append(tag)

def hide_labels():
    global texts
    gcf().scene.disable_render = True
    for t in texts:
        t.remove()
    gcf().scene.disable_render = False
    texts = []
    
def hide_paths():
    global paths
    if not paths:
        return
    gcf().scene.disable_render = True
    for p in paths:
        p.remove()
    gcf().scene.disable_render = False
    paths = []

def init():
    global lines
    global pts
    global colors
    global texts
    global names
    global positions
    # first read gets body names
    read()
    # second read gets initial positions
    read()
    do_colors()
    plot_positions(True, True)
    do_tags()

def key_intercept(vtk_obj, event):
    global name_toggle
    global path_toggle
    global scale_factor
    global tag_toggle
    keypress = vtk_obj.GetKeyCode()
    if keypress == '>':
        scale_factor *= 2
        plot_positions(False, True)
        hide_paths()
    elif keypress == '<':
        scale_factor /= 2
        plot_positions(False, True)
        hide_paths()
    elif keypress == 'o':
        path_toggle = not path_toggle
        if path_toggle:
            plot_paths()
        else:
            hide_paths()
    elif keypress == 'n' or keypress == 't':
        if keypress == 'n':
            name_toggle = not name_toggle
        elif keypress == 't':
            tag_toggle = not tag_toggle
        do_labels()
#        if name_toggle:
#            show_names()
#        else:
#            hide_names()
    
def modify_labels():
    global texts
    gcf().scene.disable_render = True
    for i in xrange(nbodies):
        if name_toggle and tag_toggle:
            texts[i].text = names[i] + " " + tag_map[i]
        elif name_toggle:
            texts[i].text = names[i]
        elif tag_toggle:
            texts[i].text = tag_map[i] 
    gcf().scene.disable_render = False

@animate(delay=50)
def myanim():
    global count
    global done
    global positions
    prev = 0
    mycount = 0 
    while True:
        if not done:
            read()
            replot_all(False)
        yield

def picker_callback(picker):
    """ Picker callback: this get called when on pick events.
    """
    global pts
    if picker.actor in pts.actor.actors:
        glyph_points = pts.glyph.glyph_source.glyph_source.output.points.to_array()
        # Find which data point corresponds to the point picked:
        # we have to account for the fact that each data point is
        # represented by a glyph with several points
        point_id = picker.point_id/glyph_points.shape[0]
        # If the no points have been selected, we have '-1'
        if point_id != -1:
            # Retrieve the coordinnates coorresponding to that data
            # point
            global center_id
            center_id = point_id
            #clf()
            replot_all(False)

def plot_paths():
    if not path_toggle:
        return
    (x, y, z) = rel_positions(True)
    if x.shape[1] < 2:
        return
    gcf().scene.disable_render = True
    global paths
    (azimuth, elevation, distance, focalpoint) = view()
    tr = scale_factor/4.0
    n = x.shape[1]
    if not paths:
        if paths:
            for p in paths:
                p.remove()
        paths = []
        zero = np.zeros([10000])
        for i in xrange(nbodies):
            xx = zero
            xx[:n] = x[i]
            xx[n+1:] = None
            yy = zero
            yy[:n] = y[i]
            yy[n+1:] = None
            zz = zero
            zz[:n] = z[i]
            zz[n+1:] = None
            path = plot3d(
                xx, yy, zz, color=norm_colors[i],
                tube_radius=tr
            )
            paths.append(path)
    else:
        for i in xrange(nbodies):
            if i != center_id:
                xx = paths[i].mlab_source.x
                xx[:n] = x[i]
                paths[i].mlab_source.x = xx
                yy = paths[i].mlab_source.y
                yy[:n] = y[i]
                paths[i].mlab_source.y = yy
                zz = paths[i].mlab_source.z
                zz[:n] = z[i]
                paths[i].mlab_source.z = zz
        
    view(azimuth=azimuth, elevation=elevation, distance=distance, focalpoint=focalpoint)
    gcf().scene.disable_render = False

def plot_positions(reset_zoom, rescale_data):
    gcf().scene.disable_render = True
    global pts
    (azimuth, elevation, distance, focalpoint) = view()
    (x, y, z) = rel_positions(False)
    if pts and not rescale_data:
        pts.mlab_source.x = x
        pts.mlab_source.y = y
        pts.mlab_source.z = z
    else:
        if pts:
            pts.remove()
        pts = points3d(
            x, y, z, np.arange(nbodies), scale_factor=scale_factor,
            reset_zoom=reset_zoom
        )
        pts.glyph.color_mode = 'color_by_scalar' # Color by scalar
        pts.glyph.scale_mode = 'scale_by_vector' # scale by vector
        # Set look-up table and redraw
        pts.module_manager.scalar_lut_manager.lut.table = colors
    view(azimuth=azimuth, elevation=elevation, distance=distance, focalpoint=focalpoint)
    gcf().scene.disable_render = False

def read():
    global count
    global nbodies
    global done
    global positions
    global names
    x = f.readline().rstrip()
    if x == "end":
        f.close()
        done = True
        return positions
    elif x[0].isalpha():
        names = np.array(x.split())
        return
    data = np.fromstring(x, sep=" ")
    if count == 0:
        nbodies = len(data)/3
    data = data.reshape(nbodies, 3)
    if count == 0:
        positions = np.expand_dims(data, axis=2)
    else:
        positions = np.append(positions, np.expand_dims(data, axis=2), axis=2)
    count += 1

def rel_positions(all):
    if all:
        rp = (positions[:, :] - positions[center_id, :])/au
    else:
        rp = (positions[:, :, -1] - positions[center_id, :, -1])/au
    return (rp[:,0], rp[:,1], rp[:,2])

def replot_all(reset_zoom):
    if path_toggle:
        plot_paths()
    plot_positions(reset_zoom, False)
    if name_toggle or tag_toggle:
        update_labels()

def show_labels():
    (x, y, z) = rel_positions(False)
    (azimuth, elevation, distance, focalpoint) = view()
    scale = 0.005*distance
    x += 2*scale
    y += 2*scale
    z += 2*scale
    gcf().scene.disable_render = True
    global texts
    texts = []
    for i in xrange(nbodies):
        if name_toggle and tag_toggle:
            text = names[i] + " " + tag_map[i]
        elif name_toggle:
            text = names[i]
        elif tag_toggle:
            text = tag_map[i]
        xx = text3d(
            x[i], y[i], z[i], text,
            scale=scale, color=norm_colors[i]
        )
        texts.append(xx)
    gcf().scene.disable_render = False

def update_labels():
    """animate text labels to follow objects"""
    if not name_toggle and not tag_toggle:
        return
    (x, y, z) = rel_positions(False)
    gcf().scene.disable_render = True
    for (tt, xx, yy, zz) in zip(texts, x, y, z):
        tt.position = (xx, yy, zz)
    gcf().scene.disable_render = False

tag_chars = [
    '0','1','2','4','5','6','7','8','9','b','d','g','h','i','k','m'
]

picker = gcf().on_mouse_pick(picker_callback)

# Decrease the tolerance, so that we can more easily select a precise
# point.
picker.tolerance = 0.01

pipename = '/tmp/fifo_kepler'
if not os.path.exists(pipename):
    os.mkfifo(pipename) 
f = open(pipename, 'r')
nbodies = 0

names = []
centeridx = -1
plotlines = True
showtext = False
update = True
live = True
lastidx = -1

scale_factor = 0.03
center_id = 0
tag_map = []

gcf().scene.interactor.add_observer('KeyPressEvent', key_intercept); 
colors = []
norm_colors = []
pts = None
paths = None
texts = []
done = False
positions = np.zeros([nbodies, 3])
count = 0
name_toggle = False
path_toggle = False
tag_toggle = False
cur_count = count
au = 1.495e8
figure(gcf(), bgcolor=(0, 0, 0))
init()
#t = threading.Thread(target=read_loop)
#t.start() 
print "call myanim"
myanim()
print "exiting"

