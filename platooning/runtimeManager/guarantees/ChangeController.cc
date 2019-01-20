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
#include <iomanip>
#include "ChangeController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeController's methods implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ChangeController::ChangeController(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to) : Guarantees(rm), to(to){
    // TODO Auto-generated constructor stub

}

ChangeController::~ChangeController() {
    // TODO Auto-generated destructor stub
}

void ChangeController::actionOnTransition() const {
    if(to == Plexe::ACC) {
        traciVehicle->setACCHeadwayTime(rmParam.accHeadwaytimeGap);
    } else if(to == Plexe::PLOEG){
        traciVehicle->setACCHeadwayTime(rmParam.ploegHeadwayTimeGap);
    } else if(to == Plexe::CACC) {
        traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
    } else {
        std::cerr << "Error: Wrong Controller!!!" << __FILE__
                  << "\n\tLine: " << __LINE__
                  << "\n\tCompiled on: " << __DATE__
                  << " at " << __TIME__
                  << "\n\tfunction " << __func__
                  << std::endl;
    }
}

void ChangeController::operator()(std::shared_ptr<Contract> contract) const {
#if DEBUG_RM
    std::cout << std::setw(30) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " ::: "
              << "Contract's Status BEFORE Transition "
              << std::setw(30) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIContract>(contract))
              << std::setfill(' ') << std::endl;
#endif

#if DEBUG_RM
    std::cout << std::setw(37) << std::setfill('#') << ""
              <<" VEHICLE " << positionHelper->getId() << " :: "
              << "Controller Transition "
              << std::setw(37) << std::setfill('#') << "" << "\n"
              << std::setw(37) << std::setfill(' ') << ""
              << (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController() << " => " << to << std::endl
              << std::setfill(' ');
#endif
    // update the vehicle's current contract status for the Active controller
    // As the consecutive Guarantee requires the current active controller (included in the key_type of the Contract-Guarantee unordered_map )
    contract->updateOnTransition(to);

    // perform actionOnTransition if enabled.
    // Do it before transition, so that new controller can use it straight away
    if(rmParam.actionOnTransitionEnabled) actionOnTransition();

    // Perform the transition
    traciVehicle->setActiveController(to);

#if DEBUG_RM
    std::cout << std::setw(30) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " :::: "
              << "Contract's Status AFTER Transition "
              << std::setw(30) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIContract>(contract))
              << std::setfill(' ') << std::endl;
#endif
}
