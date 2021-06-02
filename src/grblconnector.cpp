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

#include <iostream>

namespace grblconnector {

    GrblConnector::GrblConnector() {
//        gInterpreter = new GParser();
//        gTransceiver = new GTransceiver(gInterpreter);
//        gCommand = new GCommand(gTransceiver);
//
//        mode = none;
    }

    GrblConnector::~GrblConnector() {
        Disconnect();
    }

    bool GrblConnector::Connect(const std::string &device, int baudrate) {
        gTransceiver.Connect(device, baudrate);
        gCommand.GetGCodeState();
        return true;
    }

    void GrblConnector::Disconnect() {
        gCommand.RtSoftReset();
        gTransceiver.Disconnect();
    }

    int GrblConnector::SetProgram(GProgram &program) {
        gProgram = program;
        return 0;
    }

    int GrblConnector::SetProgram(std::list<std::string> &program) {
        gProgram = GProgram(program);
        return 0;
    }

    GProgram GrblConnector::GetProgram() {
        return gProgram;
    }

    int GrblConnector::StartProgram(bool repeated = false) {
        if (this->gTransceiver.GetStatus() != GTransceiver::active) {
            return 1;
        }

        if (this->mode != automatic) {
            return 2;
        }

        if (this->gInterpreter.GetState() == GStatus::STATE::Hold_0) {
            this->gCommand.RtCycleStart();
            return 3;
        }

        if (!repeated && this->gInterpreter.GetState() == GStatus::STATE::Run) {
            return 4;
        }

        this->gTransceiver.SendCommand(gProgram.GetGCode());
        return 0;
    }

    int GrblConnector::PauseProgram() {
        if (this->gInterpreter.GetState() == GStatus::STATE::Run) {
            this->gCommand.RtFeedHold();
            return 0;
        } else {
            return 1;
        }
    }

    void GrblConnector::StopProgram() {
        this->PauseProgram();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->GrblReset();
    }

    void GrblConnector::GetGrblStatusReport() {
        if (this->gTransceiver.GetStatus() == GTransceiver::active)
            this->gCommand.RtStatusReport();
    }

    void GrblConnector::GrblReset() {
        gTransceiver.ClearBuffer();
        gCommand.RtSoftReset();
    }

    void GrblConnector::GrblUnlock() {
        gCommand.KillAlarm();
    }

    int GrblConnector::GrblHome() {
        if (this->gTransceiver.GetStatus() != GTransceiver::active) {
            return 1;
        }
        gCommand.HomingCylce();
        return 0;
    }

}
