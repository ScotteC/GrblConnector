//
// Created by scottec on 12.04.20.
//

#include "grbl-controller.hpp"

#include <iostream>

GrblController::GrblController() {
    gStatus = new GStatus();
    gTransceiver = new GTransceiver(gStatus);
    gCommand = new GCommand(gTransceiver);

    mode = none;

    runnable = true;
    running = false;
}
GrblController::~GrblController() {
    Disconnect();

    runnable = false;

    delete gCommand;
    delete gTransceiver;
    delete gStatus;
}

bool GrblController::Connect(const std::string& device, int baudrate) {
    gTransceiver->Connect(device, baudrate);

    gCommand->GetGCodeState();
    return true;
}


void GrblController::Disconnect() {
    gCommand->RtSoftReset();
    gTransceiver->Disconnect();
}

std::string GrblController::ProgramStart() {
    if (this->gStatus->GetState() == GStatus::Hold_0) {
        this->gCommand->RtCycleStart();
        return "Programm wird fortgesetzt...";
    }

    else if (this->gStatus->GetState() == GStatus::Run) {
        return "Programm läuft bereits";
    }

    else if (this->gTransceiver->GetState() != GTransceiver::active){
        return "GRBL nicht verbunden";
    }

    else if (!this->runnable){
        return "Maschine gesperrt";
    }

    else if (this->mode !=  automatic) {
        return "Maschine in Handbetrieb";
    }

    return "Unknown error";
}

std::string GrblController::ProgramPause() {
    if (this->gStatus->GetState() == GStatus::Run) {
        this->gCommand->RtFeedHold();
        return "Programm pausiert";
    } else {
        return "Kein laufendes Programm";
    }
}

void GrblController::ProgramStop() {
    this->ProgramPause();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    this->GrblReset();
}

void GrblController::GetGrblStatusReport() {
    if (this->gTransceiver->GetState() == GTransceiver::active)
        this->gCommand->RtStatusReport();
}

void GrblController::GrblReset() {
    gTransceiver->ClearBuffer();
    gCommand->RtSoftReset();
    running = false;
}

void GrblController::GrblUnlock() {
    gCommand->KillAlarm();
}

void GrblController::GrblHome() {
    if (runnable)
        gCommand->HomingCylce();
}
