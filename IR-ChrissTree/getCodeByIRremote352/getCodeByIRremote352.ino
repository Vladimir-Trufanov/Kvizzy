#include <IRremote.h>  // 3.5.2

// The function decode(&results)) is deprecated and may not work as expected! Just use decode() without a parameter and IrReceiver.decodedIRData.<fieldname> .
// E0E040BF - включить телевизор самсунг
// 807F2AD5 - единичка "1"

IRrecv irrecv(2); // указываем вывод, к которому подключен приемник

decode_results results;

void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
  irrecv.enableIRIn(); // запускаем прием
}

void loop() {
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX ); // печатаем данные
    irrecv.resume(); // принимаем следующую команду
  }
}
