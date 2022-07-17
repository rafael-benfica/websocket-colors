
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial

const int redPin = 16;   // 16 corresponds to GPIO16
const int greenPin = 17; // 17 corresponds to GPIO17
const int bluePin = 5;   // 5 corresponds to GPIO5

// setting PWM properties
const int freq = 5000;
const int ledChannelRed = 0;
const int ledChannelGreen = 1;
const int ledChannelBlue = 2;
const int resolution = 8;

String cor = "FFFFFF";

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        USE_SERIAL.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

        // join default namespace (no auto join in Socket.IO V3)
        socketIO.send(sIOtype_CONNECT, "/");
        break;
    case sIOtype_EVENT:
    {
        char *sptr = NULL;
        int id = strtol((char *)payload, &sptr, 10);
        USE_SERIAL.printf("[IOc] get event: %s id: %d\n", payload, id);
        if (id)
        {
            payload = (uint8_t *)sptr;
        }
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }
        String corData = doc[1];

        cor = corData.substring(0, 7);
        USE_SERIAL.printf("Cor enviada: %s\n", cor);
        long number = (long)strtol(&cor[1], NULL, 16);
        int r = number >> 16;
        int g = number >> 8 & 0xFF;
        int b = number & 0xFF;

        Serial.print("red is ");
        Serial.println(r);
        Serial.print("green is ");
        Serial.println(g);
        Serial.print("blue is ");
        Serial.println(b);

        ledcWrite(ledChannelRed, r);
        ledcWrite(ledChannelGreen, g);
        ledcWrite(ledChannelBlue, b);
    }
    break;
    case sIOtype_ACK:
        USE_SERIAL.printf("[IOc] get ack: %u\n", length);
        break;
    case sIOtype_ERROR:
        USE_SERIAL.printf("[IOc] get error: %u\n", length);
        break;
    case sIOtype_BINARY_EVENT:
        USE_SERIAL.printf("[IOc] get binary: %u\n", length);
        break;
    case sIOtype_BINARY_ACK:
        USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
        break;
    }
}

void setup()
{
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    // Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    ledcSetup(ledChannelRed, freq, resolution);
    ledcSetup(ledChannelGreen, freq, resolution);
    ledcSetup(ledChannelBlue, freq, resolution);

    ledcAttachPin(redPin, ledChannelRed);
    ledcAttachPin(greenPin, ledChannelGreen);
    ledcAttachPin(bluePin, ledChannelBlue);

    ledcWrite(ledChannelRed, 255);
    ledcWrite(ledChannelGreen, 255);
    ledcWrite(ledChannelBlue, 255);

    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("rede", "senha");

    // WiFi.disconnect();
    while (WiFiMulti.run() != WL_CONNECTED)
    {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
    socketIO.begin("192.168.0.8", 3000, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
}

void loop()
{
    socketIO.loop();
}
