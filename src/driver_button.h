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
 * @file      driver_button.h
 * @brief     driver button header file
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

#ifndef DRIVER_BUTTON_H
#define DRIVER_BUTTON_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup button_driver button driver function
 * @brief    button driver modules
 * @{
 */

/**
 * @addtogroup button_basic_driver
 * @{
 */

/**
 * @brief button status enumeration definition
 */
typedef enum
{
    BUTTON_STATUS_PRESS             = (1 << 0),        /**< press */
    BUTTON_STATUS_RELEASE           = (1 << 1),        /**< release */
    BUTTON_STATUS_SINGLE_CLICK      = (1 << 2),        /**< single click */
    BUTTON_STATUS_DOUBLE_CLICK      = (1 << 3),        /**< double click */
    BUTTON_STATUS_TRIPLE_CLICK      = (1 << 4),        /**< triple click */
    BUTTON_STATUS_REPEAT_CLICK      = (1 << 5),        /**< repeat click */
    BUTTON_STATUS_SHORT_PRESS_START = (1 << 6),        /**< short press start */
    BUTTON_STATUS_SHORT_PRESS_END   = (1 << 7),        /**< short press end */
    BUTTON_STATUS_LONG_PRESS_START  = (1 << 8),        /**< long press start */
    BUTTON_STATUS_LONG_PRESS_HOLD   = (1 << 9),        /**< long press hold */
    BUTTON_STATUS_LONG_PRESS_END    = (1 << 10),       /**< long press end */
} button_status_t;

/**
 * @brief button structure definition
 */
typedef struct button_s
{
    uint16_t status;       /**< status */
    uint16_t times;        /**< times */
} button_t;

/**
 * @brief button time structure definition
 */
typedef struct button_time_s
{
    uint64_t s;         /**< second */
    uint32_t us;        /**< microsecond */
} button_time_t;

/**
 * @brief button decode structure definition
 */
typedef struct button_decode_s
{
    button_time_t t;        /**< timestamp */
    uint32_t diff_us;       /**< diff us */
} button_decode_t;

/**
 * @brief button handle structure definition
 */
typedef struct button_handle_s
{
    uint8_t (*timestamp_read)(button_time_t *t);            /**< point to an timestamp_read function address */
    void (*delay_ms)(uint32_t ms);                          /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);        /**< point to a debug_print function address */
    void (*receive_callback)(button_t *data);               /**< point to a receive_callback function address */
    button_time_t last_time;                                /**< last time */
    button_decode_t decode[128];                            /**< decode buffer */
    uint16_t decode_len;                                    /**< decode length */
    uint8_t short_triggered;                                /**< short triggered */
    uint8_t long_triggered;                                 /**< long triggered */
    uint8_t inited;                                         /**< inited flag */
    uint32_t short_time;                                    /**< short time */
    uint32_t long_time;                                     /**< long time */
    uint32_t repeat_time;                                   /**< repeat time */
    uint32_t interval;                                      /**< interval */
    uint32_t timeout;                                       /**< timeout */
} button_handle_t;

/**
 * @brief button information structure definition
 */
typedef struct button_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} button_info_t;

/**
 * @}
 */

/**
 * @defgroup button_link_driver button link driver function
 * @brief    button link driver modules
 * @ingroup  button_driver
 * @{
 */

/**
 * @brief     initialize button_handle_t structure
 * @param[in] HANDLE points to a button handle structure
 * @param[in] STRUCTURE is button_handle_t
 * @note      none
 */
#define DRIVER_BUTTON_LINK_INIT(HANDLE, STRUCTURE)           memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link timestamp_read function
 * @param[in] HANDLE points to a button handle structure
 * @param[in] FUC points to a timestamp_read function address
 * @note      none
 */
#define DRIVER_BUTTON_LINK_TIMESTAMP_READ(HANDLE, FUC)      (HANDLE)->timestamp_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a button handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_BUTTON_LINK_DELAY_MS(HANDLE, FUC)            (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a button handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_BUTTON_LINK_DEBUG_PRINT(HANDLE, FUC)         (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to a button handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_BUTTON_LINK_RECEIVE_CALLBACK(HANDLE, FUC)    (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup button_basic_driver button basic driver function
 * @brief    button basic driver modules
 * @ingroup  button_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a button info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t button_info(button_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to a button handle structure
 * @param[in] press_release is press or release
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 trigger error
 *            - 5 trigger too fast
 * @note      press is 1
 *            release is 0
 */
uint8_t button_irq_handler(button_handle_t *handle, uint8_t press_release);

/**
 * @brief     period handler
 * @param[in] *handle points to a button handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 check error
 * @note      none
 */
uint8_t button_period_handler(button_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a button handle structure
 * @return    status code
 *            - 0 success
 *            - 1 gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t button_init(button_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a button handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_deinit(button_handle_t *handle);

/**
 * @brief     set timeout
 * @param[in] *handle points to a button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_set_timeout(button_handle_t *handle, uint32_t us);

/**
 * @brief      get timeout
 * @param[in]  *handle points to a button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t button_get_timeout(button_handle_t *handle, uint32_t *us);

/**
 * @brief     set interval
 * @param[in] *handle points to a button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_set_interval(button_handle_t *handle, uint32_t us);

/**
 * @brief      get interval
 * @param[in]  *handle points to a button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t button_get_interval(button_handle_t *handle, uint32_t *us);

/**
 * @brief     set short time
 * @param[in] *handle points to a button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_set_short_time(button_handle_t *handle, uint32_t us);

/**
 * @brief      get short time
 * @param[in]  *handle points to a button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t button_get_short_time(button_handle_t *handle, uint32_t *us);

/**
 * @brief     set long time
 * @param[in] *handle points to a button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_set_long_time(button_handle_t *handle, uint32_t us);

/**
 * @brief      get long time
 * @param[in]  *handle points to a button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t button_get_long_time(button_handle_t *handle, uint32_t *us);

/**
 * @brief     set repeat time
 * @param[in] *handle points to a button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_set_repeat_time(button_handle_t *handle, uint32_t us);

/**
 * @brief      get repeat time
 * @param[in]  *handle points to a button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t button_get_repeat_time(button_handle_t *handle, uint32_t *us);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
