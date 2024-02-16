[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BUTTON

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/button/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

押しボタンまたは単にボタンは、機械またはプロセスの一部の側面を制御するための単純なスイッチ機構です。 ボタンは通常、プラスチックまたは金属などの硬い素材で作られています。 通常、表面は平らであるか、人間の指や手に適合する形状になっており、簡単に押したり押したりできるようになります。 ほとんどの場合、ボタンはバイアスされたスイッチですが、バイアスされていないボタンの多くは (その物理的性質により)、押されていない状態に戻るためにスプリングを必要とします。

LibDriver BUTTON は、LibDriver によって起動されるボタンの全機能ドライバーです。 プレス、リリース、シングルクリック、ダブルクリック、トリプルクリック、リピートクリック、短押し、長押しなどの機能を提供します。 LibDriver は MISRA に準拠しています。

LibDriver BUTTON は単一ボタンの検出をサポートしており、より効率的な方法で実行できます。複数のボタンまたはマトリックス ボタンの検出が必要な場合は、以下を参照してください[https://github.com/libdriver/multi_button](https://github.com/libdriver/multi_button)。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver BUTTONのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver BUTTON用のプラットフォームに依存しないGPIOバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver BUTTONドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver BUTTONプログラミング例が含まれています。

/ docディレクトリには、LibDriver BUTTONオフラインドキュメントが含まれています。

/ datasheetディレクトリには、BUTTONデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないGPIOバステンプレートを参照して、指定したプラットフォームのGPIOバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/button/index.html](https://www.libdriver.com/docs/button/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。