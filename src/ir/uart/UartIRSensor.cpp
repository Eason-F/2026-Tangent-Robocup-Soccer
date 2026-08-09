#include <ir/uart/UartIRSensor.hpp>

UartIRSensor::UartIRSensor(HardwareSerial &serialPort, uint32_t baudRate)
    : serialPort(serialPort), baudRate(baudRate) {}

void UartIRSensor::setup() {
    serialPort.begin(baudRate);
    resetReceiver();
}

void UartIRSensor::updateReadings() {
    while (serialPort.available() > 0) {
        processByte(static_cast<uint8_t>(serialPort.read()));
    }
}

void UartIRSensor::processByte(uint8_t value) {
    switch (receiveState) {
        case ReceiveState::WAITING_FOR_MARKER_0:
            if (value == MARKER_0) {
                receiveState = ReceiveState::WAITING_FOR_MARKER_1;
            }
            break;

        case ReceiveState::WAITING_FOR_MARKER_1:
            if (value == MARKER_1) {
                receiveState = ReceiveState::READING_TYPE;
            } else if (value != MARKER_0) {
                receiveState = ReceiveState::WAITING_FOR_MARKER_0;
            }
            break;

        case ReceiveState::READING_TYPE:
            packetType = value;
            calculatedCrc = updateCrc(0xFFFF, value);
            receiveState = ReceiveState::READING_LENGTH;
            break;

        case ReceiveState::READING_LENGTH:
            payloadLength = value;
            calculatedCrc = updateCrc(calculatedCrc, value);
            if (payloadLength > MAX_PAYLOAD_LENGTH) {
                resetReceiver(value);
            } else {
                receiveState = ReceiveState::READING_SEQUENCE;
            }
            break;

        case ReceiveState::READING_SEQUENCE:
            sequence = value;
            calculatedCrc = updateCrc(calculatedCrc, value);
            payloadPosition = 0;
            receiveState = payloadLength == 0
                               ? ReceiveState::READING_CRC_LOW
                               : ReceiveState::READING_PAYLOAD;
            break;

        case ReceiveState::READING_PAYLOAD:
            payload[payloadPosition++] = value;
            calculatedCrc = updateCrc(calculatedCrc, value);
            if (payloadPosition >= payloadLength) {
                receiveState = ReceiveState::READING_CRC_LOW;
            }
            break;

        case ReceiveState::READING_CRC_LOW:
            receivedCrc = value;
            receiveState = ReceiveState::READING_CRC_HIGH;
            break;

        case ReceiveState::READING_CRC_HIGH:
            receivedCrc |= static_cast<uint16_t>(value) << 8;
            if (receivedCrc == calculatedCrc) {
                processPacket();
            }
            resetReceiver(value);
            break;
    }
}

void UartIRSensor::processPacket() {
    if (packetType == BLUETOOTH_MESSAGE_TYPE) {
        if (bluetoothMessageHandler != nullptr) {
            bluetoothMessageHandler(payload, payloadLength);
        }
        return;
    }

    if (packetType != IR_MEASUREMENT_TYPE ||
        payloadLength != IR_PAYLOAD_LENGTH) {
        return;
    }

    const int16_t bearingCentidegrees =
        static_cast<int16_t>(readU16(payload));
    directionDegrees = static_cast<float>(bearingCentidegrees) / 100.0f;
    signalStrength = static_cast<float>(readU16(payload + sizeof(uint16_t)));
    readingValid = true;
    lastUpdateMillis = millis();
}

void UartIRSensor::resetReceiver(uint8_t currentByte) {
    receiveState = currentByte == MARKER_0
                       ? ReceiveState::WAITING_FOR_MARKER_1
                       : ReceiveState::WAITING_FOR_MARKER_0;
    payloadPosition = 0;
    calculatedCrc = 0xFFFF;
    receivedCrc = 0;
}

uint16_t UartIRSensor::updateCrc(uint16_t crc, uint8_t value) {
    crc ^= static_cast<uint16_t>(value) << 8;
    for (uint8_t bit = 0; bit < 8; ++bit) {
        crc = (crc & 0x8000U)
                  ? static_cast<uint16_t>((crc << 1) ^ 0x1021U)
                  : static_cast<uint16_t>(crc << 1);
    }
    return crc;
}

uint16_t UartIRSensor::readU16(const uint8_t *data) {
    return static_cast<uint16_t>(data[0]) |
           (static_cast<uint16_t>(data[1]) << 8);
}

float UartIRSensor::getDirectionDegrees() const {
    return directionDegrees;
}

float UartIRSensor::getSignalStrength() const {
    return mapRange(signalStrength, 0, 500, 0, 100);
}

bool UartIRSensor::ballFound() const {
    return readingValid;
}

uint32_t UartIRSensor::getLastUpdateMillis() const {
    return lastUpdateMillis;
}

void UartIRSensor::setBluetoothMessageHandler(
    BluetoothMessageHandler handler) {
    bluetoothMessageHandler = handler;
}

float UartIRSensor::strengthToDistance(const uint16_t &strength) {
    return 0.0f;
}
