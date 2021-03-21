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

#include "AsyncSerial.h"

#include "gstatus.hpp"
#include "gsender.hpp"

class GTransceiver : public GSender {
public:
    enum STATUS {
        up,
        down,
        active
    };

    explicit GTransceiver(GStatus *gStatus);
    ~GTransceiver();

    int Connect(const std::string& device, unsigned int baudrate);
    void Disconnect();

    STATUS GetState() { return this->status; }

    bool SendCommand(std::string cmd) override ;
    bool SendCommand(std::list<std::string> *cmd_list) override ;
    bool SendRealtimeCommand(char cmd) override;
    void ClearBuffer();
    int ClineLen();

protected:

private:
    void IOTransmit();
    void IOReceive(const char *data, unsigned int len);

    void ParseLine(std::string &line);

    const int RX_BUFFER_SIZE = 128;

    STATUS status = down;

    CallbackAsyncSerial *serial{};

    std::mutex command_buffer_mutex;
    std::mutex rt_command_buffer_mutex;
    std::mutex cline_mutex;
    std::list<std::string> command_buffer{};
    std::list<char> rt_command_buffer{};
    std::list<int> cline{};

    std::string read_buffer{};

    std::thread io_thread{};
    std::atomic<bool> io_run{}, io_clear{};

    GStatus *gStatus;
};

#endif // GTRANSCEIVER_HPP
