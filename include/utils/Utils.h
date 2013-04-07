#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>

class Utils
{
    public:
        Utils();
        virtual ~Utils();

        static const long getTime();

        static int bitsToInt(char*,  bool little_endian = true );

    protected:
    private:
};

#endif // UTILS_H
