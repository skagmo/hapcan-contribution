HAPCAN compatible DALI gateway
==============================

<img src="https://github.com/skagmo/hapcan-contribution/blob/master/dali-gateway/img/pcb.jpg" alt="img" style="width:500">

The purpose of this project is to connect a DALI (Digital Addressable Lighting Interface) network with a HAPCAN network, and making it possible to control for example a DALI dimmer by triggering a HAPCAN button module.

Features
--------

Can be indirectly controlled by other CAN nodes. The behaviour is somewhat similar to "memory cells" in HAPCAN, but less flexible due to the differences between HAPCAN and DALI. A given HAPCAN node sending BUTTON frames can indirectly trigger a given DALI address, and enable "Touch DIM" behaviour:

- Short press (under 400 ms): Toggle state of DALI device. As the DALI protocol does not have a toggle command, the gateway keeps track of the state and arc level of all 81 DALI addresses (64 individual, 16 groups, 1 broadcast) to make a DALI dimmer toggle between being off and being set at a given arc level.

- Long press (over 400 ms): Will start dimming the DALI device by sending an arc level command every 100 ms until the button is released, or the maximum or minimum level has been reached.

After the state or arc level of a DALI device has been changed, either through direct or indirect control, the gateway will send the new state and arc level to the CAN bus to update all other nodes.

The gateway can also be directly controlled with an instruction (toggle or set arc level), an arc level (ignored if toggling), and a DALI address.

Firmware project
----------------

The project is written in C for a 32-bit PIC32MX564F128H microcontroller. It is therefore quite different from HAPCAN devices which uses an 8-bit PIC18F26K80, and makes it incompatible with the standard HAPCAN bootloader.

Open the project in Microchip MPLAB X and compile with the XC32 compiler.

| File       | Description |
|:------     |:----------- |
| logic.c    | Contains the gateway logic between DALI and CAN, and HAPCAN related definitions. |
| dali.c     | PIC32 DALI implementation using pin interrupt and timer interrupt. |
| can.c      | CAN initialization and send/receive routines. |

Hardware project
----------------

The schematic and PCB is made in Eagle CAD, and made to fit inside a double DIN rail enclosure similar to those used in the HAPCAN project.

--
Jon Petter Skagmo, LA3JPA, 2016
