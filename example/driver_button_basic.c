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
 * @file      driver_button_basic.c
 * @brief     driver button basic source file
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

#include "driver_button_basic.h"

static button_handle_t gs_handle;        /**< button handle */

/**
 * @brief     basic irq
 * @param[in] press_release is press or release
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      press is 1
 *            release is 0
 */
uint8_t button_basic_irq_handler(uint8_t press_release)
{
    if (button_irq_handler(&gs_handle, press_release) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic period
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t button_basic_period_handler(void)
{
    if (button_period_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example init
 * @param[in] *callback points to an irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t button_basic_init(void (*callback)(button_t *data))
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_BUTTON_LINK_INIT(&gs_handle, button_handle_t);
    DRIVER_BUTTON_LINK_TIMESTAMP_READ(&gs_handle, button_interface_timestamp_read);
    DRIVER_BUTTON_LINK_DELAY_MS(&gs_handle, button_interface_delay_ms);
    DRIVER_BUTTON_LINK_DEBUG_PRINT(&gs_handle, button_interface_debug_print);
    DRIVER_BUTTON_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* init */
    res = button_init(&gs_handle);
    if (res != 0)
    {
        button_interface_debug_print("button: init failed.\n");
       
        return 1;
    }
    
    /* set default timeout */
    res = button_set_timeout(&gs_handle, BUTTON_BASIC_DEFAULT_TIMEOUT);
    if (res != 0)
    {
        button_interface_debug_print("button: set timeout failed.\n");
        (void)button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interval */
    res = button_set_interval(&gs_handle, BUTTON_BASIC_DEFAULT_INTERVAL);
    if (res != 0)
    {
        button_interface_debug_print("button: set interval failed.\n");
        (void)button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default short time */
    res = button_set_short_time(&gs_handle, BUTTON_BASIC_DEFAULT_SHORT_TIME);
    if (res != 0)
    {
        button_interface_debug_print("button: set short time failed.\n");
        (void)button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default long time */
    res = button_set_long_time(&gs_handle, BUTTON_BASIC_DEFAULT_LONG_TIME);
    if (res != 0)
    {
        button_interface_debug_print("button: set long time failed.\n");
        (void)button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default repeat time */
    res = button_set_repeat_time(&gs_handle, BUTTON_BASIC_DEFAULT_REPEAT_TIME);
    if (res != 0)
    {
        button_interface_debug_print("button: set repeat time failed.\n");
        (void)button_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t button_basic_deinit(void)
{
    if (button_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
