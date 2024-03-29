/** Arduino UNO                         *** proverka-komand-upravleniya.ino ***
 * 
 * Определить список команд от ИКП для управления мотором паровозика
 * https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
 * 
 * v1.1, 29.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 28.03.2024
**/

#include <IRremote.hpp>

/**
 *  1 - "стоп паровоз"       - TV-VCR   
 *  2 - "включить/выключить" - ON standby  
 *  3 -                      - A.REP 
 * 20 - "начать движение"    - REC     
 * 21 - "назад"              - == 
 * 22 -                      - >|<     
 * 24 - "медленнее"          - <<    
 * 25 - "вперед"             - =>
 * 26 - "быстрее"            - >>   
 * 32 -                      - EJECT
 * 33 -                      - A.TRK
 * 44 - "подключить"         - TRACKING ON 
 * 45 - "отключить"          - TRACKING OFF 
 * 50 -                      - SYSTEM
 * 
**/

/**
 * Библиотека IRremote считывает сигнал и возвращает всю информацию о сигнале 
 * в следующей структуре:
 * 
 * struct IRData 
 * {
 *   decode_type_t protocol; // UNKNOWN, NEC, SONY, RC5, PULSE_DISTANCE, ...
 *   uint16_t address;       // Decoded address
 *   uint16_t command;       // Decoded command (Декодированная команда)
 *   uint16_t extra; 
 *   uint16_t numberOfBits;  // Количество бит, полученных при передаче данных
 *   uint8_t flags;          // IRDATA_FLAGS_IS_REPEAT, IRDATA_FLAGS_WAS_OVERFLOW 
 *   // До 32 бит декодированных необработанных данных 
 *   IRRawDataType decodedRawData; 
 *   // 32-битае декодированных необработанных данных для функции отправки
 *   uint32_t decodedRawDataArray[RAW_DATA_ARRAY_SIZE]; 
 *   // Указатель необработанных временных данных, подлежащих декодированию
 *   irparams_struct *rawDataPtr; // Pointer of the raw timing data to be decoded
 * };
**/

#define IR_RECEIVE_PIN 2

void setup() 
{
   Serial.begin(9600);
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() 
{
   if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      Serial.println(command);
      // Делаем задержку в 100 мс, чтобы замедлить печать при непрерывном 
      // нажатии клавиши, и устранить повторную распечатку
      delay(100);  
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }
}

// **************************************** proverka-komand-upravleniya.ino ***
