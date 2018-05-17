%fprintf(s,'x200 y200 z200\n');
t = 0:0.05:2*pi;


traj1 = round(200*sin(t));
traj2 = round(200*sin(t+2*pi/3));
traj3 = round(200*sin(t+4*pi/3));

kroki1 = traj1(2:end) - traj1(1:end-1);
kroki2 = traj2(2:end) - traj2(1:end-1);

kroki3 = traj3(2:end) - traj3(1:end-1);

for  i=1:size(kroki1,2) 
    fprintf(s,'x%d y%d z%d\n',[kroki1(i) kroki2(i) kroki3(i)]);
    
    while(s.BytesAvailable == 0) % czekanie na wartosci rozne od zera(dane)
    end
    
    while(s.BytesAvailable > 0) % wczytanie ok z drukarki
        fscanf(s);
    end
    [kroki1(i) kroki2(i) kroki3(i)]
end

