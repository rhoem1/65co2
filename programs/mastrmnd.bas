2 PRINT TAB(30);"MASTERMIND"
4 PRINT TAB(15);"CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY"
6 PRINT: PRINT: PRINT
10 REM
20 REM     MASTERMIND II
30 REM     STEVE NORTH
40 REM     CREATIVE COMPUTING
50 REM     PO BOX 789-M MORRISTOWN NEW JERSEY 07960
60 REM
70 REM
80 INPUT "NUMBER OF COLORS";C9
90 IF C9>8 THEN PRINT "NO MORE THAN 8, PLEASE!":GOTO 80
100 INPUT "NUMBER OF POSITIONS";P9
110 INPUT "NUMBER OF ROUNDS";R9
120 P=C9^P9
130 PRINT "TOTAL POSSIBILITIES =";P
140 H=0:C=0
150 DIM Q(P9),S(10,2),S$(10),A$(P9),G$(P9),I(P),H$(P9)
160 L$="BWRGOYPT"
170 PRINT
180 PRINT
190 PRINT "COLOR     LETTER"
200 PRINT "=====     ======"
210 FOR X=1 TO C9
220 READ X$
230 PRINT X$;TAB(13);MID$(L$,X,1)
240 NEXT X
250 PRINT
260 FOR R=1 TO R9
270 PRINT 
280 PRINT "ROUND NUMBER";R;"----"
290 PRINT
300 PRINT "GUESS MY COMBINATION.":PRINT
310 REM     GET A COMBINATION
320 A=INT(P*RND(1)+1)
330 GOSUB 3000
340 FOR X=1 TO A
350 GOSUB 3500
360 NEXT X
370 FOR M=1 TO 10
380 PRINT "MOVE # ";M;" GUESS ";:INPUT X$
390 IF X$="BOARD" THEN 2000
400 IF X$="QUIT" THEN 2500
410 IF LEN(X$)<>P9 THEN PRINT "BAD NUMBER OF POSITIONS.":GOTO 380
420 REM     UNPACK X$ INTO G$(1-P9)
430 FOR X=1 TO P9
440 FOR Y=1 TO C9
450 IF MID$(X$,X,1)=MID$(L$,Y,1) THEN 480
460 NEXT Y
470 PRINT "'"; MID$(X$,X,1); "' IS UNRECOGNIZED.":GOTO 380
480 G$(X)=MID$(X$,X,1)
490 NEXT X
500 REM     NOW WE CONVERT Q(1-P9) INTO A$(1-P9) [ACTUAL GUESS]
510 GOSUB 4000
520 REM     AND GET NUMBER OF BLACKS AND WHITES
530 GOSUB 4500
540 IF B=P9 THEN 630
550 REM     TELL HUMAN RESULTS
560 PRINT "YOU HAVE ";B;" BLACKS AND ";W;" WHITES."
570 REM     SAVE ALL THIS STUFF FOR BOARD PRINTOUT LATER
580 S$(M)=X$
590 S(M,1)=B
600 S(M,2)=W
610 NEXT M
620 PRINT "YOU RAN OUT OF MOVES!  THAT'S ALL YOU GET!":GOTO 640
622 GOSUB 4000
623 PRINT "THE ACTUAL COMBINATION WAS: ";
624 FOR X=1 TO P9
625 PRINT A$(X);
626 NEXT X
627 PRINT
630 PRINT "YOU GUESSED IT IN ";M;" MOVES!"
640 H=H+M
650 GOSUB 5000
660 REM
670 REM     NOW COMPUTER GUESSES
680 REM
690 FOR X=1 TO P
700 I(X)=1
710 NEXT X
720 PRINT "NOW I GUESS.  THINK OF A COMBINATION."
730 INPUT "HIT RETURN WHEN READY:";X$
740 FOR M=1 TO 10
750 GOSUB 3000
760 REM     FIND A GUESS
770 G=INT(P*RND(1)+1)
780 IF I(G)=1 THEN 890
790 FOR X=G TO P
800 IF I(X)=1 THEN 880
810 NEXT X
820 FOR X=1 TO G
830 IF I(X)=1 THEN 880
840 NEXT X
850 PRINT "YOU HAVE GIVEN ME INCONSISTENT INFORMATION."
860 PRINT "TRY AGAIN, AND THIS TIME PLEASE BE MORE CAREFUL."
870 GOTO 660
880 G=X
890 REM     NOW WE CONVERT GUESS #G INTO G$
900 FOR X=1 TO G
910 GOSUB 3500
920 NEXT X
930 GOSUB 6000
940 PRINT "MY GUESS IS: ";
950 FOR X=1 TO P9
960 PRINT H$(X);
970 NEXT X
980 INPUT "  BLACKS, WHITES ";B1,W1
990 IF B1=P9 THEN 1120
1000 GOSUB 3000
1010 FOR X=1 TO P
1020 GOSUB 3500
1030 IF I(X)=0 THEN 1070
1035 GOSUB 6500
1040 GOSUB 4000
1050 GOSUB 4500
1060 IF B1<>B OR W1<>W THEN I(X)=0
1070 NEXT X
1080 NEXT M
1090 PRINT "I USED UP ALL MY MOVES!"
1100 PRINT "I GUESS MY CPU IS JUST HAVING AN OFF DAY."
1110 GOTO 1130
1120 PRINT "I GOT IT IN ";M;" MOVES!"
1130 C=C+M
1140 GOSUB 5000
1150 NEXT R
1160 PRINT "GAME OVER"
1170 PRINT "FINAL SCORE:"
1180 GOSUB 5040
1190 STOP
2000 REM
2010 REM     BOARD PRINTOUT ROUTINE
2020 REM
2025 PRINT
2030 PRINT "BOARD"
2040 PRINT "MOVE     GUESS          BLACK     WHITE"
2050 FOR Z=1 TO M-1
2060 PRINT Z;TAB(9);S$(Z);TAB(25);S(Z,1);TAB(35);S(Z,2)
2070 NEXT Z
2075 PRINT
2080 GOTO 380
2500 REM
2510 REM     QUIT ROUTINE
2520 REM
2530 PRINT "QUITTER!  MY COMBINATION WAS: ";
2535 GOSUB 4000
2540 FOR X=1 TO P9
2550 PRINT A$(X);
2560 NEXT X
2565 PRINT
2570 PRINT "GOOD BYE"
2580 STOP
3000 REM
3010 REM     INITIALIZE Q(1-P9) TO ZEROS
3020 REM
3030 FOR S=1 TO P9
3040 Q(S)=0
3050 NEXT S
3060 RETURN
3500 REM
3510 REM     INCREMENT Q(1-P9)
3520 REM
3522 IF Q(1)>0 THEN 3530
3524 REM  IF ZERO, THIS IS OUR FIRST INCREMENT: MAKE ALL ONES
3526 FOR S=1 TO P9
3527 Q(S)=1
3528 NEXT S
3529 RETURN
3530 Q=1
3540 Q(Q)=Q(Q)+1
3550 IF Q(Q)<=C9 THEN RETURN
3560 Q(Q)=1
3570 Q=Q+1
3580 GOTO 3540
4000 REM
4010 REM     CONVERT Q(1-P9) TO A$(1-P9)
4020 REM
4030 FOR S=1 TO P9
4040 A$(S)=MID$(L$,Q(S),1)
4050 NEXT S
4060 RETURN
4500 REM
4510 REM     GET NUMBER OF BLACKS (B) AND WHITES (W)
4520 REM     MASHES G$ AND A$ IN THE PROCESS
4530 REM
4540 B=0:W=0:F=0
4550 FOR S=1 TO P9
4560 IF G$(S)<>A$(S) THEN 4620
4570 B=B+1
4580 G$(S)=CHR$(F)
4590 A$(S)=CHR$(F+1)
4600 F=F+2
4610 GOTO 4660
4620 FOR T=1 TO P9
4630 IF G$(S)<>A$(T) THEN 4650
4640 IF G$(T)=A$(T) THEN 4650
4645 W=W+1:A$(T)=CHR$(F):G$(S)=CHR$(F+1):F=F+2:GOTO 4660
4650 NEXT T
4660 NEXT S
4670 RETURN
5000 REM
5010 REM     PRINT SCORE
5020 REM
5030 PRINT "SCORE:"
5040 PRINT "     COMPUTER ";C
5050 PRINT "     HUMAN    ";H
5060 PRINT
5070 RETURN
5500 REM
5510 REM     CONVERT Q(1-P9) INTO G$(1-P9)
5520 REM
5530 FOR S=1 TO P9
5540 G$(S)=MID$(L$,Q(S),1)
5550 NEXT S
5560 RETURN
6000 REM
6010 REM     CONVERT Q(1-P9) TO H$(1-P9)
6020 REM
6030 FOR S=1 TO P9
6040 H$(S)=MID$(L$,Q(S),1)
6050 NEXT S
6060 RETURN
6500 REM
6510 REM     COPY H$ INTO G$
6520 REM
6530 FOR S=1 TO P9
6540 G$(S)=H$(S)
6550 NEXT S
6560 RETURN
8000 REM     PROGRAM DATA FOR COLOR NAMES
8010 DATA BLACK,WHITE,RED,GREEN,ORANGE,YELLOW,PURPLE,TAN
9998 REM   ...WE'RE SORRY BUT IT'S TIME TO GO...
9999 END
