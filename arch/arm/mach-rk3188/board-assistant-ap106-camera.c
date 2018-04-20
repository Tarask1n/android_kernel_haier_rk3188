#ifdef CONFIG_VIDEO_RK29
#include <plat/rk_camera.h>
/* Notes:

Simple camera device registration:

       new_camera_device(sensor_name,\       // sensor name, it is equal to CONFIG_SENSOR_X
                          face,\              // sensor face information, it can be back or front
                          pwdn_io,\           // power down gpio configuration, it is equal to CONFIG_SENSOR_POWERDN_PIN_XX
                          flash_attach,\      // sensor is attach flash or not
                          mir,\               // sensor image mirror and flip control information
                          i2c_chl,\           // i2c channel which the sensor attached in hardware, it is equal to CONFIG_SENSOR_IIC_ADAPTER_ID_X
                          cif_chl)  \         // cif channel which the sensor attached in hardware, it is equal to CONFIG_SENSOR_CIF_INDEX_X

Comprehensive camera device registration:

      new_camera_device_ex(sensor_name,\
                             face,\
                             ori,\            // sensor orientation, it is equal to CONFIG_SENSOR_ORIENTATION_X
                             pwr_io,\         // sensor power gpio configuration, it is equal to CONFIG_SENSOR_POWER_PIN_XX
                             pwr_active,\     // sensor power active level, is equal to CONFIG_SENSOR_RESETACTIVE_LEVEL_X
                             rst_io,\         // sensor reset gpio configuration, it is equal to CONFIG_SENSOR_RESET_PIN_XX
                             rst_active,\     // sensor reset active level, is equal to CONFIG_SENSOR_RESETACTIVE_LEVEL_X
                             pwdn_io,\
                             pwdn_active,\    // sensor power down active level, is equal to CONFIG_SENSOR_POWERDNACTIVE_LEVEL_X
                             flash_attach,\
                             res,\            // sensor resolution, this is real resolution or resoltuion after interpolate
                             mir,\
                             i2c_chl,\
                             i2c_spd,\        // i2c speed , 100000 = 100KHz
                             i2c_addr,\       // the i2c slave device address for sensor
                             cif_chl,\
                             mclk)\           // sensor input clock rate, 24 or 48
                          
*/
static struct rkcamera_platform_data new_camera[] = {
    
    new_camera_device_ex(RK29_CAM_SENSOR_GC2035,
                        back,
                        INVALID_VALUE,
                        INVALID_VALUE,
                        INVALID_VALUE,
                        INVALID_VALUE,
                        INVALID_VALUE,
                        RK30_PIN3_PB5,
                        CONS(RK29_CAM_SENSOR_GC2035, _PWRDN_ACTIVE),
                        0,
                        CONS(RK29_CAM_SENSOR_GC2035, _FULL_RESOLUTION),
                        0x00,
                        3,
                        100000,
                        CONS(RK29_CAM_SENSOR_GC2035, _I2C_ADDR),
                        2,   // 0 - i2c i2c-3: No ack, Maybe slave(addr: 0x3c) not exist or abnormal power-on
                        24),                        
    new_camera_device(RK29_CAM_SENSOR_SP2518,
                        front,
                        RK30_PIN3_PB4,
                        0,
                        0,
                        3,
                        0), 
    new_camera_device_end
};

#endif  //#ifdef CONFIG_VIDEO_RK29
/*---------------- Camera Sensor Configuration Macro End------------------------*/
#include "../../../drivers/media/video/rk30_camera.c"
/*---------------- Camera Sensor Macro Define End  ---------*/

#define PMEM_CAM_SIZE PMEM_CAM_NECESSARY
/*****************************************************************************************
 * camera  devices
 * author: ddl@rock-chips.com
 *****************************************************************************************/
#ifdef CONFIG_VIDEO_RK29
#define CONFIG_SENSOR_POWER_IOCTL_USR	   1 //define this refer to your board layout
#define CONFIG_SENSOR_RESET_IOCTL_USR	   0
#define CONFIG_SENSOR_POWERDOWN_IOCTL_USR  0
#define CONFIG_SENSOR_FLASH_IOCTL_USR	   0 //1

static void rk_cif_power(int on)
{
    struct regulator *reg;
	reg = regulator_get(NULL, "act_ldo3");

	if (reg == NULL || IS_ERR(reg)){
        printk("get cif ldo failed!\n");
		return;
	    }
    if(on == 0){
		while(regulator_is_enabled(reg)>0)
    	regulator_disable(reg);
    	regulator_put(reg);
    	mdelay(50);
        }
    else{
    	regulator_set_voltage(reg, 1800000, 1800000);
    	regulator_enable(reg);
    	regulator_put(reg);
		mdelay(50);
        }
}

#if CONFIG_SENSOR_POWER_IOCTL_USR
static int sensor_power_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	rk_cif_power(on);
	return 0;
}
#endif

#if CONFIG_SENSOR_RESET_IOCTL_USR
static int sensor_reset_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_RESET_IOCTL_USR is 1, sensor_reset_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
static int sensor_powerdown_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_POWERDOWN_IOCTL_USR is 1, sensor_powerdown_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_FLASH_IOCTL_USR

#if defined(CONFIG_PIPO_M9MAX) || defined(CONFIG_PIPO_M6PRO)//d33  
	#define CONFIG_SENSOR_FLASH_EN_PIN_0	RK30_PIN0_PD5
	#define CONFIG_SENSOR_FLASH_EN_MUX_0	GPIO0D5_SPI1TXD_NAME
	#define CONFIG_SENSOR_FLASH_MODE_PIN_0	RK30_PIN0_PD4 //high:FLASH, low:torch
	#define CONFIG_SENSOR_FLASH_MODE_MUX_0	GPIO0D4_SPI1RXD_NAME

#else 
	#if defined(CONFIG_PIPO_M7PRO)
		#define CONFIG_SENSOR_FLASH_EN_PIN_0		  RK30_PIN0_PC7   //high:enable
		#define CONFIG_SENSOR_FLASH_EN_MUX_0		  GPIO0D5_SPI1TXD_NAME
		#define CONFIG_SENSOR_FLASH_MODE_PIN_0		  RK30_PIN0_PD4   //high:FLASH, low:torch
		#define CONFIG_SENSOR_FLASH_MODE_MUX_0		  GPIO0D4_SPI1RXD_NAME
	#else

			#define CONFIG_SENSOR_FLASH_EN_PIN_0	RK30_PIN1_PB6 //orig. RK30_PIN0_PC3
			#define CONFIG_SENSOR_FLASH_EN_MUX_0	GPIO1B6_SPDIFTX_SPI1CSN1_NAME
			#define CONFIG_SENSOR_FLASH_MODE_PIN_0	RK30_PIN3_PD5 //high:FLASH, low:torch
			#define CONFIG_SENSOR_FLASH_MODE_MUX_0	GPIO3D5_SPI1RXD_NAME //GPIO3D5_PWM2_JTAGTCK_OTGDRVVBUS_NAME
	#endif
#endif

static int sensor_init_flags = 0;
static int sensor_flash_usr_cb (struct rk29camera_gpio_res *res,int on)
{
        int ret;
	if(sensor_init_flags == 0){
                rk30_mux_api_set(CONFIG_SENSOR_FLASH_MODE_MUX_0, 0);
                ret = gpio_request(CONFIG_SENSOR_FLASH_MODE_PIN_0, "camera_flash_mode");
                if (ret != 0) {
                    printk(">>>>gpio request camera_flash_mode failed !!\n");
                }
		printk(">>>>gpio request camera_flash_mode success\n");
                
                gpio_direction_output(CONFIG_SENSOR_FLASH_MODE_PIN_0, 0);
                sensor_init_flags = 1 ;
        }
        switch (on) {
		case Flash_Off: {
			gpio_set_value(CONFIG_SENSOR_FLASH_EN_PIN_0, 0);
			gpio_set_value(CONFIG_SENSOR_FLASH_MODE_PIN_0, 0);
			break;
		}

		case Flash_On: {
			gpio_set_value(CONFIG_SENSOR_FLASH_EN_PIN_0, 1);
			gpio_set_value(CONFIG_SENSOR_FLASH_MODE_PIN_0, 1);
			break;
		}

		case Flash_Torch: {
			gpio_set_value(CONFIG_SENSOR_FLASH_EN_PIN_0, 1);
			gpio_set_value(CONFIG_SENSOR_FLASH_MODE_PIN_0, 0);
			break;
		}

		default: {
			printk("%s..Flash command(%d) is invalidate \n",__FUNCTION__, on);
			gpio_set_value(CONFIG_SENSOR_FLASH_EN_PIN_0, 0);
			break;
		}
	}
	return 0;
}
#endif

static struct rk29camera_platform_ioctl_cb	sensor_ioctl_cb = {
	#if CONFIG_SENSOR_POWER_IOCTL_USR
	.sensor_power_cb = sensor_power_usr_cb,
	#else
	.sensor_power_cb = NULL,
	#endif

	#if CONFIG_SENSOR_RESET_IOCTL_USR
	.sensor_reset_cb = sensor_reset_usr_cb,
	#else
	.sensor_reset_cb = NULL,
	#endif

	#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
	.sensor_powerdown_cb = sensor_powerdown_usr_cb,
	#else
	.sensor_powerdown_cb = NULL,
	#endif

	#if CONFIG_SENSOR_FLASH_IOCTL_USR
	.sensor_flash_cb = sensor_flash_usr_cb,
	#else
	.sensor_flash_cb = NULL,
	#endif
};

static rk_sensor_user_init_data_s rk_init_data_sensor[RK_CAM_NUM] ;

#include "../../../drivers/media/video/rk30_camera.c"

#endif /* CONFIG_VIDEO_RK29 */
