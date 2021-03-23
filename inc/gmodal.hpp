//
// Created by scottec on 14.03.21.
//

#ifndef GRBL_GMODAL_H
#define GRBL_GMODAL_H

namespace grblconnector {

    class GModal {
    public:
        enum DISTANCE {
            distance_none,
            distance_absolute = 90,
            distance_incremental = 91
        };

        enum WCS {
            wcs_none,
            mcs = 53,
            wcs_1 = 54,
            wcs_2 = 55,
            wcs_3 = 56,
            wcs_4 = 57,
            wcs_5 = 58,
            wcs_6 = 59
        };

        enum PLANE {
            plane_none,
            plane_xy = 17,
            plane_zx = 18,
            plane_yz = 19
        };

        enum MOTION {
            motion_none,
            motion_rapid = 0,
            motion_linear = 1,
            motion_arc_cw = 2,
            motion_arc_ccw = 3
        };

        enum SPINDLE {
            spindle_off = 5,
            spindle_cw = 3,
            spindle_ccw = 4
        };

        enum COOLANT {
            coolant_off = 9,
            coolant_mist = 7,
            coolant_flood = 8
        };

        enum UNIT {
            unit_inch = 20,
            unit_mm = 21
        };

        enum PROGRAM {
            program_none,
            program_m0 = 0,
            program_m1 = 1,
            program_m2 = 2,
            program_m30 = 30
        };

        enum FEED {
            feed_inverse_time = 93,
            feed_units_per_minute = 94
        };

        GModal::MOTION motion{GModal::motion_none};
        GModal::WCS wcs{GModal::wcs_1};
        GModal::PLANE plane{GModal::plane_xy};
        GModal::UNIT unit{GModal::unit_mm};
        GModal::DISTANCE distance{GModal::distance_absolute};
        GModal::FEED feed{GModal::feed_units_per_minute};
        GModal::PROGRAM program{GModal::program_none};
        GModal::SPINDLE spindle{GModal::spindle_off};
        GModal::COOLANT coolant{GModal::coolant_off};

        int tool_number = 0;
        int spindle_speed = 0;
        int feed_rate = 0;
    };
}
#endif //GRBL_GMODAL_H
