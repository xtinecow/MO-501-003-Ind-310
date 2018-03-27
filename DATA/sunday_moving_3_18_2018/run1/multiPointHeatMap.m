coordinatesFile = fopen('p1.txt','r');
coordinatesFileFormat = '%s %f %f';
p1FileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

coordinatesFile = fopen('p2.txt','r');
coordinatesFileFormat = '%s %f %f';
p2FileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

coordinatesFile = fopen('p3.txt','r');
coordinatesFileFormat = '%s %f %f';
p3FileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

coordinatesFile = fopen('p4.txt','r');
coordinatesFileFormat = '%s %f %f';
p4FileData = textscan(coordinatesFile,coordinatesFileFormat,'Delimiter',',');
fclose(coordinatesFile);

p1xCoord = 0;
p1yCoord = 0;

p2xCoord = 0;
p2yCoord = 0;

p3xCoord = 0;
p3yCoord = 0;

p4xCoord = 0;
p4yCoord = 0;

for i = 2:length(p1FileData{1})
   p1xCoord = [p1xCoord p1FileData{2}(i)]; 
   p1yCoord = [p1yCoord p1FileData{3}(i)];
end

for i = 2:length(p2FileData{1})
   p2xCoord = [p2xCoord p2FileData{2}(i)]; 
   p2yCoord = [p2yCoord p2FileData{3}(i)];
end

for i = 2:length(p3FileData{1})
   p3xCoord = [p3xCoord p3FileData{2}(i)]; 
   p3yCoord = [p3yCoord p3FileData{3}(i)];
end

for i = 2:length(p4FileData{1})
   p4xCoord = [p4xCoord p4FileData{2}(i)]; 
   p4yCoord = [p4yCoord p4FileData{3}(i)];
end

p1xCoord = p1xCoord(2:end);
p1yCoord = p1yCoord(2:end);

p2xCoord = p2xCoord(2:end);
p2yCoord = p2yCoord(2:end);

p3xCoord = p3xCoord(2:end);
p3yCoord = p3yCoord(2:end);

p4xCoord = p4xCoord(2:end);
p4yCoord = p4yCoord(2:end);


figure('Name','Coordinates over time');
scatter(p1xCoord,p1yCoord,'r','.'); hold on;
scatter(7.4,4.4,100,'r','*'); hold on;

scatter(p2xCoord,p2yCoord,'b','v'); hold on;
scatter(1.73,4.59,100,'b','*'); hold on;

scatter(p3xCoord,p3yCoord,'k','d'); hold on;
scatter(10.58,7.94,100,'k','*'); hold on;

scatter(p4xCoord,p4yCoord,'g','s'); hold on;
scatter(12.24,0.65,100,'g','*'); hold on;

legend('P1 measured','P1 Actual','P2 measured','P2 Actual','P3 measured','P3 Actual','P4 measured','P4 Actual');

title('Positions During Moving Test');
xlabel('X Coordinate (m)');
ylabel('Y Coordinate (m)');

