# Example Catalog

This page lists the example sketches included in the repository and explains what each one is useful for. The official examples are included as reference checks for the board packages and built-in peripherals. The UART examples are the working application pair for the two-board setup.

## Environment

| Item | Value |
| --- | --- |
| Arduino IDE path used during setup | `D:\software\Arduino IDE` |
| Arduino CLI | Included with Arduino IDE |
| Portenta H7 board package | `arduino:mbed_portenta` |
| Nicla Vision board package | `arduino:mbed_nicla` |
| Extra libraries | `STM32duino LSM6DSOX`, `WiFiNINA`, `Arduino_SpiNINA` |

Use [Arduino IDE setup guide](arduino-ide-setup.md) to reproduce the environment on another PC.

## Portenta H7 Examples

| Example | Path | Purpose | Expected result |
| --- | --- | --- | --- |
| Blink | `examples/official-portenta-h7/Blink` | Basic board package and upload check | Compiles |
| STM32H747_getResetReason | `examples/official-portenta-h7/STM32H747_getResetReason` | Reads the STM32H747 reset reason through the Arduino system library | Prints the reset reason over USB serial |
| SDRAM_operations | `examples/official-portenta-h7/SDRAM_operations` | Checks external SDRAM access | Prints SDRAM functional status |
| ScanNetworksAdvanced | `examples/official-portenta-h7/ScanNetworksAdvanced` | Checks Wi-Fi scan functionality | Lists nearby networks |

## Nicla Vision Examples

| Example | Path | Purpose | Expected result |
| --- | --- | --- | --- |
| Blink | `examples/official-nicla-vision/Blink` | Basic board package and upload check | Compiles |
| PDMSerialPlotter | `examples/official-nicla-vision/PDMSerialPlotter` | Streams microphone samples to serial plotter | Streams numeric samples |
| CameraCaptureRawBytes | `examples/official-nicla-vision/CameraCaptureRawBytes` | Captures raw camera frame bytes over USB serial | Returns camera frame data after host sync |
| NiclaVision_MLC_Motion_Intensity | `examples/official-nicla-vision/NiclaVision_MLC_Motion_Intensity` | Demonstrates the LSM6DSOX motion classifier | Compiles after installing `STM32duino LSM6DSOX` |

## Two-Board UART Examples

| Example | Board | Path | Purpose |
| --- | --- | --- | --- |
| PortentaH7_UART_Receiver | Portenta H7 | `examples/uart-communication/PortentaH7_UART_Receiver` | Receives newline-delimited UART payloads and prints them to USB serial |
| NiclaVision_Camera_UART_Sender | Nicla Vision | `examples/uart-communication/NiclaVision_Camera_UART_Sender` | Captures QVGA camera frames, calculates a brightness summary, and sends JSON over UART |

## Known-Good UART Output

The receiver should print lines similar to the following:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_RX {"source":"nicla_vision","seq":32,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":136,"bright_samples":1019}
UART_STATUS lines=33 bytes=3936
```

The `frame_bytes` value is `153600` because the Nicla sender captures QVGA RGB565 frames:

```text
320 x 240 x 2 bytes = 153600 bytes
```

## Camera Preview

The Nicla Vision camera path can also be checked with a QQVGA capture. A complete 160 x 120 RGB565 frame contains 38400 bytes. The converted preview image is available at:

```text
docments/assets/nicla-vision-camera-preview.png
```

See [Camera preview and data format](camera-capture.md) for details.

## Notes

- The official Nicla Vision motion-classifier example requires `STM32duino LSM6DSOX`.
- Run the final UART check with Nicla Vision powered from Portenta H7 and with the Nicla USB cable disconnected.
- The official Nicla Vision `CameraMotionDetect` example is not included because the installed example source contains a compile-time stop for `ARDUINO_NICLA_VISION`.
