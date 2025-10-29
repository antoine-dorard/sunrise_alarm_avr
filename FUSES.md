// Fuses settings for ATmega328P
# Fuses settings for ATmega328P
Default fuse settings:
E: 0xFF
H: 0xD9
L: 0x62

Current fuse settings:
E: 0xFF
H: 0xD9
L: 0x62


## Extended fuse table:
| Extended Fuse Byte | Bit No | Description | Default Value | Actual Value |
|-------|---------------------------------|----------------|----------------|--------------|
| -         | 7   | -                                | 1 | 1 |
| -         | 6   | -                                | 1 | 1 |
| -         | 5   | -                                | 1 | 1 |
| -         | 4   | -                                | 1 | 1 |
| -         | 3   | -                                | 1 | 1 |
| BODLEVEL2 | 2   | Brown-out Detector trigger level | 1 | 1 |
| BODLEVEL1 | 1   | Brown-out Detector trigger level | 1 | 1 |
| BODLEVEL0 | 0   | Brown-out Detector trigger level | 1 | 1 |

## High fuse table:
| High Fuse Byte | Bit No | Description | Default Value | Actual Value |
|-------|---------------------------------|----------------|----------------|--------------|
| RSTDISBL | 7   | External Reset Disable         | 1 | 1 |
| DWEN     | 6   | Debug Wire Enable              | 1 | 1 |
| SPIEN    | 5   | SPI Programming Enable         | 0 | 0 |
| WDTON    | 4   | Watchdog Timer Always On       | 1 | 1 |
| EESAVE   | 3   | EEPROM Memory Preserve through Chip Erase | 1 | 1 |
| BOOTSZ1  | 2   | Boot Size                      | 0 | 0 |
| BOOTSZ0  | 1   | Boot Size                      | 0 | 0 |
| BOOTRST  | 0   | Boot Reset Vector Enabled      | 1 | 1 |

## Low fuse table:
| Low Fuse Byte | Bit No | Description | Default Value | Actual Value |
|-------|---------------------------------|----------------|----------------|--------------|
| CKDIV8   | 7   | Clock Division Factor          | 0 | 0 |
| CKOUT    | 6   | Clock Output                   | 1 | 1 |
| SUT1     | 5   | Start-up Time Bit 1            | 1 | 1 |
| SUT0     | 4   | Start-up Time Bit 0            | 0 | 0 |
| CKSEL3   | 3   | Clock Select Bit 3             | 0 | 0 |
| CKSEL2   | 2   | Clock Select Bit 2             | 0 | 0 |
| CKSEL1   | 1   | Clock Select Bit 1             | 1 | 1 |
| CKSEL0   | 0   | Clock Select Bit 0             | 0 | 0 |


# List of fuse settings that differ from the default values:
### Full Swing Crystal Oscillator, 16.0 MHz, Start-up time: 16K CK + 65 ms
- CKDIV8: 0 (Default: 0): Keep clock division enabled (0 means programmed)
- CKOUT: 1 (Default: 1)
- SUT1: 1 (Default: 1)
- SUT0: 1 (Default: 0)
- CKSEL3: 0 (Default: 0)
- CKSEL2: 1 (Default: 0)
- CKSEL1: 1 (Default: 1)
- CKSEL0: 1 (Default: 0)

Resulting hex values:
- Low fuse: 0x77 - 01110111
- High fuse: 0xD9 - 11011001
- Extended fuse: 0xFF - 11111111

# Command to set the fuses:
```bash
avrdude -c usbasp -p m328p -U lfuse:w:0x77:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
```