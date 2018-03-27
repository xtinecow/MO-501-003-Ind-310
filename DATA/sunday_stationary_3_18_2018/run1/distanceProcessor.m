distanceFile = fopen('ConvertedDist.txt','r');
distanceFileFormat = '%s %s %f';
distanceFileData = textscan(distanceFile, distanceFileFormat,'Delimiter',',');
fclose(distanceFile);


distanceToNode0 = 0;
distanceToNode1 = 0;
distanceToNode2 = 0;
distanceToNode3 = 0;
distanceToNode5 = 0;


measuredDistanceToNode0 = 8.53;
measuredDistanceToNode1 = 8.7;
measuredDistanceToNode2 = 4.7;
measuredDistanceToNode3 = 8.64;
measuredDistanceToNode5 = 8.1;

for i = 2:length(distanceFileData{1})
  if contains(distanceFileData{2}(i), '0')
   distanceToNode0 = [distanceToNode0 distanceFileData{3}(i)];
  elseif contains(distanceFileData{2}(i), '1')
   distanceToNode1 = [distanceToNode1 distanceFileData{3}(i)];
  elseif contains(distanceFileData{2}(i), '2')
   distanceToNode2 = [distanceToNode2 distanceFileData{3}(i)];
  elseif contains(distanceFileData{2}(i), '3')
   distanceToNode3 = [distanceToNode3 distanceFileData{3}(i)];
  elseif contains(distanceFileData{2}(i), '5')
   distanceToNode5 = [distanceToNode5 distanceFileData{3}(i)];
  end
end



distanceErrorToNode0 = distanceToNode0 - measuredDistanceToNode0;
distanceErrorToNode1 = distanceToNode1 - measuredDistanceToNode1;
distanceErrorToNode2 = distanceToNode2 - measuredDistanceToNode2;
distanceErrorToNode3 = distanceToNode3 - measuredDistanceToNode3;
distanceErrorToNode5 = distanceToNode5 - measuredDistanceToNode5;

distanceErrorToNode0 = distanceErrorToNode0(2:end);
distanceErrorToNode1 = distanceErrorToNode1(2:end);
distanceErrorToNode2 = distanceErrorToNode2(2:end);
distanceErrorToNode3 = distanceErrorToNode3(2:end);
distanceErrorToNode5 = distanceErrorToNode5(2:end);

timeIndex = 1:length(distanceErrorToNode0);
figure('Name','Error in distance to Node 0');
plot(timeIndex,distanceErrorToNode0);
xlim([0 length(distanceToNode0)]);

timeIndex = 1:length(distanceErrorToNode1);
figure('Name','Error in distance to Node 1');
plot(timeIndex,distanceErrorToNode1);
xlim([0 length(distanceToNode1)]);

timeIndex = 1:length(distanceErrorToNode2);
figure('Name','Error in distance to Node 2');
plot(timeIndex,distanceErrorToNode2);
xlim([0 length(distanceToNode2)]);

timeIndex = 1:length(distanceErrorToNode3);
figure('Name','Error in distance to Node 3');
plot(timeIndex,distanceErrorToNode3);
xlim([0 length(distanceToNode3)]);

timeIndex = 1:length(distanceErrorToNode5);
figure('Name','Error in distance to Node 5');
plot(timeIndex,distanceErrorToNode5);
xlim([0 length(distanceToNode5)]);

figure('Name','Distance Errors');
timeIndex = 1:length(distanceErrorToNode0);
p1 = plot(timeIndex,distanceErrorToNode0,'r');hold on;
timeIndex = 1:length(distanceErrorToNode1);
p2 = plot(timeIndex,distanceErrorToNode1,'b');hold on;
timeIndex = 1:length(distanceErrorToNode2);
p3 = plot(timeIndex,distanceErrorToNode2,'m');hold on;
timeIndex = 1:length(distanceErrorToNode3);
p4 = plot(timeIndex,distanceErrorToNode3,'k');hold on;
timeIndex = 1:length(distanceErrorToNode5);
p5 = plot(timeIndex,distanceErrorToNode5,'c');
legend([p1 p2 p3 p4 p5],'distanceError to node 0','distanceError to node 1','distanceError to node 2','distanceError to node 3','distanceError to node 5');
xlim([0 length(distanceErrorToNode5)]);

figure('Name','Distance error histograms');
h1 = histogram(distanceErrorToNode0,20);hold on;
h2 = histogram(distanceErrorToNode1,20); hold on;
h3 = histogram(distanceErrorToNode2,20); hold on;
h4 = histogram(distanceErrorToNode5,20); hold on;
h5 = histogram(distanceErrorToNode0,20);
legend([h1 h2 h3 h4 h5],'node 0','node 1','node 2','node 3','node 5');

distanceToNode0STD = std2(distanceErrorToNode0)
distanceToNode0VAR = var(distanceErrorToNode0)

distanceToNode1STD = std2(distanceErrorToNode1)
distanceToNode1VAR = var(distanceErrorToNode1)

distanceToNode2STD = std2(distanceErrorToNode2)
distanceToNode2VAR = var(distanceErrorToNode2)

distanceToNode3STD = std2(distanceErrorToNode3)
distanceToNode3VAR = var(distanceErrorToNode3)

distanceToNode5STD = std2(distanceErrorToNode5)
distanceToNode5VAR = var(distanceErrorToNode5)





rssiFile = fopen('RawRssi.txt','r');
rssiFileFormat = '%s %s %f';
rssiFileData = textscan(rssiFile, rssiFileFormat,'Delimiter',',');
fclose(rssiFile);

rssiToNode0 = 0;
rssiToNode1 = 0;
rssiToNode2 = 0;
rssiToNode3 = 0;
rssiToNode5 = 0;

for i = 2:length(rssiFileData{1})
  if (rssiFileData{3}(i) == 0)
    rssiFileData{3}(i) = NaN;
  end
  if contains(rssiFileData{2}(i), '0')
   rssiToNode0 = [rssiToNode0 rssiFileData{3}(i)];
  elseif contains(rssiFileData{2}(i), '1')
   rssiToNode1 = [rssiToNode1 rssiFileData{3}(i)];
  elseif contains(rssiFileData{2}(i), '2')
   rssiToNode2 = [rssiToNode2 rssiFileData{3}(i)];
  elseif contains(rssiFileData{2}(i), '3')
   rssiToNode3 = [rssiToNode3 rssiFileData{3}(i)];
  elseif contains(rssiFileData{2}(i), '5')
   rssiToNode5 = [rssiToNode5 rssiFileData{3}(i)];
  end
end


rssiToNode0 = rssiToNode0(2:end);
rssiToNode1 = rssiToNode1(2:end);
rssiToNode2 = rssiToNode2(2:end);
rssiToNode3 = rssiToNode3(2:end);
rssiToNode5 = rssiToNode5(2:end);

figure('Name','Distance Errors and RSSI');

timeIndex = 1:length(rssiToNode0);
subplot(2,1,1) , p1 = plot(timeIndex,rssiToNode0,'r'); title('RSSI');
xlabel('Epoch');
ylabel('RSSI (dBm)'); hold on;
p2 = plot(timeIndex,rssiToNode1,'b');hold on;
p3 = plot(timeIndex,rssiToNode2,'m');hold on;
p4 = plot(timeIndex,rssiToNode3,'k');hold on;
p5 = plot(timeIndex,rssiToNode5,'c');
legend([p1 p2 p3 p4 p5],'RSSI to node 0','RSSI to node 1','RSSI to node 2','RSSI to node 3','RSSI to node 5');
xlim([0 length(rssiToNode5)]);


timeIndex = 1:length(distanceErrorToNode0);
subplot(2,1,2) ,p1 = plot(timeIndex,distanceErrorToNode0,'r'); title('Distance Errors'); 
xlabel('Epoch');
ylabel('Distance Error (m)'); hold on;
timeIndex = 1:length(distanceErrorToNode1);
p2 = plot(timeIndex,distanceErrorToNode1,'b');hold on;
timeIndex = 1:length(distanceErrorToNode2);
p3 = plot(timeIndex,distanceErrorToNode2,'m');hold on;
timeIndex = 1:length(distanceErrorToNode3);
p4 = plot(timeIndex,distanceErrorToNode3,'k');hold on;
timeIndex = 1:length(distanceErrorToNode5);
p5 = plot(timeIndex,distanceErrorToNode5,'c');
legend([p1 p2 p3 p4 p5],'Distance Error to Node 0','Distance Error to Node 1','Distance Error to Node 2','Distance Error to Node 3','Distance Error to Node 5');
xlim([0 length(distanceErrorToNode5)]);
