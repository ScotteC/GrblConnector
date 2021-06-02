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

#ifndef GRBL_GPROGRAM_HPP
#define GRBL_GPROGRAM_HPP

#include <list>
#include <map>
#include <array>
#include <tuple>
#include <string>

#include "gmodal.hpp"

namespace grblconnector {

    class GProgram {
    public:
        GProgram() = default;

        explicit GProgram(std::list<std::string> &program);

        void Reset();

        GProgram SelectWcs(GModal::WCS wcs);

        GProgram ConfigWcs(
                GModal::WCS wcs,
                std::array<float, 3> xyz);

        GProgram SetUnit(GModal::UNIT unit);

        GProgram SelectPlane(GModal::PLANE plane);

        GProgram SetDistanceMode(GModal::DISTANCE distance);

        GProgram MoveRapid(
                GModal::DISTANCE distance,
                std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz);

        GProgram MoveLinear(
                GModal::DISTANCE distance,
                std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz,
                int f = 0);

        GProgram MoveArc(
                GModal::MOTION motion,
                std::array<float, 3> xyz,
                float i = 0.0, float j = 0.0,
                int f = 0);

        GProgram Dwell(float seconds = 0.0);

        GProgram StartSpindleClockwise();

        GProgram StartSpindleCounterclockwise();

        GProgram StopSpindle();

        GProgram StartCoolantMist();

        GProgram StartCoolantFlood();

        GProgram StopCoolant();

        GProgram Pause();

        GProgram End();

        std::list<std::string> GetGCode();

    protected:
        GProgram Move(
                GModal::DISTANCE distance,
                GModal::MOTION motion,
                std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz,
                int f = 0);

    private:
        GModal modal{};
        std::list<std::string> program{};
    };
}
#endif // GRBL_GPROGRAM_HPP
