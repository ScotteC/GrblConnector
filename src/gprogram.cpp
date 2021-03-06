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

#include "gprogram.hpp"

#include <sstream>
#include <iomanip>

namespace grblconnector {

    GProgram::GProgram(std::list<std::string> &program) {
        Reset();
        this->program = program;
    }

    void GProgram::Reset() {
        modal = GModal();
        program.clear();
    }

    GProgram GProgram::SetWcs(GModal::WCS wcs) {
        if (wcs != modal.wcs) {
            if (wcs != GModal::mcs)
                modal.wcs = wcs;
            program.push_back("G" + std::to_string(wcs));
        }
        return *this;
    }

    GProgram GProgram::ConfigWcs(
            GModal::WCS wcs,
            std::array<float, 3> xyz) {
        std::stringstream buf;
        buf << std::setprecision(4) << std::fixed
            << "G10 L2 P" << wcs - 53
            << " X" << xyz[0] << " Y" << xyz[1] << " Z" << xyz[2];
        program.push_back(buf.str());
        return *this;
    }

    GProgram GProgram::SetDistanceMode(GModal::DISTANCE distance) {
        if (distance != modal.distance) {
            program.push_back("G" + std::to_string(distance));
            modal.distance = distance;
        }
        return *this;
    }

    GProgram GProgram::SetFeedMode(GModal::FEED feed) {
        if (feed != modal.feed) {
            program.push_back("G" + std::to_string(feed));
            modal.feed = feed;
        }
        return *this;
    }

    GProgram GProgram::MoveRapid(GModal::DISTANCE distance,
                                 std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz) {
        return this->Move(distance, GModal::motion_rapid, xyz);
    }

    GProgram GProgram::MoveLinear(
            GModal::DISTANCE distance,
            std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz,
            int f) {
        return this->Move(distance, GModal::motion_linear, xyz, f);
    }

    GProgram GProgram::Move(
            GModal::DISTANCE distance,
            GModal::MOTION motion,
            std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz,
            int f) {
        if ((motion != GModal::motion_rapid and f == 0) or
            (!std::get<1>(xyz)[0] and !std::get<1>(xyz)[1] and !std::get<1>(xyz)[2]))
            return *this;

        this->SetDistanceMode(distance);

        std::stringstream buf;
        buf << std::setprecision(4) << std::fixed;

        if (motion != modal.motion) {
            buf << "G" << motion;
            modal.motion = motion;
        }

        if (std::get<1>(xyz)[0])
            buf << " X" << std::get<0>(xyz)[0];

        if (std::get<1>(xyz)[1])
            buf << " Y" << std::get<0>(xyz)[1];

        if (std::get<1>(xyz)[2])
            buf << " Z" << std::get<0>(xyz)[2];

        if (motion != GModal::motion_rapid and f != modal.feed_rate) {
            buf << " F" << f;
            modal.feed_rate = f;
        }

        program.push_back(buf.str());
        return *this;
    }

    GProgram GProgram::MoveArc(
            GModal::MOTION motion,
            std::array<float, 3> xyz,
            float i, float j,
            int f) {
        if (f == 0 or (motion != GModal::motion_arc_ccw and motion != GModal::motion_arc_cw) or
            (xyz[0] == 0.0 and xyz[1] == 0.0 and xyz[2] == 0.0))
            return *this;

        std::stringstream buf;
        buf << std::setprecision(4) << std::fixed;

        buf << "G" << motion;

        // XY  - endpoint X/Y
        buf << " X" << xyz[0] << " Y" << xyz[1];

        // helix, if needed
        if (xyz[2] != 0.0)
            buf << " Z" << xyz[2];

        // center offsets
        buf << " I" << i << " J" << j;

        //feed_rate
        if (f != modal.feed_rate) {
            buf << " F" << f;
            modal.feed_rate = f;
        }

        program.push_back(buf.str());
        return *this;
    }


    GProgram GProgram::Dwell(float seconds) {
        if (seconds > 0)
            program.push_back("G4 P" + std::to_string(seconds));
        return *this;
    }

    GProgram GProgram::Pause() {
        program.emplace_back("M0");
        modal.program = GModal::program_m0;
        return *this;
    }

    GProgram GProgram::End() {
        program.emplace_back("M2");
        modal.program = GModal::program_m2;
        return *this;
    }

    GProgram GProgram::SetUnit(GModal::UNIT unit) {
        if (unit != modal.unit) {
            program.emplace_back("G" + std::to_string(unit));
            modal.unit = unit;
        }
        return *this;
    }

    GProgram GProgram::SetPlane(GModal::PLANE plane) {
        if (plane != modal.plane) {
            program.emplace_back("G" + std::to_string(plane));
            modal.plane = plane;
        }
        return *this;
    }

    GProgram GProgram::StartSpindleClockwise() {
        if (modal.spindle != GModal::SPINDLE::spindle_cw) {
            program.emplace_back("M" + std::to_string(GModal::SPINDLE::spindle_cw));
            modal.spindle = GModal::SPINDLE::spindle_cw;
        }
        return *this;
    }

    GProgram GProgram::StartSpindleCounterclockwise() {
        if (modal.spindle != GModal::SPINDLE::spindle_ccw) {
            program.emplace_back("M" + std::to_string(GModal::SPINDLE::spindle_ccw));
            modal.spindle = GModal::SPINDLE::spindle_ccw;
        }
        return *this;
    }

    GProgram GProgram::StopSpindle() {
        if (modal.spindle != GModal::SPINDLE::spindle_off) {
            program.emplace_back("M" + std::to_string(GModal::SPINDLE::spindle_off));
            modal.spindle = GModal::SPINDLE::spindle_off;
        }
        return *this;
    }

    GProgram GProgram::StartCoolantMist() {
        if (modal.coolant != GModal::COOLANT::coolant_mist) {
            program.emplace_back("M" + std::to_string(GModal::COOLANT::coolant_mist));
            modal.coolant = GModal::COOLANT::coolant_mist;
        }
        return *this;
    }

    GProgram GProgram::StartCoolantFlood() {
        if (modal.coolant != GModal::COOLANT::coolant_flood) {
            program.emplace_back("M" + std::to_string(GModal::COOLANT::coolant_flood));
            modal.coolant = GModal::COOLANT::coolant_mist;
        }
        return *this;
    }

    GProgram GProgram::StopCoolant() {
        if (modal.coolant != GModal::COOLANT::coolant_off) {
            program.emplace_back("M" + std::to_string(GModal::COOLANT::coolant_off));
            modal.coolant = GModal::COOLANT::coolant_off;
        }
        return *this;
    }

    std::list<std::string> GProgram::GetGCode() {
        return program;
    }
}
