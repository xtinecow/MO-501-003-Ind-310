function [ x_hat, Cx, delta ] = InitializeKF( x_0, target, obs )

A = zeros(size(target));
w = zeros(size(target,1),1);
delta = inf;

% while (abs(delta) > 0.0001)
    A = DesignMatrix(target, x_0);
    w = Misclosure(x_0,target,obs');
    N = A'*A;
    delta = -1*inv(N)*A'*w;

    x_0 = x_0' + delta;
% end

x_hat = x_0';
Cx = inv(N);

end

