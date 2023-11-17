int ledArray[] ={8,9,10,11,12};
int count=0;
int timer=50;

void setup(){
  // we make all the declarations at once:
  for(count=5;count>=0;count--)
  {
    pinMode(ledArray[count], OUTPUT);
  }
}

void loop(){
  for(count=5;count>=0;count--)
  {
    digitalWrite(ledArray[count], HIGH);
    delay(timer);
    digitalWrite(ledArray[count],LOW);
    delay(timer);
  }
  for(count=0;count<5;count++)
  {
    digitalWrite(ledArray[count], HIGH);
    delay(timer);
    digitalWrite(ledArray[count],LOW);
    delay(timer);
  }
}
