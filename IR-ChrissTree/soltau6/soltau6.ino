
int k;
void setup() {
  Serial.begin(9600);
}

void loop() {
  //Sawtooth Wave
  //for (float i = 0; i <= 70; i = i + 1)
  for (int i = 0; i <= 70; i = i + 1)
  {
    //Serial.println(i);
    for (k = 0; k <= 70; k = k + 1)
    {
       Serial.println(0);
       //delay(10);
    }
    for (k = 0; k <= 30; k = k + 1)
    {
       Serial.println(1);
       //delay(10);
    }
  }
}
