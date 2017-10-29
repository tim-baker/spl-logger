# Build log

## Wed 18th Oct - Power

### Summary

- Laid out components on paper with designators clearly labelled
- Solder paste applied to power components (all 400 named parts)
- Used hot plate to melt solder
- Tested power ORing works as specified

### Roadblocks

1. Bridged solder joints on USB (J4) connector and TPS63001 (U400) - `RESOLVED` with with desoldering wick along edge of pins and then coated pins lightly with flux and ran tip of soldering iron along the row for solder to clump nicely on pins
2. Topology implemented was for the generic TPS63000 part not the 3.3 V specific part - `RESOLVED` with simple removal of two resistors and capacitor and jumped where one resistor was. This makes for layout that is more modular and interchangeable for other non-3.3V designs.
3. U400 pads for GND and PGND were floating due to default DRC not connecting to polygon pour - `UNRESOLVED` despite trying to run jumper wires to GND pads too tired to focus on small component.

## Thu 19th Oct - Power

### Summary
- Cleared roadblock #3 from previous day
- Tested power supply works for specified input voltage range (3 - 5V)
- Soldered MCU and peripheral components

### Roadblocks
1. U400 pads for GND and PGND were floating due to default DRC not connecting to polygon pour - `RESOLVED` utilised the PGND being connected to the heatsink pad which goes through to bottom layer and tried again to connect to GND pin on top layer successful this time.

## Fri 20th Oct - MCU

### Summary
- Programmed MCU via 2-wire JTAG header with simple GPIO example code using TI-RTOS.
- Battery power works with the MCU to light it up - USB takes over when plugged in.
- Ported Board.h file and other header files from MSP-EXP430F5529LP board to my board.
- Tested SD driver example using LaunchPad board.
- Tested output from microphone using oscilloscope.

### Roadblocks
1. MEMS microphone bottom port hole blocked with solder/flux mixture - `UNRESOLVED` despite attempts to clear out 0.3mm hole with tools. To ask Russell for assistance on Monday/Tuesday.
2. SD card driver example works with LaunchPad but not with custom SPL Logger board - `UNRESOLVED`

## Sat 21 Oct - MCU
### Summary

- Cleaned up project files for custom board and tested other inputs (switch and header jumpers) with different LED patterns.

### Roadblocks
1. USB serial device example works with Launch but not with custom SPL Logger Board - `UNRESOLVED` suspected issue either 4 MHz resonator isn’t working (solder bridge?) or the USB D+ line is missing pull-up resistor to VBUS so host devices aren’t recognising the board when plugged in.

## Sun 22 Oct - Software

### Summary

- Mass Storage device over USB to access files on uSD card tested on LaunchPad.
Continued investigating USB device not being recognised by host PC...

## Tue 24 Oct - Microphone & micro-SD

### Summary

- Drilled out microphone hole (credit: Russell) and reflowed microphone to bottom-side of board.
- Tested microphone output and verified against datasheet.
- Assembled pre-amp circuit and tested ADC12 conversion values with threshold detector and LED output.
- Successfully read/wrote to uSD card with a .txt file.
- Tested LFXTAL with RTC_A library for MSP430. Success.

### Roadblocks

1. SD card driver example works with LaunchPad but not with custom SPL Logger board - `RESOLVED` the pads of the uSD card holder were mapped in a mirror image of the required uSD contacts - had to manually rewire up the the pads to correct power, ground and data lines.