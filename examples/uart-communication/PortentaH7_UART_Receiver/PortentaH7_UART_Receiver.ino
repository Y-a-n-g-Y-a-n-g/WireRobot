const uint32_t USB_BAUD = 115200;
const uint32_t UART_BAUD = 115200;

uint32_t receivedLines = 0;
uint32_t receivedBytes = 0;
unsigned long lastStatusMs = 0;
String lineBuffer;

void setup() {
  Serial.begin(USB_BAUD);
  Serial1.begin(UART_BAUD);

  unsigned long startMs = millis();
  while (!Serial && (millis() - startMs < 5000)) {
  }

  Serial.println("Portenta H7 UART receiver ready");
  Serial.println("Listening on Serial1: D13/RX, D14/TX at 115200 baud");
}

void loop() {
  while (Serial1.available() > 0) {
    char c = (char)Serial1.read();
    receivedBytes++;

    if (c == '\n') {
      lineBuffer.trim();
      if (lineBuffer.length() > 0) {
        receivedLines++;
        Serial.print("UART_RX ");
        Serial.println(lineBuffer);
      }
      lineBuffer = "";
    } else if (c != '\r') {
      lineBuffer += c;
      if (lineBuffer.length() > 240) {
        Serial.print("UART_RX_TRUNCATED ");
        Serial.println(lineBuffer);
        lineBuffer = "";
      }
    }
  }

  if (millis() - lastStatusMs > 5000) {
    lastStatusMs = millis();
    Serial.print("UART_STATUS lines=");
    Serial.print(receivedLines);
    Serial.print(" bytes=");
    Serial.println(receivedBytes);
  }
}
