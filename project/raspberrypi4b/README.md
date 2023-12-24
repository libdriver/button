### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(button REQUIRED)
```

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
./button -i

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
./button -p

button: INT connected to GPIO17(BCM).
```

```shell
./button -t trigger

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
button: irq repeat click with 5 times.
button: irq short press start.
button: irq short press end.
button: irq short press start.
button: irq long press start.
button: irq long press hold.
button: irq long press end.
button: finish trigger test.
```

```shell
./button -e trigger

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
./button -h

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
