function [ a,b ] = GetError( Cx )
a = sqrt(0.5*(Cx(1,1) + Cx(2,2) + sqrt((Cx(1,1) - Cx(2,2)).^2 + 4*(Cx(1,2)).^2)));
b = sqrt(0.5*(Cx(1,1) + Cx(2,2) - sqrt((Cx(1,1) - Cx(2,2)).^2 + 4*(Cx(1,2)).^2)));

end

