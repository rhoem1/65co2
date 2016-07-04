130 dim a(13,13)
150 m=int(rnd(8))+5
170 for l = 1 to 3
180 t=3.14159 * rnd(1):print t
190 a(l,1)=rnd(1)*34:print a(l,1)
200 a(l,2)=rnd(1)*80:print a(l,2)
210 a(l,3)=(rnd(1)*15+2)^2:print a(l,3)
220 a(l,4)=(rnd(1)*15+2)^2:print a(l,4)
230 a(l,5)=cos(t)*cos(t):print a(l,5)
240 a(l,6)=sin(t)*sin(t):print a(l,6)
250 a(l,7)=cos(t)*sin(t):print a(l,7)
260 a(l,8)=a(l,1)*a(l,1)*a(l,5):print a(l,8)
270 a(l,9)=a(l,1)*a(l,1)*a(l,6):print a(l,9)
280 a(l,10)=a(l,1)*a(l,7):print a(l,10)
290 a(l,11)=-2*a(l,1)*a(l,6):print a(l,11)
300 a(l,12)=-2*a(l,1)*a(l,5):print a(l,12)
310 a(l,13)=a(l,6)/a(l,4)+a(l,5)/a(l,3):print a(l,13)
320 print
330 next l
