class PID():
    KU1,KU2,KE0,KE1,KE2,e1,e2,u1,u2,tmp,integral_error=0,0,0,0,0,0,0,0,0,0,0
    def __init__(self, KP,KI,KD,TS,Fcoef,KW):
        a0 = (1 + Fcoef * TS)
        a1 = -(2 + Fcoef * TS)
        a2 = 1

        b0=KP*(1+Fcoef * TS) + KI*TS*(1+ Fcoef*TS) + KD*Fcoef
        b1= -(KP*(2+Fcoef*TS) + KI*TS + 2*KD*Fcoef)
        b2=KP + KD * Fcoef

        self.KU1=a1/a0
        self.KU2=a2/a0
        self.KE0=b0/a0
        self.KE1=b1/a0
        self.KE2=b2/a0
        self.tmp=KW*(-self.KU1- self.KU2)/2
    def Call(self,Setpoint,Sensor):
        e0=Setpoint-Sensor
        u0 = -self.KU1*self.u1 - self.KU2 * self.u2 + (self.KE0*e0) \
        + (self.KE1*self.e1) + (self.KE2*self.e2)+self.tmp*self.integral_error
        if(u0 > 100):
           self. integral_error=  100-u0
        elif  ( u0 < 0):
            self.integral_error= 0-u0
        else:
           self.integral_error=0
        self.e2=self.e1
        self.e1=e0
        self.u2=self.u1
        self.u1=u0

        if(u0 > 100):
            u0= 100
        elif ( u0 < 0):
            u0= 0;    
        return u0