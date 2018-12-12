/*
 * RMUtility.h
 *
 *  Created on: Dec 12, 2018
 *      Author: ahad
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_

#include <memory>
#include <vector>
#include "veins/modules/application/platooning/CC_Const.h"


class StateParameter;

/**
     * This structure represents the RMLog, that is the information that RM
     * stores onPlatoonBeacon from leader or front vehicle
     */
    struct RMParameters {
        bool rmEnabled;
        double rmMonitorInterval;
        double expectedBeconInterval;

        int nPacketLossModerate;
        int nPacketLossPoor;
        int nPacketLossCritical;

        double minSafetyDistance;
    };

    /**
     * These structures represents logs for different vehicles (own, front, leader)
     *
     *
     */

    /**
     * This is common parameters of front and leader vehicle
     */
    struct RMLog {
        double acceleration;
        double controllerAcceleration;
        double ede;     // end to end delay ,, don't know yet how to define and use this
        // TODO more parameters
        double lastBeaconArrivalTime;
        int nBeaconReceived = 0;
    };

    /**
     * This is the log for the own vehicle
     */
    struct RMLog_Own {
        Plexe::ACTIVE_CONTROLLER activeController;
        // List of state parameters that the vehicle have to monitor during a state
        std::shared_ptr<std::vector<StateParameter *>> stateParameters;

    };
    /**
     * This is the log for the front vehicle
     */
    struct RMLog_Front {
      RMLog common;
      double distance;
      // TODO more parameters
    };

    /**
     * This is the log for the leader vehicle
     */
    struct RMLog_Leader {
      RMLog common;
      // TODO more parameters
    };


    // Some companion type
    using rm_log = std::tuple<RMLog_Own, RMLog_Front, RMLog_Leader>;

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_ */
