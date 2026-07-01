# Arduino IDE Setup

## Tested Local Toolchain

| Tool | Version or path |
| --- | --- |
| Arduino IDE | `D:\software\Arduino IDE` |
| Arduino CLI | `D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe` |
| Arduino CLI version | 1.5.0 |
| Portenta board package | `arduino:mbed_portenta@4.6.0` |
| Nicla board package | `arduino:mbed_nicla@4.6.0` |
| Extra libraries installed | `STM32duino LSM6DSOX@2.3.4`, `WiFiNINA@2.1.0`, `Arduino_SpiNINA@0.0.2` |

## Install From Download

1. Download Arduino IDE from <https://www.arduino.cc/en/software>.
2. Install it. The tested path is `D:\software\Arduino IDE`.
3. Connect the Portenta H7 and Nicla Vision by USB.
4. Open Arduino IDE.
5. Open `Tools > Board > Boards Manager`.
6. Search for and install `Arduino Mbed OS Portenta Boards`.
7. Search for and install `Arduino Mbed OS Nicla Boards`.
8. Open `Tools > Manage Libraries`.
9. Install `STM32duino LSM6DSOX`.
10. Install `WiFiNINA` if it is not already present.
11. Select the target board and USB port before each upload.

## Board Selection

| Board | IDE board menu | FQBN |
| --- | --- | --- |
| Portenta H7 | `Tools > Board > Arduino Mbed OS Portenta Boards > Arduino Portenta H7` | `arduino:mbed_portenta:envie_m7` |
| Nicla Vision | `Tools > Board > Arduino Mbed OS Nicla Boards > Arduino Nicla Vision` | `arduino:mbed_nicla:nicla_vision` |

COM ports can change after a DFU upload. Use `Tools > Port` in Arduino IDE or this PowerShell command:

```powershell
& "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" board list
```

## CLI Setup Commands

The same setup can be reproduced from PowerShell:

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli core update-index
& $cli core install arduino:mbed_portenta
& $cli core install arduino:mbed_nicla
& $cli lib install "STM32duino LSM6DSOX"
& $cli lib install "WiFiNINA"
& $cli board list
```

## Windows DFU Driver Fix

On the tested PC, early uploads failed because Windows had not bound the Arduino DFU interfaces to WinUSB. The observed Portenta error included:

```text
Cannot open DFU device 2341:035b ... LIBUSB_ERROR_NOT_FOUND
```

Install the driver INF files included with the Arduino board packages:

```powershell
pnputil /add-driver "$env:LOCALAPPDATA\Arduino15\packages\arduino\hardware\mbed_portenta\4.6.0\drivers\portentah7.inf" /install
pnputil /add-driver "$env:LOCALAPPDATA\Arduino15\packages\arduino\hardware\mbed_nicla\4.6.0\drivers\niclavision.inf" /install
```

After this driver binding, uploads through Arduino's `dfu-util` flow succeeded for both boards.

## Compile Selected Official Examples

Run these commands from the package root:

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

## Compile Final UART Sketches

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli compile --fqbn arduino:mbed_portenta:envie_m7 examples\uart-communication\PortentaH7_UART_Receiver
& $cli compile --fqbn arduino:mbed_nicla:nicla_vision examples\uart-communication\NiclaVision_Camera_UART_Sender
```

## Upload Final UART Sketches

Adjust the COM ports to match `arduino-cli board list`.

```powershell
$cli = "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli upload -p COM15 --fqbn arduino:mbed_portenta:envie_m7 examples\uart-communication\PortentaH7_UART_Receiver
& $cli upload -p COM17 --fqbn arduino:mbed_nicla:nicla_vision examples\uart-communication\NiclaVision_Camera_UART_Sender
```

## Serial Monitor Settings

| Use | Board connected to PC | Baud rate | Expected output |
| --- | --- | --- | --- |
| Portenta UART receiver | Portenta H7 | 115200 | `UART_RX ...` and `UART_STATUS ...` |
| Nicla sender debug | Nicla Vision | 115200 | `UART_TX status=...` and `Frame summary sent...` |
| Official examples | Target board | Match the sketch | Example-specific serial output |

For the final board-to-board UART validation, disconnect Nicla USB after upload and monitor only the Portenta serial port.
