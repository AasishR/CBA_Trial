/*
This model is used to represent an automatic wiper system in a car.
There are generally 2 modes in a wiper system: Auto / Manual. 
These systems are activated only after the ignition switch has been pressed to start the engine
Here I have assumed that the wiper system has been implemented using a single DC Motor.

Components used as input to system:

Push button --> (From left to right)
1) Ignition Switch
2) Wiper ON/OFF Switch
3) Auto mode ON/OFF switch
    
Potentiometer --> Used as a sensor model to represent rain sensor

Indicators: (From left to right)

1) Ignition ON/OFF indicator
2) Wiper ON/OFF Indicator
3) Wiper Speed indicator(in Auto Mode)
(No Color -> 0% Max Speed)(RED -> 25% Max speed)(BLUE -> 50% max Speed)(GREEN -> 75% max Speed)(WHITE -> 100% Max Speed)

Use Serial Monitor to view more detailed output

Vary Pot between Max and Min to simulate mild to heavy rain Condition

In Manual Mode -> Wiper Motor rotates at max rpm irrespective of the intensity of rain

In Auto Mode -> Wiper Motor speed depends upon the intensity of rain

AUTHOR : AASISH RAMESH
*/


unsigned long time;
int k;
int ignition = 0;
int ign_flag = 0;
int w_status = 0;
int auto_flag = 0;
int wiper_flag = 0;
int auto_status = 0;
int d = 0;
int prev_time = 0;
int speed_status = 0;

void setup()
{
Serial.begin(9600);
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT);
pinMode(10,OUTPUT);
pinMode(9,OUTPUT);
pinMode(3,OUTPUT);
digitalWrite(13,LOW);
digitalWrite(12,LOW);
digitalWrite(11,LOW);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(3,LOW); 
d = analogRead(A3);
}

void loop()
{  
 
 time = millis();
 
ignition = ignition_fn();
  if(ignition == 1)
  {
    Serial.println("IGNITION ON");
    digitalWrite(13,HIGH); // ignition ON indicator
    w_status = wiper_check();
    if(w_status == 1)
    { 
      digitalWrite(12,HIGH);  // wiper ON indicator
      	Serial.println("Wiper ON");
     	auto_status = auto_check();
        if(auto_status == 1)
        { Serial.println(" - Auto Mode ON");
          if((time - prev_time) >= 2000)
          {
            d = analogRead(A3); // check every 2 sec rain sensor value
            prev_time = time;
          }
          speed_status = W_auto(d);
          a_mode_indicator(speed_status);
        }
        else
        {Serial.println(" - Manual Mode ON");
          W_manual(1);
         prev_time = time;
        }
     }
    else
    {
    	digitalWrite(12,LOW);  // wiper OFF indicator
      	Serial.println("Wiper OFF");
        W_manual(0);
      	prev_time = time;
    }
  }
  else
  {
  		digitalWrite(13,LOW);  // Ignition OFF indicator
      	Serial.println("Ignition OFF");
  }
}

int ignition_fn()
{int a = 0;
a = analogRead(A0);
  if ((a < 200 && ign_flag == 0) || (a > 200 && ign_flag == 1)  )
  {
  ign_flag = 1;
  delay(35);// debounce time
  return 1;
  }
  if (a < 200 && ign_flag == 1 || (a > 200 && ign_flag == 0))
  {
  ign_flag = 0;
  delay(35); //debounce time
  return 0;
  }
}

int wiper_check()
{
int b = 0;
b = analogRead(A1);
  if ((b < 200 && wiper_flag == 0) || (b > 200 && wiper_flag == 1)  )
  {
  wiper_flag = 1;
  delay(35); // debounce time
  return 1;
  }
  if (b < 200 && wiper_flag == 1 || (b > 200 && wiper_flag == 0))
  {
  wiper_flag = 0;  
  delay(35);// debounce time  
  return 0;
  }
}

int auto_check()
{
int c = 0;
c = analogRead(A2);
  if ((c < 200 && auto_flag == 0) || (c > 200 && auto_flag == 1)  )
  {
  auto_flag = 1;
  delay(35); // debounce time
  return 1;
  }
  if (c < 200 && auto_flag == 1 || (c > 200 && auto_flag == 0))
  {
  auto_flag = 0;  
  delay(35);// debounce time  
  return 0;
  }
}

// MANUAL MODE
  
void W_manual(int z)
{
if(z == 1)
{ 
digitalWrite(3,HIGH);
 analogWrite(11,0);
 analogWrite(10,0);
 analogWrite(9,0);
}
if(z == 0)
{ 
digitalWrite(3,LOW);
}
}

// AUTO MODE
int W_auto(int y) 
{
if(y <= 204)
{
analogWrite(3,0); // 0% duty
return 0;
}
if(y > 204 && y <= 408)
{
analogWrite(3,63); // 25% duty
return 1;
}
if(y > 408 && y <= 612)
{ 
analogWrite(3,126); // 50% duty
return 2;
}
if(y > 612 && y <= 816)
{
analogWrite(3,189); // 75% duty
return 3;
}
if(y > 816 && y <= 1023)
{
analogWrite(3,255); // 100% duty
return 4;
}
}

void a_mode_indicator(int k)
{
  if(k == 0)
 {
 analogWrite(11,0); //  NO LIGHT
 analogWrite(10,0);
 analogWrite(9,0);
 }
 if(k == 1)
 {
 analogWrite(11,255); // RED LIGHT
 analogWrite(10,0);
 analogWrite(9,0);
 }
 if(k == 2)
 {
 analogWrite(11,0); // BLUE LIGHT
 analogWrite(10,255);
 analogWrite(9,0);
 }
 if(k == 3)
 {
 analogWrite(11,0); // GREEN LIGHT
 analogWrite(10,0);
 analogWrite(9,255);
 }
 if(k == 4)
 {
 analogWrite(11,255); // WHITE LIGHT
 analogWrite(10,255);
 analogWrite(9,255);
 }
 }
