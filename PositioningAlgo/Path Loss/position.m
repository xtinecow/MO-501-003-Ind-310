clear all;
close all;

%% Plot all rssi test data
Hallway_Oct = csvread('rssi.csv');
Hallway_Feb = csvread('hallway.csv');
Distance = Hallway_Feb(:,1);
Hallway_Feb = Hallway_Feb(:,2);
Lounge = csvread('lounge.csv');
Lounge = Lounge(:,2);
Lab = csvread('lab.csv');
Lab = Lab(:,2);

LogShadowingModel = Hallway_Oct(1,1) - 10 * 2 * log10(Distance);
AllRSSI = zeros(length(Distance),5);
AllRSSI(:,1) = Distance;
AllRSSI(:,2) = Hallway_Oct;
AllRSSI(:,3) = Hallway_Feb;
AllRSSI(:,4) = Lounge;
AllRSSI(:,5) = Lab;

[a, b] = LineFitParameters(AllRSSI);
LineFit = a*Distance + b;

figure;
plot(Distance, Hallway_Oct, 'r--*');
hold on;
plot(Distance, Hallway_Feb, 'b--*');
hold on;
plot(Distance, Lounge, 'g--*');
hold on;
plot(Distance, Lab, 'c--*');
hold on;
plot(Distance, LogShadowingModel, 'm');
hold on;
plot(Distance, LineFit,'k');
legend('Hallway-October', 'Hallway-February', 'Lounge-February', 'Lab-February', 'Actual (assume free space)','Linefit');

%% Convert RSSI to Distance
for i=1:length(Distance)
    Hallway_Oct_Distance(i,1) = RssiToDistanceLineFit(a, b, Hallway_Oct(i,1), Hallway_Oct(1,1));
    Hallway_Feb_Distance(i,1) = RssiToDistanceLineFit(a, b, Hallway_Feb(i,1), Hallway_Oct(1,1));
    Lounge_Distance(i,1) = RssiToDistanceLineFit(a, b, Lounge(i,1), Hallway_Oct(1,1));
    Lab_Distance(i,1) = RssiToDistanceLineFit(a, b, Lab(i,1), Hallway_Oct(1,1));
end

figure;
plot(Distance, Distance, 'g');
hold on;
plot(Hallway_Oct_Distance, Hallway_Oct_Distance, 'r^');
hold on;
plot(Hallway_Feb_Distance, Hallway_Feb_Distance, 'bo');
hold on;
plot(Lounge_Distance, Lounge_Distance, 'm*');
hold on;
plot(Lab_Distance, Lab_Distance, 'c^');
legend('Actual Distance', 'Hallway-October', 'Hallway-February', 'Lounge-February', 'Lab-February');
