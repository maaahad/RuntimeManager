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

#include "Decelerate.h"

Decelerate::Decelerate(RuntimeManager *rm) : Guarantees(rm){
    // TODO Auto-generated constructor stub

}

Decelerate::~Decelerate() {
    // TODO Auto-generated destructor stub
}


void Decelerate::operator()(Contract *contract) const {
    if(traciVehicle->getActiveController() == Plexe::ACC){
        std::cerr << "Warning: " << __FILE__
                         << "\n\tLine: " << __LINE__
                         << "\n\tCompiled on: " << __DATE__
                         << " at " << __TIME__
                         << "\n\tfunction: " << __func__
                         << " ...Not Implemented yet!!!"
                         << std::endl;
    } else if (traciVehicle->getActiveController() == Plexe::PLOEG) {
//        if(positionHelper->getId() == 1) {  // ==== Note: This is for debugging purpose
            std::cerr << "Before: " << *(static_cast<WIFIContract *>(contract)) << std::endl;
            std::cerr << "ploegHeadwayTimeGap:" << traciVehicle->getACCHeadwayTime() << ", RMParam::ploegHeadwayTimeGap: " << rmParam.ploegHeadwayTimeGap << std::endl;
            std::cerr << "CruiseControlDesiredSpeed:" << traciVehicle->getCruiseControlDesiredSpeed() << std::endl;

            // Test for active Controller CACC
//            traciVehicle->setACCHeadwayTime(rmParam.ploegHeadwayTimeGap);

            traciVehicle->setCruiseControlDesiredSpeed(20.0);

            std::cerr << "After: " << *(static_cast<WIFIContract *>(contract)) << std::endl;
//        }
    }else if(traciVehicle->getActiveController() == Plexe::CACC) {
        if(positionHelper->getId() == 7) {  // ==== Note: This is for debugging purpose
            std::cerr << "Before: " << *(static_cast<WIFIContract *>(contract)) << std::endl;
            std::cerr << "CACCConstantSpacing:" << traciVehicle->getCACCConstantSpacing() << ", RMParam::CACCConstantSpacing: " << rmParam.caccConstantSpacing << std::endl;
            // Test for active Controller CACC
            traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
            std::cerr << "After: " << *(static_cast<WIFIContract *>(contract)) << std::endl;
        }
    }
}
