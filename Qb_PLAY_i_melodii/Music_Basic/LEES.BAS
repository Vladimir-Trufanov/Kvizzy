'Dit is een ASCII text-bestand lezer...
IF COMMAND$ = "" THEN GOTO Help ELSE GOTO LeesTxt

Help:
PRINT "TextLezer v1.0 (c) 1996 Laurens de Koning "
PRINT
PRINT "U moet de volgende syntax gebruiken:  "
PRINT "LEES [Filenaam.txt]"
PRINT
GOTO Einde

LeesTxt:
ON ERROR GOTO Fout
Filenaam$ = COMMAND$
OPEN Filenaam$ FOR INPUT AS #1
COLOR 14, 1
CLS
Regel = 1
DO
IF Regel = 20 THEN PRINT : COLOR 31: PRINT "-- Druk op een toets voor meer --": a$ = INPUT$(1): COLOR 14, 1: CLS : Regel = 1
LINE INPUT #1, TxtLine$
PRINT TxtLine$
Regel = Regel + 1
LOOP UNTIL EOF(1): CLOSE #1
PRINT
COLOR 31, 1: PRINT "Einde van het bestand:  '"; Filenaam$; "'..."
COLOR 15, 1: PRINT "Druk op een toets voor einde..."
a$ = INPUT$(1)
COLOR 7, 0
CLS
GOTO Einde

Einde:
RESET
SYSTEM

Fout:
SELECT CASE ERR
CASE 53
CLS
PRINT "Het bestand '"; Filenaam$; "' kon niet gevonden worden..."
PRINT
END SELECT
RESUME Einde

