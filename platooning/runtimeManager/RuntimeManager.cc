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

#include <iostream>
#include "RuntimeManager.h"

using namespace Veins;
Define_Module(RuntimeManager);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RuntimeManager Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RuntimeManager::RuntimeManager() {
    // TODO Auto-generated constructor stub
    std::cout << "Runtime Manager is called ... " << std::endl;
}

RuntimeManager::~RuntimeManager() {
    // TODO REMOVE checkMsg
    if(rmParam.rmEnabled) {
        cancelAndDelete(checkMsg);
        checkMsg = nullptr;
    }
}

void RuntimeManager::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if(stage == 0) {
        // Read all the RMParameters
        rmParam.rmEnabled               = par("rmEnabled").boolValue();

        if(rmParam.rmEnabled) {
            rmParam.rmMonitorInterval   = par("rmMonitorInterval").doubleValue();

            rmParam.nPacketLossModerate = par("nPacketLossModerate").intValue();
            rmParam.nPacketLossPoor     = par("nPacketLossPoor").intValue();
            rmParam.nPacketLossCritical = par("nPacketLossCritical").intValue();

            rmParam.minSafetyDistance   = par("minSafetyDistance").doubleValue();
        }
    }

    if(stage == 1) {
        if(!rmParam.rmEnabled) return;

        // Get the pointer to the Plexe API
        mobility = Veins::TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        positionHelper = FindModule<BasePositionHelper*>::findSubModule(getParentModule());

        // Log own vehicle

        std::get<0>(rmLog).activeController = (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController();

        // [ Debug ***************************************************************************************************
        if(positionHelper->isLeader()) {
            std::cout << "Leader: \n\tactiveController: " << std::get<0>(rmLog).activeController << std::endl;
        } else {
            std::cout << "VehicleId: " << positionHelper->getId() << "\n\tstd::get<0>(rmLog).activeController: " << std::get<0>(rmLog).activeController << std::endl;

        }
        // ***************************************************************************************************** Debug ]

        checkMsg = new cMessage("checkMsg");
        // init statistics collection. round to 0.1 seconds
        SimTime rounded = SimTime(floor(simTime().dbl() * 1000 + 100), SIMTIME_MS);
        scheduleAt(rounded, checkMsg);
    }
}


void RuntimeManager::handleSelfMsg(cMessage* msg) {
    if(msg == checkMsg) {
        std::cout << "Nice that I can handle self message!!!..." << std::endl;
    }
}


void RuntimeManager::onPlatoonBeacon(const PlatooningBeacon *pb) {
    // We are only interested in storing log for front and leader vehicle
    if(pb->getVehicleId() == positionHelper->getFrontId()) {
        RMLog_Front &frontLog = std::get<1>(rmLog);
        frontLog.common.acceleration = pb->getAcceleration();
        frontLog.common.controllerAcceleration = pb->getControllerAcceleration();

        // get front vehicle position
        Coord frontPosition(pb->getPositionX(), pb->getPositionY());
        // get my position
        Veins::TraCICoord traciPosition = mobility->getManager()->omnet2traci(mobility->getCurrentPosition());
        Coord position(traciPosition.x, traciPosition.y);
        // compute distance
        double distance = position.distance(frontPosition) - pb->getLength();

        double distanceR, relativeSpeed;
        traciVehicle->getRadarMeasurements(distanceR, relativeSpeed);

        std::cout << "VehicleId : " << positionHelper->getId()
                  << "\n\tdistance: " <<  distance
                  << "\n\tdistanceR: " << distanceR
                  << std::endl;

        frontLog.distance = distance; // Distance can be achieved during taking action

        // TODO log for end to end delay
    } else if(pb->getVehicleId() == positionHelper->getLeaderId()) {
        RMLog_Leader &leaderLog = std::get<2>(rmLog);
        leaderLog.common.acceleration = pb->getAcceleration();
        leaderLog.common.controllerAcceleration = pb->getControllerAcceleration();
        // TODO log for end to end delay
    } else {
        return;
    }
}


