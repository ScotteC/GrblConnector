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

#define BOOST_TEST_MODULE gstatus
#include <boost/test/unit_test.hpp>

#include "../inc/gparser.hpp"

//BOOST_AUTO_TEST_SUITE( GSTATUS )

using namespace grblconnector;

BOOST_AUTO_TEST_CASE(gstatus_parse_modal_state)
{
    GParser gStatus;

    std::string line = "[GC:G1 G55 G18 G20 G91 G93 M4 M8 T1 F10 S10000]";

    gStatus.ParseLine(line);

    GModal modal = gStatus.GetMachineStatus();

    BOOST_CHECK_EQUAL(modal.motion, GModal::MOTION::motion_linear);
    BOOST_CHECK_EQUAL(modal.wcs, GModal::WCS::wcs_2);
    BOOST_CHECK_EQUAL(modal.plane, GModal::PLANE::plane_zx);
    BOOST_CHECK_EQUAL(modal.unit, GModal::UNIT::unit_inch);
    BOOST_CHECK_EQUAL(modal.distance, GModal::DISTANCE::distance_incremental);
    BOOST_CHECK_EQUAL(modal.feed, GModal::FEED::feed_inverse_time);
    BOOST_CHECK_EQUAL(modal.spindle, GModal::SPINDLE::spindle_ccw);
    BOOST_CHECK_EQUAL(modal.coolant, GModal::COOLANT::coolant_flood);
    BOOST_CHECK_EQUAL(modal.tool_number, 1);
    BOOST_CHECK_EQUAL(modal.feed_rate, 10);
    BOOST_CHECK_EQUAL(modal.spindle_speed, 10000);
}

BOOST_AUTO_TEST_CASE(gstatus_parse_status_report)
{
    GParser gStatus;

    std::string line = "[GC:G1 G55 G18 G20 G91 G93 M4 M8 T1 F10 S10000]";

    gStatus.ParseLine(line);

    GModal modal = gStatus.GetMachineStatus();

    BOOST_CHECK_EQUAL(modal.motion, GModal::MOTION::motion_linear);
    BOOST_CHECK_EQUAL(modal.wcs, GModal::WCS::wcs_2);
    BOOST_CHECK_EQUAL(modal.plane, GModal::PLANE::plane_zx);
    BOOST_CHECK_EQUAL(modal.unit, GModal::UNIT::unit_inch);
    BOOST_CHECK_EQUAL(modal.distance, GModal::DISTANCE::distance_incremental);
    BOOST_CHECK_EQUAL(modal.feed, GModal::FEED::feed_inverse_time);
    BOOST_CHECK_EQUAL(modal.spindle, GModal::SPINDLE::spindle_ccw);
    BOOST_CHECK_EQUAL(modal.coolant, GModal::COOLANT::coolant_flood);
    BOOST_CHECK_EQUAL(modal.tool_number, 1);
    BOOST_CHECK_EQUAL(modal.feed_rate, 10);
    BOOST_CHECK_EQUAL(modal.spindle_speed, 10000);
}



//BOOST_AUTO_TEST_SUITE_END()
