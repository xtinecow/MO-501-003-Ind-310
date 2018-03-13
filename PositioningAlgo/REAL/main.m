%% Open Node Client
NodeSocket = tcpip('192.168.56.1',8002);
NodeSocket.timeout = 2;
fopen(NodeSocket);

%% Open GUI Client
guiclient = tcpip('192.168.56.1',8001);
guiclient.timeout = 0.2;
fopen(guiclient);

%% Establish Constants

%RSSI Parsing
roverID = "Node4";

%RSSI to Distance Conversion Param
a = -0.69;
b = -48.7;
ref = -36.59;

%Kalman Filter Param
Q = eye(2,2);
initializeKalman = false;
x_original = [6.7, 5.1]; % Change for the program
targets = [0, 0; 0, 9.10; 14.9, 8.2; 14.9, 2.5; 6.7, 0]; %Change
targetList = ["Node1", "Node0", "Node3", "Node5", "Node2"]; %Change

%Convert to Lat Long
lat_o = 51.079948;
long_o =  -114.125534;

%Storage Constants
raw_rssi = zeros(1,3);
conv_dist = zeros(1,3);
kalman_coords = zeros(1,3);
kalman_delta = zeros(1,3);

%% While program is running
while(1)
    % Get RSSI Data from Node Program
    payload = "";
    while(1)
        payload  = fscanf(NodeSocket); 
        if(length(payload) > 1)
            break; 
        end
    end
    NodeList = ParseResults (payload);
    
    % Get Clock time
    c = clock;
    time = convertCharsToStrings(strcat(num2str(c(1,4)), ':', num2str(c(1,5)), ':', num2str(c(1,6))));
    
    % Parse RSSI Data
    rssi = GetRSSI(NodeList, roverID, targetList);
    
    for i=1:size(rssi,2)
        raw_rssi = [raw_rssi; [time, targetList(1,i), rssi(1,i)]];
    end
            
    % Convert RSSI to Distance
    for i = 1:size(rssi,2)
        dist(1,i) = RssiToDistanceLineFit(a, b, rssi(1,i));
    end
    
    for i=1:size(dist,2)
        conv_dist = [conv_dist; [time, targetList(1,i), dist(1,i)]];
    end

    % Kalman Filter
    if initializeKalman == false
        [KF_Coords, KF_Cx, KF_dx] = InitializeKF(x_original, targets, dist);
        kalman_coords = [kalman_coords; [time, KF_Coords(1,1), KF_Coords(1,2)]];
        kalman_delta = [kalman_delta; [time, KF_dx(1,1), KF_dx(2,1)]];
        initializeKalman = true;
        terminate = SentToGui(roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList);
    else
        [KF_Coords, KF_Cx, KF_dx] = KalmanFiltering(dist, targets, KF_Coords, KF_Cx, KF_dx, Q);
        kalman_coords = [kalman_coords; [time, KF_Coords(1,1), KF_Coords(1,2)]];
        kalman_delta = [kalman_delta; [time, KF_dx(1,1), KF_dx(2,1)]];
        terminate = SentToGui(roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList);
    end
    
%     Check for end response
    if(terminate) 
        break; 
    end

end


%% Close Node Client
fclose(NodeSocket);
delete(NodeSocket);
clear ClientSocket

%% Close GUI Client
fclose(guiclient);
delete(guiclient);
clear guiclient

%% Print to File
RawRssi = fopen('RawRssi.txt', 'w');
fprintf(RawRssi, '%s,%s,%s\n', raw_rssi');
fclose(RawRssi);

ConvertedDist = fopen('ConvertedDist.txt','w');
fprintf(ConvertedDist, '%s,%s,%s\n', conv_dist');
fclose(ConvertedDist);

Coordinates = fopen('Coordinates.txt','w');
fprintf(Coordinates, '%s,%s,%s\n', kalman_coords');
fclose(Coordinates);

Delta = fopen('Delta.txt','w');
fprintf(Delta, '%s,%s,%s\n', kalman_delta');
fclose(Delta);