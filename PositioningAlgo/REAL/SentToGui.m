function [  ] = SentToGui( roverID, guiclient, KF_Coords, lat_o, long_o, KF_Cx )
    [lat, long] = GetLatLong(lat_o, long_o, KF_Coords(1,1), KF_Coords(1,2))
    [er1, er2] = GetError(KF_Cx)
    
    ID = num2str(roverID);
    temp = [convertCharsToStrings(ID), num2str(lat,10)  , num2str(long,10) , num2str(er1,6), num2str(er2,6)];
    toSend = join(temp, ",")
    java.lang.Thread.sleep(300);
    fprintf(guiclient,'%s\n',toSend);
end

