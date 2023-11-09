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
 * @file      driver_button_basic.h
 * @brief     driver button basic header file
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

#ifndef DRIVER_BUTTON_BASIC_H
#define DRIVER_BUTTON_BASIC_H

#include "driver_button_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup button_example_driver button example driver function
 * @brief    button example driver modules
 * @ingroup  button_driver
 * @{
 */

/**
 * @brief button basic example default definition
 */
#define BUTTON_BASIC_DEFAULT_TIMEOUT                 1000 * 1000               /**< 1s */
#define BUTTON_BASIC_DEFAULT_INTERVAL                5 * 1000                  /**< 5ms */
#define BUTTON_BASIC_DEFAULT_SHORT_TIME              1000 * 1000               /**< 1s */
#define BUTTON_BASIC_DEFAULT_LONG_TIME               3 * 1000 * 1000           /**< 3s */
#define BUTTON_BASIC_DEFAULT_REPEAT_TIME             200 * 1000                /**< 200ms */

/**
 * @brief     basic irq
 * @param[in] press_release is press or release
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      press is 1
 *            release is 0
 */
uint8_t button_basic_irq_handler(uint8_t press_release);

/**
 * @brief  basic period
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t button_basic_period_handler(void);

/**
 * @brief     basic example init
 * @param[in] *callback points to an irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t button_basic_init(void (*callback)(button_t *data));

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t button_basic_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
