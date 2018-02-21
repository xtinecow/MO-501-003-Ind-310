
guiclient = tcpip('10.13.115.218',8001);
guiclient.timeout = 0.2;
fopen(guiclient);

ID = "UofC";
ID2 = "test1";
ID3 = "test2";
lat = 51.079948;
longi = -114.125534;
er1 = 10;
er2 = 10;

while(1)
    
temp = [ID, num2str(lat,10)  , num2str(longi,10) , num2str(er1,6), num2str(er2,6)];
toSend = join(temp, ",");
java.lang.Thread.sleep(300); 
fprintf(guiclient,'%s\n',toSend);




check = fscanf(guiclient, '%s');

checkResponse = contains(check, 'DONE');
flushinput(guiclient);
if(checkResponse) 
break; 
end

lat = lat + 0.000001;
longi = longi + 0.00001;
er1 = er1 + 0.01;
er2 = er2 + 0.02;

end


fclose(guiclient);
delete(guiclient);
clear guiclient
