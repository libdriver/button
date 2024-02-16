[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BUTTON

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/button/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Ein Druckknopf oder einfach Knopf ist ein einfacher Schaltmechanismus zur Steuerung bestimmter Aspekte einer Maschine oder eines Prozesses. Knöpfe bestehen typischerweise aus hartem Material, meist Kunststoff oder Metall. Die Oberfläche ist normalerweise flach oder so geformt, dass sie sich dem menschlichen Finger oder der Hand anpasst und leicht gedrückt oder gedrückt werden kann. Bei Knöpfen handelt es sich meist um vorgespannte Schalter, obwohl viele nicht vorgespannte Knöpfe (aufgrund ihrer physikalischen Beschaffenheit) immer noch eine Feder benötigen, um in ihren nicht gedrückten Zustand zurückzukehren.

LibDriver BUTTON ist der voll funktionsfähige Button-Treiber, der von LibDriver gestartet wird. Es bietet Drücken, Loslassen, Einzelklick, Doppelklick, Dreifachklick, Wiederholungsklick, kurzes Drücken, langes Drücken und andere Funktionen. LibDriver ist MISRA-kompatibel.

LibDriver BUTTON unterstützt die Erkennung einzelner Tasten und kann effizienter ausgeführt werden. Wenn Sie eine Erkennung mehrerer Tasten oder Matrixtasten benötigen, lesen Sie bitte [https://github.com/libdriver/multi_button](https://github.com/libdriver/multi_button).

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver BUTTON-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver BUTTON GPIO.

/test enthält den Testcode des LibDriver BUTTON-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver BUTTON-Beispielcode.

/doc enthält das LibDriver BUTTON-Offlinedokument.

/Datenblatt enthält BUTTON-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige GPIO-Schnittstellenvorlage und stellen Sie Ihren Plattform-GPIO-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/button/index.html](https://www.libdriver.com/docs/button/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.