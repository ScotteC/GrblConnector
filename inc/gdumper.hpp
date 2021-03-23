//
// Created by scottec on 14.04.20.
//

#ifndef GRBL_GDUMPER_HPP
#define GRBL_GDUMPER_HPP

#include <list>
#include <string>
#include <ostream>

#include "gsender.hpp"

namespace grblconnector {

    class GDumper : public GSender {
    public:
        explicit GDumper(std::ostream *stream) {
            this->stream = stream;
        }

        bool SendCommand(std::string cmd) override {
            *stream << cmd << std::endl;
            return true;
        }

        bool SendCommand(std::list<std::string> *cmd_list) override {
            for (auto &cmd : *cmd_list)
                *stream << cmd << std::endl;
            return true;
        }

        bool SendRealtimeCommand(const char cmd) override {
            *stream << cmd << std::endl;
            return true;
        }

    private:
        std::ostream *stream;
    };

}

#endif // GRBL_GDUMPER_HPP
