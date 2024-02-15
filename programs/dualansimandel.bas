REM MANDELBROT
REM INPUT VARS
REM ZOOM
1 MX = 0.03
2 MY = 0.06
REM TOP BOTTOM LEFT RIGHT
3 T = -20
4 B = 20
5 L = -59
6 R = 19
REM COLOR RANGE
7 CB = 32
8 CE = 1
9 CS = -1

REM ANSI MAGIC
100 E$=CHR$(27)+"[":M$=CHR$(109):C$=CHR$(109)


1010 FOR Y=T TO B
1020 FOR X=L TO R 
1030 CX=X*MX
1040 CY=Y*MY
1050 EX=CX
1060 EY=CY
1070 FOR C=CB TO CE STEP CS
1080 ET=EX*EX-EY*EY+CX
1090 EY=2*EX*EY+CY
1100 EX=ET
1110 IF(EX*EX+EY*EY)>4 GOTO 1130
1120 NEXT C
1130 PRINT E$;"48;5;";C;M$;" ";
1140 NEXT X
1150 PRINT E$;"0";M$
1160 NEXT Y

2000 END


