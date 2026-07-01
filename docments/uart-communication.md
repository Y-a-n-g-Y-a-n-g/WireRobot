# UART Integration Guide

## Objective

The UART link sends processed Nicla Vision camera results directly to the Portenta H7 over a four-wire power and signal connection. The Nicla Vision performs the frame capture and brightness summary. The Portenta H7 receives the result lines and exposes them to the PC over USB serial.

## Wiring

| Portenta H7 | Nicla Vision | Purpose |
| --- | --- | --- |
| +5V | J2-9 VIN | Nicla Vision power input |
| GND | J2-6 GND | Common ground |
| D14 | J2-4 UART_RX | Portenta `Serial1` TX to Nicla `Serial1` RX |
| D13 | J2-3 UART_TX | Portenta `Serial1` RX from Nicla `Serial1` TX |

This connection crosses TX to RX and RX to TX. Both boards must share GND.

## Pin Mapping Confirmed From Arduino Cores

| Board | UART object | TX pin | RX pin |
| --- | --- | --- | --- |
| Portenta H7 | `Serial1` | D14 | D13 |
| Nicla Vision | `Serial1` | D1 / J2-3 | D2 / J2-4 |

## UART Settings

| Setting | Value |
| --- | --- |
| Baud rate | 115200 |
| Data format | 8 data bits, no parity, 1 stop bit |
| Active direction | Nicla Vision to Portenta H7 |
| Payload framing | One JSON object per line, terminated by `\n` |
| Portenta monitor baud | 115200 |

## Sketches

| Board | Sketch |
| --- | --- |
| Portenta H7 | `examples/uart-communication/PortentaH7_UART_Receiver/PortentaH7_UART_Receiver.ino` |
| Nicla Vision | `examples/uart-communication/NiclaVision_Camera_UART_Sender/NiclaVision_Camera_UART_Sender.ino` |

## Nicla Processing Payload

The Nicla sketch captures QVGA RGB565 camera frames at 320 x 240. The full frame buffer is 153600 bytes. The sketch samples one pixel every 16 pixels, calculates an average luminance, counts bright samples, and sends a compact summary:

```json
{"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
```

Payload fields:

| Field | Meaning |
| --- | --- |
| `source` | Fixed source name, `nicla_vision` |
| `seq` | Incrementing frame sequence number |
| `status` | `ok`, `boot`, `camera_ready`, or an error status |
| `frame_bytes` | Camera frame buffer size in bytes |
| `samples` | Number of pixels sampled for the brightness summary |
| `avg_luma` | Average sampled luminance, 0 to 255 |
| `bright_samples` | Number of sampled pixels with luminance above the sketch threshold |

Replace the brightness summary with the final application result when the production image-processing algorithm is selected.

## Upload Procedure

1. Connect both boards to the PC by USB.
2. Confirm current COM ports with `arduino-cli board list`.
3. Upload `PortentaH7_UART_Receiver` to the Portenta H7.
4. Upload `NiclaVision_Camera_UART_Sender` to the Nicla Vision.
5. Disconnect the Nicla Vision USB cable.
6. Keep Portenta H7 connected to the PC by USB.
7. Keep the four-wire board-to-board connection in place.
8. Open the Portenta serial monitor at 115200 baud.

## Expected Portenta Output

At startup:

```text
Portenta H7 UART receiver ready
Listening on Serial1: D13/RX, D14/TX at 115200 baud
```

During a working link:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_RX {"source":"nicla_vision","seq":32,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":136,"bright_samples":1019}
UART_STATUS lines=33 bytes=3936
```

`UART_STATUS` prints every five seconds. Both counters should increase while the Nicla sender is running.

## Known-Good Result

With Nicla USB disconnected and Portenta USB connected, the Portenta receiver should print continuous `UART_RX` JSON lines from the Nicla Vision camera sender. A stable link keeps increasing both the line counter and byte counter in `UART_STATUS`.

## Troubleshooting

| Symptom | Check |
| --- | --- |
| `UART_STATUS lines=0 bytes=0` | Disconnect Nicla USB, keep Portenta USB connected, and confirm Nicla is powered from Portenta +5V |
| No Portenta serial output | Confirm the Portenta runtime COM port and reopen the serial monitor at 115200 baud |
| Upload fails with DFU errors | Install the board-package INF files described in [Arduino IDE setup](arduino-ide-setup.md) |
| Garbled payloads | Confirm both sketches use 115200 baud and that TX/RX are crossed correctly |
| Camera error status | Confirm the Nicla camera initializes and that the final sender sketch is loaded |

## Integration Notes

- The current payload is an example result format, not a final perception algorithm.
- JSON over UART is easy to inspect during setup. If throughput becomes important, the same wiring can carry a smaller binary packet later.
- The Portenta receiver currently forwards received lines to USB serial. A later application can parse the JSON and act on the result locally.
