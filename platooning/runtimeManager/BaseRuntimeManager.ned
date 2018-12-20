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

package org.car2x.veins.modules.application.platooning.runtimeManager;
import org.car2x.veins.base.modules.IBaseApplLayer;


simple BaseRuntimeManager like IBaseApplLayer {
    parameters:
        int headerLength = default(88bit) @unit(bit); //header length of the application
        
        bool rmEnabled	 = default(true); 
        
        double rmMonitorInterval @unit("s") 	= default(.10s);  
        double expectedBeconInterval @unit("s") = default(.05s);
        
        // WIFI connection quality check        
        int nPacketLossModerate  = default(1);
        int nPacketLossPoor      = default(2);
        int nPacketLossCritical  = default(3);
        
        // minimum distance between two vehicles
        double minSafetyDistance @unit("m")= default(2m);
        
        // actions to be taken on transition if enabled actionOnTransition
        bool actionOnTransitionEnabled        = default(true);
        double accHeadwaytimeGap @unit("s")   = default(1.2s);
        double ploegHeadwayTimeGap @unit("s") = default(1.0s);
        double caccConstantSpacing @unit("m") = default(5m);
    gates:
        input lowerLayerIn; // from mac layer
        output lowerLayerOut; // to mac layer
        input lowerControlIn;
        output lowerControlOut;
}