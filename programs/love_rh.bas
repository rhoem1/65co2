60 INPUT "YOUR MESSAGE, PLEASE ";A$
61 L = LEN(A$)
70 DIM T$(120)
100 FOR J = 0 TO (60/L)
110 FOR I = 1 TO L
120 T$(J*L+I) = MID$(A$,I,1)
130 NEXT I
131 NEXT J
140 C = 0
200 A1 = 1
201 P = 1
202 C = C+1
203 IF C = 37 THEN 999
205 PRINT
210 READ A
211 A1 = A1+A
212 IF P = 1 THEN 300
240 FOR I = 1 TO A
241 PRINT " ";
242 NEXT I
243 P = 1
244 GOTO 400
300 FOR I = A1-A TO A1-1
301 PRINT T$(I);
302 NEXT I
303 P = 0
400 IF A1 > 60 THEN 200
410 GOTO 210
600 DATA 60,1,12,26,9,12,3,8,24,17,8,4,6,23,21,6,4,6,22,12,5,6,5
610 DATA 4,6,21,11,8,6,4,4,6,21,10,10,5,4,4,6,21,9,11,5,4
620 DATA 4,6,21,8,11,6,4,4,6,21,7,11,7,4,4,6,21,6,11,8,4
630 DATA 4,6,19,1,1,5,11,9,4,4,6,19,1,1,5,10,10,4,4,6,18,2,1,6,8,11,4
640 DATA 4,6,17,3,1,7,5,13,4,4,6,15,5,2,23,5,1,29,5,17,8
650 DATA 1,29,9,9,12,1,13,5,40,1,1,13,5,40,1,4,6,13,3,10,6,12,5,1
660 DATA 5,6,11,3,11,6,14,3,1,5,6,11,3,11,6,15,2,1
670 DATA 6,6,9,3,12,6,16,1,1,6,6,9,3,12,6,7,1,10
680 DATA 7,6,7,3,13,6,6,2,10,7,6,7,3,13,14,10,8,6,5,3,14,6,6,2,10
690 DATA 8,6,5,3,14,6,7,1,10,9,6,3,3,15,6,16,1,1
700 DATA 9,6,3,3,15,6,15,2,1,10,6,1,3,16,6,14,3,1,10,10,16,6,12,5,1
710 DATA 11,8,13,27,1,11,8,13,27,1,60
999 PRINT : END
