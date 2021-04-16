//
// Created by scottec on 11.04.20.
//

#include "gtransceiver.hpp"

#include <iostream>
#include <functional>
#include <numeric>

namespace grblconnector {

    GTransceiver::GTransceiver(GStatus *gStatus) {
        this->gStatus = gStatus;
        this->io_run = false;
        this->io_clear = false;
    }

    GTransceiver::~GTransceiver() {
        Disconnect();
        delete serial;
    }

    int GTransceiver::Connect(const std::string &device, unsigned int baudrate) {
        try {
//        delete serial;
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
        std::cout << "GTransceiver closed" << std::endl;
    }

    bool GTransceiver::SendCommand(std::string cmd) {
        if (io_run and !io_clear and status != down) {
            std::lock_guard<std::mutex> l(command_buffer_mutex);
            command_buffer.push_back(cmd);
            sigFlag = true;
            return true;
        }
        return false;
    }

    bool GTransceiver::SendCommand(std::list<std::string> cmd_list) {
        if (io_run and !io_clear and status != down) {
            std::lock_guard<std::mutex> l(command_buffer_mutex);
            command_buffer.splice(command_buffer.end(), cmd_list);
            sigFlag = true;
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

    void GTransceiver::IOTransmit() {

        if (serial->errorStatus() || !serial->isOpen()) {
            std::cout << "ERROR: Serial port closed" << std::endl;
            status = down;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        serial->write("\r\n\r\n", 4);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        command_buffer.clear();
        rt_command_buffer.clear();
        cline.clear();
        std::string tosend;
        status = active;

        while (io_run and serial->isOpen() and !serial->errorStatus()) {
            //process realtime command buffer
            if (!rt_command_buffer.empty()) {
                rt_command_buffer_mutex.lock();
                const char rt_cmd = rt_command_buffer.front();
                rt_command_buffer.pop_front();
                rt_command_buffer_mutex.unlock();
                serial->write(&rt_cmd, 1);
            }

            if (tosend.empty() and !command_buffer.empty() and !io_clear) {
                command_buffer_mutex.lock();
                tosend = command_buffer.front();
                command_buffer.pop_front();
                command_buffer_mutex.unlock();
                if (!tosend.empty()) {
                    if (tosend.back() != '\n')
                        tosend += '\n';
                    std::transform(tosend.begin(), tosend.end(), tosend.begin(), ::toupper);
                    std::unique_lock<std::mutex> l(cline_mutex);
                    cline.push_back(static_cast<int>(tosend.length()));
                }
            }

            if (io_clear) {
                tosend.clear();
            }

            // send
            if (!tosend.empty() and !io_clear and ClineLen() < RX_BUFFER_SIZE) {
                serial->write(tosend.c_str(), tosend.length());
                tosend.clear();
            }

            if (command_buffer.empty() and sigFlag) {
                sigFlag = false;
                sig();
            }

            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        status = down;
        std::cout << "SerialWorker stopped" << std::endl;
    }

    int GTransceiver::ClineLen() {
        std::unique_lock<std::mutex> l(cline_mutex);
        return std::accumulate(cline.begin(), cline.end(), 0);
    }

    void GTransceiver::IOReceive(const char *data, unsigned int len) {
        std::vector<char> v(data, data + len);
        for (auto const &c : v) {
            if (c == '\n') {
                switch (gInterpreter.ParseLine(read_buffer)) {
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
}
