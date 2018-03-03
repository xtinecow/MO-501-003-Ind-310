function [ rssi ] = GetRSSI( NodeList, roverID )

% numNodes = size(NodeList.NodeList, 2);
numNodes = size(NodeList, 2);
for i=1:numNodes-1
%     rssiNode = NodeList.NodeList(roverID).NeighborTable(i).RSSI;
    rssiNode = NodeList(roverID).NeighborTable(i).RSSI;
    rssi(i) = rssiNode;
end

end

