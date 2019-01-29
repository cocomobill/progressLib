#include <Progress.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Progress myProgress(0,100000); // graphs 100 seconds
void setup() {
  lcd.begin(16, 2);
}

void loop() {
  //feed the current data to your 'Progress'
  myProgress.update(millis());
  
  //The padding for lcd will be added automatically
  lcd.setCursor(0, 0);
  lcd.print(myProgress.toBar(11)); // smaller progressBars

  lcd.print(myProgress.toPaddedPercentage(1));

  lcd.setCursor(0, 1);
  lcd.print(myProgress.toBar(16)); // A bar with a total lenght of 16 will be created. 
  
 
  delay(200);
}
 
