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
 * @file      driver_button.c
 * @brief     driver button source file
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

#include "driver_button.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "General BUTTON"     /**< chip name */
#define MANUFACTURER_NAME         "General"            /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.8f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                 /**< chip max supply voltage */
#define MAX_CURRENT               1.0f                 /**< chip max current */
#define TEMPERATURE_MIN           -40.0f               /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f               /**< chip max operating temperature */
#define DRIVER_VERSION            1000                 /**< driver version */

/**
 * @brief button check definition
 */
#define BUTTON_SHORT_TIME       (1000 * 1000)          /**< 1s */
#define BUTTON_LONG_TIME        (3 * 1000 * 1000)      /**< 3s */
#define BUTTON_REPEAT_TIME      (200 * 1000)           /**< 200ms */
#define BUTTON_INTERVAL         (5 * 1000)             /**< 5ms */
#define BUTTON_TIMEOUT          (1000 * 1000)          /**< 1s */

/**
 * @brief     set param
 * @param[in] *handle points to a button handle structure
 * @note      none
 */
static void a_button_set_param(button_handle_t *handle)
{
    handle->short_time = BUTTON_SHORT_TIME;          /* set default short time */
    handle->long_time = BUTTON_LONG_TIME;            /* set default long time */
    handle->repeat_time = BUTTON_REPEAT_TIME;        /* set default repeat time */
    handle->interval = BUTTON_INTERVAL;              /* set default interval */
    handle->timeout = BUTTON_TIMEOUT;                /* set default timeout */
}

/**
 * @brief     reset all
 * @param[in] *handle points to a button handle structure
 * @note      none
 */
static void a_button_reset(button_handle_t *handle)
{
    uint8_t res;
    button_time_t t;
    
    handle->decode_len = 0;                  /* reset the decode */
    handle->short_triggered = 0;             /* init 0 */
    handle->long_triggered = 0;              /* init 0 */
    
    res = handle->timestamp_read(&t);        /* timestamp read */
    if (res != 0)                            /* check result */
    {
        return;                              /* return error */
    }
    handle->last_time.s = t.s;               /* save last time */
    handle->last_time.us = t.us;             /* save last time */
}

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
uint8_t button_period_handler(button_handle_t *handle)
{
    uint8_t res;
    uint16_t i;
    uint16_t len;
    int64_t diff;
    button_time_t t;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    if (handle->decode_len != 0)                                                  /* if decode len is not 0 */
    {
        res = handle->timestamp_read(&t);                                         /* timestamp read */
        if (res != 0)                                                             /* check result */
        {
            handle->debug_print("button: timestamp read failed.\n");              /* timestamp read failed */
            
            return 1;                                                             /* return error */
        }
        if (handle->decode_len == 1)                                              /* short or long press */
        {
            diff = (int64_t)(t.s - handle->decode[0].t.s) * 1000000 + 
                   (int64_t)(t.us - handle->decode[0].t.us) ;                     /* now - last time */
            if ((uint32_t)(diff) >= handle->short_time)                           /* check short time */
            {
                if (handle->short_triggered == 0)                                 /* if no triggered */
                {
                    button_t button;
                    
                    button.status = BUTTON_STATUS_SHORT_PRESS_START;              /* short press start */
                    button.times = 0;                                             /* 0 times */
                    handle->receive_callback(&button);                            /* run the reception callback */
                    handle->short_triggered = 1;                                  /* set triggered */
                }
            }
            if ((uint32_t)(diff) >= handle->long_time)                            /* check long time */
            {
                if (handle->long_triggered == 0)                                  /* if no triggered */
                {
                    button_t button;
                    
                    button.status = BUTTON_STATUS_LONG_PRESS_START;               /* long press start */
                    button.times = 0;                                             /* 0 times */
                    handle->receive_callback(&button);                            /* run the reception callback */
                    handle->long_triggered = 1;                                   /* set triggered */
                }
                else
                {
                    button_t button;
                    
                    button.status = BUTTON_STATUS_LONG_PRESS_HOLD;                /* long press hold */
                    button.times = 0;                                             /* 0 times */
                    handle->receive_callback(&button);                            /* run the reception callback */
                }
            }
        }
        else if (handle->decode_len == 2)                                         /* single click, short or long press */
        {
            diff = (int64_t)(t.s - handle->decode[1].t.s) * 1000000 + 
                   (int64_t)(t.us - handle->decode[1].t.us) ;                     /* now - last time */
            if (handle->long_triggered != 0)                                      /* if long no triggered */
            {
                button_t button;
                
                button.status = BUTTON_STATUS_LONG_PRESS_END;                     /* long press end */
                button.times = 0;                                                 /* 0 times */
                handle->receive_callback(&button);                                /* run the reception callback */
                a_button_reset(handle);                                           /* reset all */
            }
            else if (handle->short_triggered != 0)                                /* if short no triggered */
            {
                button_t button;
                
                button.status = BUTTON_STATUS_SHORT_PRESS_END;                    /* long press end */
                button.times = 0;                                                 /* 0 times */
                handle->receive_callback(&button);                                /* run the reception callback */
                a_button_reset(handle);                                           /* reset all */
            }
            else
            {
                if ((uint32_t)(diff) >= handle->repeat_time)                      /* check repeat time */
                {
                    button_t button;
                    
                    button.status = BUTTON_STATUS_SINGLE_CLICK;                   /* single click */
                    button.times = 1;                                             /* 1 times */
                    handle->receive_callback(&button);                            /* run the reception callback */
                    a_button_reset(handle);                                       /* reset all */
                }
            }
        }
        else if (handle->decode_len == 4)                                         /* double click */
        {
            diff = (int64_t)(t.s - handle->last_time.s) * 1000000 + 
                   (int64_t)(t.us - handle->last_time.us) ;                       /* now - last time */
            if ((uint32_t)(diff) >= handle->repeat_time)                          /* check repeat time */
            {
                button_t button;
                
                len = handle->decode_len - 1;                                     /* len - 1 */
                for (i = 0; i < len; i++)                                         /* diff all time */
                {
                    int64_t diff2;
                    
                    diff2 = (int64_t)((int64_t)handle->decode[i + 1].t.s -
                            (int64_t)handle->decode[i].t.s) * 1000000 + 
                            (int64_t)((int64_t)handle->decode[i + 1].t.us -
                            (int64_t)handle->decode[i].t.us);                     /* diff time */
                    handle->decode[i].diff_us = (uint32_t)diff2;                  /* save the time diff2 */
                }
                handle->decode[3].diff_us = (uint32_t)diff;                       /* save the time diff */
                
                for (i = 1; i < len; i += 2)                                      /* check decode length */
                {
                    if (handle->decode[i].diff_us >= handle->repeat_time)         /* check repeat time */
                    {
                        handle->debug_print("button: double click error.\n");     /* double click error */
                        a_button_reset(handle);                                   /* reset all */
                        
                        return 4;                                                 /* return error */
                    }
                }
                
                button.status = BUTTON_STATUS_DOUBLE_CLICK;                       /* double click */
                button.times = 2;                                                 /* 2 times */
                handle->receive_callback(&button);                                /* run the reception callback */
                a_button_reset(handle);                                           /* reset all */
            }
        }
        else if (handle->decode_len == 6)                                         /* triple click */
        {
            diff = (int64_t)(t.s - handle->last_time.s) * 1000000 + 
                   (int64_t)(t.us - handle->last_time.us) ;                       /* now - last time */
            if ((uint32_t)(diff) >= handle->repeat_time)                          /* check repeat time */
            {
                button_t button;
                
                len = handle->decode_len - 1;                                     /* len - 1 */
                for (i = 0; i < len; i++)                                         /* diff all time */
                {
                    int64_t diff2;
                    
                    diff2 = (int64_t)((int64_t)handle->decode[i + 1].t.s -
                            (int64_t)handle->decode[i].t.s) * 1000000 + 
                            (int64_t)((int64_t)handle->decode[i + 1].t.us -
                            (int64_t)handle->decode[i].t.us);                     /* diff time */
                    handle->decode[i].diff_us = (uint32_t)diff2;                  /* save the time diff2 */
                }
                handle->decode[5].diff_us = (uint32_t)diff;                       /* save the time diff */
                
                for (i = 1; i < len; i += 2)                                      /* check decode length */
                {
                    if (handle->decode[i].diff_us >= handle->repeat_time)         /* check repeat time */
                    {
                        handle->debug_print("button: triple click error.\n");     /* triple click error */
                        a_button_reset(handle);                                   /* reset all */
                        
                        return 4;                                                 /* return error */
                    }
                }
                
                button.status = BUTTON_STATUS_TRIPLE_CLICK;                       /* triple click */
                button.times = 3;                                                 /* 3 times */
                handle->receive_callback(&button);                                /* run the reception callback */
                a_button_reset(handle);                                           /* reset all */
            }
        }
        else
        {
            if ((handle->decode_len > 6) && (handle->decode_len % 2 == 0))        /* check time */
            {
                diff = (int64_t)(t.s - handle->last_time.s) * 1000000 + 
                       (int64_t)(t.us - handle->last_time.us) ;                   /* now - last time */
                if ((uint32_t)(diff) >= handle->repeat_time)                      /* check repeat time */
                {
                    button_t button;
                    
                    len = handle->decode_len - 1;                                 /* len - 1 */
                    for (i = 0; i < len; i++)                                     /* diff all time */
                    {
                        int64_t diff2;
                        
                        diff2 = (int64_t)((int64_t)handle->decode[i + 1].t.s -
                                (int64_t)handle->decode[i].t.s) * 1000000 + 
                                (int64_t)((int64_t)handle->decode[i + 1].t.us -
                                (int64_t)handle->decode[i].t.us);                 /* diff time */
                        handle->decode[i].diff_us = (uint32_t)diff2;              /* save the time diff2 */
                    }
                    handle->decode[len].diff_us = (uint32_t)diff;                 /* save the time diff */
                    
                    for (i = 1; i < len; i += 2)                                  /* check decode length */
                    {
                        if (handle->decode[i].diff_us >= handle->repeat_time)     /* check repeat time */
                        {
                            handle->debug_print("button: repeat click error.\n"); /* repeat click error */
                            a_button_reset(handle);                               /* reset all */
                            
                            return 4;                                             /* return error */
                        }
                    }
                    
                    button.status = BUTTON_STATUS_REPEAT_CLICK;                   /* repeat click */
                    button.times = handle->decode_len / 2;                        /* decode times */
                    handle->receive_callback(&button);                            /* run the reception callback */
                    a_button_reset(handle);                                       /* reset all */
                }
            }
            else
            {
                diff = (int64_t)(t.s - handle->last_time.s) * 1000000 + 
                       (int64_t)(t.us - handle->last_time.us) ;                   /* now - last time */
                if ((uint32_t)(diff) >= handle->timeout)                          /* check timeout */
                {
                    handle->debug_print("button: reset checking.\n");             /* reset checking */
                    a_button_reset(handle);                                       /* reset all */
                    
                    return 4;                                                     /* return error */
                }
            }
        }
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t button_irq_handler(button_handle_t *handle, uint8_t press_release)
{
    uint8_t res;
    int64_t diff;
    button_time_t t;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = handle->timestamp_read(&t);                                      /* timestamp read */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("button: timestamp read failed.\n");           /* timestamp read failed */
        
        return 1;                                                          /* return error */
    }
    diff = (int64_t)(t.s - handle->last_time.s) * 1000000 + 
           (int64_t)(t.us - handle->last_time.us) ;                        /* now - last time */
    if (press_release != 0)                                                /* if press */
    {
        if ((handle->decode_len % 2) == 0)                                 /* press */
        {
            if ((uint32_t)(diff) < handle->interval)                       /* check diff */
            {
                handle->debug_print("button: press too fast.\n");          /* trigger too fast */
                a_button_reset(handle);                                    /* reset all */
                
                return 5;                                                  /* success return 0 */
            }
        }
    }
    else                                                                   /* if release */
    {
        if ((handle->decode_len % 2) != 0)                                 /* release */
        {
            if ((uint32_t)(diff) < handle->interval)                       /* check diff */
            {
                handle->debug_print("button: release too fast.\n");        /* release too fast */
                a_button_reset(handle);                                    /* reset all */
                
                return 5;                                                  /* success return 0 */
            }
        }
    }
    
    if (press_release != 0)                                                /* check press release */
    {
        if (handle->receive_callback != NULL)                              /* if not null */
        {
            button_t button;
            
            button.status = BUTTON_STATUS_PRESS;                           /* press */
            button.times = 0;                                              /* 0 times */
            handle->receive_callback(&button);                             /* run the reception callback */
        }
    }
    else
    {
        if (handle->receive_callback != NULL)                              /* if not null */
        {
            button_t button;
            
            button.status = BUTTON_STATUS_RELEASE;                         /* release */
            button.times = 0;                                              /* 0 times */
            handle->receive_callback(&button);                             /* run the reception callback */
        }
    }
    
    if (handle->decode_len >= 127)                                         /* check the max length */
    {
        a_button_reset(handle);                                            /* reset all */
    }
    if (press_release != 0)                                                /* if press */
    {
        if ((handle->decode_len % 2) == 0)                                 /* press */
        {
            handle->decode[handle->decode_len].t.s = t.s;                  /* save s */
            handle->decode[handle->decode_len].t.us = t.us;                /* save us */
            handle->decode_len++;                                          /* length++ */
        }
        else
        {
            handle->debug_print("button: double press.\n");                /* double press */
            a_button_reset(handle);                                        /* reset all */
            
            return 4;                                                      /* return error */
        }
    }
    else                                                                   /* if release */
    {
        if ((handle->decode_len % 2) != 0)                                 /* release */
        {
            handle->decode[handle->decode_len].t.s = t.s;                  /* save s */
            handle->decode[handle->decode_len].t.us = t.us;                /* save us */
            handle->decode_len++;                                          /* length++ */
        }
        else
        {
            handle->debug_print("button: double release.\n");              /* double release */
            a_button_reset(handle);                                        /* reset all */
            
            return 4;                                                      /* return error */
        }
    }
    handle->last_time.s = t.s;                                             /* save last time */
    handle->last_time.us = t.us;                                           /* save last time */
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t button_init(button_handle_t *handle)
{
    uint8_t res;
    button_time_t t;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->debug_print == NULL)                                       /* check debug_print */
    {
        return 3;                                                          /* return error */
    }
    if (handle->timestamp_read == NULL)                                    /* check timestamp_read */
    {
        handle->debug_print("button: timestamp_read is null.\n");          /* timestamp_read is null */
        
        return 3;                                                          /* return error */
    }
    if (handle->delay_ms == NULL)                                          /* check delay_ms */
    {
        handle->debug_print("button: delay_ms is null.\n");                /* delay_ms is null */
        
        return 3;                                                          /* return error */
    }
    if (handle->receive_callback == NULL)                                  /* check receive_callback */
    {
        handle->debug_print("button: receive_callback is null.\n");        /* receive_callback is null */
        
        return 3;                                                          /* return error */
    }
    
    res = handle->timestamp_read(&t);                                      /* timestamp read */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("button: timestamp read failed.\n");           /* timestamp read failed */
        
        return 1;                                                          /* return error */
    }
    a_button_reset(handle);                                                /* reset all */
    a_button_set_param(handle);                                            /* set params */
    handle->last_time.s = t.s;                                             /* save last time */
    handle->last_time.us = t.us;                                           /* save last time */
    handle->inited = 1;                                                    /* flag inited */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a button handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t button_deinit(button_handle_t *handle)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->inited = 0;             /* flag close */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_set_timeout(button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->timeout = us;           /* set timeout */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_get_timeout(button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->timeout;          /* get timeout */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_set_interval(button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->interval = us;          /* set interval */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_get_interval(button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->interval;         /* get interval */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_set_short_time(button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->short_time = us;        /* set short time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_get_short_time(button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->short_time;       /* get short time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_set_long_time(button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->long_time = us;         /* set long time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_get_long_time(button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->long_time;        /* get long time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_set_repeat_time(button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->repeat_time = us;       /* set repeat time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t button_get_repeat_time(button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->repeat_time;      /* get repeat time */
    
    return 0;                       /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a button info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t button_info(button_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(button_info_t));                         /* initialize button info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "GPIO", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
