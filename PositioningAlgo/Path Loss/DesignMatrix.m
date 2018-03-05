function [ A ] = DesignMatrix( x )
%Linear A matrix for line fitting

A = ones(4*size(x,1),2);
A_counter = 1;
for i=1:size(x,1)
    A(A_counter:A_counter+3,1) = x(i);
    A_counter = A_counter + 4;
end

end

