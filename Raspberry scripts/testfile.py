import time
i=0
step_freq=10
while True:
   i+=1
   duty=10*(int(i/step_freq)%11)
   duty=-1
   print(max(0,min(duty, 100)))
   #print(duty)