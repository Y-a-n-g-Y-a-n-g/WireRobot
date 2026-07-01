# Example Build And Hardware Test Report

## Scope

This report records the examples selected for the Portenta H7 and Nicla Vision bring-up, the Arduino environment used to compile them, and the hardware observations made during upload and serial testing.

Temporary build outputs and raw logs are stored under `test/`. That folder is not required for a clean handoff package.

## Environment

| Item | Value |
| --- | --- |
| Arduino IDE path | `D:\software\Arduino IDE` |
| Arduino CLI version | 1.5.0 |
| Portenta core | `arduino:mbed_portenta@4.6.0` |
| Nicla core | `arduino:mbed_nicla@4.6.0` |
| Portenta FQBN | `arduino:mbed_portenta:envie_m7` |
| Nicla FQBN | `arduino:mbed_nicla:nicla_vision` |
| Extra libraries | `STM32duino LSM6DSOX`, `WiFiNINA`, `Arduino_SpiNINA` |

## Portenta H7 Official Examples

| Example | Local path | Compile | Upload/run result |
| --- | --- | --- | --- |
| Blink | `examples/official-portenta-h7/Blink` | Passed | Compile baseline only |
| STM32H747_getResetReason | `examples/official-portenta-h7/STM32H747_getResetReason` | Passed | Uploaded; USB serial printed `Reset Reason Software` |
| SDRAM_operations | `examples/official-portenta-h7/SDRAM_operations` | Passed | Uploaded; USB serial printed `Oops, too big :)` followed by `SDRAM completely functional` |
| ScanNetworksAdvanced | `examples/official-portenta-h7/ScanNetworksAdvanced` | Passed | Uploaded; detected one WPA2 network; SSID, BSSID, and MAC values were redacted from the retained report |

## Nicla Vision Official Examples

| Example | Local path | Compile | Upload/run result |
| --- | --- | --- | --- |
| Blink | `examples/official-nicla-vision/Blink` | Passed | Compile baseline only |
| PDMSerialPlotter | `examples/official-nicla-vision/PDMSerialPlotter` | Passed | Uploaded; USB serial streamed numeric microphone samples |
| CameraCaptureRawBytes | `examples/official-nicla-vision/CameraCaptureRawBytes` | Passed | Uploaded; host sent the sync byte and received camera frame bytes |
| NiclaVision_MLC_Motion_Intensity | `examples/official-nicla-vision/NiclaVision_MLC_Motion_Intensity` | Passed after installing `STM32duino LSM6DSOX` | Uploaded; startup text was not captured because the example does not wait for USB serial |

## Project UART Examples

| Example | Board | Purpose | Compile | Upload/run result |
| --- | --- | --- | --- | --- |
| PortentaH7_UART_Receiver | Portenta H7 | Reads `Serial1`, counts bytes and lines, prints received payloads to USB serial | Passed | Uploaded and verified through Portenta USB serial |
| NiclaVision_Camera_UART_Sender | Nicla Vision | Captures camera frames, calculates brightness summaries, sends JSON over `Serial1` | Passed | Uploaded and verified through Portenta receiver after Nicla USB was disconnected |

## Final UART Test Evidence

The successful UART test used the following condition:

- Portenta H7 connected to the PC over USB.
- Nicla Vision powered from Portenta +5V and GND.
- Nicla Vision USB disconnected after its sender sketch had been uploaded.
- Four-wire connection kept in place: +5V, GND, Portenta TX to Nicla RX, Portenta RX to Nicla TX.

Representative Portenta serial output:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_RX {"source":"nicla_vision","seq":32,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":136,"bright_samples":1019}
UART_STATUS lines=33 bytes=3936
```

The 60-second retest reached receive counters above `lines=88` and `bytes=10677`, confirming sustained one-way communication from Nicla Vision to Portenta H7.

## Camera Capture Evidence

Two camera capture checks were performed:

| Capture | Resolution | Expected byte count | Result |
| --- | --- | --- | --- |
| Official raw-byte capture | QVGA, 320 x 240, RGB565 | 153600 bytes | A large binary payload was captured and converted for visual inspection |
| Chunked diagnostic capture | QQVGA, 160 x 120, RGB565 | 38400 bytes | Complete 38400-byte frames were captured repeatedly after sending data in smaller serial chunks |

The image conversion result used for the documentation preview is stored as `docments/assets/nicla-vision-camera-preview.png`. Details are in [Nicla Vision camera capture notes](camera-capture.md).

## Issues Found And Resolution

| Issue | Impact | Resolution |
| --- | --- | --- |
| Windows DFU interface did not have the required WinUSB driver on first upload | Upload failed before the sketch could be written | Installed the signed INF files from the Arduino board packages using `pnputil` |
| Nicla Vision UART output was not seen by Portenta while both USB cables were connected | Portenta receiver counters stayed at zero | Retested with Nicla USB disconnected and Nicla powered from Portenta +5V; UART output was then received |
| Nicla Vision motion-classifier example needed an external sensor library | Compile failed on a clean library setup | Installed `STM32duino LSM6DSOX` |
| Full-frame serial image capture can lose bytes if streamed in one large burst | Incomplete raw image file on the host | Used smaller serial chunks and flushes for diagnostic image capture |

## Excluded Example

The official Nicla Vision `CameraMotionDetect` example was not selected because the installed example source contains an explicit compile-time stop for `ARDUINO_NICLA_VISION`. The tested camera path instead uses `CameraCaptureRawBytes` and the final UART sender.
