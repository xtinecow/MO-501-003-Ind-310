%% Open GUI Client
guiclient = tcpip('10.13.115.218',8001);
guiclient.timeout = 0.2;
fopen(guiclient);

%% Establish Constants

%RSSI Parsing
roverID = 1;

%RSSI to Distance Conversion Param
ref = -36.59;

%Kalman Filter Param
Q = eye(2,2);
initializeKalman = false;
x_original = [0, 1.75]; % Change for the program
targets = [0, 0; 0, 3.5; 20, 0; 20, 3.5; 30, 1.75];

%Convert to Lat Long
lat_o = 51.079948;
long_o =  -114.125534;

%% While program is running
    
% Use Simulated RSSI Data
rssiObservation = csvread('SimulatedRSSI.csv');
for epoch = 1:size(rssiObservation,1)
    NodeList = RSSIToStruct(rssiObservation(epoch,:), roverID);
    % Parse RSSI Data
    rssi = GetRSSI(NodeList, roverID);

    % Convert RSSI to Distance
    for i = 1:size(rssi,2)
        dist(1,i) = RssiToDistanceFreeSpace(rssi(1,i), ref );
    end

    % Kalman Filter
    if initializeKalman == false
        [KF_Coords, KF_Cx, KF_dx] = InitializeKF(x_original, targets, dist);
        initializeKalman = true;
%         [lat, long, er1, er2] = SentToGui(guiclient, KF_Coords, lat_o, long_o, KF_Cx);
        SentToGui(guiclient, KF_Coords, lat_o, long_o, KF_Cx);
    else
        [KF_Coords, KF_Cx, KF_dx] = KalmanFiltering(dist, targets, KF_Coords, KF_Cx, KF_dx, Q);
%         [lat, long, er1, er2] = SentToGui(guiclient, KF_Coords, lat_o, long_o, KF_Cx);
        SentToGui(guiclient, KF_Coords, lat_o, long_o, KF_Cx);
    end
end

%% Close GUI Client
fclose(guiclient);
delete(guiclient);
clear guiclient