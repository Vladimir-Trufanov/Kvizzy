const int ir = 2; // с выхода ИК приёмника
int t = 600;      // период стробирования, мкс

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
  delay(13);      // пропустим стандартное начало пакета
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
  if (r2 == LOW) {
    return 0;
  }
  else {
    // третий строб 
    delayMicroseconds(t);  // ждём
    return 1;
  }
}
