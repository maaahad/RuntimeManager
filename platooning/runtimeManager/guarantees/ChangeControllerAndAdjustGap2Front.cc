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

#include "ChangeControllerAndAdjustGap2Front.h"

ChangeControllerAndAdjustGap2Front::ChangeControllerAndAdjustGap2Front(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to, GAP2FRONT gap2front) :
Guarantees(rm), AdjustGap2Front(rm, gap2front), ChangeController(rm, to) {
    // TODO Auto-generated constructor stub

}

ChangeControllerAndAdjustGap2Front::~ChangeControllerAndAdjustGap2Front() {
    // TODO Auto-generated destructor stub
}

void ChangeControllerAndAdjustGap2Front::operator()(std::shared_ptr<Contract> contract) const {
    // Guarantees are performed through the base classes
    ChangeController::operator ()(contract);
    AdjustGap2Front::operator ()(contract);   // This is not done for ploeg yet
}


