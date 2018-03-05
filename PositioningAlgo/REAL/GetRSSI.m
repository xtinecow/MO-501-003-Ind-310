function [ rssi ] = GetRSSI( NodeList, roverID, targetList )

numNodes = size(NodeList, 2);
rssi = zeros(size(targetList,1));
for i=1:numNodes
    if NodeList(i).name == roverID
        table = NodeList(i).NeighborTable;
        for j=1:size(targetList,2)
            for k=1:size(table,2)
                if table(k).name == targetList(j)
                    rssi(j) = -1 * table(k).RSSI;
                end
            end
        end
    end
end

end

