#include "Utils.h"
#include "unistd.h"

Utils::Utils()
{
    //ctor
}


const long Utils::getTime() {
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}


int Utils::bitsToInt(char* bits, bool little_endian)
  {
      int result = 0;

      if (little_endian) {
        for (int n = sizeof( result ); n >= 0; n--)
          result = (result << 8) +bits[ n ];
      } else {
        for (unsigned n = 0; n < sizeof( result ); n++)
          result = (result << 8) +bits[ n ];
      }

      return result;
  }


Utils::~Utils()
{
    //dtor
}
