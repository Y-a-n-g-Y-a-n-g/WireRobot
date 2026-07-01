# Handoff Checklist

## Include

| Item | Path |
| --- | --- |
| Package overview | `readme.md` |
| Board and interface documents | `docments/*.md` |
| Official reference assets | `docments/assets/*pinout*`, `docments/assets/*datasheet*`, `docments/assets/*schematics*` |
| Camera preview asset | `docments/assets/nicla-vision-camera-preview.png` |
| Selected official examples | `examples/official-portenta-h7/`, `examples/official-nicla-vision/` |
| Final UART sketches | `examples/uart-communication/` |

## Exclude For Clean External Delivery

| Item | Reason |
| --- | --- |
| `test/` | Temporary build directories, raw logs, capture experiments, and diagnostic sketches |

## Final Firmware State

| Board | Expected sketch |
| --- | --- |
| Portenta H7 | `examples/uart-communication/PortentaH7_UART_Receiver` |
| Nicla Vision | `examples/uart-communication/NiclaVision_Camera_UART_Sender` |

## Retest Before Delivery

1. Confirm wiring: Portenta +5V to Nicla VIN, GND to GND, D14 to Nicla UART_RX, D13 to Nicla UART_TX.
2. Connect Portenta H7 to the PC by USB.
3. Leave Nicla Vision USB disconnected for the UART validation.
4. Open Portenta serial monitor at 115200 baud.
5. Confirm `UART_RX` JSON lines appear.
6. Confirm `UART_STATUS` line and byte counters increase.

## Known Good UART Output

```text
UART_RX {"source":"nicla_vision","seq":31,"status":"ok","frame_bytes":153600,"samples":4800,"avg_luma":137,"bright_samples":1074}
UART_STATUS lines=33 bytes=3936
```

## Clean Package Check

Before sharing the package, verify that:

- No private Wi-Fi identifiers or MAC addresses are present in the delivered documents.
- COM ports are described as observed local values, not fixed requirements.
- The `test/` folder is excluded if only clean documentation and examples are needed.
- All Markdown links in `readme.md` point to existing files.
