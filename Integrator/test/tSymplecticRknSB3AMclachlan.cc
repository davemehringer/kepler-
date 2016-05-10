//# tSymplecticRknSB3AMclachlan.cc
//# Copyright (C) 2016 David Mehringer
//# 
//#     This program is free software: you can redistribute it and/or modify
//#     it under the terms of the GNU General Public License as published by
//#     the Free Software Foundation, either version 3 of the License, or
//#     (at your option) any later version.
//# 
//#     This program is distributed in the hope that it will be useful,
//#     but WITHOUT ANY WARRANTY; without even the implied warranty of
//#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#     GNU General Public License for more details.
//# 
//#     You should have received a copy of the GNU General Public License
//#     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//# 
/*
 * tMath.cc
 *
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <Integrator/SymplecticRknSB3AMclachlan.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <DistanceCalculator/UnthreadedDistanceCalculator.h>
#include <IntegrationEnder/NStepsIntegrationEnder.h>
#include <Math/Constants.h>
#include <Physics/Body.h>
#include <Physics/NBodySystem.h>
#include <SolarSystem/SSObjects.h>
#include <TimeStepManager/AccelerationTimeStepManager.h>

#include <cmath>

using namespace kepler;

TEST (SymplecticRknSB3AMclachlanTest, integrate) {
	Body star0 = SSObjects::createBody("sun");
	auto star1 = star0;
	star1.name = "sun2";
	PrecType au = KM_PER_AU;
	star1.x[0] = au;
	PrecType period = M_PI*sqrt(2/star0.mu*au*au*au);
	star1.v[1] = 2*M_PI*au/period;
	vector<Body> bodies {star0, star1};
	NBodySystem system(bodies, 0, DAY);
	UnthreadedDistanceCalculator udc;
	PairWiseAccelerationCalculator pwac(&udc, &bodies);
	//AccelerationTimeStepManager atsm(0.002, 0.005);
    AccelerationTimeStepManager atsm(0.02, 0.05);

	NStepsIntegrationEnder nsie(1e6);
	SymplecticRknSB3AMclachlan stepper(&system, &pwac, &atsm, &nsie);
	cout << "initial " << setprecision(10) << system << endl;
	auto e0 = system.getEnergy();
	stepper.integrate();
	auto systemFinal = system;
	systemFinal.setTime(system.getTime(SECOND) + stepper.getElapsedTime(SECOND), SECOND);
	Vvector xFinal(bodies.size());
    Vvector vFinal(bodies.size());
    stepper.getXV(xFinal, vFinal);
	systemFinal.setXV(xFinal, vFinal);
	cout << "final " << setprecision(10) << systemFinal << endl;
	auto ef = systemFinal.getEnergy();
	cout << "frac energy change " << (ef/e0 - 1) << endl;
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


