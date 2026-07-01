# Camera Preview And Data Format

![Nicla Vision camera capture preview](assets/nicla-vision-camera-preview.png)

## Purpose

This page explains the Nicla Vision camera data format used by the UART sender sketch and provides a preview image from a diagnostic capture. The final UART sketch does not transmit full images. It captures frames locally and sends only a compact processed result to the Portenta H7.

## Camera Configuration

| Use | Resolution | Format | Frame size |
| --- | --- | --- | --- |
| Final UART sender | QVGA, 320 x 240 | RGB565 | 153600 bytes |
| Diagnostic preview capture | QQVGA, 160 x 120 | RGB565 | 38400 bytes |

Frame-size calculations:

```text
320 x 240 x 2 bytes = 153600 bytes
160 x 120 x 2 bytes = 38400 bytes
```

The preview image was converted from a complete 38400-byte QQVGA RGB565 capture. The raw byte stream is interpreted as big-endian RGB565 during conversion.

## UART Sender Output

The final Nicla Vision sender captures a QVGA RGB565 frame, samples the frame, and sends a JSON summary:

```json
{"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
```

| Field | Meaning |
| --- | --- |
| `source` | Fixed board identifier |
| `seq` | Incrementing frame counter |
| `status` | Frame or system status |
| `frame_bytes` | Size of the captured camera frame |
| `samples` | Number of sampled pixels used for the summary |
| `avg_luma` | Average sampled luminance, 0 to 255 |
| `bright_samples` | Count of sampled pixels above the brightness threshold |

## Diagnostic Capture Method

The preview image can be reproduced with a temporary USB serial capture sketch:

1. Configure the Nicla Vision camera for QQVGA RGB565.
2. Wait for a host sync byte over USB serial.
3. Capture one frame.
4. Send the frame buffer in small chunks.
5. Convert the received RGB565 bytes to PNG.

Small chunks are recommended for full-frame serial capture. A single large serial write can cause incomplete host-side captures.

## When To Use Full-Frame Capture

Use full-frame capture for camera orientation checks, lighting checks, and image-processing development. Use the final UART sender when the Portenta H7 only needs the processed result.
