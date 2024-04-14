/* izmenenie-napravleniya-vrashcheniya.ino  */

// Определяем выводы подключения 2 - направление, 3 - скорость
#define DIR_PIN 2
#define PWM_PIN 3

// Создаём переменную направления
bool dir = false;

void setup()
{
    // Устанавливаем выводы в режиме выходов
    pinMode(DIR_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);
}

void loop()
{
    // Устанавливаем ШИМ
    // analogWrite(PWM_PIN, 255);
    analogWrite(PWM_PIN, 190);
    delay(500);
    analogWrite(PWM_PIN, 255);
    delay(500);
    analogWrite(PWM_PIN, 180);
    delay(500);
    // Устанавливаем направление, заданное переменной dir
    digitalWrite(DIR_PIN, dir);
    // Ждём 2 секунды
    delay(2000);
    // Выключаем мотор
    analogWrite(PWM_PIN, 0);
    // Ждём секунду
    delay(1000);
    // Меняем направление
    dir = !dir;
}
