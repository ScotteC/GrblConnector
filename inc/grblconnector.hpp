//
// Created by scottec on 12.04.20.
//

#ifndef GRBL_CONTROLLER_HPP
#define GRBL_CONTROLLER_HPP

#include <map>
#include <list>

#include "gprogram.hpp"
#include "gparser.hpp"
#include "gcommand.hpp"
#include "gtransceiver.hpp"

namespace grblconnector {
    class GrblConnector {
    public:
        GrblConnector();

        ~GrblConnector();

        bool Connect(const std::string &device, int baudrate);

        void Disconnect();

        int SetProgram(GProgram &program);

        int SetProgram(std::list<std::string> &program);

        GProgram GetProgram();

        int StartProgram(bool repeated);

        int PauseProgram();

        void StopProgram();

        void GrblReset();

        void GrblUnlock();

        int GrblHome();

        void GetGrblStatusReport();

        enum MODE {
            none = -1,
            automatic = 0,
            hand = 1
        };

    public:
        GParser gInterpreter{};
        GTransceiver gTransceiver{};
        GCommand gCommand{gTransceiver};

    private:
        MODE mode = none;
        GProgram gProgram{};
    };
}

#endif //GRBL_CONTROLLER_HPP
