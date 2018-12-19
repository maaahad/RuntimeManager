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

#include <iostream>
#include "Guarantees.h"
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarantees's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Guarantees::Guarantees(RuntimeManager *rm) : changeController(false), accelerate(false), decelerate(false){
    // TODO Auto-generated constructor stub
}

Guarantees::Guarantees(RuntimeManager *rm, bool changeController, Plexe::ACTIVE_CONTROLLER to) :changeController(changeController), to(to),
        accelerate(false), decelerate(false){
        mobility = rm->mobility;
        traci = rm->traci;
        traciVehicle = rm->traciVehicle;
        positionHelper = rm->positionHelper;
}

Guarantees::~Guarantees() {
    // TODO Auto-generated destructor stub
}

void Guarantees::provideGuarantee() {
    if(changeController) {
        traciVehicle->setActiveController(to);
        std::cout << "Vehicle " << positionHelper->getId()
                  << "\n\t Controller changed!!!" << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contracts's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ControllerTransition::ControllerTransition() {
//
//}
//
//void ControllerTransition::provideGuarantee() {
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " Has not been implemented yet...!!!"
//              << std::endl;
//}
