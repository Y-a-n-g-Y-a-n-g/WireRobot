#include "camera.h"
#include "gc2145.h"
#include "Nicla_Vision_System.h"

const uint32_t USB_BAUD = 115200;
const uint32_t UART_BAUD = 115200;
const unsigned long FRAME_PERIOD_MS = 1000;

GC2145 galaxyCore;
Camera cam(galaxyCore);
FrameBuffer frameBuffer;

bool cameraReady = false;
uint32_t sequenceNumber = 0;
unsigned long lastFrameMs = 0;

struct BrightnessSummary {
  uint32_t samples;
  uint32_t averageLuma;
  uint32_t brightSamples;
};

static uint8_t rgb565Luma(uint16_t pixel) {
  uint8_t r = (uint8_t)(((pixel >> 11) & 0x1F) * 255 / 31);
  uint8_t g = (uint8_t)(((pixel >> 5) & 0x3F) * 255 / 63);
  uint8_t b = (uint8_t)((pixel & 0x1F) * 255 / 31);
  return (uint8_t)((30UL * r + 59UL * g + 11UL * b) / 100UL);
}

BrightnessSummary summarizeFrame(uint8_t *buffer, size_t byteCount) {
  BrightnessSummary summary = {0, 0, 0};
  uint32_t lumaSum = 0;

  const size_t bytesPerPixel = 2;
  const size_t sampleStridePixels = 16;
  const size_t stepBytes = bytesPerPixel * sampleStridePixels;

  for (size_t i = 0; i + 1 < byteCount; i += stepBytes) {
    uint16_t pixel = ((uint16_t)buffer[i + 1] << 8) | buffer[i];
    uint8_t luma = rgb565Luma(pixel);
    lumaSum += luma;
    summary.samples++;
    if (luma > 180) {
      summary.brightSamples++;
    }
  }

  if (summary.samples > 0) {
    summary.averageLuma = lumaSum / summary.samples;
  }

  return summary;
}

void sendStatusLine(const char *status) {
  Serial1.print("{\"source\":\"nicla_vision\",\"status\":\"");
  Serial1.print(status);
  Serial1.println("\"}");

  Serial.print("UART_TX status=");
  Serial.println(status);
}

void setup() {
  Serial.begin(USB_BAUD);
  Nicla_Vision.begin();
  Nicla_Vision.enable3V3VDDIO_EXT();
  Serial1.begin(UART_BAUD);

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, HIGH);

  delay(1000);
  sendStatusLine("boot");
  sendStatusLine("vddio_ext_3v3_enabled");

  cameraReady = cam.begin(CAMERA_R320x240, CAMERA_RGB565, 15);
  if (cameraReady) {
    sendStatusLine("camera_ready");
  } else {
    sendStatusLine("camera_begin_failed");
  }
}

void loop() {
  if (millis() - lastFrameMs < FRAME_PERIOD_MS) {
    return;
  }
  lastFrameMs = millis();

  if (!cameraReady) {
    sendStatusLine("camera_not_ready");
    return;
  }

  int result = cam.grabFrame(frameBuffer, 3000);
  if (result != 0) {
    Serial1.print("{\"source\":\"nicla_vision\",\"seq\":");
    Serial1.print(sequenceNumber++);
    Serial1.print(",\"status\":\"grab_failed\",\"code\":");
    Serial1.print(result);
    Serial1.println("}");
    return;
  }

  uint8_t *buffer = frameBuffer.getBuffer();
  size_t frameBytes = cam.frameSize();
  BrightnessSummary summary = summarizeFrame(buffer, frameBytes);

  Serial1.print("{\"source\":\"nicla_vision\",\"seq\":");
  Serial1.print(sequenceNumber++);
  Serial1.print(",\"status\":\"ok\",\"frame_bytes\":");
  Serial1.print(frameBytes);
  Serial1.print(",\"samples\":");
  Serial1.print(summary.samples);
  Serial1.print(",\"avg_luma\":");
  Serial1.print(summary.averageLuma);
  Serial1.print(",\"bright_samples\":");
  Serial1.print(summary.brightSamples);
  Serial1.println("}");

  Serial.print("Frame summary sent, avg_luma=");
  Serial.println(summary.averageLuma);

  digitalWrite(LEDB, !digitalRead(LEDB));
}
