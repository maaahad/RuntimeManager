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
#include "Contract.h"

Contract::Contract(CONTRACT_TYPE type, Plexe::ACTIVE_CONTROLLER controller) : type(type), controller(controller) {

}

Contract::~Contract() {
    // TODO Auto-generated destructor stub
}

void Contract::evaluate(const RMParameters &rmParam, const rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to be overriden by the derived class!!!"
              << std::endl;
}


bool Contract::equal(const Contract &contract) const {
    return (type == contract.type) &&
           (controller == contract.controller);
}

void Contract::updateOnTransition(Plexe::ACTIVE_CONTROLLER to) {
    std::cerr << "Error: " << __FILE__
              << "\n\tLine: " << __LINE__
              << "\n\tCompiled on: " << __DATE__
              << " at " << __TIME__
              << "\n\tfunction " << __func__
              << " must have to be overriden by the derived class!!!"
              << std::endl;
}

Plexe::ACTIVE_CONTROLLER Contract::getController() const {
    return controller;
}

// Contract free == operator
bool operator==(const Contract &cr1, const Contract &cr2 ) {
    return (typeid(cr1) == typeid(cr2)) && cr1.equal(cr2);
}
