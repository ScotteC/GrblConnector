//
// Created by scottec on 12.04.20.
//

#include "gstatus.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

std::regex status_report_regex {"\\<.*\\>"};
std::regex modal_state_regex {"(\\[GC:[\\w\\s]*\\])"};
std::regex wcs_offset_regex {"\\[(G5[4-9]):([+\\-]?\\d+.?\\d*),([+\\-]?\\d+.?\\d*),([+\\-]?\\d+.?\\d*)\\]"};
std::regex msg_regex {"\\[(MSG:)(.*)\\]"};


GStatus::GStatus() {
    str_to_state["Idle"]    = Idle;
    str_to_state["Run"]     = Run;
    str_to_state["Hold:0"]  = Hold_0;
    str_to_state["Hold:1"]  = Hold_1;
    str_to_state["Jog"]     = Jog;
    str_to_state["Alarm"]   = Alarm;
    str_to_state["Door:0"]  = Door;
    str_to_state["Door:1"]  = Door;
    str_to_state["Door:2"]  = Door;
    str_to_state["Door:3"]  = Door;
    str_to_state["Check"]   = Check;
    str_to_state["Home"]    = Home;
    str_to_state["Sleep"]   = Sleep;

    state_to_str[Idle] = "Idle";
    state_to_str[Run] = "Run";
    state_to_str[Hold_0] = "Hold:0";
    state_to_str[Hold_1] = "Hold:1";
    state_to_str[Jog] = "Jog";
    state_to_str[Alarm] = "Alarm";
    state_to_str[Door] = "Door";
    state_to_str[Check] = "Check";
    state_to_str[Home] = "Home";
    state_to_str[Sleep] = "Sleep";

    position[GModal::WCS::mcs]   = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_1] = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_2] = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_3] = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_4] = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_5] = {0.0, 0.0, 0.0};
    position[GModal::WCS::wcs_6] = {0.0, 0.0, 0.0};

    offset[GModal::WCS::wcs_1] = {0.0, 0.0, 0.0};
    offset[GModal::WCS::wcs_2] = {0.0, 0.0, 0.0};
    offset[GModal::WCS::wcs_3] = {0.0, 0.0, 0.0};
    offset[GModal::WCS::wcs_4] = {0.0, 0.0, 0.0};
    offset[GModal::WCS::wcs_5] = {0.0, 0.0, 0.0};
    offset[GModal::WCS::wcs_6] = {0.0, 0.0, 0.0};
}

void GStatus::ParseLine(std::string &line) {
    if (std::regex_match(line, status_report_regex))
        ParseStatusReport(line);

    else if (std::regex_match(line, modal_state_regex))
        ParseModalState(line);

    else if(std::regex_match(line, wcs_offset_regex))
        ParseWcsOffset(line);

    else if (std::regex_match(line, msg_regex))
        ParseMessage(line);

}

void GStatus::ParseStatusReport(std::string &line) {
    if (std::regex_match(line, status_report_regex)) {
        std::smatch match;

        // machine status
        if(std::regex_search(line, match, std::regex{"((?<=\\<)\\w+:?[0-3]?(?=\\|))"})) {
            gstate = str_to_state[match[1]];
        }

        // machine position
        if(std::regex_search(line, match, std::regex{"(?<=\\|)MPos:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
            position[GModal::WCS::mcs][0] = std::stof(match[1]);
            position[GModal::WCS::mcs][1] = std::stof(match[2]);
            position[GModal::WCS::mcs][2] = std::stof(match[3]);
            position[modal.wcs][0] = position[GModal::WCS::mcs][0] - offset[modal.wcs][0];
            position[modal.wcs][1] = position[GModal::WCS::mcs][1] - offset[modal.wcs][1];
            position[modal.wcs][2] = position[GModal::WCS::mcs][2] - offset[modal.wcs][2];
        }

        // working position
        if(std::regex_search(line, match, std::regex{"(?<=\\|)WPos:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
            position[modal.wcs][0] = std::stof(match[1]);
            position[modal.wcs][1] = std::stof(match[2]);
            position[modal.wcs][2] = std::stof(match[3]);
            position[GModal::WCS::mcs][0] = position[modal.wcs][0] + offset[modal.wcs][0];
            position[GModal::WCS::mcs][1] = position[modal.wcs][1] + offset[modal.wcs][1];
            position[GModal::WCS::mcs][2] = position[modal.wcs][2] + offset[modal.wcs][2];
        }

        // feed rate and spindle speed
        if(std::regex_search(line, match, std::regex{"(?<=\\|)FS:([+\\-]?\\d+),([+\\-]?\\d+)(?=[\\|\\>])"})) {
            modal.feed_rate = std::stoi(match[1]);
            modal.spindle_speed = std::stoi(match[2]);
        }

        // only feed rate
        if(std::regex_search(line, match, std::regex{"(?<=\\|)F:([+\\-]?\\d+)(?=[\\|\\>])"})) {
            modal.feed_rate = std::stoi(match[1]);
        }

        // limits and flags
        if(std::regex_search(line, match, std::regex{"(?<=\\|)Pn:([XYZPDHRS])([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)(?=[\\|\\>])"})) {
            // ToDo
        }

        // working coordinates offsets
        if(std::regex_search(line, match, std::regex{"(?<=\\|)WCO:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
            // ToDo
        }

        // buffer status
        if(std::regex_search(line, match, std::regex{"(?<=\\|)Bf:(\\d+),(\\d+)(?=[\\|\\>])"})) {
            buffer_planner = std::stoi(match[1]);
            buffer_rx = std::stoi(match[2]);
        }

        // line number
        if(std::regex_search(line, match, std::regex{"(?<=\\|)Ln:(\\d+)(?=[\\|\\>])"})) {
            line_number = std::stoi(match[1]);
        }

        // override rates
        if(std::regex_search(line, match, std::regex{"(?<=\\|)Ov:(\\d+),(\\d+),(\\d+)(?=[\\|\\>])"})) {
            ovr_feed = std::stoi(match[1]);
            ovr_rapid = std::stoi(match[2]);
            ovr_spindle = std::stoi(match[3]);
        }
    }
}

void GStatus::ParseModalState(std::string &line){
    if (std::regex_match(line, modal_state_regex)) {
        // program mode is only present if in mode pause or end
        modal.program = GModal::PROGRAM::program_none;

        std::smatch match;
        // motion mode
        if (std::regex_search(line, match, std::regex{"((?<=G)[0123](?= ))"})) {
            modal.motion = static_cast<GModal::MOTION>(std::stoi(match[1]));
        }

        // wcs mode
        if (std::regex_search(line, match, std::regex{"((?<=G)5[4-9](?= ))"})) {
            modal.wcs = static_cast<GModal::WCS>(std::stoi(match[1]));
        }

        // plane mode
        if (std::regex_search(line, match, std::regex{"((?<=G)1[789](?= ))"})) {
            modal.plane = static_cast<GModal::PLANE>(std::stoi(match[1]));
        }

        // unit mode
        if (std::regex_search(line, match, std::regex{"((?<=G)2[01](?= ))"})) {
            modal.unit = static_cast<GModal::UNIT>(std::stoi(match[1]));
        }

        // distance mode
        if (std::regex_search(line, match, std::regex{"((?<=G)9[01](?= ))"})) {
            modal.distance = static_cast<GModal::DISTANCE>(std::stoi(match[1]));
        }

        // feed rate mode
        if (std::regex_search(line, match, std::regex{"((?<=G)9[34](?= ))"})) {
            modal.feed = static_cast<GModal::FEED>(std::stoi(match[1]));
        }

        // program mode
        if (std::regex_search(line, match, std::regex{"((?<=M)([012]|30)(?= ))"})) {
            modal.program = static_cast<GModal::PROGRAM>(std::stoi(match[1]));
        }

        // spindle mode
        if (std::regex_search(line, match, std::regex{"((?<=M)[345](?= ))"})) {
            modal.spindle = static_cast<GModal::SPINDLE>(std::stoi(match[1]));
        }

        // coolant mode
        if (std::regex_search(line, match, std::regex{"((?<=M)[789](?= ))"})) {
            modal.coolant = static_cast<GModal::COOLANT>(std::stoi(match[1]));
        }

        // tool number
        if (std::regex_search(line, match, std::regex{"((?<=T)[0-9]+(?= ))"})) {
            modal.tool_number = std::stoi(match[1]);
        }

        // feed rate
        if (std::regex_search(line, match, std::regex{"((?<=F)[0-9]+(?= ))"})) {
            modal.feed_rate = std::stoi(match[1]);
        }

        // spindle speed
        if (std::regex_search(line, match, std::regex{"((?<=S)[0-9]+(?= ))"})) {
            modal.spindle_speed = std::stoi(match[1]);
        }
    }
}

void GStatus::ParseWcsOffset(std::string &line) {
    std::smatch match;
    if (std::regex_search(line, match, wcs_offset_regex)) {
        auto w = static_cast<GModal::WCS>(std::stoi(match[1].str().substr(1, 2)));
        offset[w][0] = std::stof(match[2]);
        offset[w][1] = std::stof(match[3]);
        offset[w][2] = std::stof(match[4]);
    }
}

void GStatus::ParseAlarm(std::string &line) {
    std::vector<std::string> fields;
    boost::remove_erase_if(line, boost::is_any_of("\n\r"));
    boost::split(fields, line, boost::is_any_of(":"));
    try{
        alarms.push_back(std::stoi(fields[1]));
        alarm = true;
    } catch (std::invalid_argument &err){
        std::cout << "ParseError in line: " << line << std::endl;
    }
}

void GStatus::ParseError(std::string &line) {
    std::vector<std::string> fields;
    boost::remove_erase_if(line, boost::is_any_of("\n\r"));
    boost::split(fields, line, boost::is_any_of(":"));
    try{
        errors.push_back(std::stoi(fields[1]));
        error = true;
    } catch (std::invalid_argument &err){
        std::cout << "ParseError in line: " << line << std::endl;
    }
}

GModal GStatus::GetMachineStatus() {
    return modal;
}

void GStatus::ParseMessage(std::string &line) {
    std::smatch match;
    if (std::regex_search(line, match, msg_regex)) {
        messages.push_back(match[1]);
        message = true;
    }
}
