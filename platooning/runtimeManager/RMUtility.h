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
#include <string>
#include "veins/modules/application/platooning/CC_Const.h"


#define DEBUG_RM 1
#define DEBUG_RM1 1


// Forward declaration
class StateParameter;
class Contract;

std::ostream &operator<<(std::ostream &os, const Plexe::ACTIVE_CONTROLLER value);

namespace RM {
    /**
         * This structure represents the RMLog, that is the information that RM
         * stores onPlatoonBeacon from leader or front vehicle
         */
        struct RMParameters {
            bool rmEnabled;
            double rmMonitorInterval;
            double expectedBeconInterval;

//            int nPacketLossModerate;
            int nPacketLossPoor;
            int nPacketLossCritical;

            double minSafetyDistance;

            bool actionOnTransitionEnabled;
            double accHeadwaytimeGap;
            double ploegHeadwayTimeGap;
            double caccConstantSpacing;

            double emergencyPloegHeadwayTimeGapFactor;
            double emergencyCaccConstantSpacingFactor;
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
            bool c2xInitiated = false;              // helper variable to decide whether logging has started or not
            double time;                            // information generated time
            double lastBeaconArrivalTime;           // received time of last beacon (can be done based on time)
            double acceleration;                    // Acceleration of the vehicle
            double maxDeceleration = 100;           // Default initialized with a positive value, Deceleration have to be negative
            double controllerAcceleration;          // Acceleration computed by the Controller
        };

        /**
         * This is the log for the own vehicle
         */
        struct RMLog_Own {
            Plexe::ACTIVE_CONTROLLER activeController;
            double time;                            // information generated time
            double acceleration;                    // Acceleration of the vehicle
            double maxDeceleration = 100;           // Default initialized with a positive value, Deceleration have to be negative
            double dist2pred;                       // distance to the front vehicle

            // TODO need to pass custom deleter to shared_ptr or using use of shared_ptr for vector elements
            //std::shared_ptr<std::vector<Contract *>> contracts;
            std::shared_ptr<std::vector<std::shared_ptr<Contract>>> contracts;
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
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_ */
