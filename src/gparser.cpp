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

#include "gparser.hpp"

#include <string>
#include <boost/regex.hpp>

namespace grblconnector {

    int GParser::ParseLine(std::string &line) {
        if (boost::regex_match(line, boost::regex{"ok"}))
            return 1;

        if (ParseAlarm(line))
            return 1;

        if (ParseError(line))
            return 1;

        if (boost::regex_match(line, boost::regex{"Grbl"}))
            return 2;

        if (ParseStatusReport(line))
            return 0;

        if (ParseModalState(line))
            return 0;

        if (ParseWcsOffset(line))
            return 0;

        if (ParseMessage(line))
            return 0;

        return 0;
    }

    bool GParser::ParseStatusReport(std::string &line) {
        if (boost::regex_match(line, boost::regex{"\\<.*\\>"})) {
            boost::smatch match;

            // machine status
            if (boost::regex_search(line, match, boost::regex{"((?<=\\<)\\w+:?[0-3]?(?=\\|))"})) {
                status.state = status.str_to_state[match[1]];
            }

            // machine position
            if (boost::regex_search(line, match,boost::regex{"(?<=\\|)MPos:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
                status.position[GModal::WCS::mcs][0] = std::stof(match[1]);
                status.position[GModal::WCS::mcs][1] = std::stof(match[2]);
                status.position[GModal::WCS::mcs][2] = std::stof(match[3]);
                status.position[modal.wcs][0] = status.position[GModal::WCS::mcs][0] - status.offset[modal.wcs][0];
                status.position[modal.wcs][1] = status.position[GModal::WCS::mcs][1] - status.offset[modal.wcs][1];
                status.position[modal.wcs][2] = status.position[GModal::WCS::mcs][2] - status.offset[modal.wcs][2];
            }

            // working position
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)WPos:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
                status.position[modal.wcs][0] = std::stof(match[1]);
                status.position[modal.wcs][1] = std::stof(match[2]);
                status.position[modal.wcs][2] = std::stof(match[3]);
                status.position[GModal::WCS::mcs][0] = status.position[modal.wcs][0] + status.offset[modal.wcs][0];
                status.position[GModal::WCS::mcs][1] = status.position[modal.wcs][1] + status.offset[modal.wcs][1];
                status.position[GModal::WCS::mcs][2] = status.position[modal.wcs][2] + status.offset[modal.wcs][2];
            }

            // feed rate and spindle speed
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)FS:([+\\-]?\\d+),([+\\-]?\\d+)(?=[\\|\\>])"})) {
                modal.feed_rate = std::stoi(match[1]);
                modal.spindle_speed = std::stoi(match[2]);
            }

            // only feed rate
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)F:([+\\-]?\\d+)(?=[\\|\\>])"})) {
                modal.feed_rate = std::stoi(match[1]);
            }

            // limits and flags
            if (boost::regex_search(line, match, boost::regex{
                    "(?<=\\|)Pn:([XYZPDHRS])([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)(?=[\\|\\>])"})) {
                // ToDo
            }

            // working coordinates offsets
            if (boost::regex_search(line, match, boost::regex{
                    "(?<=\\|)WCO:([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*),([+\\-]?\\d+\\.?\\d*)(?=[\\|\\>])"})) {
                // ToDo
            }

            // buffer status
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)Bf:(\\d+),(\\d+)(?=[\\|\\>])"})) {
                status.buffer_planner = std::stoi(match[1]);
                status.buffer_rx = std::stoi(match[2]);
            }

            // line number
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)Ln:(\\d+)(?=[\\|\\>])"})) {
                status.line_number = std::stoi(match[1]);
            }

            // override rates
            if (boost::regex_search(line, match, boost::regex{"(?<=\\|)Ov:(\\d+),(\\d+),(\\d+)(?=[\\|\\>])"})) {
                status.ovr_feed = std::stoi(match[1]);
                status.ovr_rapid = std::stoi(match[2]);
                status.ovr_spindle = std::stoi(match[3]);
            }

            callback_status(status);
            return true;
        }
        return false;
    }

    bool GParser::ParseModalState(std::string &line) {
        if (boost::regex_match(line, boost::regex{"(\\[GC:[\\w\\s]*\\])"})) {
            // program mode is only present if in mode pause or end
            modal.program = GModal::PROGRAM::program_none;

            boost::smatch match;
            // motion mode
            if (boost::regex_search(line, match, boost::regex("((?<=G)[0123](?=[ \\]]))"))) {
                modal.motion = static_cast<GModal::MOTION>(std::stoi(match[1]));
            }

            // wcs mode
            if (boost::regex_search(line, match, boost::regex{"((?<=G)5[4-9](?=[ \\]]))"})) {
                modal.wcs = static_cast<GModal::WCS>(std::stoi(match[1]));
            }

            // plane mode
            if (boost::regex_search(line, match, boost::regex{"((?<=G)1[789](?=[ \\]]))"})) {
                modal.plane = static_cast<GModal::PLANE>(std::stoi(match[1]));
            }

            // unit mode
            if (boost::regex_search(line, match, boost::regex{"((?<=G)2[01](?=[ \\]]))"})) {
                modal.unit = static_cast<GModal::UNIT>(std::stoi(match[1]));
            }

            // distance mode
            if (boost::regex_search(line, match, boost::regex{"((?<=G)9[01](?=[ \\]]))"})) {
                modal.distance = static_cast<GModal::DISTANCE>(std::stoi(match[1]));
            }

            // feed rate mode
            if (boost::regex_search(line, match, boost::regex{"((?<=G)9[34](?=[ \\]]))"})) {
                modal.feed = static_cast<GModal::FEED>(std::stoi(match[1]));
            }

            // program mode
            if (boost::regex_search(line, match, boost::regex{"((?<=M)([012]|30)(?=[ \\]]))"})) {
                modal.program = static_cast<GModal::PROGRAM>(std::stoi(match[1]));
            }

            // spindle mode
            if (boost::regex_search(line, match, boost::regex{"((?<=M)[345](?= ))"})) {
                modal.spindle = static_cast<GModal::SPINDLE>(std::stoi(match[1]));
            }

            // coolant mode
            if (boost::regex_search(line, match, boost::regex{"((?<=M)[789](?=[ \\]]))"})) {
                modal.coolant = static_cast<GModal::COOLANT>(std::stoi(match[1]));
            }

            // tool number
            if (boost::regex_search(line, match, boost::regex{"((?<=T)[0-9]+(?=[ \\]]))"})) {
                modal.tool_number = std::stoi(match[1]);
            }

            // feed rate
            if (boost::regex_search(line, match, boost::regex{"((?<=F)[0-9]+(?=[ \\]]))"})) {
                modal.feed_rate = std::stoi(match[1]);
            }

            // spindle speed
            if (boost::regex_search(line, match, boost::regex{"((?<=S)[0-9]+(?=[ \\]]))"})) {
                modal.spindle_speed = std::stoi(match[1]);
            }

            callback_modal(modal);
            return true;
        }
        return false;
    }

    bool GParser::ParseWcsOffset(std::string &line) {
        boost::smatch match;
        if (boost::regex_search(line, match, boost::regex{"\\[(G5[4-9]):([+\\-]?\\d+.?\\d*),([+\\-]?\\d+.?\\d*),([+\\-]?\\d+.?\\d*)\\]"})) {
            auto w = static_cast<GModal::WCS>(std::stoi(match[1].str().substr(1, 2)));
            status.offset[w][0] = std::stof(match[2]);
            status.offset[w][1] = std::stof(match[3]);
            status.offset[w][2] = std::stof(match[4]);
            return true;
        }
        return false;
    }

    bool GParser::ParseAlarm(std::string &line) {
        boost::smatch match;
        if (boost::regex_match(line, match, boost::regex{"(?<=ALARM:)(\\d+)"})){
            callback_alarm(std::stoi(match[1]), alarm.GetAlarmMessage(std::stoi(match[1])));
            return true;
        }
        return false;
    }

    bool GParser::ParseError(std::string &line) {
        boost::smatch match;
        if (boost::regex_match(line, match, boost::regex{"(?<=error:)(\\d+)"})){
            callback_error(std::stoi(match[1]), error.GetErrorMessage(std::stoi(match[1])));
            return true;
        }
        return false;
    }

    bool GParser::ParseMessage(std::string &line) {
        boost::smatch match;
        if (boost::regex_search(line, match, boost::regex{"\\[(MSG:)(.*)\\]"})) {
            callback_message(match[1]);
            return true;
        }
        return false;
    }
}
