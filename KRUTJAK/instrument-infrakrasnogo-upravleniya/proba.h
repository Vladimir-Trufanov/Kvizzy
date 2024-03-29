#ifndef proba_h
#define proba_h
 
#pragma once            // обеспечили разовое подключение файла
#include <Arduino.h>    // подключили оьщие функции Arduino

class ProbaX 
{
   private:
      int PinRes;       // аналоговый вход, к которому подключен потенциометр
   public:
      void Driver(int ValRes); 
};

#endif
