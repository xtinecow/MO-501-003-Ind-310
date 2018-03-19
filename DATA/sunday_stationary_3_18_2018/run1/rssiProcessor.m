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

timeIndex = 1:length(rssiToNode0);


figure('Name','rssi to Node 0');
plot(timeIndex,rssiToNode0);
xlim([0 length(rssiToNode0)]);

figure('Name','rssi to Node 1');
plot(timeIndex,rssiToNode1);
xlim([0 length(rssiToNode1)]);

figure('Name','rssi to Node 2');
plot(timeIndex,rssiToNode2);
xlim([0 length(rssiToNode2)]);

figure('Name','rssi to Node 3');
plot(timeIndex,rssiToNode3);
xlim([0 length(rssiToNode3)]);

figure('Name','rssi to Node 5');
plot(timeIndex,rssiToNode5);
xlim([0 length(rssiToNode5)]);


figure('Name','rssi');
p1 = plot(timeIndex,rssiToNode0,'r');hold on;
p2 = plot(timeIndex,rssiToNode1,'b');hold on;
p3 = plot(timeIndex,rssiToNode2,'m');hold on;
p4 = plot(timeIndex,rssiToNode3,'k');hold on;
p5 = plot(timeIndex,rssiToNode5,'c');
legend([p1 p2 p3 p4 p5],'RSSI to node 0','RSSI to node 1','RSSI to node 2','RSSI to node 3','RSSI to node 5');
xlim([0 length(rssiToNode5)]);



rssiToNode0STD = std2(rssiToNode0)
rssiToNode0VAR = var(rssiToNode0)

rssiToNode1STD = std2(rssiToNode1)
rssiToNode1VAR = var(rssiToNode1)

rssiToNode2STD = std2(rssiToNode2)
rssiToNode2VAR = var(rssiToNode2)

rssiToNode3STD = std2(rssiToNode3)
rssiToNode3VAR = var(rssiToNode3)

rssiToNode5STD = std2(rssiToNode5)
rssiToNode5VAR = var(rssiToNode5)