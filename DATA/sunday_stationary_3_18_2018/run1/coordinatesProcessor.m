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







figure('Name','Coordinate error');
scatter(xCoordError,yCoordError); hold on;
scatter(0,0,'r','*'); hold on;
title('Coordinate Error Plot');
xlabel('X Coordinate Error (m)');
ylabel('Y Coordinate Error (m)');
t = linspace(-10,10,1000);

circx = 5*cos(t);
circy = 5*sin(t);
plot(circx,circy);
legend('Coordinate Error','Actual Position','5m Error Boundary');
set(gca,'XTick',-10:1:10);
set(gca,'YTick',-10:1:10);

figure('Name','Coordinate Error Histograms');
subplot(2,1,1), histogram(xCoordError,40), title('X Coordinate Error Histogram'); xlabel('Error (m)');ylabel('Frequency');
set(gca,'XTick',-10:1:10);
set(gca,'YTick',0:5:100);
subplot(2,1,2), histogram(yCoordError,40); title('Y Coordinate Error Histogram'); xlabel('Error (m)');ylabel('Frequency');
set(gca,'XTick',-10:1:10);
set(gca,'YTick',0:5:100);

xCoordErrorSTD = std2(xCoordError)
xCoordErrorVAR = var(xCoordError)

yCoordErrorSTD = std2(yCoordError)
yCoordErrorVAR = var(yCoordError)

count = 0;
for i=1:length(xCoordError)
if(((xCoordError(i) < 5.0) && (yCoordError(i) < 5.0)) && ((xCoordError(i) > -5.0) && (yCoordError(i) >  -5.0)))
count = count+1;
end
end

percentWithin5 = (count/length(xCoordError))*100

count2 = 0;
for i=1:length(xCoordError)
if(((xCoordError(i) < 6.0) && (yCoordError(i) < 6.0)) && ((xCoordError(i) > -6.0) && (yCoordError(i) >  -6.0)))
count2 = count2+1;
end
end

percentWithin6 = (count2/length(xCoordError))*100

count3 = 0;
for i=1:length(xCoordError)
if(((xCoordError(i) < 7.0) && (yCoordError(i) < 7.0)) && ((xCoordError(i) > -7.0) && (yCoordError(i) >  -7.0)))
count3 = count3+1;
end
end

percentWithin7 = (count3/length(xCoordError))*100