
int k;
float biSign; // 0 или 1
void setup() 
{
  Serial.begin(9600);
  Serial.println(-0.3);
  Serial.println(0.5);
  Serial.println(1.3);
  /*
  Serial.println(-5);
  Serial.println(15);
  Serial.println(7);
  */
}

void loop() 
{
  //for (float i = 0; i <= 70; i = i + 1)
  for (int i = 0; i <= 70; i = i + 1)
  {

    pic(0); 
    pic(0); 
    pic(0); 
    pic(0); 

    pic(1); 
    pic(1); 
    pic(1); 

    /*
    pic0(0); 
    pic0(0); 
    pic0(0); 
    pic0(0); 
    */
    
    /*
    for (k = 0; k <= 30; k = k + 1)
    {
       Serial.println(0);
       delay(30);
    }
    for (k = 0; k <= 2; k = k + 1)
    {
       Serial.println(-1);
       delay(30);
    }
    */

    /*
    pic1(1); 
    pic1(1); 
    pic1(1); 
    */

    /*
    for (k = 0; k <= 30; k = k + 1)
    {
       Serial.println(10);
       delay(30);
    }
    for (k = 0; k <= 2; k = k + 1)
    {
       Serial.println(11);
       delay(30);
    }
    */
  }
}

byte lastSign=0.1;

void pic(float biSign) 
{
  if (biSign>0) pic1(biSign); 
  else pic0(biSign);
}


void pic0(float biSign) 
{
  int k;
  if (biSign!=lastSign)
  {
    lastSign=biSign;  
    for (k = 0; k <= 2; k = k + 1)
    {
      Serial.println(-0.1);
      delay(30);
    }
  }
  
  for (k = 0; k <= 30; k = k + 1)
  {
    Serial.println(0);
    delay(30);
  }
  for (k = 0; k <= 2; k = k + 1)
  {
    Serial.println(-0.1);
    delay(30);
  }
}

void pic1(float biSign) 
{
  int k;
  if (biSign!=lastSign)
  {
    lastSign=biSign;  
    for (k = 0; k <= 2; k = k + 1)
    {
      Serial.println(1.1);
      delay(30);
    }
  }
  for (k = 0; k <= 30; k = k + 1)
  {
     Serial.println(1);
     delay(30);
  }
  for (k = 0; k <= 2; k = k + 1)
  {
     Serial.println(1.1);
     delay(30);
  }
}
