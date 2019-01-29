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
    _progressBar = NULL;
    _paddedPercentage = NULL;
}
Progress::~Progress(){
    free(_progressBar);
    free(_paddedPercentage);
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
    _paddedPercentage = realloc(_paddedPercentage,arrayLen + 1); // + \0
    // In other platforms, the padding can be done via sprintf. Float operations for sprintf are absent in arduino afaik. 
    dtostrf(percentage, arrayLen, scale, _paddedPercentage);
    return _paddedPercentage;
}

char* Progress::toBar(unsigned short width){
    _progressBar = realloc(_progressBar,width+1); // +1 for null
    memset(_progressBar,' ', width);
    _progressBar[0] = '[';
    memset(&_progressBar[1],'#',(int)valMap(width-2)); // -2 for [, ]
    _progressBar[width-1] = ']';
    _progressBar[width] = '\0';
    return _progressBar;
}

float Progress::valMap(short out_max){
    // adjusts the range of the _min, _max values to 0-out_max
    float mapped = (_currentValue - _min) * (out_max) / (_max - _min);
    return (mapped<=out_max) ? mapped : out_max; // Ensure safety with limiting the maximum value. 
}
