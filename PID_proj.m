%% odczyt topicu z MQTT i OBIEKTU Z SIMULINKA

myMQTT = mqttclient('tcp://localhost', ClientID="myClient", Timeout=100); %connect to broker
topic = 'Sensors'; %odczyt sensora?

Controltopic = 'Control'

S = subscribe(myMQTT, topic);  % klient i topic jako argumenty

u = 25; %??
message = jsonencode(u); %parsowanie przez jsona?
write(myMQTT,Controltopic, message); %zapis


MSeconds_ = [];
Seconds_ = [];
Temperature_ = [0]; 
SetPoint=25;
Ts=1;


Kp = 10000;
Ki = 1.13;
Kd = 0.5;
FCoef = 2.3;

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


figure(1);
Temperature_plot = plot(Temperature_,'r.-');
%legend('Temperatura od czasu');
title("Odczyt temperatury");
xlabel('Próbka w sekundach');
ylabel('Temperatura');
set(gcf,'color','w'); 

Temperature_plot.YDataSource = 'Temperature_'
Temperature_=[];
while 1
    % Receive data by MQTT
    message_read = read(myMQTT, Topic=topic);
    for k=0:length(message_read.Properties.RowTimes)-1
        text = message_read.Data(k+1);
        % JSON decode
        try
            sensors = jsondecode(text)
            MSeconds_ = [MSeconds_,sensors.ms]
            Seconds_ = [Seconds_,sensors.s]
            Temperature_ = [Temperature_,sensors.temp]

            %równanie regulatora
            e0= SetPoint - Temperature_(end)
            u0= -(ku1*u1) - (ku2 * u2) + (ke0*e0) + (ke1*e1) + (ke2*e2);
            e1=e0;
            e2=e1;
            u1=u0;
            u2=u1;
            message = jsonencode(u0); %parsowanie przez jsona?
            write(myMQTT,Controltopic, message); %zapis
        catch
            disp('JSON FAULT');
            disp(fprintf("%s\r\n", text));

            continue;
        end
        refreshdata(Temperature_plot);
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
%y sygna³ steruj¹cy  

%x - wejscie
%y - wyjscie
%n -od chwili czasowej


Ts=0.01; %time sampling PID

setPoint = 100;
Plant_Output = PlantSim;
FCoef = 20;

% A0 = Kp + Ki + Kd;
% A1 = (-Kp ) - (2 * Kd );
% A2 = Kd;
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


%wykonanie PID
PiDAlgorythm(e2,e1,u2,u1,Plant_Output,setPoint,ku1,ku2,ke0,ke1,ke2);


function PiDAlgorythm(Er2,Er1,uu2,uu1,PlantO,SetP,kuu1,kuu2,kee0,kee1,kee2)
%nadpisywanie zmiennych

    %tutaj odczyt plików
    y = PlantO;
    
    e0= SetP - y; %uchyb
    
    u0= -(kuu1*uu1) - (kuu2 * uu2) + (kee0*e0) + (kee1*Er1) + (kee2*Er2); %sygna³ steruj¹cy

    Er1=e0;
    Er2=Er1;
    uu1=u0;
    uu2=uu1;
   
    %IF trzeba zrobic
    T=table(u0);
    writetable(T,'excel_file.txt','WriteVariableNames',false);
    %write(T,"excel_plik.mat",mat)
    %sys=feedback(y*u0,1);
end

%load("excel_file.txt")



%u[n] = u[n-1] + A0 * e[n] + A1 * e[n-1] + A2 * e[n-2];
  