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

#include "ChangeController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enum Plexe::ACTIVE_CONTROLLERY << implementation :: Utility
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const Plexe::ACTIVE_CONTROLLER value) {
    std::map<Plexe::ACTIVE_CONTROLLER, std::string> enum2string;
    if (enum2string.size() == 0) {
#define INSERT(v) enum2string[v] = #v
    INSERT(Plexe::ACTIVE_CONTROLLER::DRIVER);
    INSERT(Plexe::ACTIVE_CONTROLLER::ACC);
    INSERT(Plexe::ACTIVE_CONTROLLER::CACC);
    INSERT(Plexe::ACTIVE_CONTROLLER::FAKED_CACC);
    INSERT(Plexe::ACTIVE_CONTROLLER::PLOEG);
    INSERT(Plexe::ACTIVE_CONTROLLER::CONSENSUS);
    INSERT(Plexe::ACTIVE_CONTROLLER::FLATBED);
#undef INSERT
    }
    return os << enum2string[value];
}


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
        std::cerr << "Error: " << __FILE__
                  << "\n\tLine: " << __LINE__
                  << "\n\tCompiled on: " << __DATE__
                  << " at " << __TIME__
                  << "\n\tfunction " << __func__
                  << " Wrong Controller!!!"
                  << std::endl;
    }

    std::cerr << "Warning: " << __FILE__
                     << "\n\tLine: " << __LINE__
                     << "\n\tCompiled on: " << __DATE__
                     << " at " << __TIME__
                     << "\n\tfunction " << __func__
                     << " This method requires experiment on different action... not implemented properly!!!"
                     << std::endl;
}

void ChangeController::operator()(Contract *contract) const {
    // [ debug
//    if(to > traciVehicle->getActiveController()) {
//        std::cout << "Vehicle " << positionHelper->getId()
//                  << "\n\t Upgrade : From " << traciVehicle->getActiveController() << " to " << to << std::endl;
//    } else {
//        std::cout << "Vehicle " << positionHelper->getId()
//                  << "\n\t Degrade : From " << traciVehicle->getActiveController() << " to " << to << std::endl;
//    }

    std::cout << "Vehicle " << positionHelper->getId()
              << "\n\t Controller Change : From "
              << (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController()
              << " to " << (Plexe::ACTIVE_CONTROLLER)to << std::endl;

    // debug ]

    // update the vehicle's current contract status for the Active controller
    // As the consecutive Guarantee requires the current active controller (included in the key_type of the Contract-Guarantee unordered_map )
    contract->updateOnTransition(to);

    // perform actionOnTransition if enabled
    if(rmParam.actionOnTransitionEnabled) actionOnTransition();

    // Perform the transition
    traciVehicle->setActiveController(to);
}
