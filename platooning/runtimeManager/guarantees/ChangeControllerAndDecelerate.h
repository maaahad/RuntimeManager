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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDDECELERATE_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDDECELERATE_H_
#include "veins/modules/application/platooning/runtimeManager/guarantees/Decelerate.h"
#include "veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h"


class ChangeControllerAndDecelerate : public Decelerate, public ChangeController {
public:
    ChangeControllerAndDecelerate(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to);
    virtual ~ChangeControllerAndDecelerate();
    virtual void operator()(Contract *contract) const override;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDDECELERATE_H_ */
