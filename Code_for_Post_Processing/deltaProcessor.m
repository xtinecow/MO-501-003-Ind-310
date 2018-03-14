deltaFile = fopen('Delta.txt','r');
deltaFileFormat = '%s %f %f';
deltaFileData = textscan(deltaFile,deltaFileFormat,'Delimiter',',');
fclose(deltaFile);

xdelta = 0;
ydelta = 0;


for i = 2:length(deltaFileData{1})
   xdelta = [xdelta deltaFileData{2}(i)]; 
   ydelta = [ydelta deltaFileData{3}(i)];
end

xdelta = xdelta(2:end);
ydelta = ydelta(2:end);

timeIndex = 1:length(xdelta);

figure('Name','X delta over time.');
plot(timeIndex,xdelta);
xlim([0 length(xdelta)]);

figure('Name','Y delta over time.');
plot(timeIndex,xdelta);
xlim([0 length(xdelta)]);

figure('Name','X and Y deltas over time.');
plot(timeIndex,xdelta,'r');
hold on;
plot(timeIndex,ydelta,'b');
xlim([0 length(xdelta)]);

xdeltaSTD = std2(xdelta)
xdeltaVAR = var(xdelta)

ydeltaSTD = std2(ydelta)
ydeltaVAR = var(ydelta)

