#ifndef Progress_h
#define Progress_h

typedef struct allocatedSpace{
    unsigned short len;
    char* adress;
} allocatedSpace;
// this struct is used to check if the allocated lenght equals to new lenght before realloc
// to reduce memory fragmentation and increase performance.
// Even though this is unnecessary for GNU libc, it's undefined and may increase performance in other platforms.

class Progress
{
  public:
    Progress(float, float);
    ~Progress();
    void update(float);
    float toPercentage(unsigned char);
    char* toPaddedPercentage(unsigned char);
    char* toBar(unsigned short);

  private:
    float _min, _max, _currentValue;
    float valMap(short);
    allocatedSpace _progressBar;
    allocatedSpace _paddedPercentage;

};

#ifndef __AVR__ // in non avr compilers, this function needs to be added.
// also this is better than sprintf in our case since some of the
// avr compilers don't implement floating point stuff with sprintf.
char *dtostrf (double val, signed char width, unsigned char prec, char *sout);
// this function converts floats into strings. Normally included in avr c.
#endif

#endif