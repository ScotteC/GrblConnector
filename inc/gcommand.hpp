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

#ifndef GRBL_GCOMMAND_HPP
#define GRBL_GCOMMAND_HPP

#include "gprogram.hpp"

#include <sstream>
#include <iomanip>
#include <utility>

#include "gtransceiver.hpp"

namespace grblconnector {

    class GCommand {
    public:
        // Commands executed in program buffer
        void HomingCycle(std::list<std::string> *queue = nullptr) {
            gStatus.referenceState = GStatus::REFERENCE_STATE::Reference_Cycle_Issued;
            Command("$H", queue);
        }

        void GetGCodeState(std::list<std::string> *queue = nullptr) {
            Command("$G", queue);
        }

        void GetGCodeParameters(std::list<std::string> *queue = nullptr) {
            Command("$#", queue);
        }

        void KillAlarm(std::list<std::string> *queue = nullptr) {
            Command("$X", queue);
        }


        void Sleep(std::list<std::string> *queue = nullptr) {
            Command("$SLP", queue);
        }


        void Jog(GModal::DISTANCE distance,
                 std::tuple<std::array<float, 3>, std::array<bool, 3>> xyz,
                 int f = 0,
                 bool mcs = false) {
            if (f == 0 or
                (!std::get<1>(xyz)[0] and !std::get<1>(xyz)[1] and !std::get<1>(xyz)[2]))
                return;

            if(!gTransceiver.cline.empty())
                return;

            std::stringstream cmd;
            cmd << std::setprecision(4) << std::fixed;

            cmd << "$J=" << "G" + std::to_string(distance);

            if (mcs)
                cmd << " G53";

            if (std::get<1>(xyz)[0])
                cmd << " X" << std::get<0>(xyz)[0];

            if (std::get<1>(xyz)[1])
                cmd << " Y" << std::get<0>(xyz)[1];

            if (std::get<1>(xyz)[2])
                cmd << " Z" << std::get<0>(xyz)[2];

            cmd << " F" << f;

            Command(cmd.str());
        }

        // commands altering modal state
        void StartSpindleClockwise(std::list<std::string> *queue = nullptr) {
            Command("M3", queue);
        }

        void StartSpindleCounterclockwise(std::list<std::string> *queue = nullptr) {
            Command("M4", queue);
        }

        void StopSpindle(std::list<std::string> *queue = nullptr) {
            Command("M5", queue);
        }

        void StartCoolantMist(std::list<std::string> *queue = nullptr) {
            Command("M7", queue);
        }

        void StartCoolantFlood(std::list<std::string> *queue = nullptr) {
            Command("M8", queue);
        }

        void StopCoolant(std::list<std::string> *queue = nullptr) {
            Command("M9", queue);
        }

        // Commands executed by realtime buffer
        void RtSoftReset() {
            RealtimeCommand(static_cast<const char>(0x18));
            gTransceiver.ClearBuffer();
            RealtimeCommand(static_cast<const char>(0x18));
            gStatus.referenceState = GStatus::REFERENCE_STATE::Unreferenced;
        }

        void RtStatusReport() {
            RealtimeCommand(static_cast<const char>(0x3f)); // '?'
        }

        void RtCycleStart() {
            RealtimeCommand(static_cast<const char>(0x7e)); // '~'
        }

        void RtFeedHold() {
            RealtimeCommand(static_cast<const char>(0x21)); // '!'
        }

        void RtSafetyDoor() {
            RealtimeCommand(static_cast<const char>(0x84));
        }

        void RtJogCancel() {
            gTransceiver.ClearBuffer();
            RealtimeCommand(static_cast<const char>(0x85));
        }

        void RtOvrFeedReset() {
            RealtimeCommand(static_cast<const char>(0x90));
        }

        void RtOvrFeedInc10() {
            RealtimeCommand(static_cast<const char>(0x91));
        }

        void RtOvrFeedInc1() {
            RealtimeCommand(static_cast<const char>(0x93));
        }

        void RtOvrFeedDec10() {
            RealtimeCommand(static_cast<const char>(0x92));
        }

        void RtOvrFeedDec1() {
            RealtimeCommand(static_cast<const char>(0x94));
        }

        void RtOvrRapid100() {
            RealtimeCommand(static_cast<const char>(0x95));
        }

        void RtOvrRapid50() {
            RealtimeCommand(static_cast<const char>(0x96));
        }

        void RtOvrRapid25() {
            RealtimeCommand(static_cast<const char>(0x97));
        }

    private:
        friend class GrblConnector;

        explicit GCommand(GTransceiver &gTransceiver, GStatus& gStatus) :
            gTransceiver(gTransceiver), gStatus(gStatus) {}

        void Command(const std::string &cmd, std::list<std::string> *queue = nullptr) {
            if (queue != nullptr)
                queue->push_back(cmd);
            else
                gTransceiver.SendCommand(cmd);
        }

        void RealtimeCommand(const char cmd) {
            gTransceiver.SendRealtimeCommand(cmd);
        }

        GTransceiver& gTransceiver;
        GStatus& gStatus;
    };

}

#endif // GRBL_GCOMMAND_HPP
