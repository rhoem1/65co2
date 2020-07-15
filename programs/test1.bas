trace
let X=int(rnd(1) * 6)
print "X is: ";X

on X gosub 100, 200, 300, 400, 500

print "bogus":end
100 print 100:return
200 print 200:return
300 print 300:return
400 print 400:return
500 print 500:return
