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

#include "gtransceiver.hpp"

#include <iostream>
#include <functional>
#include <numeric>

#include "asyncserial/AsyncSerial.hpp"

namespace grblconnector {

    GTransceiver::GTransceiver(GParser& parser) : gParser(parser) {
        this->io_run = false;
        this->io_clear = false;
    }

    GTransceiver::~GTransceiver() {
        Disconnect();
        delete serial;
    }

    int GTransceiver::Connect(const std::string &device, unsigned int baudrate) {
        try {
            delete serial;
            this->serial = new CallbackAsyncSerial(device, baudrate);
            this->serial->flush();
            this->serial->setCallback([this](auto &&PH1, auto &&PH2) {
                IOReceive(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
            });
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        status = up;
        io_run = true;
        io_thread = std::thread(&GTransceiver::IOTransmit, this);
        if (callback_status_changed != nullptr)
            callback_status_changed(status);
        return 0;
    }

    void GTransceiver::Disconnect() {
        io_run = false;
        if (io_thread.joinable()) {
            std::cout << "Wait for SerialWorker..." << std::endl;
            io_thread.join();
        }
        if (serial != nullptr)
            serial->close();
        status = down;
        if (callback_status_changed != nullptr)
            callback_status_changed(status);
        std::cout << "GTransceiver closed" << std::endl;
    int GTransceiver::SetStatusReportFrequency(float frequency) {
        if (frequency > 5.0 or frequency < 0)
            return 2;

        // update frequency
        status_frequency_ = frequency;

        // stop worker
        if (frequency == 0){
            io_status = false;
            return 0;
        }

        io_status = true;

        // worker is running, nothing to do
        if (status_thread.joinable()) {
            return 0;
        }

        // start worker
        status_thread = std::thread(&GTransceiver::IOStatus, this);
        return 0;
    }

    bool GTransceiver::SendCommand(std::string cmd) {
        if (io_run and !io_clear and status != down) {
            std::lock_guard<std::mutex> l(command_buffer_mutex);
            command_buffer.push_back(cmd);
            command_buffer_empty_call_flag = true;
            return true;
        }
        return false;
    }

    bool GTransceiver::SendCommand(std::list<std::string> cmd_list) {
        if (io_run and !io_clear and status != down) {
            std::lock_guard<std::mutex> l(command_buffer_mutex);
            command_buffer.splice(command_buffer.end(), cmd_list);
            command_buffer_empty_call_flag = true;
            return true;
        }
        return false;
    }

    bool GTransceiver::SendRealtimeCommand(const char cmd) {
        if (io_run and status != down) {
            std::lock_guard<std::mutex> l(rt_command_buffer_mutex);
            rt_command_buffer.push_back(cmd);
            return true;
        }
        return false;
    }

    void GTransceiver::ClearBuffer() {
        io_clear = true;
        std::unique_lock<std::mutex> l1(cline_mutex);
        std::unique_lock<std::mutex> l2(command_buffer_mutex);
        cline.clear();
        command_buffer.clear();
        io_clear = false;
    }

    int GTransceiver::ClineLen() {
        std::unique_lock<std::mutex> l(cline_mutex);
        return std::accumulate(cline.begin(), cline.end(), 0);
    }

    void GTransceiver::IOTransmit() {
        if (serial == nullptr or serial->errorStatus() or !serial->isOpen()) {
            std::cout << "ERROR: Serial port closed" << std::endl;
            status = down;
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        serial->write("\r\n\r\n", 4);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        command_buffer.clear();
        rt_command_buffer.clear();
        cline.clear();
        std::string to_send;
        status = active;
        if (callback_status_changed != nullptr)
            callback_status_changed(status);

        // Get GCodeState
        serial->write("$G\n", 3);

        while (io_run and serial != nullptr and serial->isOpen() and !serial->errorStatus()) {
            //process realtime command buffer
            if (!rt_command_buffer.empty()) {
                rt_command_buffer_mutex.lock();
                const char rt_cmd = rt_command_buffer.front();
                rt_command_buffer.pop_front();
                rt_command_buffer_mutex.unlock();
                serial->write(&rt_cmd, 1);
            }

            if (to_send.empty() and !command_buffer.empty() and !io_clear) {
                command_buffer_mutex.lock();
                to_send = command_buffer.front();
                command_buffer.pop_front();
                command_buffer_mutex.unlock();
                if (!to_send.empty()) {
                    if (to_send.back() != '\n')
                        to_send += '\n';
                    std::transform(to_send.begin(), to_send.end(), to_send.begin(), ::toupper);
                    std::unique_lock<std::mutex> l(cline_mutex);
                    cline.push_back(static_cast<int>(to_send.length()));
                }
            }

            if (io_clear) {
                to_send.clear();
            }

            // send
            if (!to_send.empty() and !io_clear and ClineLen() < RX_BUFFER_SIZE) {
                serial->write(to_send.c_str(), to_send.length());
                to_send.clear();
            }

            if (callback_command_buffer_empty != nullptr
                    and command_buffer.empty() and command_buffer_empty_call_flag) {
                command_buffer_empty_call_flag = false;
                callback_command_buffer_empty();
            }

            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        io_run = false;
        status = down;
        if (callback_status_changed != nullptr)
            callback_status_changed(status);
        std::cout << "SerialWorker stopped" << std::endl;
    }

    void GTransceiver::IOReceive(const char *data, unsigned int len) {
        std::vector<char> v(data, data + len);
        for (auto const &c : v) {
            if (c == '\n') {
                switch (gParser.ParseLine(read_buffer)) {
                    case 1:
                        if (!cline.empty()) {
                            std::unique_lock<std::mutex> l(cline_mutex);
                            cline.pop_front();
                        }
                        break;

                    case 2:
                        cline.clear();
                        break;

                    default:
                        break;
                }
                read_buffer.clear();
            } else {
                read_buffer += c;
            }
        }
    }

    void GTransceiver::IOStatus() {
        auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto wait_until = std::chrono::system_clock::time_point(start) + std::chrono::milliseconds(500);

        while(io_status) {
            std::this_thread::sleep_until(wait_until);
            wait_until += std::chrono::milliseconds((int)(1000 / status_frequency_));
            SendRealtimeCommand(static_cast<const char>(0x3f));
        }
    }
}
