function [ LogShadowingModel ] = DistanceToRSSIFreeSpace( ref, Distance )
if Distance < 10^-13
    Distance = 0;
end

LogShadowingModel = ref - 10 * 2 * log10(Distance);
end

