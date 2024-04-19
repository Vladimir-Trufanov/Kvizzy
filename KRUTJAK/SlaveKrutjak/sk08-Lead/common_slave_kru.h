/** KRUTJAK-SLAVE-PRO_MINI                           *** common_slave_kru.h ***
 * 
 * Определить общие функции управляющей системы паровозика "КРУТЯК"
 * 
 * v3.0, 19.04.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 19.04.2024
**/

#ifndef common_slave_kru
#define common_slave_kru
#pragma once            

// ****************************************************************************
// *          Подготовить и передать данные в управляющую систему             *
// ****************************************************************************
void sendState(byte currShim, byte currDir)
{
      // Определяем напряжение батареи
      VccSlave=analogRead_VCC();
      // Определяем напряжение на контакте мотора
      //            VccSlave --> 255
      //            x        --> currShim
      float  x = VccSlave * currShim / 255;
      // Определяем мощность на контакте мотора: P=U*U/R, где R = 13 Ом
      float  p = x * x / 13;
      
      
      /*
      if (ModeSlave==modeDebug) 
      {
         Serial.print("Vcc="); 
         Serial.println(VccSlave); 
      }
      */
      // Готовим и передаем данные в управляющую систему  
      sVcc = String(VccSlave,2);     // напряжение питания 
      sPwr = String(p,2);            // мощность на контакте 
      String d=String(currDir);      // направление движения     
      String shim=String(currShim);  // ШИМ на контакте     

      //serialSlave.println(sVcc+" "+sPwr+"#");
      serialSlave.println(shim+" "+sPwr+"  345");

      //serialSlave.println("v="+sVcc+" p="+sPwr+" d="+d+" sh"+shim+".");
      //serialSlave.println("ATvcc="+sVcc+".");
      delay(40); // выдержали паузу, чтобы команда спокойно ушла
}

#endif

// ***************************************************** common_slave_kru.h ***
