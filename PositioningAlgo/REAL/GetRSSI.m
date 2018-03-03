function [ rssi ] = GetRSSI( NodeList, roverID )

numNodes = size(NodeList, 2);
for i=1:numNodes-1
    rssiNode = NodeList(roverID).NeighborTable(i).RSSI;
    rssi(i) = rssiNode;
end

end

