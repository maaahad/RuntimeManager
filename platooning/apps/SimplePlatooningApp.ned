//
// Copyright (C) 2012-2018 Michele Segata <segata@ccs-labs.org>
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

simple SimplePlatooningApp like BaseApp
{
    parameters:
        int headerLength @unit("bit") = default(0 bit);
        
        volatile double runtimeManagerCallbackInterval @unit(seconds) = default(0.33s);
        bool runtimeManagerEnabled = default(false);
        
        volatile double timeToTransition @unit(seconds) = default(.01s);				    // default(.010 seconds), set value 0 to degrade straight away
        
        volatile double acceptedAvgBeaconInterval @unit(seconds) = default(0.100s);			// = default(.10 seconds)
    	volatile double waitTimeToAcknoledgeConnectionEstd @unit(seconds) = default(0.32s); // Not used 
    	int nBeaconToAcknoledgeConnectionEstd = default(3);									// Not used 
    	int nAcceptedBeaconMiss = default(3);											    // =deault(3) 
        
        @display("i=block/app2");
        @class(SimplePlatooningApp);
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
        input lowerControlIn;
        output lowerControlOut;
}
