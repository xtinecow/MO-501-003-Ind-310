function [ v ] = ComputeResidual( data, maxEpoch, target, x_hat )
%Compute residual vector v
l_obs = reshape(data(1:maxEpoch,2:5)',[maxEpoch*4,1]);
l = zeros(size(l_obs));
for i=1:size(l,1)
    target_number = mod(i,4);
    if target_number == 0
        target_number = 4;
    end
    l(i,1) = sqrt((target(target_number,1) - x_hat(1,1)).^2+(target(target_number,2) - x_hat(2,1)).^2);
end
v = l_obs - l;

end

