%% odczyt topicu z MQTT i OBIEKTU Z SIMULINKA
clc
clear all;
%Stworzenie klienta
myMQTT = mqttclient('tcp://localhost', ClientID="myClient", Timeout=100); %connect to broker
SensorTopic = 'Sensors'; %odczyt sensora?

%Nazwa Topicu dla MQTT
Controltopic = 'Control';
%Subskrybcja topicu
S = subscribe(myMQTT, SensorTopic);  % klient i topic jako argumenty

u = 100; % WARTOSC ZADANA
message = jsonencode(u); %parsowanie przez jsona

%DANE DO POMIARU
MSeconds_ = []; %wektor milisekund
Seconds_ = []; %wektor sekund (sekundy od 1970 roku)
Light_ = [0]; %temperatura dla każdej próbki
STimerStop = [];
U_signal = [0];
error_ = [0];
SetPoint=u; %nadpisanie
Ts=0.15; %czas próbki
u0=0;

%NASTAWY RĘCZNE PID  - TEMPERATURA
% Kp = 371.248;
% Ki =1.95;
% Kd = 0;
% FCoef = 1.015;

%NASTAWY - OSWIETLENIE
 Kp = 0.1; %bylo 0.08
 Ki =0.3;
 Kd = 0;
 FCoef = 0;
%algorytm na podstawowy regulator PID
e2=0;
e1=0;
u2=0;
u1=0;
integral_error=0;
KW=0.1;


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

tmp=KW(-ku1-ku2)/2;

% WYKRESY

figure(1);
subplot(3,1,1)
Light_Plot = plot(Light_,'r.-');
title("Jasność w Luksach");
xlabel('Nr Próbki');
ylabel('Luks');
set(gcf,'color','w'); 

Light_Plot.YDataSource = 'Temperature_';
Light_=[];

subplot(3,1,2)

Control_plot = plot(U_signal,'b.-');

title('Sygnał Sterujący');
xlabel('Nr Próbki')
ylabel('Wartość sygnału sterującego');
set(gcf,'color','w'); 

Control_plot.YDataSource = 'U_signal';
U_signal=[];

subplot(3,1,3)
error_Plot = plot(error_,'r.-');
title("Uchyb");
xlabel('Nr Próbki');
ylabel('Wartość uchybu');
set(gcf,'color','w'); 

error_Plot.YDataSource = 'error_';
error_=[];
%set()

first=1; %tmp 
while 1
    % Dane otrzymywane od MQTT
    message_read = read(myMQTT, Topic=SensorTopic);
    buf=size(message_read); %bufor otrzymywanej wiadomości z topicu
    k=0;
    if  buf(1)>0
        text = message_read.Data(k+1);
        if first==0 
        else
            first=0;
           
        end
        try
            %if(STimerStop(end)>=0.1)
            sensors = jsondecode(text);
    
            %UCHYB        
            Light_ = [Light_,sensors.lux];
            U_signal = [U_signal,u0];
            e0= SetPoint - Light_(end);
            error_ = [error_,e0];
            
            %Sensor Val to nasz "czujnik"
            SensorVal = Light_(end);

            %równanie regulatora
            
            u0= -(ku1*u1) - (ku2 * u2) + (ke0*e0) + (ke1*e1) + (ke2*e2)+tmp*integral_error;
            if(u0>100)
                integral_error=100-u0;
            elseif(u0<0)
                integral_error=0-u0;
            else
                integral_error=0;
            end
            e2=e1;
            e1=e0;
            u2=u1;
            u1=u0;
            if(u0>100)
                u0=100;
            elseif(u0<0)
                u0=0;
            end

            message = jsonencode(u0); %parsowanie przez json

            write(myMQTT,Controltopic, message); %zapis

        catch
            disp('JSON FAULT');
            disp(fprintf("%s\r\n", text));

            continue;
        end
        refreshdata(Light_Plot);
        refreshdata(Control_plot);
        refreshdata(error_Plot);
    end

    if strcmp(get(gcf,'currentcharacter'),'q') %comapre strings
      close(gcf);
      break
    end 
end



