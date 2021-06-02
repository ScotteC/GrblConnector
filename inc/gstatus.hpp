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
