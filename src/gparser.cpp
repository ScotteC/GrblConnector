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
        if (boost::regex_match(line, boost::regex{R"(Grbl.*)"}))
            return 2;

        if (boost::regex_match(line, boost::regex{R"(ok[\r\n]*)"}))
            return 1;

        if (ParseAlarm(line))
            return 1;

        if (ParseError(line))
            return 1;

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
        if (boost::regex_match(line, boost::regex{R"(<.*>[\r\n]*)"})) {
            boost::smatch match;

            // machine status
            if (boost::regex_search(line, match, boost::regex{R"(((?<=\<)\w+:?[0-3]?(?=\|)))"})) {
                status.state = status.str_to_state[match[1]];
            }

            // machine position
            if (boost::regex_search(line, match,boost::regex{R"((?<=\|)MPos:([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*)(?=[\|\>]))"})) {
                status.position[GModal::WCS::mcs][0] = std::stof(match[1]);
                status.position[GModal::WCS::mcs][1] = std::stof(match[2]);
                status.position[GModal::WCS::mcs][2] = std::stof(match[3]);
                status.position[modal.wcs][0] = status.position[GModal::WCS::mcs][0] - status.offset[modal.wcs][0];
                status.position[modal.wcs][1] = status.position[GModal::WCS::mcs][1] - status.offset[modal.wcs][1];
                status.position[modal.wcs][2] = status.position[GModal::WCS::mcs][2] - status.offset[modal.wcs][2];
            }

            // working position
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)WPos:([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*)(?=[\|\>]))"})) {
                status.position[modal.wcs][0] = std::stof(match[1]);
                status.position[modal.wcs][1] = std::stof(match[2]);
                status.position[modal.wcs][2] = std::stof(match[3]);
                status.position[GModal::WCS::mcs][0] = status.position[modal.wcs][0] + status.offset[modal.wcs][0];
                status.position[GModal::WCS::mcs][1] = status.position[modal.wcs][1] + status.offset[modal.wcs][1];
                status.position[GModal::WCS::mcs][2] = status.position[modal.wcs][2] + status.offset[modal.wcs][2];
            }

            // feed rate and spindle speed
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)FS:([+\-]?\d+),([+\-]?\d+)(?=[\|\>]))"})) {
                status.feed_rate = std::stoi(match[1]);
                status.spindle_speed = std::stoi(match[2]);
            }

            // only feed rate
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)F:([+\-]?\d+)(?=[\|\>]))"})) {
                status.feed_rate = std::stoi(match[1]);
            }

            // limits and flags
            if (boost::regex_search(line, match, boost::regex{
                    R"((?<=\|)Pn:([XYZPDHRS])([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)([XYZPDHRS]?)(?=[\|\>]))"})) {
                // ToDo
            }

            // working coordinates offsets
            if (boost::regex_search(line, match, boost::regex{
                    R"((?<=\|)WCO:([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*),([+\-]?\d+\.?\d*)(?=[\|\>]))"})) {
                // ToDo
            }

            // buffer status
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)Bf:(\d+),(\d+)(?=[\|\>]))"})) {
                status.buffer_planner = std::stoi(match[1]);
                status.buffer_rx = std::stoi(match[2]);
            }

            // line number
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)Ln:(\d+)(?=[\|\>]))"})) {
                status.line_number = std::stoi(match[1]);
            }

            // override rates
            if (boost::regex_search(line, match, boost::regex{R"((?<=\|)Ov:(\d+),(\d+),(\d+)(?=[\|\>]))"})) {
                status.ovr_feed = std::stoi(match[1]);
                status.ovr_rapid = std::stoi(match[2]);
                status.ovr_spindle = std::stoi(match[3]);
            }

            if (callback_status != nullptr)
                callback_status(status);
            return true;
        }
        return false;
    }

    bool GParser::ParseModalState(std::string &line) {
        if (boost::regex_match(line, boost::regex{R"((\[GC:.*\][\r\n]*))"})) {
            // program mode is only present if in mode pause or end
            modal.program = GModal::PROGRAM::program_none;

            boost::smatch match;
            // motion mode
            if (boost::regex_search(line, match, boost::regex(R"(((?<=G)[0123](?=[ \]])))"))) {
                modal.motion = static_cast<GModal::MOTION>(std::stoi(match[1]));
            }

            // wcs mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=G)5[4-9](?=[ \]])))"})) {
                modal.wcs = static_cast<GModal::WCS>(std::stoi(match[1]));
            }

            // plane mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=G)1[789](?=[ \]])))"})) {
                modal.plane = static_cast<GModal::PLANE>(std::stoi(match[1]));
            }

            // unit mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=G)2[01](?=[ \]])))"})) {
                modal.unit = static_cast<GModal::UNIT>(std::stoi(match[1]));
            }

            // distance mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=G)9[01](?=[ \]])))"})) {
                modal.distance = static_cast<GModal::DISTANCE>(std::stoi(match[1]));
            }

            // feed rate mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=G)9[34](?=[ \]])))"})) {
                modal.feed = static_cast<GModal::FEED>(std::stoi(match[1]));
            }

            // program mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=M)([012]|30)(?=[ \]])))"})) {
                modal.program = static_cast<GModal::PROGRAM>(std::stoi(match[1]));
            }

            // spindle mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=M)[345](?=[ \])))"})) {
                modal.spindle = static_cast<GModal::SPINDLE>(std::stoi(match[1]));
            }

            // coolant mode
            if (boost::regex_search(line, match, boost::regex{R"(((?<=M)[789](?=[ \]])))"})) {
                modal.coolant = static_cast<GModal::COOLANT>(std::stoi(match[1]));
            }

            // tool number
            if (boost::regex_search(line, match, boost::regex{R"(((?<=T)[0-9]+(?=[ \]])))"})) {
                modal.tool_number = std::stoi(match[1]);
            }

            // feed rate
            if (boost::regex_search(line, match, boost::regex{R"(((?<=F)(\d+\.?\d*)(?=[ \]])))"})) {
                modal.feed_rate = std::stoi(match[1]);
            }

            // spindle speed
            if (boost::regex_search(line, match, boost::regex{R"(((?<=S)(\d+\.?\d*)(?=[ \]])))"})) {
                modal.spindle_speed = std::stoi(match[1]);
            }

            if (callback_modal != nullptr)
                callback_modal(modal);

            return true;
        }
        return false;
    }

    bool GParser::ParseWcsOffset(std::string &line) {
        boost::smatch match;
        if (boost::regex_search(line, match, boost::regex{R"(\[(G5[4-9]):([+\-]?\d+.?\d*),([+\-]?\d+.?\d*),([+\-]?\d+.?\d*)\])"})) {
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
        if (boost::regex_search(line, match, boost::regex{R"((?<=ALARM:)(\d+))"})){
            if (callback_alarm != nullptr)
                callback_alarm(std::stoi(match[1]), alarm.GetAlarmMessage(std::stoi(match[1])));
            return true;
        }
        return false;
    }

    bool GParser::ParseError(std::string &line) {
        boost::smatch match;
        if (boost::regex_search(line, match, boost::regex{R"((?<=error:)(\d+))"})){
            if (callback_error != nullptr)
                callback_error(std::stoi(match[1]), error.GetErrorMessage(std::stoi(match[1])));
            return true;
        }
        return false;
    }

    bool GParser::ParseMessage(std::string &line) {
        boost::smatch match;
        if (boost::regex_search(line, match, boost::regex{R"(\[(MSG:)(.*)\])"})) {
            if (callback_message != nullptr)
                callback_message(match[2]);
            return true;
        }
        return false;
    }
}
