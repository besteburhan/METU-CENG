%Beste Burhan
%2171395
function vec = hw2_script3(wA,wB,wC)

syms th D R;
rot_speeds=[wA*R ; wB*R ; wC*R];
A = [ -sin(th +60) cos(th +60) D; sin(th) -cos(th) D; -sin(th -60) cos(th -60) D];
inverseOfA = simplify(inv(A));
vec = inverseOfA*rot_speeds;
end