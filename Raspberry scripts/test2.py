from PID import PID
pid=PID(371.248,1.95,0,0.5,1.015,0.1)
sens=20
while sens<22.5:
    sens=sens+0.01
    print(sens,"   ",pid.Call(22,sens))