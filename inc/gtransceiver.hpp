//
// Created by scottec on 11.04.20.
//

#ifndef GRBL_GTRANSCEIVER_HPP
#define GRBL_GTRANSCEIVER_HPP

#include <list>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include "AsyncSerial.hpp"
#include "gsender.hpp"
#include "gparser.hpp"

namespace grblconnector {

    class GTransceiver : public GSender {
    public:
        enum STATUS {
            up,
            down,
            active
        };

        GTransceiver();

        ~GTransceiver();

        int Connect(const std::string &device, unsigned int baudrate);

        void Disconnect();

        void BindBufferEmptyCallback(const std::function<void (void)> &callback) {
            callback_command_buffer_empty = callback;
        }

        STATUS GetStatus() { return this->status; }

        bool SendCommand(std::string cmd) override;

        bool SendCommand(std::list<std::string> cmd_list) override;

        bool SendRealtimeCommand(char cmd) override;

        void ClearBuffer();

        int ClineLen();

        const GParser& GetInterpreter(){
            return gInterpreter;
        }

    protected:

    private:
        void IOTransmit();

        void IOReceive(const char *data, unsigned int len);

        const int RX_BUFFER_SIZE = 128;

        STATUS status = down;

        CallbackAsyncSerial *serial{};

        std::mutex command_buffer_mutex{};
        std::mutex rt_command_buffer_mutex{};
        std::mutex cline_mutex{};
        std::list<std::string> command_buffer{};
        std::list<char> rt_command_buffer{};
        std::list<int> cline{};

        std::string read_buffer{};

        std::thread io_thread{}, status_thread{};
        std::atomic<bool> io_run{}, io_clear{};

        GParser gInterpreter{};

        std::function<void (void)> callback_command_buffer_empty = {};
        bool command_buffer_empty_call_flag = true;
    };
}
#endif // GRBL_GTRANSCEIVER_HPP
