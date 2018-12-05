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
#include "RuntimeManager.h"

using namespace Veins;

Define_Module(RuntimeManager);

RuntimeManager::RuntimeManager() {
    // TODO Auto-generated constructor stub
    std::cout << "Runtime Manager is called ... " << std::endl;
}

RuntimeManager::~RuntimeManager() {
    // TODO Auto-generated destructor stub
    cancelAndDelete(checkMsg);
    checkMsg = nullptr;
}

void RuntimeManager::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if(stage == 0) {
        allowedNPacketLoss = par("allowedNPacketLoss").intValue();

        std::cout << "allowedNPacketLoss: " << allowedNPacketLoss << std::endl;
    }

    if(stage == 1) {
        checkMsg = new cMessage("checkMsg");
        // init statistics collection. round to 0.1 seconds
        SimTime rounded = SimTime(floor(simTime().dbl() * 1000 + 100), SIMTIME_MS);
        scheduleAt(rounded, checkMsg);
    }
}


void RuntimeManager::handleSelfMsg(cMessage* msg) {
    if(msg == checkMsg) {
        std::cout << "Nice that I am handling self message!!!..." << std::endl;
    }
}

void RuntimeManager::checkingRM() const {
    std::cout << "Nice that I am called!!!..." << std::endl;
}

