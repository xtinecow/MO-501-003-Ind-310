function [ distance ] = RssiToDistanceLineFit( a, b, rssi, ref )
distance = (rssi - b) ./ a;
if distance < 0
    distance = 0;
end
% rssi = Hallway_Oct(1,1) - 10 * (-1 / a) * log10(Distance);
% distance = 10.^((ref - rssi) ./ (10 * (-1 / a)));
end

