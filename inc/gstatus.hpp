//
// Created by scottec on 12.04.20.
//

#ifndef GRBL_GSTATUS_HPP
#define GRBL_GSTATUS_HPP

//#include "GModal.hpp"
#include "gmodal.h"

#include <list>
#include <map>
#include <string>

class GStatus {
public:
    enum GSTATE {
        Idle,
        Run,
        Hold_0,
        Hold_1,
        Jog,
        Alarm,
        Door,
        Check,
        Home,
        Sleep
    };

    struct grbl_status {
        std::array<float, 3> mpos;
        std::array<float, 3> wpos;
        std::string state;
        float feed;
        float ovr_feed;
        float ovr_rapid;
    };

    GStatus();

    void ParseLine(std::string &line);

    void ParseStatusReport(std::string &line);
    void ParseModalState(std::string &line);
    void ParseWcsOffset(std::string &line);
    void ParseAlarm(std::string &line);
    void ParseError(std::string &line);
    void ParseMessage(std::string &line);

    GSTATE GetState() { return this->gstate; }
    GModal GetMachineStatus();

private:
    GSTATE gstate{};
    std::map<std::string, GSTATE> str_to_state;
    std::map<GSTATE, std::string> state_to_str;

    GModal modal{};

    std::map<GModal::WCS, std::array<float, 3>> position{};
    std::map<GModal::WCS, std::array<float, 3>> offset{};

    int ovr_feed{};
    int ovr_rapid{};
    int ovr_spindle{};

    int buffer_planner{};
    int buffer_rx{};
    int line_number{};

    bool error{};
    std::list<int> errors;
    bool alarm{};
    std::list<int> alarms;
    bool message{};
    std::list<std::string> messages;
};

#endif // GSTATUS_HPP
