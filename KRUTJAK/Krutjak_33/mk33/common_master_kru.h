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
// *        Проверить данные по напряжениям питания и вывести на монитор      *
// ****************************************************************************
void viewSupplyVoltage()
{
   // Считаем, что данные не изменились
   isChanged = false;
   // Переопределяем напряжения УПС
   oldVccMst = VccMaster;
   Vcc=analogRead_VCC();
   VccMaster = String(Vcc,2);
   // Если напряжение УПС изменилось, то установливаем признак
   if (VccMaster != oldVccMst) isChanged = true;
   // Иначе переопределяем напряжения ИСКР
   {
      oldVccSlv = VccSlave;     
      VccSlave=viewData.substring(3,7);
      // Если напряжение ИСКР изменилось, то установливаем признак
      if (VccSlave != oldVccSlv) isChanged = true;
   }
   // Если данные изменились, то выводим их в монитор
   if (isChanged == true)
   {
      myOLED.print(F("v"),0,7); myOLED.print(F(":"),10,7);                 
      myOLED.print(VccSlave,22,7);
      myOLED.print(F("-"),69,7); 
      myOLED.print(VccMaster,80,7);                   
   }
} 

// ****************************************************************************
// *                         Вывести информацию на монитор                    *
// ****************************************************************************
void viewState()
{
   if (typeData=="-" || typeData=="+")
   {
      myOLED.print(viewData,3,4); 
   }
   // Выводим напряжения питания систем
   else if (typeData=="~")
   {
      viewSupplyVoltage();
   }
   // Подтверждаем отправленную команду
   else if (typeData=="!")
   {
      myOLED.print("c"+viewData.substring(3,5),0,1); 
   }
   else
   {
      myOLED.print("NoDefine",0,1); 
   }
}

#endif

// **************************************************** common_master_kru.h ***
