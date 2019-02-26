%Beste Burhan
%2171395
for g= 0:0.01:1
func(g);
end
function func(g)
C_gamma =[ 
  cos(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10);
 sin(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10)
 ];

diff_c_gamma =[
 (32*pi*cos(2*pi*g)*cos(8*pi*g)*sin(8*pi*g))/5-2*pi*sin(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10);
 2*pi*cos(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10) + (32*pi*cos(8*pi*g)*sin(2*pi*g)*sin(8*pi*g))/5
];

theta_desired = atan2(diff_c_gamma(2),diff_c_gamma(1))-pi/2;

end_eff_pose = [
  cos(theta_desired) -sin(theta_desired) C_gamma(1);  
  sin(theta_desired) cos(theta_desired) C_gamma(2); 
    0   ,   0   ,   1;
];

hold on;
trplot2(end_eff_pose);
pause(0.05);
end