#pragma once

#include <Arduino.h>
#include <util/util.hpp>

class UartIRSensor {
    public:
        using BluetoothMessageHandler =
            void (*)(const uint8_t *data, uint8_t length);

        explicit UartIRSensor(HardwareSerial &serialPort,
                              uint32_t baudRate = 115200);

        void setup();
        void update();

        float getDirectionDegrees() const;
        float getDirectionRadians() const;
        float getSignalStrength() const;
        bool ballFound() const;
        uint32_t getLastUpdateMillis() const;
        void setBluetoothMessageHandler(BluetoothMessageHandler handler);

        static float strengthToDistance(const uint16_t &strength);

    private:
        static constexpr uint8_t MARKER_0 = 0xA5;
        static constexpr uint8_t MARKER_1 = 0x5A;
        static constexpr uint8_t IR_MEASUREMENT_TYPE = 0x01;
        static constexpr uint8_t BLUETOOTH_MESSAGE_TYPE = 0x02;
        static constexpr uint8_t IR_PAYLOAD_LENGTH = 4;
        static constexpr uint8_t MAX_PAYLOAD_LENGTH = 64;

        enum class ReceiveState : uint8_t {
            WAITING_FOR_MARKER_0,
            WAITING_FOR_MARKER_1,
            READING_TYPE,
            READING_LENGTH,
            READING_SEQUENCE,
            READING_PAYLOAD,
            READING_CRC_LOW,
            READING_CRC_HIGH,
        };

        HardwareSerial &serialPort;
        uint32_t baudRate;

        ReceiveState receiveState = ReceiveState::WAITING_FOR_MARKER_0;
        uint8_t packetType = 0;
        uint8_t payloadLength = 0;
        uint8_t sequence = 0;
        uint8_t payload[MAX_PAYLOAD_LENGTH] = {};
        uint8_t payloadPosition = 0;
        uint16_t calculatedCrc = 0xFFFF;
        uint16_t receivedCrc = 0;

        float directionDegrees = 0.0f;
        float signalStrength = 0.0f;
        bool readingValid = false;
        uint32_t lastUpdateMillis = 0;
        BluetoothMessageHandler bluetoothMessageHandler = nullptr;

        void processByte(uint8_t value);
        void processPacket();
        void resetReceiver(uint8_t currentByte = 0);

        static uint16_t updateCrc(uint16_t crc, uint8_t value);
        static uint16_t readU16(const uint8_t *data);
};
