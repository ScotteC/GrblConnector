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
                 float x = 0.0, float y = 0.0, float z = 0.0,
                 int f = 0,
                 bool mcs = false) {
            if (f == 0)
                return;

            std::stringstream cmd;
            cmd << std::setprecision(4) << std::fixed;

            cmd << "$J=" << "G" + std::to_string(distance);

            if (mcs)
                cmd << " G53";

            cmd << " X" << x << " Y" << y << " Z" << z << " F" << f;

            Command(cmd.str());
        }

        // Commands executed by realtime buffer
        void RtSoftReset() {
            gTransceiver.ClearBuffer();
            RealtimeCommand(static_cast<const char>(0x18));
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

        explicit GCommand(GTransceiver &gTransceiver) : gTransceiver(gTransceiver) {}

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
    };

}

#endif // GRBL_GCOMMAND_HPP
