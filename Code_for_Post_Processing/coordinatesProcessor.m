coordinatesFile = fopen('Coordinates.txt','r');
coordinatesFileFormat = '%s %f %f';
coordinatesFileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

xCoord = 0;
yCoord = 0;

xTrueCoord = 6.7;
yTrueCoord = 5.1;

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


xCoordErrorSTD = std2(xCoordError)
xCoordErrorVAR = var(xCoordError)

yCoordErrorSTD = std2(yCoordError)
yCoordErrorVAR = var(yCoordError)