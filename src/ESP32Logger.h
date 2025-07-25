// 
// ESP32Logger.h
// Logger for ESP32 for debugging C++-code
// https://github.com/MartinXBcn/ESP32Logger
// Version 1.0.3
//

#ifndef _ESP32Logger_h
#define _ESP32Logger_h

#include "Arduino.h"

enum class ESP32LogLevel {
	Error,
	Warn,
	Info,
	Debug
};

enum class ESP32Timestamp {
	TimestampNone,
	TimestampSinceStart,
	TimestampLocaltime
};

class ESP32Logger {
public:
	void init(Print* output, ESP32LogLevel logLevel = ESP32LogLevel::Info, ESP32Timestamp useTimestamp = ESP32Timestamp::TimestampSinceStart);
	void start();
	void stop();
	void setLogLevel(ESP32LogLevel logLevel);
	void log(ESP32LogLevel logLevel, const char* message, ...);

private:
	ESP32LogLevel _logLevel = ESP32LogLevel::Info;
	Print* _output = nullptr;
	ESP32Timestamp _useTimestamp = ESP32Timestamp::TimestampSinceStart;
	bool _isActive = false;

	void print(const char* logLine);
	void printEoL();
};


#ifdef ESP32DEBUGGING

extern ESP32Logger Log;

// For "##__VA_ARGS__" see https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define DBGINI(output, ...) Log.init(output, ##__VA_ARGS__);
#define DBGSTA Log.start();
#define DBGSTP Log.stop();
#define DBGLEV(loglevel) Log.setLogLevel(ESP32LogLevel::loglevel);
#define DBGCOD(debugexpression) debugexpression
#define DBGLOG(loglevel, logmsg, ...) { String logstr = String(__func__) + ": " + String(logmsg); Log.log(ESP32LogLevel::loglevel, logstr.c_str(), ##__VA_ARGS__); }
#define DBGCHK(loglevel, cond, logmsg, ...) if(!(cond)) DBGLOG(loglevel, logmsg, ##__VA_ARGS__)

#else // #ifdef ESP32DEBUGGING

// No debugging, i.e. empty definitions.
#define DBGINI(output, ...) ;
#define DBGSTA ;
#define DBGSTP ;
#define DBGLEV(loglevel) ;
#define DBGCOD(debugexpression) ;
#define DBGLOG(loglevel, logmsg, ...) ;
#define DBGCHK(loglevel, cond, logmsg, ...) ;

#endif // #ifdef ESP32DEBUGGING

// Set remplacing macros for more readable code
#define LOG_INIT DBGINI
#define LOG_START() DBGSTA
#define LOG_STOP() DBGSTP
#define LOG_LEVEL DBGLEV
#define LOG_CODE DBGCOD
#define LOG_MESSAGE DBGLOG
#define LOG_CHECK DBGCHK

#define LOG_DEBUG(logmsg, ...) LOG_MESSAGE(Debug, logmsg, ##__VA_ARGS__)
#define LOG_INFO(logmsg, ...) LOG_MESSAGE(Info, logmsg, ##__VA_ARGS__)
#define LOG_WARN(logmsg, ...) LOG_MESSAGE(Warn, logmsg, ##__VA_ARGS__)
#define LOG_ERROR(logmsg, ...) LOG_MESSAGE(Error, logmsg, ##__VA_ARGS__)

#endif // #ifndef _ESP32Logger_h