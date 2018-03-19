coordinatesFile = fopen('Coordinates.txt','r');
coordinatesFileFormat = '%s %f %f';
coordinatesFileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

xCoord = 0;
yCoord = 0;

xTrueCoord = 7.4;
yTrueCoord = 4.4;

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

figure('Name','Coordinates over time');
scatter(xCoord,yCoord); hold on;
scatter(7.4,4.4,'r','*'); hold on;
scatter(1.73,4.59,'r','*'); hold on;
scatter(12.24,0.65,'r','*'); hold on;
scatter(10.58,7.94,'r','*'); hold on;

xCoordErrorSTD = std2(xCoordError)
xCoordErrorVAR = var(xCoordError)

yCoordErrorSTD = std2(yCoordError)
yCoordErrorVAR = var(yCoordError)