//
// Created by scottec on 10.04.20.
//

#ifndef GRBL_GPROGRAM_HPP
#define GRBL_GPROGRAM_HPP

#include <list>
#include <map>
#include <array>
#include <tuple>
#include <string>

#include "gmodal.h"

class GProgram {
public:
    GProgram() {};

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

#endif // GRBL_GPROGRAM_HPP
