//
// Created by scottec on 12.04.20.
//

#ifndef GRBL_INTERPRETER_HPP
#define GRBL_INTERPRETER_HPP

#include "gmodal.hpp"
#include "gstatus.hpp"
#include "gerror.hpp"
#include "galarm.hpp"

#include <list>
#include <map>
#include <string>
#include <functional>

namespace grblconnector {

    class GParser {
    public:
        GParser();

        void BindStatusCallback(std::function<void (GStatus)> &callback) {
            callback_status = callback;
        }

        void BindModalCallback(std::function<void (GModal)> &callback) {
            callback_modal = callback;
        }

        void BindErrorCallback(std::function<void (int, std::string)> &callback) {
            callback_error = callback;
        }

        void BindAlarmCallback(std::function<void (int, std::string)> &callback) {
            callback_alarm = callback;
        }

        void BindMessageCallback(std::function<void (std::string)> &callback) {
            callback_message = callback;
        }

        int ParseLine(std::string &line);

        bool ParseStatusReport(std::string &line);

        bool ParseModalState(std::string &line);

        bool ParseWcsOffset(std::string &line);

        bool ParseAlarm(std::string &line);

        bool ParseError(std::string &line);

        bool ParseMessage(std::string &line);

        GStatus GetMachineStatus() {
            return status;
        }
        GModal GetMachineModal() {
            return modal;
        }

        GStatus::STATE GetState() {
            return status.state;
        }

    private:
        GStatus status{};
        GModal modal{};
        GError error{};
        GAlarm alarm{};

        std::function<void (GStatus status)> callback_status = {};
        std::function<void (GModal modal)> callback_modal = {};
        std::function<void (int error_code, std::string error_message)> callback_error = {};
        std::function<void (int alarm_code, std::string alarm_message)> callback_alarm = {};
        std::function<void (std::string message)> callback_message = {};
    };
}
#endif // GINTERPRETER_HPP
