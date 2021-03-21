//
// Created by scottec on 12.04.20.
//

#ifndef GRBL_CONTROLLER_HPP
#define GRBL_CONTROLLER_HPP

#include <map>
#include <list>

#include "gprogram.hpp"
#include "gstatus.hpp"
#include "gcommand.hpp"
#include "gtransceiver.hpp"

class GrblController {
public:
    GrblController();
    ~GrblController();

    bool Connect(const std::string& device, int baudrate);
    void Disconnect();

    bool is_runnable() const { return this->runnable; }

    std::string ProgramStart();
    std::string ProgramPause();
    void ProgramStop();

    void GrblReset();
    void GrblUnlock();
    void GrblHome();

    void GetGrblStatusReport();

    enum MODE {
        none = -1,
        automatic = 0,
        hand = 1
    };

public:
    GStatus *gStatus;
    GTransceiver *gTransceiver;
    GCommand *gCommand;

private:
    MODE mode;
    bool runnable, running;
};


#endif //WELDBOT_GUI_2_BACKENDHANDLER_HPP
