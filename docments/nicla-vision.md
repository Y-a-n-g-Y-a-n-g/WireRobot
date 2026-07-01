# Nicla Vision Hardware Reference

![Nicla Vision pinout](assets/nicla-vision-pinout-preview.png)

## Role In This Repository

The Nicla Vision is the sensing and processing board for the two-board setup. Its final sketch captures camera frames, computes a compact brightness summary, and sends newline-terminated JSON messages to the Portenta H7 through `Serial1`.

## Main Hardware

| Area | Details |
| --- | --- |
| Board variant used | Nicla Vision, SKU ABX00051 |
| Microcontroller | ST STM32H747 dual-core microcontroller |
| High-performance core | Arm Cortex-M7 up to 480 MHz, double-precision FPU, L1 cache |
| Secondary core | Arm Cortex-M4 up to 240 MHz, FPU |
| Wireless module | Murata 1DX Wi-Fi and Bluetooth |
| Camera | GC2145 2 MP color camera |
| IMU | LSM6DSOX 6-axis accelerometer and gyroscope |
| Microphone | MP34DT06JTR digital MEMS microphone |
| Distance sensor | VL53L1 time-of-flight sensor |
| External flash | 16 MB serial flash |
| Security device | NXP SE050C2 secure element |
| Power management | On-board PMIC with battery support |
| Arduino FQBN | `arduino:mbed_nicla:nicla_vision` |

## Interfaces Used By The Examples

| Interface | Use in this repository |
| --- | --- |
| USB CDC | Upload, serial monitor, and diagnostic camera capture |
| `Serial1` UART | Sends camera summary JSON to Portenta H7 |
| Camera | Used by the official raw capture example and final UART sender |
| PDM microphone | Used by the official serial plotter example |
| IMU classifier | Used by the motion-intensity example |
| Wi-Fi and distance sensor | Available on board for future expansion |

## UART And Power Pins Used

The installed Arduino Mbed Nicla core maps `Serial1` to the J2 header pins below:

| Nicla Vision pin | Arduino pin | Function | Connected to |
| --- | --- | --- | --- |
| J2-3 | D1 | `Serial1` TX / UART_TX | Portenta D13 RX |
| J2-4 | D2 | `Serial1` RX / UART_RX | Portenta D14 TX |
| J2-9 | VIN | Power input | Portenta +5V |
| J2-6 | GND | Common reference | Portenta GND |

The final sender sketch enables the external I/O rail before starting UART:

```cpp
Nicla_Vision.begin();
Nicla_Vision.enable3V3VDDIO_EXT();
Serial1.begin(115200);
```

This is required for reliable external header signaling with the J2 UART connection.

## Arduino IDE Configuration

| Item | Value |
| --- | --- |
| Board package | Arduino Mbed OS Nicla Boards |
| Board package version used | `arduino:mbed_nicla@4.6.0` |
| IDE board selection | `Tools > Board > Arduino Mbed OS Nicla Boards > Arduino Nicla Vision` |
| Example local USB ports | COM13 initially, COM17 after DFU upload |
| Upload transport | USB DFU through Arduino tooling |

If Windows shows a DFU driver error during upload, bind the Arduino WinUSB driver:

```powershell
pnputil /add-driver "$env:LOCALAPPDATA\Arduino15\packages\arduino\hardware\mbed_nicla\4.6.0\drivers\niclavision.inf" /install
```

## Included Examples

| Example | Location | Result |
| --- | --- | --- |
| Blink | `examples/official-nicla-vision/Blink` | Basic board package check |
| PDMSerialPlotter | `examples/official-nicla-vision/PDMSerialPlotter` | Microphone serial plotter example |
| CameraCaptureRawBytes | `examples/official-nicla-vision/CameraCaptureRawBytes` | USB serial raw camera capture example |
| NiclaVision_MLC_Motion_Intensity | `examples/official-nicla-vision/NiclaVision_MLC_Motion_Intensity` | IMU motion-classifier example |
| NiclaVision_Camera_UART_Sender | `examples/uart-communication/NiclaVision_Camera_UART_Sender` | Final UART sender for this repository |

## Camera Data Notes

The camera examples use RGB565 frame buffers. A full QVGA frame from the final sender is 320 x 240 x 2 bytes, so the JSON payload includes `frame_bytes` as `153600`. The diagnostic QQVGA preview capture uses 160 x 120 x 2 bytes, or 38400 bytes per frame.

The captured image preview is documented in [Camera preview and data format](camera-capture.md).

## Operating Notes

- Recheck the COM port after every upload. The Nicla Vision can re-enumerate when moving between bootloader and runtime modes.
- For the final UART link check, disconnect Nicla USB after uploading the sender sketch. Leave only Portenta USB connected to the PC and power Nicla from the Portenta +5V pin.
- The final sender prints local USB debug messages when Nicla USB is connected, but the board-to-board link uses the `Serial1` UART path.
- The motion-classifier example needs `STM32duino LSM6DSOX` installed manually on a clean Arduino setup.

## Official Resources

- Product page: <https://store.arduino.cc/products/nicla-vision>
- Hardware documentation: <https://docs.arduino.cc/hardware/nicla-vision/>
- Pinout PDF: <https://docs.arduino.cc/resources/pinouts/ABX00051-full-pinout.pdf>
- Datasheet PDF: <https://docs.arduino.cc/resources/datasheets/ABX00051-datasheet.pdf>
- Schematics PDF: <https://docs.arduino.cc/resources/schematics/ABX00051-schematics.pdf>

Local copies are stored in `docments/assets`.
