# progressLib
ProgressLib is a lightweight Arduino compatible, cross-platform progressbar and progress library

![Lcd showing progressbars](https://raw.githubusercontent.com/cocomobill/progressLib/lcd.png)
```
Hey! Now doing some stuff for next 120 seconds
[#####################################                                         ] 48.33% 58 seconds so far
```
## Getting Started
ProgressLib can be easily used with arduino. Some usage examples below.
```
#include <Progress.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Progress myProgress(0,1023);

void setup() {
  lcd.begin(16, 2);
  pinMode(A0,INPUT);
}

void loop() {
  //feed the current data to your 'Progress'
  myProgress.update(analogRead(A0));

  lcd.setCursor(0, 0);
  lcd.print(myProgress.toPaddedPercentage(1));

  lcd.setCursor(0, 1);
  lcd.print(myProgress.toBar(16)); // A bar with a total lenght of 16 will be created. 
  
}
```

## Running the tests
```
./test.py
```