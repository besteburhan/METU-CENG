%Beste Burhan
%2171395
L(1) = Link([0 0 0.75 0]);
L(2) = Link([0 0 0.5 0 ]);
L(3) = Link([0 0 0.2 0 ]);
three_link = SerialLink(L,'name','three_link');



for g=0:0.125:1
    pose = func(g);
 if(pose(3) == pi/4 || pose(3)==5*pi/4)
    x_des = pose(1);
    y_des = pose(2);
    theta_des = pose(3);

    x2 = x_des - 0.2*cos(theta_des);
    y2 = y_des - 0.2*sin(theta_des);
    theta_2 = acos(x2^2+y2^2 -(0.75^2)-(0.5^2) / 2*0.75*0.5);
    theta_1 = atan2(y2,x2)-atan2(0.5*sin(theta_2), 0.2+0.5*cos(theta_2));
    theta_3 = theta_des-theta_1-theta_2;
    three_link.plot([theta_1 theta_2 theta_3]);
 else
     continue
 end
end

function pose= func(g)
C_gamma =[ 
  cos(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10);
 sin(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10)
 ];

diff_c_gamma =[
 (32*pi*cos(2*pi*g)*cos(8*pi*g)*sin(8*pi*g))/5-2*pi*sin(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10);
 2*pi*cos(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10) + (32*pi*cos(8*pi*g)*sin(2*pi*g)*sin(8*pi*g))/5
];

theta_desired = atan2(diff_c_gamma(2),diff_c_gamma(1))-pi/2;

pose=[ C_gamma(1);
       C_gamma(2);
       theta_desired;
    ];
end
function gm = s_fun(g)
diff_c_gamma =[
 (32*pi*cos(2*pi*g)*cos(8*pi*g)*sin(8*pi*g))/5-2*pi*sin(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10);
 2*pi*cos(2*pi*g)*((2*sin(8*pi*g)^2)/5 + 9/10) + (32*pi*cos(8*pi*g)*sin(2*pi*g)*sin(8*pi*g))/5
];
 for t=0:1:10000
magn =((2*pi*cos(2*pi*t)*((2*sin(8*pi*t)^2)/5 + 9/10) + (32*pi*cos(8*pi*t)*sin(2*pi*t)*sin(8*pi*t))/5)^2 + (2*pi*sin(2*pi*t)*((2*sin(8*pi*t)^2)/5 + 9/10) - (32*pi*cos(2*pi*t)*cos(8*pi*t)*sin(8*pi*t))/5)^2)^(1/2);
if(magn==1) 
    gm = t/10000;

else
end
end
end