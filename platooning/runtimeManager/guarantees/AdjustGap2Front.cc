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

#include <map>
#include <string>
#include "AdjustGap2Front.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AdjustGap2Front's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AdjustGap2Front::AdjustGap2Front(RuntimeManager *rm, GAP2FRONT gap2front) : Guarantees(rm), gap2front(gap2front){
    // TODO Auto-generated constructor stub

}

AdjustGap2Front::~AdjustGap2Front() {
    // TODO Auto-generated destructor stub
}


void AdjustGap2Front::gapControll(std::shared_ptr<Contract> contract) const {
    switch(traciVehicle->getActiveController()) {
    case Plexe::ACC:
        break;
    case Plexe::PLOEG:
        //
        break;
    case Plexe::CACC:
        // C2F::OK, C2L::POOR
        // get the Radar measurements
        double distance, relativeSpeed;
        traciVehicle->getRadarMeasurements(distance, relativeSpeed);

        if(distance != -1 && relativeSpeed != 0 ) {
            double caccSpacingUpperLimit = 30.0;                // TODO as it as a Simulation parameter in omnetpp.ini
            double dt = 0.01;                                   // TODO this should get from the omnetpp.ini
            double emergencyCaccConstantSpacingFactor2 = .25;   // TODO as it as a Simulation parameter in omnetpp.ini
            // Valid radar has been received
            double spacingError1 = distance - rmParam.caccConstantSpacing;
            if(spacingError1 > 0) {
                // Spacing is ok, but as the connection to leader is poor,
                // We will increase the spacing to be at a safe side
                double newSpacing = rmParam.caccConstantSpacing + rmParam.emergencyCaccConstantSpacingFactor * rmParam.caccConstantSpacing;
                traciVehicle->setCACCConstantSpacing(std::max(rmParam.minSafetyDistance, std::min(newSpacing, caccSpacingUpperLimit)));

                std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed "
                          << gap2front << " distance(" << traciVehicle->getCACCConstantSpacing() << "m)"
                          << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                          << std::endl;
            } else {
                // TODO Need to check
                // Spacing is not ok, We need more spacing
                // we need the acceleration of the front vehicle
                RM::RMLog_Front &front = std::get<1>(rmLog);
                Plexe::VEHICLE_DATA data;
                traciVehicle->getVehicleData(&data);
                double relAcceleration = data.acceleration - front.common.acceleration;
                double spacingError2 = relativeSpeed * dt + 0.5 * relAcceleration * dt * dt;
                double newSpacing = rmParam.caccConstantSpacing + rmParam.emergencyCaccConstantSpacingFactor * rmParam.caccConstantSpacing
                                    - emergencyCaccConstantSpacingFactor2 * spacingError2;
                traciVehicle->setCACCConstantSpacing(std::max(rmParam.minSafetyDistance, std::min(newSpacing, caccSpacingUpperLimit)));
                std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed (double)"
                          << gap2front << " distance(" << traciVehicle->getCACCConstantSpacing() << "m)"
                          << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                          << std::endl;
            }
        }
        break;
    default:
        std::cerr << "Error: " << __FILE__
                                 << "\n\tLine: " << __LINE__
                                 << "\n\tCompiled on: " << __DATE__
                                 << " at " << __TIME__
                                 << "\n\tfunction: " << __func__
                                 << " Wrong Active Controller!!!"
                                 << std::endl;
        break;
    }
}

void AdjustGap2Front::operator()(std::shared_ptr<Contract> contract) const {
    if(traciVehicle->getActiveController() == Plexe::ACC){
        std::cerr << "Warning: " << __FILE__
                         << "\n\tLine: " << __LINE__
                         << "\n\tCompiled on: " << __DATE__
                         << " at " << __TIME__
                         << "\n\tfunction: " << __func__
                         << " ...Not Implemented yet!!!"
                         << std::endl;
    } else if (traciVehicle->getActiveController() == Plexe::PLOEG) {

        if(gap2front == GAP2FRONT::DEFAULT) {
            // go back to the default spacing
            traciVehicle->setParameter(CC_PAR_PLOEG_H, rmParam.ploegHeadwayTimeGap);

            std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed "
                      << gap2front << " headwayTimeGap(" << rmParam.ploegHeadwayTimeGap << "s)"
                      << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                      << std::endl;

        } else if (gap2front == GAP2FRONT::INCREASE){
            // increase the gap
            traciVehicle->setParameter(CC_PAR_PLOEG_H, rmParam.ploegHeadwayTimeGap + rmParam.ploegHeadwayTimeGap * rmParam.emergencyPloegHeadwayTimeGapFactor);
            std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed "
                      << gap2front << " headwayTimeGap(" << (rmParam.ploegHeadwayTimeGap + rmParam.ploegHeadwayTimeGap * rmParam.emergencyPloegHeadwayTimeGapFactor) << "s)"
                      << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                      << std::endl;

        } else {
            // TODO decrease the gap
        }

    } else if(traciVehicle->getActiveController() == Plexe::CACC) {
//        if(positionHelper->getId() == 7) {  // ==== Note: This is for debugging purpose
//            std::cerr << "Vehicle " << positionHelper->getId() << "Contract Status: \t\tBefore\n\t " << *(static_cast<WIFIContract *>(contract)) << std::endl;
//            std::cerr << "CACCConstantSpacing:" << traciVehicle->getCACCConstantSpacing() << ", RMParam::CACCConstantSpacing: " << rmParam.caccConstantSpacing << std::endl;
            if(gap2front == GAP2FRONT::DEFAULT) {
                // go back to the default spacing
                traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
                std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed "
                          << gap2front << " distance(" << rmParam.caccConstantSpacing << "m)"
                          << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                          << std::endl;
            } else if (gap2front == GAP2FRONT::INCREASE){

                // increase the gap
                traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing + rmParam.caccConstantSpacing * rmParam.emergencyCaccConstantSpacingFactor);
                if(positionHelper->getId() == 7) std::cout << "Vehicle " << positionHelper->getId() << "\n\t" << "performed "
                          << gap2front << " distance(" << (rmParam.caccConstantSpacing + rmParam.caccConstantSpacing * rmParam.emergencyCaccConstantSpacingFactor) << "m)"
                          << "\n\t" << "Contract Status: " << *(std::static_pointer_cast<WIFIContract>(contract))
                          << std::endl;


                // ===== Testing slowDown method =====
//                if(positionHelper->getId() == 7 && !checked ) {
//                    traciVehicle->slowDown(20.0, 1);
//                    checked = true;
//                }

            } else if (gap2front == GAP2FRONT::ADJUST) {
                // gap controll
                gapControll(contract);
            } else {
                // TODO decrease the gap
            }
//            std::cerr << "Vehicle " << positionHelper->getId() << " Contract Status: \t\tAfter\n\t " << *(static_cast<WIFIContract *>(contract)) << std::endl;
//        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GAP2FRONT enum's << operator implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const GAP2FRONT gap2front) {
    std::map<GAP2FRONT, std::string> enum2string;

    if(enum2string.size() == 0) {
#define INSERT(gap2front) enum2string[gap2front] = #gap2front
        INSERT(GAP2FRONT::DEFAULT);
        INSERT(GAP2FRONT::DECREASE);
        INSERT(GAP2FRONT::INCREASE);
#undef INSERT
    }

    return os << enum2string[gap2front];
}
