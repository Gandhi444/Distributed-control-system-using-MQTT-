%% odczyt topicu z MQTT i OBIEKTU Z SIMULINKA
clc
clear all

%Stworzenie klienta
myMQTT = mqttclient('tcp://localhost', ClientID="myClient", Timeout=100); %connect to broker
SensorTopic = 'Sensors'; %odczyt sensora?

%Nazwa Topicu dla MQTT
Controltopic = 'Control'
%Subskrybcja topicu

message = jsonencode(0); %parsowanie przez jsona
write(myMQTT,Controltopic, message); %wrzucenie u  jako setpint

Temperature_ = [0]; %temperatura dla każdej próbki
STimerStop = [];
%CTimerStop = [];
U_signal = [0];
error_ = [0];
SetPoint=21; %nadpisanie


%NASTAWY RĘCZNE PID OBIEKT SZYBKI
% Kp = 0.1;
% Ki =0.3;
% Kd = 0;
% FCoef = 1;

%NASTAWY RECZNE PID OBIEKT WOLNY
Kp = 371.248;
Ki =1.95;
Kd = 0;
FCoef = 1.015;

integral_error=0;
KW=0.1;
u0=0;


% WYKRESY

figure(1);
subplot(3,1,1)
Temperature_plot = plot(Temperature_,'r.-');
hold on;
plot(SetPoint);
%legend('Temperatura od czasu');
title("Pomiar temperatury w czasie");
xlabel('Nr Próbki');
ylabel('Temperatura [°C]');
set(gcf,'color','w'); 

Temperature_plot.YDataSource = 'Temperature_'
Temperature_=[];

subplot(3,1,2)
Control_plot = plot(U_signal,'b.-');
title('Sygnał Sterujący');
xlabel('Nr Próbki')
ylabel('Wartość sygnału sterującego');
set(gcf,'color','w'); 

Control_plot.YDataSource = 'U_signal'
U_signal=[];

subplot(3,1,3)
error_Plot = plot(error_,'r.-');
title("Uchyb");
xlabel('Nr Próbki');
ylabel('Wartość uchybu');
set(gcf,'color','w'); 

error_Plot.YDataSource = 'error_';
error_=[];

first=0; %tmp 
STimerStop=0;
Seconds_=[];
S = subscribe(myMQTT, SensorTopic);
while 1
    % Dane otrzymywane od MQTT POMIAR
    message_read = read(myMQTT, Topic=SensorTopic);

    buf=size(message_read); %bufor otrzymywanej wiadomości z topicu
    k=0;
    if  buf(1)>0
        text = message_read.Data(k+1);
        if first==1 
        %Zatrzymanie - pomiar czasu
        STimerStop=[STimerStop,toc(STimerStart)]; %CZas szacowanego opóźnienia
        aktualne_tp=toc(STimerStart)
        
        %REGULACJA
        a0 = (1 + FCoef * aktualne_tp);
        a1 = -(2 + FCoef * aktualne_tp);
        a2 = 1;

        b0=Kp*(1+FCoef * aktualne_tp) + Ki*aktualne_tp*(1+ FCoef*aktualne_tp) + Kd*FCoef;
        b1= -(Kp*(2+FCoef*aktualne_tp) + Ki*aktualne_tp + 2*Kd*FCoef);
        b2=Kp + Kd * FCoef;

        %uproszczenie równania
        ku1=a1/a0;
        ku2=a2/a0;
        ke0=b0/a0;
        ke1=b1/a0;
        ke2=b2/a0
        tmp=KW*(-ku1-ku2)/2;
        else
           
        %algorytm na podstawowy regulator PID
        e2=0;
        e1=0;
        u2=0;
        u1=0;
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
        tmp=KW*(-ku1-ku2)/2;

        first=1;
           
        end
        %początek pomiaru czasu
        STimerStart = tic;
        % DEKODOWANIE JSONA
        try
            sensors = jsondecode(text);
            Temperature_ = [Temperature_,sensors.Temperature];
            U_signal = [U_signal,u0];
            e0= SetPoint - Temperature_(end);
            error_ = [error_,e0];
            

            %Sensor Val to nasz "czujnik"
            SensorVal = Temperature_(end);

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
            message = jsonencode(u0); %parsowanie przez jsona?

            %end
            if (u0~=0)
            write(myMQTT,Controltopic, message); %zapis
            end
        catch
            disp('JSON FAULT');
            disp(fprintf("%s\r\n", text));

            continue;
        end
        refreshdata(Temperature_plot);
        refreshdata(Control_plot);
        refreshdata(error_Plot);
    end

    if strcmp(get(gcf,'currentcharacter'),'q') %comapre strings
      close(gcf);
      break
    end 
end