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

#ifndef GRBL_INTERPRETER_HPP
#define GRBL_INTERPRETER_HPP

#include "gmodal.hpp"
#include "gstatus.hpp"
#include "gerror.hpp"
#include "galarm.hpp"
#include "eventpp/include/eventpp/callbacklist.h"

#include <list>
#include <map>
#include <string>
#include <functional>

namespace grblconnector {

    class GParser {
    public:
        GParser(GParser&) = delete;
        GParser& operator=(GParser&) = delete;

        void BindStatusCallback(const std::function<void(GStatus)> &callback) {
            callback_status.append(callback);
        }

        void BindModalCallback(const std::function<void(GModal)> &callback) {
            callback_modal.append(callback);
        }

        void BindErrorCallback(const std::function<void(int, std::string)> &callback) {
            callback_error.append(callback);
        }

        void BindAlarmCallback(const std::function<void(int, std::string)> &callback) {
            callback_alarm.append(callback);
        }

        void BindMessageCallback(const std::function<void(std::string)> &callback) {
            callback_message.append(callback);
        }

    private:
        friend class GrblConnector;
        friend class GTransceiver;

        GParser(GStatus &status, GModal &modal, GError &error, GAlarm &alarm)
        : status(status), modal(modal), error(error), alarm(alarm) {}

        int ParseLine(std::string &line);

        bool ParseStatusReport(std::string &line);

        bool ParseModalState(std::string &line);

        bool ParseWcsOffset(std::string &line);

        bool ParseAlarm(std::string &line);

        bool ParseError(std::string &line);

        bool ParseMessage(std::string &line);

    private:
        GStatus &status;
        GModal &modal;
        GError &error;
        GAlarm &alarm;

        eventpp::CallbackList<void(GStatus)> callback_status;
        eventpp::CallbackList<void(GModal)> callback_modal;
        eventpp::CallbackList<void(int, std::string)> callback_error;
        eventpp::CallbackList<void(int, std::string)> callback_alarm;
        eventpp::CallbackList<void(std::string)> callback_message;
    };
}
#endif // GRBL_INTERPRETER_HPP
