// analogRead: on a 1023 scale, .3 volts is .3/.0049 = approximately 61
// 1.5 Volts/.0049 = 306
// max run is 50 days. we should be good.

void setup()
{
  // this pin accepts input from the logic unit
  pinMode(A0, INPUT); 

  Serial.begin(115200);
  
  Serial.println();
  Serial.println("starting");
}

int state = 0;
boolean trigger = true;

void loop()
{ 
  if((state = analogRead(A0)) && trigger)
  {
    trigger = false;    
    Serial.println(millis(), DEC);    
  }
  if(!state) trigger = true;

} // loop






