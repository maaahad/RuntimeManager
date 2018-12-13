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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_

#include "veins/base/utils/MiXiMDefs.h"         // This is required for SimTime, ASSERT

#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"


enum class QUALITY {
    CRITICAL,
    POOR,
    MODERATE,
    OK,
};


class StateParameter {
public:
    StateParameter();
    virtual ~StateParameter();
    virtual void evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) = 0;
};

enum class ROLE {
    FRONT,
    LEADER,
};

class C2X : public StateParameter {
private:
    template <typename T> void c2xQualityCheck(const RMParameters &rmParam, const T &other);
public:
    C2X(ROLE role);
    C2X(QUALITY quality, ROLE role);
    virtual void evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) override;

    // TODO make these private and the user's class defined as friend
    QUALITY quality;
    ROLE role;

    friend std::ostream &operator<<(std::ostream &os, const C2X &c2x);
};


#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_STATEPARAMETER_H_ */
