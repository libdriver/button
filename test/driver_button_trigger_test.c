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
 * @file      driver_button_trigger_test.c
 * @brief     driver button trigger test source file
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

#include "driver_button_trigger_test.h"

static button_handle_t gs_handle;        /**< button handle */
static volatile uint16_t gs_flag;        /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] *data points to an button_t structure
 * @note      none
 */
static void a_receive_callback(button_t *data)
{
    switch (data->status)
    {
        case BUTTON_STATUS_PRESS :
        {
            gs_flag |= (1 << 0);
            
            break;
        }
        case BUTTON_STATUS_RELEASE :
        {
            gs_flag |= (1 << 1);
            
            break;
        }
        case BUTTON_STATUS_SINGLE_CLICK :
        {
            gs_flag |= (1 << 2);
            button_interface_debug_print("button: irq single click.\n");
            
            break;
        }
        case BUTTON_STATUS_DOUBLE_CLICK :
        {
            gs_flag |= (1 << 3);
            button_interface_debug_print("button: irq double click.\n");
            
            break;
        }
        case BUTTON_STATUS_TRIPLE_CLICK :
        {
            gs_flag |= (1 << 4);
            button_interface_debug_print("button: irq triple click.\n");
            
            break;
        }
        case BUTTON_STATUS_REPEAT_CLICK :
        {
            gs_flag |= (1 << 5);
            button_interface_debug_print("button: irq repeat click with %d times.\n", data->times);
            
            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_START :
        {
            gs_flag |= (1 << 6);
            button_interface_debug_print("button: irq short press start.\n");
            
            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_END :
        {
            gs_flag |= (1 << 7);
            button_interface_debug_print("button: irq short press end.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_START :
        {
            gs_flag |= (1 << 8);
            button_interface_debug_print("button: irq long press start.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            gs_flag |= (1 << 9);
            button_interface_debug_print("button: irq long press hold.\n");
            
            break;
        }
        case BUTTON_STATUS_LONG_PRESS_END :
        {
            gs_flag |= (1 << 10);
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

/**
 * @brief     trigger test irq
 * @param[in] press_release is press or release
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      press is 1
 *            release is 0
 */
uint8_t button_trigger_test_irq_handler(uint8_t press_release)
{
    if (button_irq_handler(&gs_handle, press_release) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  trigger test period
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t button_trigger_test_period_handler(void)
{
    if (button_period_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  trigger test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t button_trigger_test(void)
{
    uint8_t res;
    uint32_t timeout;
    button_info_t info;
    
    /* link interface function */
    DRIVER_BUTTON_LINK_INIT(&gs_handle, button_handle_t);
    DRIVER_BUTTON_LINK_TIMESTAMP_READ(&gs_handle, button_interface_timestamp_read);
    DRIVER_BUTTON_LINK_DELAY_MS(&gs_handle, button_interface_delay_ms);
    DRIVER_BUTTON_LINK_DEBUG_PRINT(&gs_handle, button_interface_debug_print);
    DRIVER_BUTTON_LINK_RECEIVE_CALLBACK(&gs_handle, a_receive_callback);
    
    /* get information */
    res = button_info(&info);
    if (res != 0)
    {
        button_interface_debug_print("button: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        button_interface_debug_print("button: chip is %s.\n", info.chip_name);
        button_interface_debug_print("button: manufacturer is %s.\n", info.manufacturer_name);
        button_interface_debug_print("button: interface is %s.\n", info.interface);
        button_interface_debug_print("button: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        button_interface_debug_print("button: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        button_interface_debug_print("button: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        button_interface_debug_print("button: max current is %0.2fmA.\n", info.max_current_ma);
        button_interface_debug_print("button: max temperature is %0.1fC.\n", info.temperature_max);
        button_interface_debug_print("button: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* init */
    res = button_init(&gs_handle);
    if (res != 0)
    {
        button_interface_debug_print("button: init failed.\n");
       
        return 1;
    }
    
    /* start trigger test */
    button_interface_debug_print("button: start trigger test.\n");
    
    /* 100s timeout */
    timeout = 10000;
    
    /* init 0 */
    gs_flag = 0;
    
    /* check timeout */
    while (timeout != 0)
    {
        /* check the flag */
        if ((gs_flag & 0x07FF) == 0x07FF)
        {
            break;
        }
        
        /* timeout -- */
        timeout--;
        
        /* delay 10ms */
        button_interface_delay_ms(10);
    }
    
    /* check the timeout */
    if (timeout == 0)
    {
        /* receive timeout */
        button_interface_debug_print("button: trigger timeout.\n");
        (void)button_deinit(&gs_handle);
            
        return 1;
    }
    
    /* finish trigger test */
    button_interface_debug_print("button: finish trigger test.\n");
    (void)button_deinit(&gs_handle);
    
    return 0;
}
