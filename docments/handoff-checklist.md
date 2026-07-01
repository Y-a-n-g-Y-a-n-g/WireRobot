# Release Package Checklist

Use this checklist before sharing the repository or a release archive with another developer.

## Required Content

| Item | Path |
| --- | --- |
| Main repository guide | `readme.md` |
| Hardware and setup documentation | `docments/*.md` |
| Official reference PDFs and previews | `docments/assets/` |
| Selected official examples | `examples/official-portenta-h7/`, `examples/official-nicla-vision/` |
| Final UART examples | `examples/uart-communication/` |

## Optional Local Content

| Item | Use |
| --- | --- |
| `test/` | Build logs, raw serial captures, temporary diagnostics, and development-only files |

The `test/` folder is useful during development but is not required for normal setup or operation.

## Expected Firmware

| Board | Sketch |
| --- | --- |
| Portenta H7 | `examples/uart-communication/PortentaH7_UART_Receiver` |
| Nicla Vision | `examples/uart-communication/NiclaVision_Camera_UART_Sender` |

## Release Check

1. Confirm that the two UART sketches compile with the documented Arduino board packages.
2. Confirm wiring: Portenta +5V to Nicla VIN, GND to GND, D14 to Nicla UART_RX, D13 to Nicla UART_TX.
3. Connect Portenta H7 to the PC by USB.
4. Leave Nicla Vision USB disconnected for the UART check.
5. Open the Portenta serial monitor at 115200 baud.
6. Confirm that `UART_RX` JSON lines appear.
7. Confirm that the `UART_STATUS` counters increase.

Known-good output:

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_STATUS lines=33 bytes=3936
```

## Documentation Check

- Links in `readme.md` should point to existing files.
- COM ports should be treated as local examples, not fixed requirements.
- Local network identifiers should not be committed in logs or documents.
- Official PDFs and pinout images should remain in `docments/assets/`.
