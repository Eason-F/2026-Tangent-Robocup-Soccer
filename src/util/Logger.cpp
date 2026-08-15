#include <util/Logger.hpp>
#include <cstring>

Logger *Logger::activeLogger = nullptr;

Logger::Logger(Print &output, const uint32_t intervalMs)
    : output(output),
      intervalMs(intervalMs),
      lastLogTime(millis()) {
    activeLogger = this;
}

void Logger::appendQueuedFields() {
    if (queuedFields.getLength() == 0) {
        return;
    }

    if (!firstField) {
        lineBuffer.print("  | ");
    }

    lineBuffer.append(queuedFields);

    queuedFields.clear();
    queuedFieldCount = 0;
    firstQueuedField = true;
    firstField = false;
}

void Logger::flush() {
    if (!linePending) {
        return;
    }

    const size_t remaining = lineBuffer.getLength() - sentLength;
    const int available = output.availableForWrite();

    if (available <= 0) {
        return;
    }

    const size_t writeLength = min(remaining, static_cast<size_t>(available));

    sentLength += output.write(
        lineBuffer.getData() + sentLength,
        writeLength
    );

    if (sentLength == lineBuffer.getLength()) {
        sentLength = 0;
        linePending = false;
    }
}

void Logger::setInterval(const uint32_t newIntervalMs) {
    intervalMs = newIntervalMs;
}

uint32_t Logger::getInterval() const {
    return intervalMs;
}