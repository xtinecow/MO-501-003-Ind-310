function [ NodeList ] = RSSIToStruct( rssi, roverID )

numNode = size(rssi,2) + 1;

for i=1:numNode
    NodeList(i).name = [];
    for j = 1:numNode-1
        NodeList(i).NeighborTable(j).name = [];
        if i == roverID
            NodeList(i).NeighborTable(j).RSSI = rssi(j);
        else
            NodeList(i).NeighborTable(j).RSSI = 0;
        end
    end
end
% NodeList.NodeList = NodeList;
end

