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

#ifndef GRBL_GALARM_HPP
#define GRBL_GALARM_HPP

#include <list>

namespace grblconnector {
class GAlarm {
  public:
    std::string GetAlarmMessage(int alarm_code) {
        return alarm_messages.at(alarm_code);
    }

  private:
    const std::map<int, std::string> alarm_messages = {
        {1,  "Hard limit has been triggered. Machine position is likely lost due to sudden halt. Re-homing is highly recommended."},
        {2,  "Soft limit alarm. G-code motion target exceeds machine travel. Machine position retained. Alarm may be safely unlocked."},
        {3,  "Reset while in motion. Machine position is likely lost due to sudden halt. Re-homing is highly recommended."},
        {4,  "Probe fail. Probe is not in the expected initial state before starting probe cycle when G38.2 and G38.3 is not triggered and G38.4 and G38.5 is triggered."},
        {5,  "Probe fail. Probe did not contact the workpiece within the programmed travel for G38.2 and G38.4."},
        {6,  "Homing fail. The active homing cycle was reset."},
        {7,  "Homing fail. Safety door was opened during homing cycle."},
        {8,  "Homing fail. Pull off travel failed to clear limit switch. Try increasing pull-off setting or check wiring."},
        {9,  "Homing fail. Could not find limit switch within search distances. Try increasing max travel, decreasing pull-off distance, or check wiring."},
        {10, "Homing fail. Second dual axis limit switch failed to trigger within configured search distance after first. Try increasing trigger fail distance or check wiring."},
    };
};
}
#endif //GRBL_GALARM_HPP
