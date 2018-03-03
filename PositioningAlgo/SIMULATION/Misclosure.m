function [ w ] = Misclosure( x_0, ctrlCoord, L )
%Misclosure w = F(x_0) - L
%range = sqrt((x-x)^2+(y-y)^2)
w = zeros(size(L,1),1);
for i=1:size(w,1)
    dist = sqrt((ctrlCoord(i,1) - x_0(1))^2 + (ctrlCoord(i,2) - x_0(2))^2);
    w(i,1) = L(i) - dist;
end

end

