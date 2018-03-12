function [ a,b,theta ] = GetError( Cx )
a = sqrt(0.5*(Cx(1,1) + Cx(2,2) + sqrt((Cx(1,1) - Cx(2,2)).^2 + 4*(Cx(1,2)).^2)));
b = sqrt(0.5*(Cx(1,1) + Cx(2,2) - sqrt((Cx(1,1) - Cx(2,2)).^2 + 4*(Cx(1,2)).^2)));
alpha = 0.5*atan2((2*Cx(1,2)), (Cx(2,2) - Cx(1,1)));
theta = (90 - alpha) * pi / 180;

end

