/*
 * RuntimeManager.h
 *
 *  Created on: Oct 18, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGER_H_
#define RUNTIMEMANAGER_H_

// The runtime manager requires to have a reference to the Application under observation
class SimplePlatooningApp;


#include <string>
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

// this is for debugging purpose
#define DEBUG_RUNTIMEMANAGER


class RuntimeManager {
public:
    RuntimeManager(SimplePlatooningApp* app);

    // We want all derived classes to override the destructor
    virtual ~RuntimeManager();

    // method called by the application periodically
    // This is a virtual method. Complex derived class of RuntimeManager can implement their own version (May be!!!!! Not sure yet)
    virtual void monitor();

    // this method is called every time a beacon is received and used to update StateMachine and beaconRecordData
    virtual void record(const int sourceVehicleId, simtime_t currentSimTime);

protected:

//    enum class ControllerStabilityState {
//        NOT_INITIALIZED,
//        STABLE,
//        UNSTABLE,
//    };


    // this method update the state machine
    virtual void updateStateMachine(const int sourceVehicleId);
    // this method update the beacon record
    virtual void updateBeaconRecord(const std::string &key, simtime_t currentSimTime);

    struct BeaconData {
        int vehicleId;
        simtime_t previousBeaconArrivalTime;
        simtime_t timeIntervalBetweenBeacon;
        double packetLossRate;
    };

    enum class StateMachine {
        NOT_INITIALIZED,
        CAR2X_ENGAGED,
        CAR2X_DISENGAGED,
        CAR2LEADER_ENGAGED,
        CAR2LEADER_DISENGAGED,
        CAR2X_AND_CAR2LEADER_ENGAGED,
        PLATOON_ESTABLISHED,
        PLATOON_ESTABLISHED_AND_CAR2X_ENGAGED,
        COMMUNICATION_FAILURE,
        LOCAL_CONTROL_FAILURE
    };

    enum class SwitchController {
        NOT_INITIALIZED,
        ACC_TO_CACC,
        ACC_TO_PLATOON,
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
    std::map<std::string, BeaconData> vehicleBeaconData;

    // required module. for example: PositionHelper, TraCI interface etc
    SimplePlatooningApp* app;
    BasePositionHelper* positionHelper;
    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;

};



#endif /* RUNTIMEMANAGER_H_ */
