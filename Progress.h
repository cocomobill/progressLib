#ifndef Progress_h
#define Progress_h
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
    char* _progressBar;
    char* _paddedPercentage;

}; 


#endif