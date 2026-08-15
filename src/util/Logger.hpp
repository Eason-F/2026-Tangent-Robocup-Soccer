#pragma once

#include <Arduino.h>
#include <cstring>

class Logger {
    public:
        static constexpr size_t BUFFER_SIZE = 512;
        static constexpr size_t MAX_QUEUED_FIELDS = 32;

        Logger(Print &output, const uint32_t intervalMs);

        template <typename Callback>
        void update(Callback callback) {
            flush();

            if (linePending) {
                return;
            }

            const uint32_t now = millis();

            if (intervalMs != 0 && now - lastLogTime < intervalMs) {
                return;
            }

            lastLogTime = now;
            firstField = true;
            lineBuffer.clear();

            callback(*this);

            appendQueuedFields();
            lineBuffer.println();

            linePending = true;
            flush();
        }

        template <typename Value>
        void log(const char *name, const Value &value) {
            if (!firstField) {
                lineBuffer.print("  | ");
            }

            lineBuffer.print(name);
            lineBuffer.print(": ");
            lineBuffer.print(value);

            firstField = false;
        }

        template <typename Value>
        static void queue(const char *name, const Value &value) {
            if (activeLogger == nullptr) {
                return;
            }

            activeLogger->queueValue(name, value);
        }

        void setInterval(const uint32_t intervalMs);
        uint32_t getInterval() const;

    private:
        class BufferPrint : public Print {
            public:
                size_t write(uint8_t value) override {
                    if (length >= BUFFER_SIZE) {
                        return 0;
                    }

                    data[length++] = value;
                    return 1;
                }

                void clear() {
                    length = 0;
                }

                const uint8_t *getData() const {
                    return data;
                }

                size_t getLength() const {
                    return length;
                }

                void append(const BufferPrint &other) {
                    for (size_t i = 0; i < other.getLength(); ++i) {
                        write(other.getData()[i]);
                    }
                }

            private:
                uint8_t data[BUFFER_SIZE];
                size_t length = 0;
        };

        template <typename Value>
        void queueValue(const char *name, const Value &value) {
            for (size_t i = 0; i < queuedFieldCount; ++i) {
                if (strcmp(queuedFieldNames[i], name) == 0) {
                    return;
                }
            }

            if (queuedFieldCount >= MAX_QUEUED_FIELDS) {
                return;
            }

            queuedFieldNames[queuedFieldCount++] = name;

            if (!firstQueuedField) {
                queuedFields.print("  | ");
            }

            queuedFields.print(name);
            queuedFields.print(": ");
            queuedFields.print(value);

            firstQueuedField = false;
        }

        void appendQueuedFields();
        void flush();

        static Logger *activeLogger;

        Print &output;

        BufferPrint lineBuffer;
        BufferPrint queuedFields;

        uint32_t intervalMs;
        uint32_t lastLogTime;

        size_t sentLength = 0;
        size_t queuedFieldCount = 0;

        bool linePending = false;
        bool firstField = true;
        bool firstQueuedField = true;

        const char *queuedFieldNames[MAX_QUEUED_FIELDS];
};