// C++ code
//
#include <SoftwareSerial.h>

SoftwareSerial serialRX(4,3);   // RX=4, TX=3
//byte 
 
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  serialRX.begin(300);  
  Serial.begin(115200);
}

void loop()
{
  //serialRX.available()?digitalWrite(13,1):digitalWrite(13,0);
  if (serialRX.available())
  {
    while(serialRX.available())
    {
       char simb=serialRX.read();
       Serial.print(simb);
    }
  }
    delay(1500);
    Serial.println("Second=11");
}
