function [ A ] = DesignMatrix( ctrlCoord, x_0 )

A = zeros(size(ctrlCoord));

for i=1:size(ctrlCoord,1)
    dist = sqrt((ctrlCoord(i,1) - x_0(1))^2 + (ctrlCoord(i,2) - x_0(2))^2);
    A(i,1) = (ctrlCoord(i,1) - x_0(1))/dist;
    A(i,2) = (ctrlCoord(i,2) - x_0(2))/dist;
end
end

