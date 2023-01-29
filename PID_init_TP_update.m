function [KU1,KU2,KE0,KE1,KE2] = PID_init(KP,KI,KD,TS,DELAY)
Kp=KP %                    Kp = 0.339771816474273;
Ki=KI %                    Ki =0.00523547502460297;
Kd=KD %                    Kd = 1.03197566814099;
FCoef = 1.01565558025853
%FCoef=0
delay=DELAY
CorrectedTp=TS; % 
persistent init actualTp previousTp time_vec i; 

if(isempty(init))
    previousTp=0;
    time_vec=0;
    actualTp=0;
    i=0;
    init = 1;
end
actualTp=i+delay;
time_vec = [time_vec,actualTp];
previousTp=time_vec(end-1);

CorrectedTp = actualTp - previousTp


a0 = (1 + FCoef * CorrectedTp);
a1 = -(2 + FCoef * CorrectedTp);
a2 = 1;

b0=Kp*(1+FCoef * CorrectedTp) + Ki*CorrectedTp*(1+ FCoef*CorrectedTp) + Kd*FCoef;
b1= -(Kp*(2+FCoef*CorrectedTp) + Ki*CorrectedTp + 2*Kd*FCoef);
b2=Kp + Kd * FCoef;


%uproszczenie równania
ku1=a1/a0;
ku2=a2/a0;
ke0=b0/a0;
ke1=b1/a0;
ke2=b2/a0;


KU1=ku1;
KU2=ku2;
KE0=ke0;
KE1=ke1;
KE2=ke2;

i=i+0.5;
end

