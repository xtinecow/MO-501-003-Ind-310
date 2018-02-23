ClientSocket = tcpip('192.168.1.100',8002);
ClientSocket.timeout = 2;
fopen(ClientSocket);

while(1)
    payload  = fscanf(ClientSocket); 
    if(length(payload) > 1)
        break; 
    end
end
NodeList = ParseResults (payload)


fclose(ClientSocket);
delete(ClientSocket);
clear ClientSocket

% Parse results from the payload string
% Must undo formatting done by SendNodesToSocket in TCP.cpp of
% ComputerProgram
function NodeList = ParseResults (payload)
i = 1; 


    % 6 nodes in total
    for node = 1:6
        NodeList(node).name = ""; 
        % Look for first parsing character
        iter = 1; 
        while(payload(i) ~= '-')
            NodeList(node).name = NodeList(node).name + payload(i); 
            i = i+1; 
            iter = iter+1;
            if(iter>10) % Name should never exceed this
                break; 
            end
        end
        i = i+1; % Skip parsing character
        
        % Max number of possible neighboring nodes is 5
        for neighbor = 1:5
            NodeList(node).NeighborTable(neighbor).name = ""; 
        % Look for next parsing character
            iter = 1; 
            while(payload(i) ~= '.')
                NodeList(node).NeighborTable(neighbor).name = NodeList(node).NeighborTable(neighbor).name + payload(i); 
                i = i+1; 
                iter = iter+1;
                if(iter>10) % Name should never exceed this
                    break; 
                end
            end
            i = i+1; % Skip parsing character
            RSSI_str = ""; 
            iter = 1; 
            while(payload(i) ~= '|')
                RSSI_str = RSSI_str + payload(i);
                i = i+1; 
                iter = iter+1;
                if(iter>10) % Name should never exceed this
                    break; 
                end
            end
            RSSI_char = char(RSSI_str); % Have to conver to char before it can be converted to int
            NodeList(node).NeighborTable(neighbor).RSSI = str2num(RSSI_char); % convert to int
            i = i+1; % Skip parsing character
        end
        i = i+1; % Skip parsing '>' character (don't need it)
    end 
    
    
end