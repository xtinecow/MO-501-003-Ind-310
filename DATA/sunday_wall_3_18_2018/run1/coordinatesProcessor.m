coordinatesFile = fopen('Coordinates.txt','r');
coordinatesFileFormat = '%s %f %f';
coordinatesFileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

xCoord = 0;
yCoord = 0;

xTrueCoord = 6.5;
yTrueCoord = 7.87;

for i = 2:length(coordinatesFileData{1})
   xCoord = [xCoord coordinatesFileData{2}(i)]; 
   yCoord = [yCoord coordinatesFileData{3}(i)];
end

xCoord = xCoord(2:end);
yCoord = yCoord(2:end);

xCoordError = xCoord - xTrueCoord;
yCoordError = yCoord - yTrueCoord;

timeIndex = 1:length(xCoordError);

figure('Name','X Coordinate Error.');
plot(timeIndex,xCoordError);
xlim([0 length(xCoordError)]);

figure('Name','Y Coordinate Error.');
plot(timeIndex,yCoordError);
xlim([0 length(yCoordError)]);

figure('Name','Coordinate error');
scatter(xCoordError,yCoordError); hold on;
scatter(0,0,'r','*'); hold on;
title('Coordinate Error Plot With Wall Between Nodes');
xlabel('X Coordinate Error (m)');
ylabel('Y Coordinate Error (m)');
t = linspace(-10,10,1000);

circx = 5*cos(t);
circy = 5*sin(t);
plot(circx,circy);
legend('Coordinate Error','Actual Position','5m Error Boundary');



xCoordErrorSTD = std2(xCoordError)
xCoordErrorVAR = var(xCoordError)

yCoordErrorSTD = std2(yCoordError)
yCoordErrorVAR = var(yCoordError)