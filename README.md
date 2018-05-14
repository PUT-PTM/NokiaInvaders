# Overview

A game inspired by Space Invaders.
# Description

The project is based on a STM32F4DISCOVERY kit. The game is displayed using a Nokia 5110 screen. An in-built LIS3DSH accelerometer and a button are used as controls.
# Tools

STM32F4xx STDPeriph library www.st.com/STM32

CMSIS libraries http://arm-software.github.io/CMSIS_5/General/html/index.htm

Tilen Majerle STM32F4 libraries http://stm32f4-discovery.com :
- LIS302DL and LIS3DSH - accelerometer 
- PCD8544 - nokia display
- GPIO
- SPI

STM32 System Workbench (IDE) and STM Studio (debugging) www.st.com/STM32

# How to run

Upload the compiled project onto the STM32F4DISCOVERY board. A connected Nokia display is required to run it.

# How to compile

All needed libraries are included in the project. Release build was tested with MCU GCC Compiler and STM32 System Workbench IDE.

# Attributions

License and copyright information of used libraries are included in their source codes. 
Included tools and libraries with official download links are listed in the Tools section.

# Credits

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.

Supervisor: Tomasz Mańkowski
