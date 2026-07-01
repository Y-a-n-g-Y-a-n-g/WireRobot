# WireRobot Arduino Board Setup

This repository provides the Arduino setup package for a two-board WireRobot development platform using:

- Arduino Portenta H7 as the host-side receiver board
- Arduino Nicla Vision as the camera and sensor processing board
- A four-wire UART connection between the two boards

The included sketches configure the Nicla Vision to capture camera frames, calculate a compact brightness summary, and send the result to the Portenta H7 as newline-delimited JSON. The Portenta H7 receives the UART stream and prints it to the USB serial monitor.

## Repository Layout

| Path | Description |
| --- | --- |
| `docments/` | Hardware references, setup guides, wiring guide, and example catalog |
| `docments/assets/` | Pinout previews, datasheets, schematics, and camera preview asset |
| `examples/official-portenta-h7/` | Selected official examples for Portenta H7 reference testing |
| `examples/official-nicla-vision/` | Selected official examples for Nicla Vision reference testing |
| `examples/uart-communication/` | Working UART sender and receiver sketches for the two-board setup |
| `test/` | Optional local build logs and temporary diagnostics; not required for normal use |

## Documentation

- [Portenta H7 hardware reference](docments/portenta-h7.md)
- [Nicla Vision hardware reference](docments/nicla-vision.md)
- [Arduino IDE setup guide](docments/arduino-ide-setup.md)
- [UART integration guide](docments/uart-communication.md)
- [Camera preview and data format](docments/camera-capture.md)
- [Example catalog](docments/example-catalog.md)
- [Release package checklist](docments/handoff-checklist.md)

## Hardware Connection

| Portenta H7 | Nicla Vision | Function |
| --- | --- | --- |
| +5V | J2-9 VIN | Powers Nicla Vision from Portenta H7 |
| GND | J2-6 GND | Common ground |
| D14 | J2-4 UART_RX | Portenta TX to Nicla RX |
| D13 | J2-3 UART_TX | Portenta RX from Nicla TX |

The UART link uses `Serial1` on both boards at 115200 baud, 8N1.

## Quick Start

1. Install Arduino IDE and the required board packages using [Arduino IDE setup guide](docments/arduino-ide-setup.md).
2. Wire the boards according to the table above.
3. Upload `examples/uart-communication/PortentaH7_UART_Receiver` to the Portenta H7.
4. Upload `examples/uart-communication/NiclaVision_Camera_UART_Sender` to the Nicla Vision.
5. Disconnect the Nicla Vision USB cable.
6. Keep the Portenta H7 connected to the PC by USB.
7. Open the Portenta H7 serial monitor at 115200 baud.
8. Confirm that `UART_RX` JSON lines are printed.

Expected output:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_STATUS lines=33 bytes=3936
```

## Final Sketches

| Board | Sketch | Role |
| --- | --- | --- |
| Portenta H7 | `examples/uart-communication/PortentaH7_UART_Receiver` | Receives UART lines and prints them over USB serial |
| Nicla Vision | `examples/uart-communication/NiclaVision_Camera_UART_Sender` | Captures camera frames and sends processed JSON results over UART |

## Reference Links

- [Arduino Portenta H7 product page](https://store-usa.arduino.cc/products/portenta-h7)
- [Arduino Portenta H7 hardware documentation](https://docs.arduino.cc/hardware/portenta-h7/)
- [Arduino Nicla Vision product page](https://store.arduino.cc/products/nicla-vision)
- [Arduino Nicla Vision hardware documentation](https://docs.arduino.cc/hardware/nicla-vision/)

Local copies of official pinout, datasheet, and schematic PDFs are available in `docments/assets/`.
