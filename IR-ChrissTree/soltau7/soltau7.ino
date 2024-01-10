const int ir = 2; // с выхода ИК приёмника
int t = 600;      // период стробирования, мкс
//int t = 300;      // период стробирования, мкс
byte bits[100];   // 100 бит должно хватить

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ir, INPUT);
}

void loop() {
  int r = digitalRead(ir);            // читаем значение ИК сенсора
  digitalWrite(LED_BUILTIN, r!=HIGH); // зажигаем светодиод, если сенсор сработал
  // Если зафиксировали ИК излучение, обрабатываем команду с пульта:
  if (r==LOW) 
  {
    precess_ir(); 
  }
}

// читает пакет ИК пульта
void precess_ir()
{
  //delay(13);      // пропустим стандартное начало пакета
  // читаем пакет
  for (int i=0; i<100; i++)
  {
    int bit = readBit();
    bits[i] = bit;    
  }
  // выводим пакет в монитор;
  Serial.println("com");
  toCom();
  //toPlotter();
  
  /*
  Serial.println("plotter");
  for (int i=0; i<100; i++)
  {
    Serial.print(bits[i]);
  }
  Serial.println();
  */
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


// выводим пакет в монитор;
// com для "единички"
// 0100000000111111100101010110101011111111111111111111111101111111111111111111111111111111111111111111

void toCom()
{
  // выводим пакет в монитор;
  for (int i=0; i<100; i++)
  {
    Serial.print(bits[i]);
  }
  Serial.println();
}
//       Serial.println(1);

  // выводим пакет на плоттер;
void toPlotter()
{
  for (int i=0; i<100; i++)
  {
    if (bits[i]>0)
    {
      Serial.println(100);
      delay13(bits[i]);
      //delay(13);
    }
    else
    {
      Serial.println(0);
      delay13(bits[i]);
      //delay(13);
    }
    
    //Serial.println(bits[i]);
    //delay(13);
         
  }
  //Serial.println();
}

void delay13(byte bits)
{
  delay(13);  
  if (bits>0)
  {
    Serial.println(99);
    delay(5);  
  }
  else 
  {
    Serial.println(1);
    delay(5);  
  }
}
