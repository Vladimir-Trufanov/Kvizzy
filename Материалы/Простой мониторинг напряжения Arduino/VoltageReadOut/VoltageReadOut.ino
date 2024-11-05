//Voltage ReadOut
//Written by MrSirLRD
// Will read in voltage accross diode and compare it to Vref
//analog read Val=(Vin/Vref)*1023
//so Vref=(Vin*1023)/Val

float val;
float DiodeVoltage;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val=(DiodeVoltage*1023)/(analogRead(0));
Serial.println(val);
if (val>4){
  digitalWrite(13,HIGH); 
}//end if
else{
    digitalWrite(13,LOW); 
}//end else
}
