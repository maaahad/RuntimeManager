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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_

#include "veins/base/modules/BaseApplLayer.h"


class RuntimeManager : public Veins::BaseApplLayer {
public:
    RuntimeManager();
    virtual ~RuntimeManager();
    virtual void initialize(int stage) override;

    void checkingRM() const;

protected:
    virtual void handleSelfMsg(cMessage* msg) override;

private:
    int allowedNPacketLoss;
    cMessage *checkMsg;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_ */
