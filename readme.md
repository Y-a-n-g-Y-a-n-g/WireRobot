# WireRobot Board Bring-Up Package

This package contains the board documentation, Arduino setup notes, selected official examples, and verified UART communication sketches for the Arduino Portenta H7 and Arduino Nicla Vision.

## Package Layout

| Path | Purpose |
| --- | --- |
| `readme.md` | Package entry point and quick handoff summary |
| `docments/` | Board notes, IDE setup, interface notes, and test reports |
| `docments/assets/` | Local copies of official pinout, datasheet, schematic, and preview assets |
| `examples/official-portenta-h7/` | Selected Portenta H7 official examples copied for reference |
| `examples/official-nicla-vision/` | Selected Nicla Vision official examples copied for reference |
| `examples/uart-communication/` | Final two-board UART sketches |
| `test/` | Temporary build logs, diagnostic sketches, and capture files; exclude this folder from external delivery if only clean handoff files are required |

The directory name `docments` is kept to match the requested handoff structure.

## Main Documents

- [Portenta H7 board notes](docments/portenta-h7.md)
- [Nicla Vision board notes](docments/nicla-vision.md)
- [Arduino IDE setup](docments/arduino-ide-setup.md)
- [Example build and hardware test report](docments/examples-test-report.md)
- [UART communication test](docments/uart-communication.md)
- [Nicla Vision camera capture notes](docments/camera-capture.md)
- [Handoff checklist](docments/handoff-checklist.md)

## Local Board Mapping

| Board | Initial USB port | Observed upload/runtime port | Arduino FQBN |
| --- | --- | --- | --- |
| Arduino Portenta H7 | COM12 | COM15 after DFU upload | `arduino:mbed_portenta:envie_m7` |
| Arduino Nicla Vision | COM13 | COM17 after DFU upload | `arduino:mbed_nicla:nicla_vision` |

Windows can assign a different COM number after a board enters or exits bootloader mode. Confirm the current port before upload:

```powershell
& "D:\software\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe" board list
```

## Validated State

The Arduino cores and required libraries are installed under the local Arduino IDE environment. Selected official examples compile for both boards. Portenta H7 reset reason, SDRAM, and Wi-Fi scan examples were uploaded and observed through USB serial. Nicla Vision PDM, camera raw-byte, and motion-classifier examples were uploaded or compiled as documented in the test report.

The final UART test uses the Nicla Vision as the sender and the Portenta H7 as the receiver:

- Portenta H7 sketch: `examples/uart-communication/PortentaH7_UART_Receiver`
- Nicla Vision sketch: `examples/uart-communication/NiclaVision_Camera_UART_Sender`
- UART rate: 115200 baud, 8N1
- Payload: newline-terminated JSON from Nicla Vision to Portenta H7
- Verified condition: Nicla Vision powered from Portenta H7, Nicla USB disconnected, Portenta USB connected to the PC

Example received line:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
```

## Wiring Used

| Portenta H7 | Nicla Vision | Purpose |
| --- | --- | --- |
| +5V | J2-9 VIN | Nicla power input |
| GND | J2-6 GND | Common ground |
| D14 | J2-4 UART_RX | Portenta `Serial1` TX to Nicla `Serial1` RX |
| D13 | J2-3 UART_TX | Portenta `Serial1` RX from Nicla `Serial1` TX |

## Quick Retest

1. Connect both boards by the four wires listed above.
2. Connect Portenta H7 to the PC by USB.
3. Keep Nicla Vision USB disconnected for the UART link test.
4. Open Portenta serial monitor at 115200 baud.
5. Confirm that `UART_RX` JSON lines appear and that the `UART_STATUS` counters increase.

Use [Arduino IDE setup](docments/arduino-ide-setup.md) for compile and upload commands if the firmware must be reloaded.

## Reference Sources

- [Arduino Portenta H7 product page](https://store-usa.arduino.cc/products/portenta-h7)
- [Arduino Portenta H7 hardware documentation](https://docs.arduino.cc/hardware/portenta-h7/)
- [Arduino Nicla Vision product page](https://store.arduino.cc/products/nicla-vision)
- [Arduino Nicla Vision hardware documentation](https://docs.arduino.cc/hardware/nicla-vision/)

Local copies of the official pinout, datasheet, and schematic PDFs are stored in `docments/assets`.
