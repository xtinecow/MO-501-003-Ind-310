function [ lat, long ] = GetLatLong( latref, longref, x, y )

dlat = y / 111111;
dlong = x / (111111 * cos(latref + dlat));

lat = latref + dlat;
long = longref + dlong;

end

