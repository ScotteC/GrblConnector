//
// Created by scottec on 12.04.20.
//

#include "grblconnector.hpp"

#include <iostream>

namespace grblconnector {

    GrblConnector::GrblConnector() {
        gStatus = new GStatus();
        gTransceiver = new GTransceiver(gStatus);
        gCommand = new GCommand(gTransceiver);

        mode = none;

        runnable = true;
        running = false;
    }

    GrblConnector::~GrblConnector() {
        Disconnect();

        runnable = false;

        delete gCommand;
        delete gTransceiver;
        delete gStatus;
    }

    bool GrblConnector::Connect(const std::string &device, int baudrate) {
        gTransceiver->Connect(device, baudrate);

        gCommand->GetGCodeState();
        return true;
    }


    void GrblConnector::Disconnect() {
        gCommand->RtSoftReset();
        gTransceiver->Disconnect();
    }

    int GrblConnector::ProgramRun(std::list<std::string> &program) {
        if (this->gStatus->GetState() == GStatus::Hold_0) {
            this->gCommand->RtCycleStart();
            return 0;
        } else if (this->gStatus->GetState() == GStatus::Run) {
            return 1;
        } else if (this->gTransceiver->GetState() != GTransceiver::active) {
            return 2;
        }

        this->gTransceiver->SendCommand(program);
        return 0;
    }

    std::string GrblConnector::ProgramStart() {
        if (this->gStatus->GetState() == GStatus::Hold_0) {
            this->gCommand->RtCycleStart();
            return "Programm wird fortgesetzt...";
        } else if (this->gStatus->GetState() == GStatus::Run) {
            return "Programm läuft bereits";
        } else if (this->gTransceiver->GetState() != GTransceiver::active) {
            return "GRBL nicht verbunden";
        } else if (!this->runnable) {
            return "Maschine gesperrt";
        } else if (this->mode != automatic) {
            return "Maschine in Handbetrieb";
        }

        return "Unknown error";
    }

    std::string GrblConnector::ProgramPause() {
        if (this->gStatus->GetState() == GStatus::Run) {
            this->gCommand->RtFeedHold();
            return "Programm pausiert";
        } else {
            return "Kein laufendes Programm";
        }
    }

    void GrblConnector::ProgramStop() {
        this->ProgramPause();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->GrblReset();
    }

    void GrblConnector::GetGrblStatusReport() {
        if (this->gTransceiver->GetState() == GTransceiver::active)
            this->gCommand->RtStatusReport();
    }

    void GrblConnector::GrblReset() {
        gTransceiver->ClearBuffer();
        gCommand->RtSoftReset();
        running = false;
    }

    void GrblConnector::GrblUnlock() {
        gCommand->KillAlarm();
    }

    void GrblConnector::GrblHome() {
        if (runnable)
            gCommand->HomingCylce();
    }

}
