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
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"

using namespace Veins;
Define_Module(RuntimeManager);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RuntimeManager Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RuntimeManager::RuntimeManager() {
    // TODO Auto-generated constructor stub
    std::cout << "Module :: RuntimeManager created." << std::endl;
}

RuntimeManager::~RuntimeManager() {
    // TODO REMOVE checkMsg
    if(rmParam.rmEnabled) {
        cancelAndDelete(monitoringMsg);
        monitoringMsg = nullptr;
    }
}

void RuntimeManager::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if(stage == 0) {
        // Read all the RMParameters
        rmParam.rmEnabled               = par("rmEnabled").boolValue();

        if(rmParam.rmEnabled) {
            rmParam.rmMonitorInterval     = par("rmMonitorInterval").doubleValue();
            rmParam.expectedBeconInterval = par("expectedBeconInterval").doubleValue();

//            rmParam.nPacketLossModerate = par("nPacketLossModerate").intValue();
            rmParam.nPacketLossPoor     = par("nPacketLossPoor").intValue();
            rmParam.nPacketLossCritical = par("nPacketLossCritical").intValue();

            rmParam.minSafetyDistance   = par("minSafetyDistance").doubleValue();

            rmParam.actionOnTransitionEnabled  = par("actionOnTransitionEnabled").boolValue();
            rmParam.accHeadwaytimeGap   = par("accHeadwaytimeGap").doubleValue();
            rmParam.ploegHeadwayTimeGap = par("ploegHeadwayTimeGap").doubleValue();
            rmParam.caccConstantSpacing = par("caccConstantSpacing").doubleValue();


            rmParam.emergencyPloegHeadwayTimeGapFactor = par("emergencyPloegHeadwayTimeGapFactor").doubleValue();
            rmParam.emergencyCaccConstantSpacingFactor = par("emergencyCaccConstantSpacingFactor").doubleValue();


            // [ Debug
//            std::cerr << "rmMonitorInterval: " << rmParam.rmMonitorInterval << std::endl;
//            std::cerr << "expectedBeconInterval: " << rmParam.expectedBeconInterval << std::endl;
////            std::cerr << "nPacketLossModerate: " << rmParam.nPacketLossModerate << std::endl;
//            std::cerr << "nPacketLossPoor: " << rmParam.nPacketLossPoor << std::endl;
//            std::cerr << "nPacketLossCritical: " << rmParam.nPacketLossCritical << std::endl;
//            std::cerr << "minSafetyDistance: " << rmParam.minSafetyDistance << std::endl;
//            std::cerr << "actionOnTransitionEnabled: " << rmParam.actionOnTransitionEnabled << std::endl;
//            std::cerr << "accHeadwaytimeGap: " << rmParam.accHeadwaytimeGap << std::endl;
//            std::cerr << "ploegHeadwayTimeGap: " << rmParam.ploegHeadwayTimeGap << std::endl;
//            std::cerr << "caccConstantSpacing: " << rmParam.caccConstantSpacing << std::endl;
//
//            std::cerr << "emergencyCaccConstantSpacingFactor: " << rmParam.emergencyCaccConstantSpacingFactor << std::endl;
//             std::cerr << "emergencyPloegHeadwayTimeGapFactor: " << rmParam.emergencyPloegHeadwayTimeGapFactor << std::endl;

//            double ploegH;
//            traciVehicle->getParameter(CC_PAR_PLOEG_H, ploegH);
//            std::cerr << "CC_PAR_PLOEG_H: " << ploegH << std::endl;


            // Debug ]


        }
    }

    if(stage == 1) {
        if(!rmParam.rmEnabled) return;

        // Get the pointer to the Plexe API
        mobility = Veins::TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        positionHelper = FindModule<BasePositionHelper*>::findSubModule(getParentModule());

        // [ Debug ***************************************************************************************************
//        if(positionHelper->isLeader()) {
//            std::cout << "Leader: \n\tactiveController: " << std::get<0>(rmLog).activeController << std::endl;
//        } else {
//            std::cout << "VehicleId: " << positionHelper->getId() << "\n\tstd::get<0>(rmLog).activeController: " << std::get<0>(rmLog).activeController << std::endl;
//
//        }
        // ***************************************************************************************************** Debug ]

        // initialize contract list
        initializeContracts();

        // Initialize the Contracts + Guarantees
        contractGuarantees = std::make_shared<Contract_Guarantee>(this);

        // Schedule the monitoring self message
        monitoringMsg = new cMessage("monitoringMsg");
        SimTime callBackTime = simTime() + rmParam.rmMonitorInterval;
        scheduleAt(callBackTime, monitoringMsg);
    }
}


void RuntimeManager::handleSelfMsg(cMessage* msg) {
    if(msg == monitoringMsg) {
        EV << "Monitoring message has been arrived. Evaluation started..." << std::endl;
        // TODO toggle comments for the following statement

        evaluate();

        // Sanity Check for now (WIFIContract only) TODO: need to generalize this
        ASSERT(traciVehicle->getActiveController() == (std::get<0>(rmLog).contracts)->front()->getController());

        // [ debug

//        if(traciVehicle->getActiveController() == Plexe::CACC) {
//            std::cout << "Vehicle " << positionHelper->getId()
//                      << ": \tActive Controller: " << traciVehicle->getActiveController()
//                      << "\tConstantSpacing : "<< traciVehicle->getCACCConstantSpacing()
//                      <<std::endl;
//        } else {
//            std::cout << "Vehicle " << positionHelper->getId()
//                      << ": \tActive Controller: " << traciVehicle->getActiveController()
//                      << "\tHeadwayTimeGap : " << traciVehicle->getACCHeadwayTime()
//                      << std::endl;
//        }

        // debug ]

        // Reschedule the monitoring message. TODO make sure to reschedule the self message
        // after the current evaluation and transition (if there is any)
        SimTime callBackTime = simTime() + rmParam.rmMonitorInterval;
        scheduleAt(callBackTime, monitoringMsg);
    }
}


template <typename T> void RuntimeManager::commonLog(const PlatooningBeacon *pb, T &loggedVehicle, const SimTime currentTime) {
    if(!loggedVehicle.common.c2xInitiated) loggedVehicle.common.c2xInitiated = true;

    loggedVehicle.common.acceleration = pb->getAcceleration();
    loggedVehicle.common.controllerAcceleration = pb->getControllerAcceleration();

    loggedVehicle.common.lastBeaconArrivalTime = currentTime.dbl();
    loggedVehicle.common.nBeaconReceived++;

    // TODO log for end to end delay
}


void RuntimeManager::onPlatoonBeacon(const PlatooningBeacon *pb, const SimTime currentTime) {
    // We are only interested in storing log for front and leader vehicle
    if(pb->getVehicleId() == positionHelper->getFrontId()) {
        RM::RMLog_Front &frontLog = std::get<1>(rmLog);
        commonLog(pb, frontLog, currentTime);

        // TODO : log if there is any front specific log required
        // get front vehicle position
        Coord frontPosition(pb->getPositionX(), pb->getPositionY());
        // get my position
        Veins::TraCICoord traciPosition = mobility->getManager()->omnet2traci(mobility->getCurrentPosition());
        Coord position(traciPosition.x, traciPosition.y);
        // compute distance
        double distance = position.distance(frontPosition) - pb->getLength();
        frontLog.distance = distance; // Distance can be achieved during taking action

        // [ debug
//        double distanceR, relativeSpeed;
//        traciVehicle->getRadarMeasurements(distanceR, relativeSpeed);
//
//        std::cout << "VehicleId : " << positionHelper->getId()
//                  << "\n\tdistance: " <<  distance
//                  << "\n\tdistanceR: " << distanceR
//                  << std::endl;
        // debug ]

        // TODO log for end to end delay

        // Evaluate StateParameters for possible upgrade
        // Second argument is the index of front vehicle log in rmLog
//        evaluate(true, 1);

    }

    // We need to perform both for front and leader subsequently, as the vehicle with id 0, got the same vehicle as leader and front
    // In that case we want to logged for both leader and the front vehicle
    // In case of other vehicles onley one of the condition will be satisfied
    if(pb->getVehicleId() == positionHelper->getLeaderId()) {
        RM::RMLog_Leader &leaderLog = std::get<2>(rmLog);
        commonLog(pb, leaderLog, currentTime);
        // TODO : log if there is any leader specific log required
        // Evaluate StateParameters for possible upgrade
        // Second argument is the index of leader vehicle log in rmLog
//        evaluate(true, 2);
    }
}



void RuntimeManager::initializeContracts() {
    RM::RMLog_Own &ego = std::get<0>(rmLog);
    ego.contracts  = std::make_shared<std::vector<Contract *>>();

    // Old WIFIContract
//    (ego.contracts)->push_back(new WIFIContract(CONTRACT_TYPE::WIFI, (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController(), C2X(ROLE::FRONT), C2X(ROLE::LEADER)));


    // New WIFIContract
    (ego.contracts)->push_back(new WIFIContract(CONTRACT_TYPE::WIFI, (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController(), C2F(), C2L()));

    // TODO other Contracts, if there is any
    // [debug
    if(positionHelper->getId() == 0) {
        std::cout << "======================================== Default Contract ========================================"
                  << "\n==================================================================================================" << std::endl;
    }
    std::cout << "Vehicle Id : " << positionHelper->getId() << "\n\t" << *(static_cast<WIFIContract *>((*ego.contracts)[0]))
              << "\n--------------------------------------------------------------------------------------------------" << std::endl;
    if(positionHelper->getId() == 7) {
        std::cout << "=================================================================================================="
                  << "\n=================================================================================================="
                  << "\n\n\n";


    }

//    Contract *cr1 = new WIFIContract(CONTRACT_TYPE::WIFI, (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController(), C2X(ROLE::FRONT), C2X(ROLE::FRONT));
//    Contract *cr2 = new WIFIContract(CONTRACT_TYPE::WIFI, (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController(), C2X(ROLE::LEADER), C2X(ROLE::LEADER));
//    if (*cr1 == *cr2) {
//        std::cout << "Checking == Operator,,,Equal" << std::endl;
//    } else {
//        std::cout << "Checking == Operator,,,Not-Equal" << std::endl;
//    }

    // debug ]
}


void RuntimeManager::evaluate(bool onPlatoonBeacon, int index) {

    // Sanity Check
    ASSERT2(onPlatoonBeacon ? index >= 0 : index < 0, "Problem with default argument in evaluate() methods in RuntimeManager");

    RM::RMLog_Own &ego = std::get<0>(rmLog);
    for(auto it = (ego.contracts)->begin(); it != (ego.contracts)->end(); ++it) {
        if(onPlatoonBeacon) {
            (*it)->evaluate(rmParam, rmLog, onPlatoonBeacon, index);
        } else {
            (*it)->evaluate(rmParam, rmLog);
        }
    }

    // [ Debug
    if(positionHelper->getId() == 7) {
//        std::cerr << "Vehicle Id : " << positionHelper->getId() << "\n\t" << *(static_cast<WIFIContract *>((*ego.contracts)[0])) << std::endl;
    }
    // Debug ]



    // Vehicles StateParameters related to the provided contract has been evaluated,
    // Now, we need to match the current state of the vehicle's contract with the Contracts-Guarantee
    // to ensure the appropriate Guarantee

    if(!onPlatoonBeacon) {
        contractGuarantees->evaluate(std::get<0>(rmLog));
    }

}


