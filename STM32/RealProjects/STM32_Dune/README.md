# STM32F103 - Blue pill
![Embedded](/STM32/RealProjects/STM32_Dune/Resources/img/Blue_pill-STM32F103C8T6.jpg?raw=true "Blue pill board")

## Quick Introduction
Our purpose is to implement different projects based on STM32F103 microcontrollers. For this, we first create some basic drivers (GPIO, RCC, UART, SPI, etc) and then on an upper layer we will create different applications (Light Controller, Data Router, Display Controller, etc)

## Resources
- **IDE**: we are using MDK-Lite, version 5.29. This tool supports a lot of Cortex-M based microcontrollers.
- **Libraries**: StdPeriph library (Which will be called *Low Drivers* from now on)
- **Programmer**: ST-LINK-V2 (chinese low cost variant)

## Target
As I said before, we first have to create some *driver abstraction layer* for ease of application development.

### High Drivers
- **Port**
- **System**
- **Timer**
- **Task**
- **Uart**
- **Spi**

### Applications
- **Lighting**
- **Com**