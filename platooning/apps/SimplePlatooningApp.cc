//
// Copyright (c) 2012-2018 Michele Segata <segata@ccs-labs.org>
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

#include "veins/modules/application/platooning/apps/SimplePlatooningApp.h"

#include "veins/modules/application/platooning/runtimeManagers/SimpleRuntimeManager.h"


Define_Module(SimplePlatooningApp);

//================================ Ahad :: Start of Runtime Manager ============================//

void SimplePlatooningApp::initialize(int stage)
{
    BaseApp::initialize(stage);

    if (stage == 1) {
        if(runtimeManagerEnabled) {
            // runtimeManager = new RuntimeManager(this);

            //================= Adding new runtimeManager ==============//
            rtManager = new SimpleRuntimeManager(this);

            callBackRuntimeManager = new cMessage("callBackRuntimeManager");
            // runtime manager is called in every (rounded) .11 (.11 * 1000 = 110 ms)
            // TODO :: exceptedBeaconInterval is hard coded right now. Need to define in the configuration file .ned and .ini
            SimTime callBackTime = simTime() + runtimeManagerCallbackInterval;
            scheduleAt(callBackTime, callBackRuntimeManager);
        }
    }
}

void SimplePlatooningApp::handleSelfMsg(cMessage* msg)
{
    BaseApp::handleSelfMsg(msg);
    if (runtimeManagerEnabled) {
        if (msg == callBackRuntimeManager) {
            // callback runtime manager
            EV << "Waiting to call the Runtime Manager!!!" << std::endl;
            // Runtime manager analyze safety requirements to decide whether the current state is stable or not
            // And take appropriate measures

            // TODO :: exceptedBeaconInterval is hard coded right now. Need to define in the configuration file .ned and .ini
            rtManager->monitor();
            // re-schedule the self message
            // TODO :: Callback time should come from configuration file
            scheduleAt(simTime() + runtimeManagerCallbackInterval, callBackRuntimeManager);

        }
    }
}


void SimplePlatooningApp::onPlatoonBeacon(const PlatooningBeacon* pb)
{
    //================================ Ahad :: Start of Runtime Manager ============================//
    int vehicleId = pb->getVehicleId();
    simtime_t currentSimTime = simTime();
    //================================ Ahad :: End of Runtime Manager ==============================//

    // maintain platoon
    BaseApp::onPlatoonBeacon(pb);

    //================================ Ahad :: Start of Runtime Manager ============================//
    if (runtimeManagerEnabled) {
        rtManager->record(vehicleId, currentSimTime);
    }
    //================================ Ahad :: End of Runtime Manager ==============================//
}

SimplePlatooningApp::~SimplePlatooningApp() {
    cancelAndDelete(callBackRuntimeManager);
    callBackRuntimeManager = nullptr;
}

//================================ Ahad :: End of Runtime Manager ==============================//

