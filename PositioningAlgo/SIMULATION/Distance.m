function [ dist ] = Distance( A, B )
%Distance between 2 points

dist = sqrt((A(1,1) - B(1,1))^2 + (A(1,2) - B(1,2))^2);

end

