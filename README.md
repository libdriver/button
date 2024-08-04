[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BUTTON

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/button/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

A push-button or simply button is a simple switch mechanism to control some aspect of a machine or a process. Buttons are typically made out of hard material, usually plastic or metal. The surface is usually flat or shaped to accommodate the human finger or hand, so as to be easily depressed or pushed. Buttons are most often biased switches, although many un-biased buttons (due to their physical nature) still require a spring to return to their un-pushed state.

LibDriver BUTTON is the full function driver of button launched by LibDriver. It provides press, release,  single click, double click , triple click, repeat click, short press, long press and other functions. LibDriver is MISRA compliant.

LibDriver BUTTON supports single button detection and can be run in a more efficient method, if you need multi-button or matrix-button detection, please refer to [https://github.com/libdriver/multi_button](https://github.com/libdriver/multi_button).

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver BUTTON source files.

/interface includes LibDriver BUTTON GPIO platform independent template.

/test includes LibDriver BUTTON driver test code and this code can test the chip necessary function simply.

/example includes LibDriver BUTTON sample code.

/doc includes LibDriver BUTTON offline document.

/datasheet includes BUTTON datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface GPIO platform independent template and finish your platform GPIO driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_button_basic.h"

uint8_t res;
uint8_t (*g_gpio_irq)(uint8_t) = NULL;
uint8_t (*g_period_irq)(void) = NULL;

static void a_receive_callback(button_t *data)
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
}

...
    
while (1)
{
    ...
}

...

/* basic deinit */
(void)button_basic_deinit();

/* gpio deinit */
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;
g_period_irq = NULL;

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/button/index.html](https://www.libdriver.com/docs/button/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.