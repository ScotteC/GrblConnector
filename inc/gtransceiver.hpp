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

#ifndef GRBL_GTRANSCEIVER_HPP
#define GRBL_GTRANSCEIVER_HPP

#include <list>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include "gsender.hpp"
#include "gparser.hpp"

class CallbackAsyncSerial;

namespace grblconnector {

    class GTransceiver : public GSender {
    public:
        enum STATUS {
            up,
            down,
            active
        };

        GTransceiver(GTransceiver&) = delete;
        GTransceiver& operator=(GTransceiver&) = delete;

        void BindStatusChangedCallback(const std::function<void(STATUS)> &callback) {
            callback_status_changed = callback;
        }

        void BindBufferEmptyCallback(const std::function<void()> &callback) {
            callback_command_buffer_empty = callback;
        }

    private:
        friend class GrblConnector;
        friend class GCommand;

        explicit GTransceiver(GParser& parser);

        ~GTransceiver();

        int Connect(const std::string &device, unsigned int baudrate);

        void Disconnect();

        bool SendCommand(std::string cmd) override;

        bool SendCommand(std::list<std::string> cmd_list) override;

        bool SendRealtimeCommand(char cmd) override;

        void ClearBuffer();

        int ClineLen();

        void IOTransmit();

        void IOReceive(const char *data, unsigned int len);

        const int RX_BUFFER_SIZE = 128;

        STATUS status = down;

        GParser& gParser;
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

        std::function<void()> callback_command_buffer_empty{};
        std::function<void(STATUS)> callback_status_changed{};

        bool command_buffer_empty_call_flag = true;
    };
}
#endif // GRBL_GTRANSCEIVER_HPP
