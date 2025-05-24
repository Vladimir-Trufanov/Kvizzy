## [Programming-Arduino-with-Regular-Expressions](#)


### [Regexp](https://github.com/nickgammon/Regexp/blob/master/README.md)

#### Пример 1.

```
// sketch_jan22b.ino

#include <Regexp.h>

// called for each match
void match_callback  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms)      // MatchState in use (to get captures)
{
char cap [10];   // must be large enough to hold captures
  
  Serial.print ("Matched: ");
  Serial.write ((byte *) match, length);
  Serial.println ();
  
  for (byte i = 0; i < ms.level; i++)
    {
    Serial.print ("Capture "); 
    Serial.print (i, DEC);
    Serial.print (" = ");
    ms.GetCapture (cap, i);
    Serial.println (cap); 
    }  // end of for each capture

}  // end of match_callback 


void setup ()
{
  Serial.begin (115200);
  Serial.println ();
  unsigned long count;

  // what we are searching (the target)
  char buf [100] = "The quick brown fox jumps over the lazy wolf";

  // match state object
  MatchState ms (buf);

  // original buffer
  Serial.println (buf);

  // search for three letters followed by a space (two captures)
  count = ms.GlobalMatch ("(%a+)( )", match_callback);

  // show results
  Serial.print ("Found ");
  Serial.print (count);            // 8 in this case
  Serial.println (" matches.");
 

}  // end of setup  

void loop () {}
```

#### Реакция:

```
The quick brown fox jumps over the lazy wolf
Matched: The 
Capture 0 = The
Capture 1 =  
Matched: quick 
Capture 0 = quick
Capture 1 =  
Matched: brown 
Capture 0 = brown
Capture 1 =  
Matched: fox 
Capture 0 = fox
Capture 1 =  
Matched: jumps 
Capture 0 = jumps
Capture 1 =  
Matched: over 
Capture 0 = over
Capture 1 =  
Matched: the 
Capture 0 = the
Capture 1 =  
Matched: lazy 
Capture 0 = lazy
Capture 1 =  
Found 8 matches.

```

#### Пример 2.

```
```





### Библиография

#### [string.find](https://www.gammon.com.au/scripts/doc.php?lua=string.find)

#### [Programming-Arduino-with-Regular-Expressions](https://blog.banjobatman.com/2021/07/programming-arduino-with-regular.html)

#### [Regular expressions (C++)](https://learn.microsoft.com/en-us/cpp/standard-library/regular-expressions-cpp?view=msvc-170)

#### [Напоминание про String => data.c_str()](https://arduino.ru/forum/programmirovanie/regexp)










###### [в начало](#)
