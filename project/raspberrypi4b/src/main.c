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
 * @file      main.c
 * @brief     main source file
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
#include "driver_button_trigger_test.h"
#include "gpio.h"
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t (*g_gpio_irq)(uint8_t press_release) = NULL;        /**< gpio irq */
uint8_t (*g_period_irq)(void) = NULL;                       /**< period irq */
volatile uint16_t g_flag;                                   /**< interrupt flag */

/**
 * @brief     timer handler
 * @param[in] signum is the signal number
 * @note      none
 */
static void a_timer_handler(int signum)
{
    if (g_period_irq != NULL)
    {
        g_period_irq();
    }
}

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
            g_flag |= (1 << 0);

            break;
        }
        case BUTTON_STATUS_RELEASE :
        {
            g_flag |= (1 << 1);

            break;
        }
        case BUTTON_STATUS_SINGLE_CLICK :
        {
            g_flag |= (1 << 2);
            button_interface_debug_print("button: irq single click.\n");

            break;
        }
        case BUTTON_STATUS_DOUBLE_CLICK :
        {
            g_flag |= (1 << 3);
            button_interface_debug_print("button: irq double click.\n");

            break;
        }
        case BUTTON_STATUS_TRIPLE_CLICK :
        {
            g_flag |= (1 << 4);
            button_interface_debug_print("button: irq triple click.\n");

            break;
        }
        case BUTTON_STATUS_REPEAT_CLICK :
        {
            g_flag |= (1 << 5);
            button_interface_debug_print("button: irq repeat click with %d times.\n", data->times);

            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_START :
        {
            g_flag |= (1 << 6);
            button_interface_debug_print("button: irq short press start.\n");

            break;
        }
        case BUTTON_STATUS_SHORT_PRESS_END :
        {
            g_flag |= (1 << 7);
            button_interface_debug_print("button: irq short press end.\n");

            break;
        }
        case BUTTON_STATUS_LONG_PRESS_START :
        {
            g_flag |= (1 << 8);
            button_interface_debug_print("button: irq long press start.\n");

            break;
        }
        case BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            g_flag |= (1 << 9);
            button_interface_debug_print("button: irq long press hold.\n");

            break;
        }
        case BUTTON_STATUS_LONG_PRESS_END :
        {
            g_flag |= (1 << 10);
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
 * @brief     button full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t button(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    char short_options[] = "hipe:t:";
    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_trigger", type) == 0)
    {
        uint8_t res;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the irq */
        g_gpio_irq = button_trigger_test_irq_handler;
        g_period_irq = button_trigger_test_period_handler;

        /* run the trigger test */
        res = button_trigger_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            g_period_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        g_period_irq = NULL;

        return 0;
    }
    else if (strcmp("e_trigger", type) == 0)
    {
        uint8_t res;
        uint32_t timeout;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the irq */
        g_gpio_irq = button_basic_irq_handler;
        g_period_irq = button_basic_period_handler;

        /* basic init */
        res = button_basic_init(a_receive_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            g_period_irq = NULL;
            
            return 1;
        }

        /* 100s timeout */
        timeout = 10000;

        /* init 0 */
        g_flag = 0;

        /* check timeout */
        while (timeout != 0)
        {
            /* check the flag */
            if ((g_flag & 0x07FF) == 0x07FF)
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
            (void)button_basic_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            g_period_irq = NULL;

            return 1;
        }

        /* trigger all */
        button_interface_debug_print("button: trigger all.\n");

        /* basic deinit */
        (void)button_basic_deinit();

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        g_period_irq = NULL;

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        button_interface_debug_print("Usage:\n");
        button_interface_debug_print("  button (-i | --information)\n");
        button_interface_debug_print("  button (-h | --help)\n");
        button_interface_debug_print("  button (-p | --port)\n");
        button_interface_debug_print("  button (-t trigger | --test=trigger)\n");
        button_interface_debug_print("  button (-e trigger | --example=trigger)\n");
        button_interface_debug_print("\n");
        button_interface_debug_print("Options:\n");
        button_interface_debug_print("  -e <trigger>, --example=<trigger>    Run the driver example.\n");
        button_interface_debug_print("  -h, --help                           Show the help.\n");
        button_interface_debug_print("  -i, --information                    Show the chip information.\n");
        button_interface_debug_print("  -p, --port                           Display the pin connections of the current board.\n");
        button_interface_debug_print("  -t <trigger>, --test=<trigger>       Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        button_info_t info;

        /* print button info */
        button_info(&info);
        button_interface_debug_print("button: chip is %s.\n", info.chip_name);
        button_interface_debug_print("button: manufacturer is %s.\n", info.manufacturer_name);
        button_interface_debug_print("button: interface is %s.\n", info.interface);
        button_interface_debug_print("button: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        button_interface_debug_print("button: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        button_interface_debug_print("button: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        button_interface_debug_print("button: max current is %0.2fmA.\n", info.max_current_ma);
        button_interface_debug_print("button: max temperature is %0.1fC.\n", info.temperature_max);
        button_interface_debug_print("button: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        button_interface_debug_print("button: INT connected to GPIO17(BCM).\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;
    struct itimerval timer;
    struct sigaction sa;

    /* init timer handler */
    sa.sa_handler = a_timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    /* init timer */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100 * 1000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100 * 1000;
    setitimer(ITIMER_REAL, &timer, NULL);

    res = button(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        button_interface_debug_print("button: run failed.\n");
    }
    else if (res == 5)
    {
        button_interface_debug_print("button: param is invalid.\n");
    }
    else
    {
        button_interface_debug_print("button: unknown status code.\n");
    }

    return 0;
}
