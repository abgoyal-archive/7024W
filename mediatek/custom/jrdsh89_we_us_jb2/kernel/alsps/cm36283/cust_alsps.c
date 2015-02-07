#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 3,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    //.i2c_addr   = {0x0C, 0x48, 0x78, 0x00},

	//.als_level  = { 4, 40,  80,   120,   160, 250,  400, 800, 1200,  1600, 2000, 3000, 5000, 10000, 65535}, 
	//.als_value  = {10, 20, 20, 120, 120, 280,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240}, 
	.als_level  = { 0, 275,  550, 1100, 2750, 3382,  5450, 7000, 9000, 13740, 21000, 37213, 45000, 50000, 65535},
    .als_value  = {5, 10, 20, 40, 50, 70, 150, 210, 290, 500, 900, 1800, 2900, 3900, 7900, 10240},
    .ps_threshold_high = 80,//110,//modify by rzwang
    .ps_threshold_low = 60,//90, //modify by rzwang
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

