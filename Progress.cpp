#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Progress.h"
#include <string.h>

Progress::Progress(float min, float max)
{
    // initialize with min and max values. 
    _min = min;
    _max = max;
    _currentValue = min;

    _progressBar.adress = NULL;
    _progressBar.len = 0;
    _paddedPercentage.adress = NULL;
    _paddedPercentage.len = 0;
}
Progress::~Progress(){
    free(_progressBar.adress);
    free(_paddedPercentage.adress);
}
void Progress::update(float currentValue){
    _currentValue = currentValue;
}

float Progress::toPercentage(unsigned char scale){
    int _pow = pow(10,scale);
    return floorf(valMap(100) * _pow) / _pow;
}

char* Progress::toPaddedPercentage(unsigned char scale){
    float percentage = toPercentage(scale);
    unsigned char arrayLen = scale + (scale > 0) + 3; // 000 + . + scale
    if (_paddedPercentage.len != arrayLen + 1){ // if the array size changed, realloc.
        _paddedPercentage.adress = (char*)realloc(_paddedPercentage.adress,arrayLen + 1); // + \0
    }
    // In other platforms, the padding can be done via sprintf. Float operations for sprintf are absent in arduino afaik. 
    dtostrf(percentage, arrayLen, scale, _paddedPercentage.adress);
    return _paddedPercentage.adress;
}

char* Progress::toBar(unsigned short width){ // maximum width can be uint16_max - 1
    if (_progressBar.len != width + 1){
        _progressBar.adress = (char*)realloc(_progressBar.adress,width + 1); // +1 for null
    }
    memset(_progressBar.adress,' ', width);
    _progressBar.adress[0] = '[';
    memset(&_progressBar.adress[1],'#',(int)valMap(width-2)); // -2 for [, ]
    _progressBar.adress[width-1] = ']';
    _progressBar.adress[width] = '\0';
    return _progressBar.adress;
}

float Progress::valMap(short out_max){
    // adjusts the range of the _min, _max values to 0-out_max
    float mapped = (_currentValue - _min) * (out_max) / (_max - _min);
    return (mapped<=out_max) ? mapped : out_max; // Ensure safety with limiting the maximum value. 
}

#ifndef __AVR__ // see header file.
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
#endif