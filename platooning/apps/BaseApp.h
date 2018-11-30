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

#ifndef BASEAPP_H_
#define BASEAPP_H_


//================================ Ahad :: Start of Runtime Manager ============================//
#include <iostream>
//================================ Ahad :: Start of Runtime Manager ============================//


#include "veins/base/modules/BaseApplLayer.h"

#include "veins/modules/application/platooning/UnicastProtocol.h"
#include "veins/modules/application/platooning/messages/PlatooningBeacon_m.h"

#include "veins/modules/mobility/traci/TraCIMobility.h"

#include "veins/modules/application/platooning/CC_Const.h"

#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"

#include "veins/modules/application/platooning/runtimeManagers/BaseRuntimeManager.h"




class BaseProtocol;

class BaseApp : public Veins::BaseApplLayer {

public:
    virtual void initialize(int stage) override;

protected:
    // id of this vehicle
    int myId;

    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;

    // determines position and role of each vehicle
    BasePositionHelper* positionHelper;

    // lower layer protocol
    BaseProtocol* protocol;

    /**
     * Log data about vehicle
     */
    virtual void logVehicleData(bool crashed = false);

    // output vectors for mobility stats
    // id of the vehicle
    cOutVector nodeIdOut;
    // distance and relative speed
    cOutVector distanceOut, relSpeedOut;
    // speed and position
    cOutVector speedOut, posxOut, posyOut;
    // real acceleration and controller acceleration
    cOutVector accelerationOut, controllerAccelerationOut;

    // messages for scheduleAt
    cMessage* recordData;
    // message to stop the simulation in case of collision
    cMessage* stopSimulation;


    //================================ Ahad :: Start of Runtime Manager ============================//
    SimTime runtimeManagerCallbackInterval;
    cMessage* callBackRuntimeManager;
    bool runtimeManagerEnabled;

    cMessage* msgToTransition;
    SimTime timeToTransition;

    SimTime acceptedAvgBeaconInterval;
    SimTime waitTimeToAcknoledgeConnectionEstd;
    int nBeaconToAcknoledgeConnectionEstd;
    int nAcceptedBeaconMiss;

    // TODO replace runtimeManager from the derived class
    BaseRuntimeManager *rtManager;

//    // self message to activate runtime manager.
//    // TODO : CALLBACK TIME SHOULD BE COME FROM CONFIGURATION FILE. BaseScenario.ned, BBaseScenario.ned and .ini
//    cMessage* callBackRuntimeManager;
//    // Runtime manager
//    RuntimeManager *runtimeManager;
    //================================ Ahad :: End of Runtime Manager ==============================//

public:
    BaseApp()
    {
        recordData = 0;
        stopSimulation = nullptr;
    }
    virtual ~BaseApp();


    //================================ Ahad :: Start of Runtime Manager ============================//

    BasePositionHelper* getPositionHelper()
    {
        return positionHelper;
    }
    Veins::TraCIMobility* getMobility()
    {
        return mobility;
    }
    Veins::TraCICommandInterface* getTraci()
    {
        return traci;
    }
    Veins::TraCICommandInterface::Vehicle* getTraciVehicle()
    {
        return traciVehicle;
    }

    SimTime getExpectedBeaconInterval() {
            return runtimeManagerCallbackInterval;
    }

    double getTimeToTransition() {
            return timeToTransition.dbl();
    }

    double getAcceptedAvgBeaconInterval() {
        return acceptedAvgBeaconInterval.dbl();
    }
    SimTime getWaitTimeToAcknoledgeConnectionEstd() {
        return waitTimeToAcknoledgeConnectionEstd;
    }

    int getNBeaconToAcknoledgeConnectionEstd() {
        return nBeaconToAcknoledgeConnectionEstd;
    }

    int getNAcceptedBeaconMiss() {
            return nAcceptedBeaconMiss;
    }


    /**
     * This method schedule a self message to perform controller transition by handleSelfMsg
     */
    void triggerTransitionSelfMsg();


    //================================ Ahad :: End of Runtime Manager ==============================//

    /**
     * Sends a unicast message
     *
     * @param msg message to be encapsulated into the unicast message
     * @param destination id of the destination
     */
    void sendUnicast(cPacket* msg, int destination);

protected:
    virtual void handleLowerMsg(cMessage* msg) override;
    virtual void handleSelfMsg(cMessage* msg) override;
    virtual void handleLowerControl(cMessage* msg) override;

    /**
     * Handles PlatoonBeacons
     */
    virtual void onPlatoonBeacon(const PlatooningBeacon* pb);
};

#endif /* BASEAPP_H_ */
