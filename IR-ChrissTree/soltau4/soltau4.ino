const int ir = 2; // с выхода ИК приёмника
int t = 600; // период стробирования, мкс

// указываем количество ячеек
//byte myInts[6];

int j=0;
int k;
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  //pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(ir, INPUT);
}

void loop() 
{
  j++;
  if (j<3)
  {
    if (j>=0)
    {
    Serial.println(j);
    for (int i=0; i<10; i++)
    {
      for (int k=0; k<10; k++)
      {
        Serial.print(1);
        delay(13);
      }
      //delay(50); // пропустим стандартное начало пакета
      for (int k=0; k<10; k++)
      {
        Serial.print(0);
        delay(13);
      }
      //delay(50); // пропустим стандартное начало пакета
    }
    Serial.println();
    }
  }


  
  /*
  int r = digitalRead(ir); // читаем значение ИК сенсора
  digitalWrite(LED_BUILTIN, r!=HIGH); // зажигаем светодиод, если сенсор сработал
  // Если зафиксировали ИК излучение, обрабатываем команду с пульта:
  if (r==LOW) 
  {
    precess_ir(); 
  }
  */
}

// читает пакет ИК пульта
void precess_ir()
{
  delay(13); // пропустим стандартное начало пакета
  byte bits[100]; // 100 бит должно хватить
  // читаем пакет
  for (int i=0; i<100; i++)
  {
    int bit = readBit();
    bits[i] = bit;    
  }
  // выводим пакет в монитор;
  for (int i=0; i<100; i++)
  {
    Serial.print(bits[i]);
    //delay(500);
  }
  Serial.println();

  // выводим пакет в монитор;
  for (int i=0; i<100; i++)
  {
    Serial.print(bits[i]);
    //delay(5);
  }
  Serial.println();


}

// читает 1 бит пакета
int readBit() 
{
  // дожидаемся уровня HIGH и ставим первый строб
  int r1;
  do 
  { 
    r1 = digitalRead(ir);
  } 
  
  while (r1 != HIGH);
  delayMicroseconds(t); // ждём

  // затем ставим второй строб
  int r2 = digitalRead(ir);
  delayMicroseconds(t);  // ждём
  
  if (r2 == LOW) 
  {
    return 0;
  }
  else 
  {
    // третий строб 
    delayMicroseconds(t);  // ждём
    return 1;
  }
}
