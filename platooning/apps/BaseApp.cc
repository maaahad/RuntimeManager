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


#include "veins/modules/application/platooning/apps/BaseApp.h"

#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/base/messages/MacPkt_m.h"
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"

#include "veins/modules/application/platooning/protocols/BaseProtocol.h"


using namespace Veins;

Define_Module(BaseApp);

void BaseApp::initialize(int stage)
{

    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        // set names for output vectors
        // distance from front vehicle
        distanceOut.setName("distance");
        // relative speed w.r.t. front vehicle
        relSpeedOut.setName("relativeSpeed");
        // vehicle id
        nodeIdOut.setName("nodeId");
        // current speed
        speedOut.setName("speed");
        // vehicle position
        posxOut.setName("posx");
        posyOut.setName("posy");
        // vehicle acceleration
        accelerationOut.setName("acceleration");
        controllerAccelerationOut.setName("controllerAcceleration");

        //================================ Ahad :: Start of Runtime Manager ============================//

        // beaconing interval in seconds
        expectedBeaconInterval = SimTime(par("expectedBeaconInterval").doubleValue());
        runtimeManagerEnabled  = par("runtimeManagerEnabled").boolValue();


        avgExceptedBeaconInterval          = SimTime(par("avgExceptedBeaconInterval").doubleValue());
        waitTimeToAcknoledgeConnectionEstd = SimTime(par("waitTimeToAcknoledgeConnectionEstd").doubleValue());
        nBeaconToAcknoledgeConnectionEstd  = par("nBeaconToAcknoledgeConnectionEstd").intValue();

//        std::cout << "expectedBeaconInterval: " << expectedBeaconInterval
//                  << ", runtimeManagerEnabled: "<< runtimeManagerEnabled << std::endl;
        //================================ Ahad :: End of Runtime Manager ==============================//

    }

    if (stage == 1) {
        mobility = Veins::TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        positionHelper = FindModule<BasePositionHelper*>::findSubModule(getParentModule());
        protocol = FindModule<BaseProtocol*>::findSubModule(getParentModule());
        myId = positionHelper->getId();

        // connect application to protocol
        protocol->registerApplication(BaseProtocol::BEACON_TYPE, gate("lowerLayerIn"), gate("lowerLayerOut"), gate("lowerControlIn"), gate("lowerControlOut"));

        recordData = new cMessage("recordData");
        // init statistics collection. round to 0.1 seconds
        SimTime rounded = SimTime(floor(simTime().dbl() * 1000 + 100), SIMTIME_MS);
        scheduleAt(rounded, recordData);


        //================================ Ahad :: Start of Runtime Manager ============================//
//        runtimeManager = new RuntimeManager(this);
//        callBackRuntimeManager = new cMessage("callBackRuntimeManager");
//        // runtime manager is called in every (rounded) .11 (.11 * 1000 = 110 ms)
//        // TODO :: exceptedBeaconInterval is hard coded right now. Need to define in the configuration file .ned and .ini
//        SimTime callBackTime = SimTime(floor(simTime().dbl() * 1000 + 110), SIMTIME_MS);
//        scheduleAt(callBackTime, callBackRuntimeManager);
        //================================ Ahad :: End of Runtime Manager ==============================//
    }
}

BaseApp::~BaseApp()
{
    cancelAndDelete(recordData);
    recordData = nullptr;
    cancelAndDelete(stopSimulation);
    stopSimulation = nullptr;

    //================================ Ahad :: Start of Runtime Manager ============================//
    // TODO Release memory : delete the runtime manager

//    cancelAndDelete(callBackRuntimeManager);
//    callBackRuntimeManager = nullptr;
//
//    delete runtimeManager;
    //================================ Ahad :: End of Runtime Manager ==============================//
}

void BaseApp::handleLowerMsg(cMessage* msg)
{
    UnicastMessage* unicast = check_and_cast<UnicastMessage*>(msg);

    cPacket* enc = unicast->decapsulate();
    ASSERT2(enc, "received a UnicastMessage with nothing inside");

    if (enc->getKind() == BaseProtocol::BEACON_TYPE) {
        onPlatoonBeacon(check_and_cast<PlatooningBeacon*>(enc));
    }
    else {
        error("received unknown message type");
    }

    delete unicast;
}

void BaseApp::logVehicleData(bool crashed)
{
    // get distance and relative speed w.r.t. front vehicle
    double distance, relSpeed;
    Plexe::VEHICLE_DATA data;
    traciVehicle->getRadarMeasurements(distance, relSpeed);
    traciVehicle->getVehicleData(&data);
    if (crashed) {
        distance = 0;
        stopSimulation = new cMessage("stopSimulation");
        scheduleAt(simTime() + SimTime(1, SIMTIME_MS), stopSimulation);
    }
    // write data to output files
    distanceOut.record(distance);
    relSpeedOut.record(relSpeed);
    nodeIdOut.record(myId);
    accelerationOut.record(data.acceleration);
    controllerAccelerationOut.record(data.u);
    speedOut.record(data.speed);
    posxOut.record(data.positionX);
    posyOut.record(data.positionY);
}

void BaseApp::handleLowerControl(cMessage* msg)
{
    delete msg;
}

void BaseApp::sendUnicast(cPacket* msg, int destination)
{
    UnicastMessage* unicast = new UnicastMessage();
    unicast->setDestination(destination);
    unicast->encapsulate(msg);
    sendDown(unicast);
}

void BaseApp::handleSelfMsg(cMessage* msg)
{
    if (msg == recordData) {
        // log mobility data
        logVehicleData(traciVehicle->isCrashed());
        // re-schedule next event
        scheduleAt(simTime() + SimTime(100, SIMTIME_MS), recordData);
    }

    //================================ Ahad :: Start of Runtime Manager ============================//

//    if (msg == callBackRuntimeManager) {
//        // callback runtime manager
//        EV << "Waiting to call the Runtime Manager!!!" << std::endl;
//        // Runtime manager analyze safety requirements to decide whether the current state is stable or not
//        // And take appropriate measures
//
//        // TODO :: exceptedBeaconInterval is hard coded right now. Need to define in the configuration file .ned and .ini
//        runtimeManager->monitor();
//        // re-schedule the self message
//        // TODO :: Callback time should come from configuration file
//        scheduleAt(simTime() + SimTime(110, SIMTIME_MS), callBackRuntimeManager);
//
//    }

    //================================ Ahad :: End of Runtime Manager ============================//


    if (msg == stopSimulation) {
        endSimulation();
    }
}

void BaseApp::onPlatoonBeacon(const PlatooningBeacon* pb)
{
    if (positionHelper->isInSamePlatoon(pb->getVehicleId())) {
        // if the message comes from the leader
        if (pb->getVehicleId() == positionHelper->getLeaderId()) {
            traciVehicle->setLeaderVehicleData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), pb->getPositionX(), pb->getPositionY(), pb->getTime());
        }
        // if the message comes from the vehicle in front
        if (pb->getVehicleId() == positionHelper->getFrontId()) {
            traciVehicle->setFrontVehicleData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), pb->getPositionX(), pb->getPositionY(), pb->getTime());

            //================================ Ahad :: Start of Runtime Manager ============================//
//           traciVehicle->setActiveController(Plexe::CACC);
//           if (positionHelper->getId() == 8)
//           std::cout << "VehicleId: " << positionHelper->getId() << "\n\tRuntimeManager performed transition from ACC to CACC!!!" << std::endl;


             // runtimeManager->monitor();

             // update state machine
                //runtimeManager->updateStateMachine(pb->getVehicleId());
             // update beaconRecordData

            //================================ Ahad :: End of Runtime Manager ==============================//

        }

        //================================================== Ahad:: Debug Start ==================================================//
        // TODO :: Need to keep record
        //std::cout << "Source Vehicle ID: " << pb->getVehicleId() << std::endl;
//        if (pb->getVehicleId() != positionHelper->getLeaderId() && pb->getVehicleId() != positionHelper->getFrontId()) {
//            //EV << "I am neither vehicle in front nor the leader!!!" << std::endl;
//        }
        //=================================================== Ahad:: Debug Start =================================================//


        //================================ Ahad :: Start of Runtime Manager ============================//
//        simtime_t currentSimTime = simTime();
//        runtimeManager->record(pb->getVehicleId(), currentSimTime);

        //================================ Ahad :: End of Runtime Manager ==============================//

        // send data about every vehicle to the CACC. this is needed by the consensus controller
        struct Plexe::VEHICLE_DATA vehicleData;
        vehicleData.index = positionHelper->getMemberPosition(pb->getVehicleId());
        vehicleData.acceleration = pb->getAcceleration();
        vehicleData.length = pb->getLength();
        vehicleData.positionX = pb->getPositionX();
        vehicleData.positionY = pb->getPositionY();
        vehicleData.speed = pb->getSpeed();
        vehicleData.time = pb->getTime();
        vehicleData.u = pb->getControllerAcceleration();
        vehicleData.speedX = pb->getSpeedX();
        vehicleData.speedY = pb->getSpeedY();
        vehicleData.angle = pb->getAngle();
        // send information to CACC
        traciVehicle->setVehicleData(&vehicleData);
    }
    delete pb;
}



//================================ Ahad :: Start of Runtime Manager ============================//

//BasePositionHelper* BaseApp::getPositionHelper() {
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to override by the derived class!!!"
//              << std::endl;
//}
//Veins::TraCIMobility* BaseApp::getMobility() {
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to override by the derived class!!!"
//              << std::endl;
//}
//Veins::TraCICommandInterface* BaseApp::getTraci() {
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to override by the derived class!!!"
//              << std::endl;
//}
//Veins::TraCICommandInterface::Vehicle* BaseApp::getTraciVehicle() {
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to override by the derived class!!!"
//              << std::endl;
//}

//================================ Ahad :: Start of Runtime Manager ============================//
