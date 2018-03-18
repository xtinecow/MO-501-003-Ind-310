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
a1 = -0.69;
b1 = -48.7;
a2 = -3.79;
b2 = -37.55;
ref = -36.59;

%Kalman Filter Param
Q = eye(2,2);
initializeKalman = false;
x_original = [7.4, 4.4]; % Change for the program
targets = [0, 0; 6.5, 0; 14.9, 0; 14.9, 8.4; 0, 8.04]; %Change
targetList = ["Node3", "Node2", "Node1", "Node0", "Node5"]; %Change

%Convert to Lat Long
lat_o = 51.079948;
long_o =  -114.125534;

%Open File
RawRssi = fopen('RawRssi.txt', 'w');
ConvertedDist = fopen('ConvertedDist.txt','w');
Coordinates = fopen('Coordinates.txt','w');
Delta = fopen('Delta.txt','w');

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
        fprintf(RawRssi, '%s,%s,%s\n', [time, targetList(1,i), rssi(1,i)]');
    end
            
    % Convert RSSI to Distance
    for i = 1:size(rssi,2)
        dist(1,i) = RssiToDistanceLineFit(a1, b1, a2, b2, rssi(1,i));
    end
    
    for i=1:size(dist,2)
        fprintf(ConvertedDist, '%s,%s,%s\n', [time, targetList(1,i), dist(1,i)]');
    end

    % Kalman Filter
    if initializeKalman == false
        [KF_Coords, KF_Cx, KF_dx] = InitializeKF(x_original, targets, dist);
        fprintf(Coordinates, '%s,%s,%s\n', [time, KF_Coords(1,1), KF_Coords(1,2)]');
        fprintf(Delta, '%s,%s,%s\n', [time, KF_dx(1,1), KF_dx(2,1)]');
        initializeKalman = true;
        terminate = SentToGui(roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList);
    else
        [KF_Coords, KF_Cx, KF_dx] = KalmanFiltering(dist, targets, KF_Coords, KF_Cx, KF_dx, Q);
        fprintf(Coordinates, '%s,%s,%s\n', [time, KF_Coords(1,1), KF_Coords(1,2)]');
        fprintf(Delta, '%s,%s,%s\n', [time, KF_dx(1,1), KF_dx(2,1)]');
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
fclose(RawRssi);
fclose(ConvertedDist);
fclose(Coordinates);
fclose(Delta);