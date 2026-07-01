# Arduino IDE Setup Guide

This guide describes how to prepare Arduino IDE for the Portenta H7 and Nicla Vision examples in this repository.

## Required Software

| Item | Requirement |
| --- | --- |
| Arduino IDE | Arduino IDE 2.x |
| Portenta board package | Arduino Mbed OS Portenta Boards |
| Nicla board package | Arduino Mbed OS Nicla Boards |
| Required library | `STM32duino LSM6DSOX` |
| Optional library dependency | `WiFiNINA` |

The development PC used `D:\software\Arduino IDE`. If Arduino IDE is installed elsewhere, replace the path in the command examples.

## Install Arduino IDE

1. Download Arduino IDE from <https://www.arduino.cc/en/software>.
2. Install Arduino IDE.
3. Start Arduino IDE.
4. Connect the Portenta H7 and Nicla Vision to the PC by USB.

## Install Board Packages

1. Open `Tools > Board > Boards Manager`.
2. Search for `Arduino Mbed OS Portenta Boards`.
3. Install the package.
4. Search for `Arduino Mbed OS Nicla Boards`.
5. Install the package.

## Install Libraries

1. Open `Tools > Manage Libraries`.
2. Search for `STM32duino LSM6DSOX`.
3. Install the library.
4. Search for `WiFiNINA`.
5. Install it if it is not already present.

`STM32duino LSM6DSOX` is required by the Nicla Vision motion-classifier example.

## Select Boards

| Board | Arduino IDE board menu | FQBN |
| --- | --- | --- |
| Portenta H7 | `Tools > Board > Arduino Mbed OS Portenta Boards > Arduino Portenta H7` | `arduino:mbed_portenta:envie_m7` |
| Nicla Vision | `Tools > Board > Arduino Mbed OS Nicla Boards > Arduino Nicla Vision` | `arduino:mbed_nicla:nicla_vision` |

After selecting the board, select the corresponding USB port from `Tools > Port`.

## Check Ports From PowerShell

Arduino boards can receive a new COM number after bootloader upload. Check the current port before uploading:

```powershell
& "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" board list
```

## Command-Line Setup

The same environment can be prepared with Arduino CLI:

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli core update-index
& $cli core install arduino:mbed_portenta
& $cli core install arduino:mbed_nicla
& $cli lib install "STM32duino LSM6DSOX"
& $cli lib install "WiFiNINA"
& $cli board list
```

## Compile The UART Sketches

Run these commands from the repository root:

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\uart-communication\PortentaH7_UART_Receiver
& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\uart-communication\NiclaVision_Camera_UART_Sender
```

## Upload The UART Sketches

Replace `COM15` and `COM17` with the current ports shown by Arduino IDE or `arduino-cli board list`.

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli upload -p COM15 --fqbn arduino:mbed_portenta:envie_m7 examples\uart-communication\PortentaH7_UART_Receiver
& $cli upload -p COM17 --fqbn arduino:mbed_nicla:nicla_vision examples\uart-communication\NiclaVision_Camera_UART_Sender
```

## Compile Official Examples

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\official-portenta-h7\Blink
& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\official-portenta-h7\STM32H747_getResetReason
& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\official-portenta-h7\SDRAM_operations
& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\official-portenta-h7\ScanNetworksAdvanced

& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\official-nicla-vision\Blink
& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\official-nicla-vision\PDMSerialPlotter
& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\official-nicla-vision\CameraCaptureRawBytes
& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\official-nicla-vision\NiclaVision_MLC_Motion_Intensity
```

## Serial Monitor

| Use | Board connected to PC | Baud rate | Expected output |
| --- | --- | --- | --- |
| UART receiver | Portenta H7 | 115200 | `UART_RX ...` and `UART_STATUS ...` |
| Nicla sender debug | Nicla Vision | 115200 | `UART_TX status=...` and frame summary debug text |
| Official examples | Target board | Match the sketch | Example-specific serial output |

For the final UART check, disconnect Nicla Vision USB after uploading its sketch and monitor only the Portenta H7 serial port.

## DFU Driver Troubleshooting On Windows

If upload fails with a DFU or `LIBUSB_ERROR_NOT_FOUND` message, install the WinUSB driver INF files included with the Arduino board packages:

```powershell
pnputil /add-driver "$env:LOCALAPPDATA\Arduino15\packages\arduino\hardware\mbed_portenta\4.6.0\drivers\portentah7.inf" /install
pnputil /add-driver "$env:LOCALAPPDATA\Arduino15\packages\arduino\hardware\mbed_nicla\4.6.0\drivers\niclavision.inf" /install
```

Update the version folder if a different board package version is installed.
