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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_

#include <iostream>
#include "veins/modules/application/platooning/runtimeManager/Guarantees.h"
#include "veins/modules/application/platooning/runtimeManager/contracts/WIFIContract.h"

enum class GAP2FRONT {
    DEFAULT,
    INCREASE,
    DECREASE,
    ADJUST,
};

std::ostream &operator<<(std::ostream &os, const GAP2FRONT gap2front);


class AdjustGap2Front : virtual public Guarantees {
public:
    AdjustGap2Front(RuntimeManager *rm, GAP2FRONT gap2front);
    virtual ~AdjustGap2Front();
    virtual void operator()(std::shared_ptr<Contract> contract) const override;
private:
    void gapControll(std::shared_ptr<Contract> contract) const;
    GAP2FRONT gap2front;


    // ==== Testing ====
//public:
//    mutable bool checked = false;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_ */
