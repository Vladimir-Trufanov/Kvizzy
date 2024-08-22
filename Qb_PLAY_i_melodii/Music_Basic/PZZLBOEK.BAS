REM ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
REM ³          Elektronisch Puzzel WoordenBoek Versie 1.0 (beta 1)          ³
REM ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

SCREEN 0
DIM maand$(12)
maand$(1) = "Jan"
maand$(2) = "Feb"
maand$(3) = "Mar"
maand$(4) = "Apr"
maand$(5) = "Mei"
maand$(6) = "Jun"
maand$(7) = "Jul"
maand$(8) = "Aug"
maand$(9) = "Sep"
maand$(10) = "Okt"
maand$(11) = "Nov"
maand$(12) = "Dec"

programmanaam$ = "Electronisch Puzzel WoordenBoek"
Versie$ = "Versie 1.0 (beta 1)"
achtergrondstring$ = "°"
achtergrondkleur1 = 1
achtergrondkleur2 = 0
bovenstring$ = programmanaam$ + " " + Versie$
bovenkleur1 = 15
bovenkleur2 = 1
onderkleur1 = 11
onderkleur2 = 1
klokkleur1 = 11
klokkleur2 = 1

TIMER OFF
ON TIMER(1) GOSUB tijd
GOSUB klokstring

GOSUB schoonscherm

vensterkleur1 = 9
vensterkleur2 = 1
vensterpositie1 = 4
vensterpositie2 = 3
vensterpositie3 = 70
vensterpositie4 = 1
GOSUB venster

vensterkleur1 = 9
vensterkleur2 = 1
vensterpositie1 = 4
vensterpositie2 = 8
vensterpositie3 = 70
vensterpositie4 = 13
GOSUB venster

DO
LOOP UNTIL INKEY$ <> ""

END
a$ = ""
FOR a = 0 TO 20
LOCATE 3 + a, 2
COLOR 11, 0
a$ = LEFT$(a$, 78)
PRINT a$;
COLOR 8, 0
PRINT STRING$(78 - LEN(a$), ".");
NEXT a

END

schoonscherm:
TIMER OFF
IF schoonscherm = 0 THEN LOCATE 25, 1, 0: COLOR 15, 0: FOR a = 0 TO 24: PRINT : FOR b = 0 TO 300: NEXT b: NEXT a: schoonscherm = 1
a$ = MID$(achtergrondstring$, INT(LEN(achtergrondstring$ + " ") / 2), LEN(achtergrondstring$))
FOR a = 0 TO 100
a$ = a$ + achtergrondstring$
NEXT a
FOR a = 0 TO 79
LOCATE 1, 1 + a
IF a < 57 THEN COLOR bovenkleur1, bovenkleur2 ELSE COLOR klokkleur1, klokkleur2
PRINT MID$(" " + LEFT$(bovenstring$ + SPACE$(56), 56) + klokstring$ + STRING$(80, 32), 1 + a, 1);
COLOR achtergrondkleur1, achtergrondkleur2
FOR b = 0 TO 22
LOCATE 2 + b, 1 + a
PRINT MID$(a$, 1 + b + a, 1);
NEXT b
LOCATE 25, 1 + a
COLOR onderkleur1, onderkleur2
PRINT " ";
NEXT a
TIMER ON
RETURN

tijd:
oudeposx = POS(1)
oudeposy = CSRLIN

LOCATE 1, 58
COLOR klokkleur1, klokkleur2
GOSUB klokstring
PRINT klokstring$;
LOCATE oudeposy, oudeposx
RETURN

klokstring:
IF klokpunt < 1 THEN klokpunt = klokpunt + 1: klokpunt$ = ":" ELSE klokpunt = 0: klokpunt$ = " "
klokstring$ = "³ " + MID$(DATE$, 4, 2) + "-" + maand$(VAL(LEFT$(DATE$, 2))) + "-" + RIGHT$(DATE$, 4) + " ³ " + LEFT$(TIME$, 2) + klokpunt$ + MID$(TIME$, 4, 2) + " ³"
RETURN

venster:
TIMER OFF
LOCATE vensterpositie2, vensterpositie1
COLOR vensterkleur1, vensterkleur2
PRINT " Ú" + STRING$(vensterpositie3, "Ä") + "¿ ";
FOR a = 1 TO vensterpositie4
LOCATE vensterpositie2 + a, vensterpositie1
COLOR vensterkleur1, vensterkleur2
PRINT " ³" + STRING$(vensterpositie3, " ") + "³ ";
COLOR 15, 0
PRINT "  ";
NEXT a
LOCATE vensterpositie2 + a, vensterpositie1
COLOR vensterkleur1, vensterkleur2
PRINT " À" + STRING$(vensterpositie3, "Ä") + "Ù ";
COLOR 15, 0
PRINT "  ";
LOCATE 1 + vensterpositie2 + a, 2 + vensterpositie1
PRINT STRING$(4 + vensterpositie3, " ");
TIMER ON
RETURN

