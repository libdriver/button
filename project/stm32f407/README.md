### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. BUTTON

#### 3.1 Command Instruction

1. Show button chip and driver information.

    ```shell
    button (-i | --information)  
    ```

2. Show button help.

    ```shell
    button (-h | --help)        
    ```

3. Show button pin connections of the current board.

    ```shell
    button (-p | --port)              
    ```

4. Run button trigger test.

    ```shell
    button (-t trigger | --test=trigger)
    ```

5. Run button trigger function.

    ```shell
    button (-e trigger | --example=trigger)  
    ```
    
#### 3.2 Command Example

```shell
button -i

button: chip is General BUTTON.
button: manufacturer is General.
button: interface is GPIO.
button: driver version is 1.0.
button: min supply voltage is 1.8V.
button: max supply voltage is 5.5V.
button: max current is 1.00mA.
button: max temperature is 125.0C.
button: min temperature is -40.0C.
```

```shell
button -p

button: INT connected to GPIOB PIN0.
```

```shell
button -t trigger

button: chip is General BUTTON.
button: manufacturer is General.
button: interface is GPIO.
button: driver version is 1.0.
button: min supply voltage is 1.8V.
button: max supply voltage is 5.5V.
button: max current is 1.00mA.
button: max temperature is 125.0C.
button: min temperature is -40.0C.
button: start trigger test.
button: irq single click.
button: irq double click.
button: irq triple click.
button: irq repeat click with 6 times.
button: irq short press start.
button: irq short press end.
button: irq short press start.
button: irq long press start.
button: irq long press hold.
button: irq long press end.
button: finish trigger test.
```

```shell
button -e trigger

button: irq single click.
button: irq double click.
button: irq triple click.
button: irq repeat click with 5 times.
button: irq short press start.
button: irq short press end.
button: irq short press start.
button: irq long press start.
button: irq long press hold.
button: irq long press end.
button: trigger all.
```

```shell
button -h

Usage:
  button (-i | --information)
  button (-h | --help)
  button (-p | --port)
  button (-t trigger | --test=trigger)
  button (-e trigger | --example=trigger)

Options:
  -e <trigger>, --example=<trigger>    Run the driver example.
  -h, --help                           Show the help.
  -i, --information                    Show the chip information.
  -p, --port                           Display the pin connections of the current board.
  -t <trigger>, --test=<trigger>       Run the driver test.
```
