#define BOOST_TEST_MODULE gstatus
#include <boost/test/unit_test.hpp>

#include "../inc/gstatus.hpp"

//BOOST_AUTO_TEST_SUITE( GSTATUS )

using namespace grblconnector;

BOOST_AUTO_TEST_CASE(gstatus_parse_modal_state)
{
    GStatus gStatus;

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
    GStatus gStatus;

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