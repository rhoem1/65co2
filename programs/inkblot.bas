100 print "inkblot"
130 dim a1(13),a2(13),a3(13),a4(13),a5(13),a6(13),a7(13),a8(13),a9(13),aa(13),ab(13),ac(13),ad(13)
150 m=int(rnd(0)*8)+5
170 for l = 1 to m
180 a1(l)=rnd(1)*34
190 a2(l)=rnd(1)*80
200 a3(l)=(rnd(1)*15+2)^2
210 a4(l)=(rnd(1)*15+2)^2
220 t=3.14159 * rnd(1)
230 a5(l)=cos(t)
240 a6(l)=sin(t)
250 a7(l)=cos(t)*sin(t)
260 a5(l)=cos(t)*cos(t)
270 a6(l)=sin(t)*sin(t)
280 a8(l)=a1(l)*a1(l)*a5(l)
290 a9(l)=a1(l)*a1(l)*a6(l)
300 aa(l)=a1(l)*a7(l)
310 ab(l)=-2*a1(l)*a6(l)
320 ac(l)=-2*a1(l)*a5(l)
330 ad(l)=a6(l)/a4(l)+a5(l)/a3(l)
340 rem print a1(l),a2(l),a3(l),a4(l),a5(l),a6(l),a7(l),a8(l),a9(l),aa(l),ab(l),ac(l),ad(l)
350 next l
360 b$="$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
370 print b$;b$
380 print b$;b$
410 for y=79.9 to 0 step -1.6
420 a$="$$                                  "
450 for l=1 to m
460 y1=y-a2(l)
470 y2=y1*y1
480 y3=y1*aa(l)
490 y4=y1*a7(l)
500 b=(ac(l)+y4)/a3(l)+(-y4+ab(l))/a4(l)
510 c=(y2*a6(l)+a9(l)-y3)/a3(l)+(y2*a5(l)+a8(l)+y3)/a4(l)-1
530 r=b*b-4*ad(l)*c
540 if r<0 then 690
570 r1=int(0-(b+r)/2/ad(l)+1)
580 if r1>34 then 690
590 r2=int((r-b)/2/ad(l))
600 if r2<1 then 690
610 if r2<35 then 630
620 r2=34
630 if r1>0 then 660
640 r1=1
660 for j=r1+2 to r2+2
670 a$=left$(a$,j-1)+"$"+right$(a$,len(a$)-j)
680 next j
690 next l
710 print left$(a$,36);
720 for k=36 to 1 step -1
730 print mid$(left$(a$,36),k,1);
740 next k
745 print
750 next y
770 print b$;b$
780 print b$;b$
