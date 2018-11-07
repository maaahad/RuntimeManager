//
// Copyright (C) 2012-2018 Michele Segata <segata@ccs-labs.org>
// Copyright (c) 2018 Julian Heinovski <julian.heinovski@ccs-labs.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package org.car2x.veins.modules.application.platooning.apps;

import org.car2x.veins.modules.application.platooning.apps.BaseApp;

simple GeneralPlatooningApp like BaseApp {

parameters:

    // implementation of the join maneuver to be used
    string joinManeuver;

    int headerLength @unit("bit") = default(0 bit);
    volatile double runtimeManagerCallbackInterval @unit(seconds) = default(0.11s);
    bool runtimeManagerEnabled = default(false);
    
    
    volatile double acceptedAvgBeaconInterval @unit(seconds) = default(0.11s);			// = default(.11 seconds)
	volatile double waitTimeToAcknoledgeConnectionEstd @unit(seconds) = deafult(0.25s);
    int nBeaconToAcknoledgeConnectionEstd = default(2);
    int nAcceptedBeaconMiss = default(3);											// =deault(3) 
    
    @display("i=block/app2");
    @class(GeneralPlatooningApp);

gates:
    input lowerLayerIn;
    output lowerLayerOut;
    input lowerControlIn;
    output lowerControlOut;
}
