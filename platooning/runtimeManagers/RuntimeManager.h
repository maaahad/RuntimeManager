/*
 * RuntimeManager.h
 *
 *  Created on: Oct 18, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGER_H_
#define RUNTIMEMANAGER_H_

// The runtime manager requires to have a reference to the Application under observation
class BaseApp;


#include <string>
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

class RuntimeManager {
public:
    RuntimeManager(BaseApp* app);

    // We want all derived classes to override the destructor
    virtual ~RuntimeManager();

    // method called by the application periodically
    // This is a virtual method. Complex derived class of RuntimeManager can implement their own version (May be!!!!! Not sure yet)
    virtual void monitor();

    // this method is called every time a beacon is received and used to update StateMachine and beaconRecordData
    virtual void record(int vehicleId);

protected:

//    enum class ControllerStabilityState {
//        NOT_INITIALIZED,
//        STABLE,
//        UNSTABLE,
//    };


    // this method update the state machine
    virtual void updateStateMachine();
    // this method update the beacon record
    virtual void updateBeaconRecord(const std::string &key);

    struct BeaconData {
        int vehicleId;
        SimTime previousBeaconArrivalTime;
        SimTime timeIntervalBetweenBeacon;
        double packetLossRate;
    };

    enum class StateMachine {
        NOT_INITIALIZED,
        CAR2X_ENGAGED,
        CAR2X_DISENGAGED,
        PLATOON_ESTABLISHED,
        CAR2X_ENGAGED_AND_PLATOON_ESTABLISHED,
        PLATOON_LEAVE,
        LEADER_LOST,
        COMMUNICATION_FAILURE,
        LOCAL_CONTROL_FAILURE
    };

    enum class SwitchController {
        NOT_INITIALIZED,
        ACC_TO_CACC,
        CACC_TO_ACC,
        CACC_TO_PLATOON,
        PLATOON_TO_CACC,
        PLATOON_TO_ACC,
    };

    class StateManager {
    public:
        StateManager(RuntimeManager*);
        ~StateManager();
        void accStateManager();
        void caccStateManager();
        void platoonStateManager();
    private:
        RuntimeManager* myManager;
    };


    class StateController {
    public:
        StateController (RuntimeManager*);
        void accStateController();
        void caccStateController();
        void platoonController();
    private:
        RuntimeManager* myManager;
    };

    // Controller state
//    ControllerStabilityState controllerStabilityState;

    // record the current state of the vehicle within the StateMachine
    StateMachine currentState;

    // This will keep track to which state to transit from which state for the state controller
    SwitchController switchController;

    // StateManager:: It's responsibility is to check the safety requirements and propose the transition if required
    StateManager* stateManager;

    // StateController: this guy perform state transition based on instruction from the StateManager
    // And take appropriate action for stable state transition
    StateController* stateController;

    // this struct variable store the information related to received beacon for all vehicles
    std::multimap<std::string, BeaconData> beaconData;

    // required module. for example: PositionHelper, TraCI interface etc
    BaseApp* app;
    BasePositionHelper* positionHelper;
    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;

};



#endif /* RUNTIMEMANAGER_H_ */
