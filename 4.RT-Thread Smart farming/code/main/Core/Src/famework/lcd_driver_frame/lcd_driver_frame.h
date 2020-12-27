#ifndef __LCD_DRIVER_FRAME_H
#define __LCD_DRIVER_FRAME_H
#include <stdint.h>
#include <assert.h>

#include "lcd_driver.h"


typedef struct
{
    LCD_Driver *lcd_driver ;
} LCD_Driver_Model;

void Register_Driver_Model(LCD_Driver_Model *lcd_driver_model);

#endif //__LCD_DRIVER_FRAME_H

