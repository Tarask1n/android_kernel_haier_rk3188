#ifndef __LCD_HAIER_1043__
#define __LCD_HAIER_1043__

#if defined(CONFIG_MIPI_DSI)
#include "../transmitter/mipi_dsi.h"
#endif

#if  defined(CONFIG_RK610_LVDS) || defined(CONFIG_RK616_LVDS)
#define SCREEN_TYPE	    	SCREEN_MIPI
#else
#define SCREEN_TYPE	    	SCREEN_RGB
#endif
#define LVDS_FORMAT         	0     //mipi lcd don't need it, so 0 would be ok.
#define OUT_FACE	    	OUT_P888  //OUT_P888


#define DCLK	          	164000000 //148500000 //150*1000000
#define LCDC_ACLK         	300000000 //600000000           //29 lcdc axi DMA ?¦Ì?¨º

/* Timing */
#define H_PW			11 //20
#define H_BP			11 //32
#define H_VD			1920
#define H_FP			446 //112//32//112

#define V_PW			5 //2
#define V_BP			5 //16//6//16
#define V_VD			1200
#define V_FP			10 //7

#define LCD_WIDTH       	218//203    //uint mm the lenth of lcd active area
#define LCD_HEIGHT      	136

/* Other */
#if defined(CONFIG_RK610_LVDS) || defined(CONFIG_RK616_LVDS) || defined(CONFIG_MIPI_DSI)
#define DCLK_POL		1
#else
#define DCLK_POL		0
#endif
#define DEN_POL			0
#define VSYNC_POL		0
#define HSYNC_POL		0

#define SWAP_RB			0
#define SWAP_RG			0
#define SWAP_GB			0

#define MIPI_DSI_LANE		4
#define MIPI_DSI_HS_CLK 	1000000000

#define mipi_dsi_init(data) 				dsi_set_regs(data, ARRAY_SIZE(data))
#define mipi_dsi_send_dcs_packet(data) 		dsi_send_dcs_packet(data, ARRAY_SIZE(data))
#define mipi_dsi_post_init(data)			dsi_set_regs(data, ARRAY_SIZE(data))

//int lcd_init(void);
//int lcd_standby(u8 enable);
int lcd_io_init(void){
	return 0;
};

#endif
