function [ distPerEpoch ] = SimulateDistance( targetCoord, roverCoord )
distPerEpoch = zeros(1,5);

for i=1:5
    distPerEpoch(1,i) = sqrt((targetCoord(i,1) - roverCoord(1,1))^2 + (targetCoord(i,2) - roverCoord(1,2))^2);
end

end

