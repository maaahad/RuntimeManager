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

#include "ChangeControllerAndDecelerate.h"

ChangeControllerAndDecelerate::ChangeControllerAndDecelerate(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to) : Guarantees(rm),
Decelerate(rm), ChangeController(rm, to) {
    // TODO Auto-generated constructor stub

}

ChangeControllerAndDecelerate::~ChangeControllerAndDecelerate() {
    // TODO Auto-generated destructor stub
}

void ChangeControllerAndDecelerate::operator()(Contract *contract) const {
    std::cerr << "Warning: " << __FILE__
                     << "\n\tLine: " << __LINE__
                     << "\n\tCompiled on: " << __DATE__
                     << " at " << __TIME__
                     << "\n\tfunction " << __func__
                     << " ...Not Implemented yet properly!!!"
                     << std::endl;
    // Guarantees are performed through the base classes
    ChangeController::operator ()(contract);
    Decelerate::operator ()(contract);
}


