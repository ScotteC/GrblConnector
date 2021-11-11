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

#ifndef GRBL_GMODAL_H
#define GRBL_GMODAL_H

namespace grblconnector {

    class GModal {
    public:
        enum DISTANCE {
            distance_none = -1,
            distance_absolute = 90,
            distance_incremental = 91
        };

        enum WCS {
            wcs_none = -1,
            mcs = 53,
            wcs_1 = 54,
            wcs_2 = 55,
            wcs_3 = 56,
            wcs_4 = 57,
            wcs_5 = 58,
            wcs_6 = 59
        };

        enum PLANE {
            plane_none = -1,
            plane_xy = 17,
            plane_zx = 18,
            plane_yz = 19
        };

        enum MOTION {
            motion_none = -1,
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
            unit_none = -1,
            unit_inch = 20,
            unit_mm = 21
        };

        enum PROGRAM {
            program_none = -1,
            program_m0 = 0,
            program_m1 = 1,
            program_m2 = 2,
            program_m30 = 30
        };

        enum FEED {
            feed_none = -1,
            feed_inverse_time = 93,
            feed_units_per_minute = 94
        };

        GModal::MOTION motion{GModal::motion_none};
        GModal::WCS wcs{GModal::wcs_none};
        GModal::PLANE plane{GModal::plane_none};
        GModal::UNIT unit{GModal::unit_none};
        GModal::DISTANCE distance{GModal::distance_none};
        GModal::FEED feed{GModal::feed_none};
        GModal::PROGRAM program{GModal::program_none};
        GModal::SPINDLE spindle{GModal::spindle_off};
        GModal::COOLANT coolant{GModal::coolant_off};

        int tool_number = 0;
        int spindle_speed = 0;
        int feed_rate = 0;
    };
}
#endif //GRBL_GMODAL_H
