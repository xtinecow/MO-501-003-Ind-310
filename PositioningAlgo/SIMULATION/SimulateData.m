clear all;
close all;

%% Define Targets

maxEpoch = 216;
numTargets = 5;
targets = [0, 0; 0, 3.5; 20, 0; 20, 3.5; 30, 1.75];

x_update = 30 / maxEpoch;

x_original = [0, 1.75];

rover_Coord = zeros(maxEpoch+1, 2);
rover_Coord(:,2) = 1.75;
for i = 2:maxEpoch+1
    rover_Coord(i,1) = rover_Coord(i-1,1) + x_update;
end
rover_Coord(1,:) = [];

observation = zeros(maxEpoch, 5);
for i=1:maxEpoch
    observation(i,:) = SimulateDistance(targets, rover_Coord(i,:));
end

%% Kalman Filtering

Q = eye(2,2);
[KF_Coords, KF_Cx, KF_dx] = InitializeKF(x_original, targets, observation(1,:));
KFCoords_final = zeros(maxEpoch, 2);
KFCoords_final(1,:) = KF_Coords;
for i=2:maxEpoch
    [KF_Coords, KF_Cx, KF_dx] = KalmanFiltering(observation(i,:), targets, KF_Coords, KF_Cx, KF_dx, Q);
    KFCoords_final(i,:) = KF_Coords;
end


%% Distance to RSSI
ref = -36.59;
rssiObservation = zeros(size(observation));
for i = 1:size(observation,1)
    for j = 1:size(observation,2)
        rssiObservation(i,j) = DistanceToRSSIFreeSpace(ref, observation(i,j));
    end
end

csvwrite('SimulatedRSSI.csv',rssiObservation);
csvwrite('SimulatedDistance.csv',observation);
%% TO DO: FIX EKF MAYBE

% [EKF_Coords, EKF_Cx, EKF_dx] = InitializeKF(x_original, targets, observation(1,:));
% EKFCoords_final = zeros(maxEpoch, 2);
% EKFCoords_final(1,:) = EKF_Coords;
% for i=2:maxEpoch
%     for j=1:numTargets
%         [EKF_Cx, EKF_dx] = ExtendedKalmanFiltering(targets, j, EKF_Coords, observation(i,j), EKF_Cx, Q);
%     end
%     EKF_Coords = EKF_Coords + EKF_dx';
%     EKFCoords_final(i,:) = EKF_Coords;
% end
% figure;
% scatter(KFCoords_final(:,1), KFCoords_final(:,2));
% hold on;
% scatter(EKFCoords_final(:,1), EKFCoords_final(:,2));
% hold off;
% 
% maxObs = maxEpoch * 5;
% asyncObservations = zeros(maxObs, 2);
% asyncObservations (:,1) = repmat([1; 2; 3; 4; 5], maxEpoch, 1);
% asyncObservations (:,2) = reshape(observation', [maxObs, 1]);
% 
% EKalmanFilterCoords_final = zeros(maxObs, 2);
% ii = 1;
% initializeEKF = 0;
% [EKF_Coord, EKF_Cx] = InitializeEKalmanFiltering(x_original', targets, observation(1,:));
% %  = zeros(2,2);
% % for i=2:maxObs
% %     [EKF_Coord, EKF_Cx] = ExtendedKalmanFiltering(targets, asyncObservations(i,1), EKF_Coord, asyncObservations(i,2), EKF_Cx, Q);
% %     EKalmanFilterCoords_final(i,:) = EKF_Coord;
% % end
% 
% for i=2:maxEpoch
%     for j=1:numTargets
% %         if initializeEKF == 0
% %             [EKF_Coord, EKF_Cx] = InitializeEKalmanFiltering(x_original,targets(j,:),observation(i,j));
% %             initializeEKF = 1;
% %             EKalmanFilterCoords_final(ii, :) = EKF_Coord;
% %             ii = ii+1;
% %         else
%             [EKF_Coord, EKF_Cx] = ExtendedKalmanFiltering(targets, j, EKF_Coord, observation(i, j), EKF_Cx, Q);
%             ii = ii+1;
%             EKalmanFilterCoords_final(ii, :) = EKF_Coord;
% %         end
%     end
% end
        