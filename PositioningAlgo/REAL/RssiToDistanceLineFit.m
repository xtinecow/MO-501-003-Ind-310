function [ distance ] = RssiToDistanceLineFit( a, b, rssi )
distance = (rssi - b) ./ a;
if distance < 0
    distance = 0;
end
end

