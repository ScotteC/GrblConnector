//
// Created by scottec on 15.04.21.
//

#ifndef GRBL_GSTATUS_HPP
#define GRBL_GSTATUS_HPP

#include <map>
#include "gmodal.hpp"

namespace grblconnector {

class GStatus {
  public:
    enum STATE {
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

    GStatus() = default;


    GStatus::STATE state{};
    std::map<std::string, STATE> str_to_state = {
            {"Idle", Idle},
            {"Run", Run},
            {"Hold:0", Hold_0},
            {"Hold:1", Hold_1},
            {"Jog", Jog},
            {"Alarm", Alarm},
            {"Door:0", Door},
            {"Door:1", Door},
            {"Door:2", Door},
            {"Door:3", Door},
            {"Check", Check},
            {"Home", Home},
            {"Sleep", Sleep}
    };
    
    std::map<STATE, std::string> state_to_str = {
            {Idle, "Idle"},
            {Run, "Run"},
            {Hold_0, "Hold:0"},
            {Hold_1, "Hold:1"},
            {Jog, "Jog"},
            {Alarm, "Alarm"},
            {Door, "Door"},
            {Check, "Check"},
            {Home, "Home"},
            {Sleep, "Sleep"}
    };

    std::map<GModal::WCS, std::array<float, 3>> position = {
            {GModal::WCS::mcs, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_1, { 0.0, 0.0, 0.0 }},
            {GModal::WCS::wcs_2, { 0.0, 0.0, 0.0 }},
            {GModal::WCS::wcs_3, { 0.0, 0.0, 0.0 }},
            {GModal::WCS::wcs_4, { 0.0, 0.0, 0.0 }},
            {GModal::WCS::wcs_5, { 0.0, 0.0, 0.0 }},
            {GModal::WCS::wcs_6, { 0.0, 0.0, 0.0 }},
    };
    
    std::map<GModal::WCS, std::array<float, 3>> offset = {
            {GModal::WCS::wcs_1, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_2, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_3, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_4, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_5, {0.0, 0.0, 0.0}},
            {GModal::WCS::wcs_6, {0.0, 0.0, 0.0}},
    };

    std::map<std::string, bool> flags = {
            {"X", false},
            {"Y", false},
            {"Z", false},
            {"P", false},
            {"D", false},
            {"H", false},
            {"R", false},
            {"S", false},
    };

    int ovr_feed{};
    int ovr_rapid{};
    int ovr_spindle{};

    int buffer_planner{};
    int buffer_rx{};
    int line_number{};

  private:

};
}

#endif //GRBL_GSTATUS_HPP
