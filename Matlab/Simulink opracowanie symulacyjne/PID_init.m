
%Plik m inicjalizujący regulacje dla układów symulacyjnych
function [KU1,KU2,KE0,KE1,KE2] = PID_init(KP,KI,KD,TS)
Kp=KP %                    Kp = 0.339771816474273;
Ki=KI %                    Ki =0.00523547502460297;
Kd=KD %                    Kd = 1.03197566814099;
FCoef = 1;
%FCoef=0
Ts=TS %                    Ts=0.1; %czas próbki



a0 = (1 + FCoef * Ts);
a1 = -(2 + FCoef * Ts);
a2 = 1;

b0=Kp*(1+FCoef * Ts) + Ki*Ts*(1+ FCoef*Ts) + Kd*FCoef;
b1= -(Kp*(2+FCoef*Ts) + Ki*Ts + 2*Kd*FCoef);
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

end

