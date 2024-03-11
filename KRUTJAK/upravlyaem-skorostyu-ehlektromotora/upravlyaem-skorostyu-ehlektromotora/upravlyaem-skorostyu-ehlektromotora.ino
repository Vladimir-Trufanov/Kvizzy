
// upravlyaem-skorostyu-ehlektromotora.ino

const int PinRes = A0;
const int PinPWM = 10;
      int ValRes = 0;
      int ValPWM = 0;

void setup() 
{
   pinMode(PinPWM, OUTPUT);
}

void loop() 
{
   ValRes = analogRead(PinRes);
   ValPWM = map(ValRes, 0,1023, 0,255); 
   analogWrite(PinPWM,ValPWM);
}
