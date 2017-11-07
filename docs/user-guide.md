# Documentation

## User guide

### Pushbuttons, switch and jumpers

The following table describes intended functions for user input methods.

| Pushbutton   | Function                                                                 |
| :----------: | :----------------------------------------------------------------------- |
| `S1`         | Export data via serial                                                   |
| `S2`         | Trigger save manual SPL sample                                           |
| `S3`         | Set sample frequecy: OFF,OFF=1min; OFF,ON=2min; ON,OFF=5min; ON,ON=10min |
| `S4`         | Reset the microcontroller                                                |
| `J1`         | Not used                                                                 |
| `J2`         | Not used                                                                 |

### Status LEDs

The following tables describe intended meanings for device status.

| LED1 status (orange) | Meaning          |
| :------------------: | ---------------- |
| `ON`                 | Device suspended |
| `OFF`                | Device suspended |
| `FLASH` (1 Hz)       | Device heartbeat |
| `FLASH` (fast)       | n/a              |

| LED2 status (orange) | Meaning                     |
| :------------------: | --------------------------- |
| `ON`                 | Peak SPL threshold detect   |
| `OFF`                | n/a                         |
| `FLASH` (slow)       | n/a                         |
| `FLASH` (fast)       | Save SPL sample in progress |

| LED3 status (green) | Meaning                       |
| :-----------------: | ----------------------------- |
| `TOGGLE`            | Save SPL sample occurred      |
| `ON`                | n/a                           |
| `OFF`               | n/a                           |
| `FLASH` (slow)      | n/a                           |
| `FLASH` (fast)      | n/a                           |

| LED4 status (red) | Meaning                   |
| :---------------: | ------------------------- |
| `ON`              | No micro-SD card detected |
| `OFF`             | micro-SD card detected    |
| `FLASH` (slow)    | n/a                       |
| `FLASH` (fast)    | n/a                       |