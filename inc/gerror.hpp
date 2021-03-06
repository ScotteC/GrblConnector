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

#ifndef GRBL_GERROR_HPP
#define GRBL_GERROR_HPP

namespace grblconnector {
class GError {
  public:
    std::string GetErrorMessage(int error_code) {
        return errors.at(error_code);
    }

  private:
    const std::map<int, std::string> errors = {
            {1,  "G-code words consist of a letter and a value. Letter was not found."},
            {2,  "Missing the expected G-code word value or numeric value format is not valid."},
            {3,  "Grbl '$' system command was not recognized or supported."},
            {4,  "Negative value received for an expected positive value."},
            {5,  "Homing cycle failure. Homing is not enabled via settings."},
            {6,  "Minimum step pulse time must be greater than 3usec."},
            {7,  "An EEPROM read failed. Auto-restoring affected EEPROM to default values."},
            {8,  "Grbl '$' command cannot be used unless Grbl is IDLE. Ensures smooth operation during a job."},
            {9,  "G-code commands are locked out during alarm or jog state."},
            {10, "Soft limits cannot be enabled without homing also enabled."},
            {11, "Max characters per line exceeded. Received command line was not executed."},
            {12, "Grbl '$' setting value cause the step rate to exceed the maximum supported."},
            {13, "Safety door detected as opened and door state initiated."},
            {14, "Build info or startup line exceeded EEPROM line length limit. Line not stored."},
            {15, "Jog target exceeds machine travel. Jog command has been ignored."},
            {16, "Jog command has no '=' or contains prohibited g-code."},
            {17, "Laser mode requires PWM output."},
            {20, "Unsupported or invalid g-code command found in block."},
            {21, "More than one g-code command from same modal group found in block."},
            {22, "Feed rate has not yet been set or is undefined."},
            {23, "G-code command in block requires an integer value."},
            {24, "More than one g-code command that requires axis words found in block."},
            {25, "Repeated g-code word found in block."},
            {26, "No axis words found in block for g-code command or current modal state which requires them."},
            {27, "Line number value is invalid."},
            {28, "G-code command is missing a required value word."},
            {29, "G59.x work coordinate systems are not supported."},
            {30, "G53 only allowed with G0 and G1 motion modes."},
            {31, "Axis words found in block when no command or current modal state uses them."},
            {32, "G2 and G3 arcs require at least one in-plane axis word."},
            {33, "Motion command target is invalid."},
            {34, "Arc radius value is invalid."},
            {35, "G2 and G3 arcs require at least one in-plane offset word."},
            {36, "Unused value words found in block."},
            {37, "G43.1 dynamic tool length offset is not assigned to configured tool length axis."},
            {38, "Tool number greater than max supported value."}
    };
};
}
#endif //GRBL_GERROR_HPP
