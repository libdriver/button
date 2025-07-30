/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      raspberrypi4b_driver_button_interface.c
 * @brief     raspberrypi4b driver button interface source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_button_interface.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>

/**
 * @brief     interface timestamp read
 * @param[in] *t pointer to a button_time structure
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t button_interface_timestamp_read(button_time_t *t)
{
    struct timeval time_s;
    
    if (gettimeofday(&time_s, NULL) < 0)
    {
        return 1;
    }

    t->s = time_s.tv_sec;
    t->us = time_s.tv_usec;
    
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void button_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void button_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    (void)printf((uint8_t *)str);
}

/**
 * @brief     interface receive callback
 * @param[in] *data pointer to an button_t structure
 * @note      none
 */
void button_interface_receive_callback(button_t *data)
{
    switch (data->status)
    {
        case BUTTON_STATUS_PRESS :
        {
            break;
        }
        case BUTTON_STATUS_RELEASE :
        {
            break;
        }
        case BUTTON_STATUS_SINGLE_CLICK :
        {
            button_interface_debug_print("button: irq single click.\n");
            
            break;
        }
        case BUTTON_STATUS_DOUBLE_CLICK :
        {
            button_interface_debug_print("button: irq double click.\n");
            
            break;
        }
        case BUTTON_STATUS_TRIPLE_CLICK :
        {
            button_interface_debug_print("button: irq triple click.\n");
            
            break;
        }
        case BUTTON_STATUS_REPEAT_CLICK :
        {
            button_interface_debug_print("button: irq repeat click with %d times.\n", data->times);
            
            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_START :
        {
            button_interface_debug_print("button: irq short press start.\n");
            
            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_END :
        {
            button_interface_debug_print("button: irq short press end.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_START :
        {
            button_interface_debug_print("button: irq long press start.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            button_interface_debug_print("button: irq long press hold.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_END :
        {
            button_interface_debug_print("button: irq long press end.\n");
            
            break;
        }
        default :
        {
            button_interface_debug_print("button: irq unknown status.\n");
            
            break;
        }
    }
}
