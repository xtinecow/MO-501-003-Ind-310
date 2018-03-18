function [ distance ] = RssiToDistanceLineFit( a1, b1, a2, b2, rssi )
distance = (rssi - b1) ./ a1;
if distance < 0
    distance = (rssi - b2) ./ a2;
end
if distance < 0
    distance = 0;
end
end

