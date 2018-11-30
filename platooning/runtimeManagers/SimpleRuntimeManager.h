/*
 * SimpleRuntimeManager.h
 *
 *  Created on: Oct 30, 2018
 *      Author: ahad
 */

#ifndef RUNTIMEMANAGERS_SIMPLERUNTIMEMANAGER_H_
#define RUNTIMEMANAGERS_SIMPLERUNTIMEMANAGER_H_

#include "BaseRuntimeManager.h"

class SimpleRuntimeManager: public BaseRuntimeManager {
public:
    SimpleRuntimeManager(BaseApp *app);
    virtual ~SimpleRuntimeManager();
    void observe() override;
    void record(const int sourceVehicleId, simtime_t currentSimTime) override;
    void triggerDegradation() override;
protected:
    void updateStateMachine(const int sourceVehicleId, const simtime_t currentSimTime) override;
    void logSafetyRecords(const int key, simtime_t currentSimTime) override;

};

#endif /* RUNTIMEMANAGERS_SIMPLERUNTIMEMANAGER_H_ */
