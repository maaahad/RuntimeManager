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
Guarantees::Guarantees(RuntimeManager *rm) : rmParam(rm->rmParam){
    // TODO Auto-generated constructor stub
    mobility = rm->mobility;
    traci = rm->traci;
    traciVehicle = rm->traciVehicle;
    positionHelper = rm->positionHelper;
}



//Guarantees::Guarantees(RuntimeManager *rm) : changeController(false), accelerate(false), decelerate(false), rmParam(rm->rmParam){
//    // TODO Auto-generated constructor stub
//}

//Guarantees::Guarantees(RuntimeManager *rm, bool changeController, Plexe::ACTIVE_CONTROLLER to) :changeController(changeController), to(to),
//        accelerate(false), decelerate(false), rmParam(rm->rmParam){
//
//        mobility = rm->mobility;
//        traci = rm->traci;
//        traciVehicle = rm->traciVehicle;
//        positionHelper = rm->positionHelper;
//}

Guarantees::~Guarantees() {
    // TODO Auto-generated destructor stub
}

void Guarantees::actionOnTransition() const {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " This method should be implemented by the derived method!!!"
              << std::endl;

//    if(to == Plexe::ACC) {
//        traciVehicle->setACCHeadwayTime(rmParam.accHeadwaytimeGap);
//    } else if(to == Plexe::PLOEG){
//        traciVehicle->setACCHeadwayTime(rmParam.ploegHeadwayTimeGap);
//    } else if(to == Plexe::CACC) {
//        traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
//    } else {
//        std::cerr << "Error: " << __FILE__
//                  << "\n\tLine: " << __LINE__
//                  << "\n\tCompiled on: " << __DATE__
//                  << " at " << __TIME__
//                  << "\n\tfunction " << __func__
//                  << " Wrong Controller!!!"
//                  << std::endl;
//    }

}

//void Guarantees::provideGuarantee(Contract *contract) const{
////    if(changeController) {
////        // [ debug
////        if(to > traciVehicle->getActiveController()) {
////            std::cout << "Vehicle " << positionHelper->getId()
////                      << "\n\t Upgrade!!!" << std::endl;
////        } else {
////            std::cout << "Vehicle " << positionHelper->getId()
////                      << "\n\t Degrade!!!" << std::endl;
////        }
////        // debug ]
////
////        // update the vehicle's current contract status for the Active controller
////        // As the consecutive Guarantee requires the current active controller (included in the key_type of the Contract-Guarantee unordered_map )
////        contract->updateOnTransition(to);
////
////        // perform actionOnTransition if enabled
////        if(rmParam.actionOnTransitionEnabled) actionOnTransition();
////
////        // Perform the transition
////        traciVehicle->setActiveController(to);
////    }
//}


void Guarantees::operator()(Contract *contract) const {
        std::cerr << "Error: " << __FILE__
                  << "\n\tLine: " << __LINE__
                  << "\n\tCompiled on: " << __DATE__
                  << " at " << __TIME__
                  << "\n\tfunction " << __func__
                  << " This function should be implemented by the derived class!!!"
                  << std::endl;
}

