function [ x_hat,Cx, delta ] = KalmanFiltering( data, target, prevEpoch, Cx, delta, Q )
%Simple Linear Kalman Filter based on Sequential LS
Cl = eye(size(data(1,:).',1)); 

%Calculate A for next epoch
A = DesignMatrix(target, prevEpoch);

%Calculate w for next epoch
w = Misclosure(prevEpoch,target,data');

%Calculate Gain Matrix K
K = Cx * A' * inv(Cl + A * Cx * A');
delta = delta - K*(A * delta + w);
x_hat = prevEpoch' + delta;
Cx = Cx - K * A * Cx;

%Add Process Noise
Cx = Cx + Q;
x_hat = x_hat';
end

