[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BUTTON

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/button/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

푸시 버튼 또는 간단한 버튼은 기계 또는 프로세스의 일부 측면을 제어하는 간단한 스위치 메커니즘입니다. 버튼은 일반적으로 플라스틱이나 금속과 같은 단단한 재질로 만들어집니다. 표면은 일반적으로 쉽게 누르거나 밀릴 수 있도록 사람의 손가락이나 손을 수용할 수 있는 편평하거나 모양입니다. 버튼은 대부분 편향된 스위치이지만, 편향되지 않은 많은 버튼(물리적 특성으로 인해)이 눌리지 않은 상태로 돌아가려면 여전히 스프링이 필요합니다.

LibDriver BUTTON은 LibDriver가 출시한 버튼의 전체 기능 드라이버입니다. 누르기, 놓기, 한 번 클릭, 두 번 클릭, 세 번 클릭, 반복 클릭, 짧게 누르기, 길게 누르기 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

LibDriver BUTTON은 단일 버튼 감지를 지원하며 보다 효율적인 방법으로 실행할 수 있습니다. 다중 버튼 또는 매트릭스 버튼 감지가 필요한 경우 다음을 참조하세요[https://github.com/libdriver/multi_button](https://github.com/libdriver/multi_button).

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver BUTTON의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver BUTTON용 플랫폼 독립적인 GPIO버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver BUTTON드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver BUTTON프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver BUTTON오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 BUTTON데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인GPIO버스 템플릿을 참조하여 지정된 플랫폼에 대한GPIO버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/button/index.html](https://www.libdriver.com/docs/button/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.