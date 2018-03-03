function [ Distance ] = RssiToDistanceFreeSpace( RSSI, ref )

Distance = 10^((ref - RSSI) / (10 * 2));
end

