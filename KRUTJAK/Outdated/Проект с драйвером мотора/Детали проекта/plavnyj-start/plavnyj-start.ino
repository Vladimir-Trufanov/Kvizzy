/*  plavnyj-start.ino  */
// Определяем выводы
#define DIR_PIN 2
#define PWM_PIN 3
// Определяем максимальную ШИМ
#define MAX_SPEED 255
#define UP_MIN_SPEED 64
#define DOWN_MIN_SPEED 64
#define MS_TIME 20

void setup()
{
   Serial.begin(9600);
   // Устанавливаем выводы в режим выхода` 1
   pinMode(DIR_PIN, OUTPUT);
   pinMode(PWM_PIN, OUTPUT);
}

void loop()
{
   // Устанавливаем направление вращения
   digitalWrite(DIR_PIN, HIGH);
   Serial.println("=== HIGH ==="); 
   
   /*
   analogWrite(PWM_PIN, MAX_SPEED);
   delay(8000);
   analogWrite(PWM_PIN, DOWN_MIN_SPEED);
   delay(8000);
   */
   
   move();
    // Устанавливаем направление вращения
   digitalWrite(DIR_PIN, LOW);
   Serial.println("=== LOW ==="); 
   move();
}

void move()
{
   delay(8000);
   analogWrite(PWM_PIN, MAX_SPEED);
   delay(MS_TIME);
   // Медленно приращиваем скорость при помощи ШИМ
   for (uint8_t speed = UP_MIN_SPEED; speed < MAX_SPEED; speed++) 
   {
      analogWrite(PWM_PIN, speed);
      Serial.print("--> "); Serial.println(speed);
      delay(MS_TIME);
   }

   // Выходим на максимальную скорость
   analogWrite(PWM_PIN, MAX_SPEED);
   Serial.print("=== "); Serial.println(MAX_SPEED);
   delay(2000);

   // Медленно сбавляем скорость
   for (uint8_t speed = MAX_SPEED; speed > DOWN_MIN_SPEED; speed--) 
   {
      analogWrite(PWM_PIN, speed);
      Serial.print("<-- "); Serial.println(speed);
      delay(MS_TIME);
   }

   // Выключаем мотор
   analogWrite(PWM_PIN, 0);
   Serial.print("=== "); Serial.println("0");
}
