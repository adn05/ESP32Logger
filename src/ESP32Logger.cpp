// 
// ESP32Logger.cpp
// Logger for ESP32 for debugging C++-code
// https://github.com/MartinXBcn/ESP32Logger
// Version 1.0.3
// 


#include "ESP32Logger.h"
#include <stdarg.h>

ESP32Logger Log;

void ESP32Logger::init(Print* output, ESP32LogLevel logLevel, ESP32Timestamp useTimestamp) {
	_output = output;
	_logLevel = logLevel;
	_useTimestamp = useTimestamp;
	_isActive = true;
}

void ESP32Logger::start() {
	_isActive = true;
}

void ESP32Logger::stop() {
	_isActive = false;
}

void ESP32Logger::setLogLevel(ESP32LogLevel logLevel) {
	_logLevel = logLevel;
}

void ESP32Logger::log(ESP32LogLevel logLevel, const char* format, ...) {
	char buffer[128];

	if (!_isActive || _logLevel < logLevel)
		return;

	if (_useTimestamp != ESP32Timestamp::TimestampNone) {
		if (_useTimestamp == ESP32Timestamp::TimestampLocaltime) {
			struct tm timeinfo;
			if (!getLocalTime(&timeinfo))
				snprintf(buffer, sizeof(buffer), "error time ");
			else
				strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S ", &timeinfo);
		}
		else {
			unsigned long timestamp = millis();
			// Format the timestamp as min 6-digit number with leading spaces
			// This is to ensure that the timestamp is always 6 digits, e.g., "  123"
			snprintf(buffer, sizeof(buffer), "%6lu ", timestamp);
		}
		print(buffer);
	}
	switch (logLevel) {
	case ESP32LogLevel::Debug:
		print("D ");
		break;
	case ESP32LogLevel::Error:
		print("E ");
		break;
	case ESP32LogLevel::Info:
		print("I ");
		break;
	case ESP32LogLevel::Warn:
		print("W ");
		break;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	print(buffer);

	printEoL();
} // void ESP32Logger::log(ESP32LogLevel logLevel, const char* format, ...)

void ESP32Logger::print(const char* logLine) {
	if (_output != nullptr)
		_output->print(logLine);
}

void ESP32Logger::printEoL()
{
	print("\r\n");
}


