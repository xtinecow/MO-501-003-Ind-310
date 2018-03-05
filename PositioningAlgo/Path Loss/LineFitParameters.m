function [ a,b ] = LineFitParameters( inputData )
%inputData = [Distance, RSSI1, RSSI2, RSSI3, RSSI4]
A = DesignMatrix(inputData(:,1));
x = zeros(size(inputData,1)*4, 1);
inputCounter = 1;
for i = 1:size(inputData,1)
    x(inputCounter:inputCounter+3,1) = inputData(i,2:5)';
    inputCounter = inputCounter + 4;
end
delta = inv(A.'*A)*A.'*x;
a = delta(1,1);
b = delta(2,1);
end

