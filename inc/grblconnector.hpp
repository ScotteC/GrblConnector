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

#ifndef GRBL_CONTROLLER_HPP
#define GRBL_CONTROLLER_HPP

#include <map>
#include <list>

#include "gprogram.hpp"
#include "gparser.hpp"
#include "gcommand.hpp"
#include "gtransceiver.hpp"
#include "greference.hpp"

namespace grblconnector {
    class GrblConnector {
    public:
        GrblConnector();

        ~GrblConnector();

        bool Connect(const std::string &device, int baudrate);

        void Disconnect();

        int StartProgram(GProgram& program, bool repeated);

        int PauseProgram();

        int StopProgram();

    public:
        GStatus status{};
        GReference reference{};
        GModal modal{};
        GError error{};
        GAlarm alarm{};

        GParser gParser{status, modal, error, alarm, reference};
        GTransceiver gTransceiver{gParser};
        GCommand gCommand{gTransceiver, reference};
    };
}

#endif //GRBL_CONTROLLER_HPP
