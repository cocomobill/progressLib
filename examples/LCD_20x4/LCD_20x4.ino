#include <Progress.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Progress myProgress(0,100000); // graphs 100 seconds
void setup() {
  lcd.begin(20, 4);
}

void loop() {
  //feed the current data to your 'Progress'
  myProgress.update(millis());
  
  lcd.setCursor(0, 0);
  //The padding for lcd will be added automatically
  lcd.print(myProgress.toPaddedPercentage(1));
  lcd.setCursor(0, 1);
  lcd.print(myProgress.toBar(20)); // A bar with a total lenght of 20 will be created. 

  lcd.setCursor(0, 2);
  lcd.print(myProgress.toBar(10)); // smaller progressBars
  
  lcd.setCursor(10, 2);
  lcd.print(myProgress.toBar(10));
  
  lcd.setCursor(0, 3);
  lcd.print(millis());
  
  delay(200);
}
 
