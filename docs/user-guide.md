# Documentation

## User guide

### Pushbuttons, switch and jumpers

The following table describes intended functions for user input methods.

| Pushbutton   | Function                                                        |
| :----------: | :-------------------------------------------------------------- |
| `S1`         | Export data via serial                                          |
| `S2`         | Trigger save manual SPL sample                                  |
| `S3`         | Set sample frequecy: 00b = 5min, 01b = 1min, 10b=30s, 11b = 10s |
| `S4`         | Reset the microcontroller                                       |
| `J1`         | Not used                                                        |
| `J2`         | Not used                                                        |

### Status LEDs

The following tables describe intended meanings for device status.

| LED1 status (orange) | Meaning          |
| :------------------: | ---------------- |
| `ON`                 | Device suspended |
| `OFF`                | Device suspended |
| `FLASH` (slow)       | Device heartbeat |
| `FLASH` (fast)       | n/a              |

| LED2 status (orange) | Meaning                     |
| :------------------: | --------------------------- |
| `ON`                 | Peak SPL threshold detect   |
| `OFF`                |                             |
| `FLASH` (slow)       |                             |
| `FLASH` (fast)       | Save SPL sample in progress |

| LED3 status (green) | Meaning                       |
| :-----------------: | ----------------------------- |
| `ON`                | USB data transfer in progress |
| `OFF`               |                               |
| `FLASH` (slow)      |                               |
| `FLASH` (fast)      |                               |

| LED4 status (red) | Meaning                   |
| :---------------: | ------------------------- |
| `ON`              | No micro-SD card detected |
| `OFF`             |                           |
| `FLASH` (slow)    |                           |
| `FLASH` (fast)    |                           |