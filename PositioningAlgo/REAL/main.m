%% Open Node Client
NodeSocket = tcpip('10.13.183.68',8002);
NodeSocket.timeout = 2;
fopen(NodeSocket);

%% Open GUI Client
guiclient = tcpip('10.13.183.68',8001);
guiclient.timeout = 0.2;
fopen(guiclient);

%% Establish Constants

%RSSI Parsing
roverID = "Node5";

%RSSI to Distance Conversion Param
a = -0.69;
b = -48.7;
ref = -36.59;

%Kalman Filter Param
Q = eye(2,2);
initializeKalman = false;
x_original = [0, 1.75]; % Change for the program
targets = [0, 0; 0, 3.5; 20, 0; 20, 3.5; 30, 1.75]; %Change
targetList = ["Node0", "Node1", "Node2", "Node3", "Node4"]; %Change

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
    time = strcat(num2str(c(1,4)), num2str(c(1,5)), num2str(c(1,6)));
    
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
        kalman_coords = [kalman_coords; [time, KF_Coords]];
        kalman_delta = [kalman_delta; [time, KF_dx']];
        initializeKalman = true;
        SentToGui(roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList);
    else
        [KF_Coords, KF_Cx, KF_dx] = KalmanFiltering(dist, targets, KF_Coords, KF_Cx, KF_dx, Q);
        kalman_coords = [kalman_coords; [time, KF_Coords]];
        kalman_delta = [kalman_delta; [time, KF_dx']];
        SentToGui(roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList);
    end
    
%     Check for end response
    check = fscanf(guiclient, '%s');
    checkResponse = contains(check, 'DONE');
    flushinput(guiclient);
    if(checkResponse) 
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

%% Print to CSV
csvwrite('RawRssi.csv',raw_rssi);
csvwrite('ConvertedDist.csv', conv_dist);
csvwrite('Coordinates.csv', kalman_coord);
csvwrite('Delta.csv', kalman_delta);