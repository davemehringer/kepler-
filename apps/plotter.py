#!/usr/bin/python
import numpy as np

import os

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib import animation
import sys

pipename = '/tmp/fifo_kepler'
if not os.path.exists(pipename):
    os.mkfifo(pipename) 
f = open(pipename, 'r')
nbodies = 0

# Set up figure & 3D axis for animation

fig = plt.figure()
ax = fig.add_axes([0, 0, 1, 1], projection='3d')
ax.set_axis_bgcolor("black")
ax.axis('off')
plt.axis("equal")

#ax.pbaspect = [1.0, 1.0, 1.0]

# prepare the axes limits
ax.set_xlim((-5e9, 5e9))
ax.set_ylim((-5e9, 5e9))
ax.set_zlim((-5e9, 5e9))

mgr = plt.get_current_fig_manager()

#mgr.resize(*mgr.window.maxsize())
mgr.resize(*(1170, 1170))

lines = None
pts = None

done = False

x = None
y = None
z = None
first = True

positions = None

#def onclick(event):
#    print 'button=%d, x=%d, y=%d, xdata=%f, ydata=%f'%(
#        event.button, event.x, event.y, event.xdata, event.ydata)

# cid = fig.canvas.mpl_connect('button_press_event', onclick)

center = "."
buffer = ""

names = []
centeridx = -1
plotlines = True
showtext = False
update = True
live = True
lastidx = -1

def press(event):
    global buffer
    global names
    global centeridx
    global plotlines
    global showtext
    global update
    global lastidx
    global live
    if event.key == "enter":
        idx = np.where(names == buffer)
        if len(idx[0]) == 0:
            print "unknown body " + buffer
        else:
            centeridx = idx[0][0]
            update = True
        buffer = ""
    elif event.key == "/":
        # center on system barycenter
        centeridx = -1
        update = True
    elif event.key == ",":
        # show names toggle
        showtext = not showtext
        update = True
    elif event.key == "-":
        # show paths toggle
        plotlines = not plotlines
        update = True
    elif event.key == ".":
        # resume to live
        lastidx = -1
        update = True
        live = True
    elif event.key == "<":
        #rewind
        if lastidx == -1:
            lastidx = positions.shape[2] - 2
        else:
            lastidx -= 1
        update = True
        live = False
    elif event.key == ">":
        # forward
        if lastidx == -1 or lastidx >= positions.shape[2] - 2:
            lastidx = 0
        else:
            lastidx += 1
        update = True
        live = False
    
    else:  
        buffer += event.key
    if update:
        do_update()
    
    #sys.stdout.flush()
    #if event.key == 'x':
    #    visible = xl.get_visible()
    #    xl.set_visible(not visible)
    #    fig.canvas.draw()

fig.canvas.mpl_disconnect(fig.canvas.manager.key_press_handler_id)
cid = fig.canvas.mpl_connect('key_press_event', press)

# initialization function: plot the background of each frame

colors = []
texts = []
def init():
    global lines
    global pts
    global colors
    global texts
    global names
    data = read()
    
    print "nbodies", nbodies
    x = np.zeros([nbodies, 1])
    y = np.zeros([nbodies, 1])
    z = np.zeros([nbodies, 1])

    # choose a different color for each orbit
    colors = plt.cm.jet(np.linspace(0, 1, nbodies))
    # set up lines and points
    lines = sum(
        [ax.plot([], [], [], '-', c=c) for c in colors], []
    )
    pts = sum(
        [ax.plot([], [], [], 'o', c=c) for c in colors], []
    )
    for c in colors:
        texts.append(ax.text(0, 0, 0, "", None, color=c))
    
    #texts = sum(
    #    [ax.text(0, 0, 0, n, None, color=c) for c, n in zip(colors,names)], []
    #)
    
    
    for line, pt in zip(lines, pts):
        line.set_data([], [])
        line.set_3d_properties([])

        pt.set_data([], [])
        pt.set_3d_properties([])
    return lines + pts + texts

# animation function.  This will be called sequentially with the frame number

def animate(i):
    #global first
    global lines
    global pts
    global names
    global colors
    global texts
    global showtext
    global f
    global positions
    global done
    global update
    global lastidx
    global live
    # we'll step two time-steps per frame.  This leads to nice results.
    #i = (2 * i) % x_t.shape[1]
    if f.closed or done:
        data = positions
    else:
        data = read()
        update = True
    if update and live:
        do_update()
    #first = False
    update = False
    return lines + pts + texts

def do_update():
    global positions
    data = positions
    k = 0
    for line, pt, name, color, text in zip(lines, pts, names, colors, texts):
        if centeridx < 0:
            x = data[k, 0, : -1]
            y = data[k, 1, : -1]
            z = data[k, 2, : -1]
        else:
            x = data[k, 0, : -1] - data[centeridx, 0, : -1]
            y = data[k, 1, : -1] - data[centeridx, 1, : -1]
            z = data[k, 2, : -1] - data[centeridx, 2, : -1]

        if plotlines:
            if lastidx == -1:
                line.set_data(x, y)
                line.set_3d_properties(z)
            else:
                line.set_data(x[: lastidx+1], y[:lastidx+1])
                line.set_3d_properties(z[:lastidx+1])
        else:
            line.set_data([], [])
            line.set_3d_properties([])
        pt.set_data(x[ lastidx], y[ lastidx])
        pt.set_3d_properties(z[ lastidx])
        
        if showtext:
            text.set_position((x[ lastidx], y[ lastidx]))
            text.set_3d_properties(z[ lastidx], None)
            text.set_text(name)
        text.set_visible(showtext)

        k += 1
        
    #ax.view_init(30, 0.3 * i)
    fig.canvas.draw()


count = 0
nbodies = 0

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
    return positions
    
try:
    # instantiate the animator.
    anim = animation.FuncAnimation(
        fig, animate, init_func=init,
        frames=500, interval=30, blit=True
    )
except TypeError:
    pass
 
plt.show()
print "plotting engine done"

