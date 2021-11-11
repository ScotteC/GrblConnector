/*********************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Fabian Schoettler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#include "grblconnector.hpp"

namespace grblconnector {

    GrblConnector::GrblConnector() = default;

    GrblConnector::~GrblConnector() = default;

    bool GrblConnector::Connect(const std::string &device, int baudrate) {
        if (gTransceiver.Connect(device, baudrate)) {
            return false;
        }
        return true;
    }

    void GrblConnector::Disconnect() {
        gTransceiver.Disconnect();
    }

    int GrblConnector::StartProgram(GProgram& program, bool repeated = false) {
        if (this->gTransceiver.status != GTransceiver::active) {
            return 1;
        }

        if (this->status.state == GStatus::STATE::Hold_1) {
            return 2;
        }

        if (this->status.state == GStatus::STATE::Hold_0) {
            this->gCommand.RtCycleStart();
            return 3;
        }

        if (this->status.state == GStatus::STATE::Run && !repeated) {
            return 4;
        }

        this->gTransceiver.SendCommand(program.GetGCode());
        return 0 ;
    }

    int GrblConnector::PauseProgram() {
        if (this->status.state == GStatus::STATE::Run) {
            this->gCommand.RtFeedHold();
            return 0;
        } else {
            return 1;
        }
    }

    int GrblConnector::StopProgram() {
        this->PauseProgram();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->gCommand.RtSoftReset();
        return 0;
    }
}
