%Beste Burhan
%2171395 
radius=0.5;
velocity_x=1;%rad/s
velocity_z=1;%m/S
hold on
xlim([-1 1])
ylim([-1 1])
zlim([0 12])
view(3)
time=[0:0.01:4*pi]; 
for t=1:length(time)
        xunit=radius*cos(velocity_x*time(t)); 
        yunit=radius*sin(velocity_x*time(t)); 
        zunit=velocity_z*time(t);
        p(1)=plot3(xunit,yunit,zunit,'o','Color','g');                
        
        pause(0.01); 
     
end
hold off

