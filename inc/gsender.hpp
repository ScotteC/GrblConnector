//
// Created by scottec on 14.04.20.
//

#ifndef GRBL_GSENDER_HPP
#define GRBL_GSENDER_HPP

namespace grblconnector {

    class GSender {
    public:
        virtual bool SendCommand(std::string cmd) = 0;

        virtual bool SendCommand(std::list<std::string> cmd_list) = 0;

        virtual bool SendRealtimeCommand(char cmd) = 0;
    };
}
#endif // GRBL_GSENDER_HPP
