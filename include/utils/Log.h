#ifndef LOG_H
#define LOG_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "Config.h"
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include <log4cpp/PropertyConfigurator.hh>
#include <stdarg.h>
#include <exception>

using namespace std;

enum LogLevel {DEBUG, INFO, NOTICE, WARN, ERROR, CRIT, ALERT, FATAL};

class Log
{
    public:
        Log();
        virtual ~Log();
        static void init(string clazz);

        static shared_ptr<Log> get();

        static void write(string msg);
        static void write(LogLevel level, string msg);
        static void write(const char*, ...);
        static void write(LogLevel level, const char*, ...);

        // examples.
        static void simple();

        static void Properties();
        // end of exmpales.

    protected:
        static const LogLevel defaultLogLevel = INFO;

        static inline void getString(const char* fommat, va_list va, string&);

    private:
        static shared_ptr<Log> _log;

        static bool _initialized;
};



#endif // LOG_H
