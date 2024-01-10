const int ir = 2;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT); // это 13-ый вывод Arduino со встроенным светодиодом
  pinMode(ir, INPUT);
}

void loop() 
{
  int r = digitalRead(ir);
  digitalWrite(LED_BUILTIN, r!=HIGH); // зажигаем светодиод, если модуль среагировал на ИК излучение
  // в противном случае - гасим
}
