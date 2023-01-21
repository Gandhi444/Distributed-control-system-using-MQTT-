%% odczyt topicu z MQTT i OBIEKTU Z SIMULINKA
clc
clear all


%Stworzenie klienta
myMQTT = mqttclient('tcp://localhost', ClientID="myClient", Timeout=100); %connect to broker
SensorTopic = 'Sensors'; %odczyt sensora?

%Nazwa Topicu dla MQTT
Controltopic = 'Control'
%Subskrybcja topicu
S = subscribe(myMQTT, SensorTopic);  % klient i topic jako argumenty

u = 200; % WARTOSC ZADANA
message = jsonencode(u); %parsowanie przez jsona
%write(myMQTT,Controltopic, message); %wrzucenie u  jako setpint


%DANE DO POMIARU
MSeconds_ = []; %wektor milisekund
Seconds_ = []; %wektor sekund (sekundy od 1970 roku)
Temperature_ = [0]; %temperatura dla ka�dej pr�bki
STimerStop = [];
%CTimerStop = [];
U_signal = [0];
error_ = [0];
SetPoint=u; %nadpisanie
Ts=0.15; %czas pr�bki - oswietlenie
%Ts=0.5 %czas probki
u0=0;

%NASTAWY R�CZNE PID  - TEMPERATURA
% Kp = 371.248;
% Ki =1.95;
% Kd = 0;
% FCoef = 1.015;

%NASTAWY - OSWIETLENIE
 Kp = 0.08;
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

%uproszczenie r�wnania
ku1=a1/a0;
ku2=a2/a0;
ke0=b0/a0;
ke1=b1/a0;
ke2=b2/a0;
tmp=KW*(-ku1-ku2)/2;

% WYKRESY

figure(1);
subplot(3,1,1)
Temperature_plot = plot(Temperature_,'r.-');
%hold on;
%plot(SetPoint);
%legend('Temperatura od czasu');
title("Pomiar temperatury w czasie");
xlabel('Nr Pr�bki');
ylabel('Temperatura [�C]');
set(gcf,'color','w'); 

Temperature_plot.YDataSource = 'Temperature_'
Temperature_=[];

subplot(3,1,2)

Control_plot = plot(U_signal,'b.-');

title('Sygna� Steruj�cy');
xlabel('Nr Pr�bki')
ylabel('Warto�� sygna�u steruj�cego');
set(gcf,'color','w'); 

Control_plot.YDataSource = 'U_signal'
U_signal=[];

subplot(3,1,3)
error_Plot = plot(error_,'r.-');
title("Uchyb");
xlabel('Nr Pr�bki');
ylabel('Warto�� uchybu');
set(gcf,'color','w'); 

error_Plot.YDataSource = 'error_'
error_=[];
%set()

first=1; %tmp 
while 1
    % Dane otrzymywane od MQTT
    message_read = read(myMQTT, Topic=SensorTopic);

    %KONIEC MIERZENIA CZASU PO wymuszeniu sygna�u steruj�cego
    %CTimerStop = [CTimerStop,toc(CTimerStop)];
    %for k=0:0%length(message_read.Properties.RowTimes)-1

    buf=size(message_read); %bufor otrzymywanej wiadomo�ci z topicu
    k=0;
    if  buf(1)>0
        text = message_read.Data(k+1);
        if first==0 
        %Zatrzymanie - pomiar czasu
        STimerStop=[STimerStop,toc(STimerStart)];
        else
            first=0;
           
        end
        %pocz�tek pomiaru
        STimerStart = tic;
        % DEKODOWANIE JSONA
        try
            %if(STimerStop(end)>=0.1)
            sensors = jsondecode(text);
           % MSeconds_ = [MSeconds_,sensors.ms];
            %Seconds_ = [Seconds_,sensors.s];

            %UCHYB
            

            Temperature_ = [Temperature_,sensors.temp];
            U_signal = [U_signal,u0];
            e0= SetPoint - Temperature_(end);
            error_ = [error_,e0];
            

            %Sensor Val to nasz "czujnik"
            SensorVal = Temperature_(end);

            %r�wnanie regulatora
            
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

            %ZATRZYMANIE TIMERA

            %STimerStop=[STimerStop,toc(STimerStart)];

            write(myMQTT,Controltopic, message); %zapis

            %CTimerStart=tic;

            %Control 
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


%PlantSim = out.PlantY2; %% nasze Y
%tim=0:0.01:5;
%plot(tim,PlantSim);


%% OBIEKT
% s=tf('s');
% 
% Plant = 100/(0.1*s + 1);
% step(Plant)


%% easy way to do
% Kp = 29.5;
% Ki = 1.13;
% Kd = 0.15;
% Nn = 2.3;
%C = pidstd(Kp,Ki,Kd,Nn)

%% Algorytm nummeryczny - testowy

%x wprowadzenie danych
%y sygna� steruj�cy  

%x - wejscie
%y - wyjscie
%n -od chwili czasowej


% Ts=0.01; %time sampling PID
% 
% setPoint = 100;
% Plant_Output = PlantSim;
% FCoef = 20;
% 
% A0 = Kp + Ki + Kd;
% A1 = (-Kp ) - (2 * Kd );
% A2 = Kd;
% e2=0;
% e1=0;
% u2=0;
% u1=0;
% 
% a0 = (1 + FCoef * Ts);
% a1 = -(2 + FCoef * Ts);
% a2 = 1;
% 
% b0=Kp*(1+FCoef * Ts) + Ki*Ts*(1+ FCoef*Ts) + Kd*FCoef;
% b1= -(Kp*(2+FCoef*Ts) + Ki*Ts + 2*Kd*FCoef);
% b2=Kp + Kd * FCoef;
% 
% uproszczen ie r�wnania
% ku1=a1/a0;
% ku2=a2/a0;
% ke0=b0/a0;
% ke1=b1/a0;
% ke2=b2/a0;
% 
% 
% wykonanie PID
% PiDAlgorythm(e2,e1,u2,u1,Plant_Output,setPoint,ku1,ku2,ke0,ke1,ke2);
% 
% function PiDAlgorythm(Er2,Er1,uu2,uu1,PlantO,SetP,kuu1,kuu2,kee0,kee1,kee2)
% nadpisywanie zmiennych
% 
%     tutaj odczyt plik�w
%     y = PlantO;
%     
%     e0= SetP - y; %uchyb
%     
%     u0= -(kuu1*uu1) - (kuu2 * uu2) + (kee0*e0) + (kee1*Er1) + (kee2*Er2); %sygna� steruj�cy
% 
%     Er1=e0;
%     Er2=Er1;
%     uu1=u0;
%     uu2=uu1;
%    
%     IF trzeba zrobic
%     T=table(u0);
%     writetable(T,'excel_file.txt','WriteVariableNames',false);
%     write(T,"excel_plik.mat",mat)
%     sys=feedback(y*u0,1);
% end
% 
% load("excel_file.txt")
% 
% 
% 
% u[n] = u[n-1] + A0 * e[n] + A1 * e[n-1] + A2 * e[n-2];
%   