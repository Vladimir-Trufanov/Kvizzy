/** KRUTJAK-MASTER-PRO_MINI                         *** common_master_kru.h ***
 * 
 * Определить общие функции управляющей системы паровозика "КРУТЯК"
 * 
 * v3.0, 17.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 17.04.2024
**/

#ifndef common_master_kru
#define common_master_kru
#pragma once            

// ****************************************************************************
// *                         Вывести информацию на монитор                    *
// ****************************************************************************
//void viewState(String sCommand, float VccSlave, float pwrSlave, byte dirMotor)
void viewState()
{
   cli();
   myOLED.print(typeData,5,4); 
   /*
   // Выводим последнюю команду, начиная с 80 столбца 1 строки 
   // (высота шрифта 2 строки, текст займёт строки 0 и 1).
   if (sCommand != oldCommand)
   {
      if (ModeSlave!=modeDebug)myOLED.print(sCommand,80,1); 
      oldCommand=sCommand;           
   }
   // Выводим текст начиная с 5 столбца 4 строки (высота шрифта 2 строки, он займёт строки 3 и 4).
   //myOLED.print(viewData,5,4); 
   myOLED.print(typeData,5,4); 
   
   // Готовим и выводим данные по напряжению питания
   if (VccSlave != oldVccSlv)
   {
      sVcc = String(VccSlave,2);
      myOLED.print(F("v"),0,7); myOLED.print(F(":"),10,7);                 
      myOLED.print(sVcc,22,7);
      oldVccSlv=VccSlave;           
   }
   // Определяем напряжение питания УПС
   VccMaster=analogRead_VCC();
   if (VccMaster != oldVccMst) 
   {
      sVcc = String(VccMaster,2);
      myOLED.print(F("-"),69,7); 
      myOLED.print(sVcc,80,7);                   
      oldVccMst=VccMaster; 
   }
   */          
   sei();
}

#endif

// **************************************************** common_master_kru.h ***
