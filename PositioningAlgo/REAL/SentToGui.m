function [ terminate ] = SentToGui( roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx, targets, targetList )
    [lat, long] = GetLatLong(lat_o, long_o, KF_Coords(1,1), KF_Coords(1,2));
    [er1, er2, theta] = GetError(KF_Cx);
    
    ID = roverID;
    temp = [convertCharsToStrings(ID), num2str(lat,10)  , num2str(long,10) , num2str(er1,6), num2str(er2,6), num2str(theta, 6)];
    toSend = join(temp, ",");
    java.lang.Thread.sleep(300);
    fprintf(guiclient,'%s\n',toSend);
    
    terminate = false(1,1);
    check = fscanf(guiclient, '%s');
    checkResponse = contains(check, 'DONE');
    flushinput(guiclient);
    if(checkResponse)
        terminate = true(1,1); 
    end
    
    if (~terminate)
        for i=1:size(targets,1)
            [lat, long] = GetLatLong(lat_o, long_o, targets(i,1), targets(i,2));
            ID = targetList(1,i);
            er1 = 0;
            er2 = 0;
            theta = 0;
            temp = [convertCharsToStrings(ID), num2str(lat,10)  , num2str(long,10) , num2str(er1,6), num2str(er2,6), num2str(theta, 6)];
            toSend = join(temp, ",");
            java.lang.Thread.sleep(300);
            fprintf(guiclient,'%s\n',toSend);
        end
    end

end

