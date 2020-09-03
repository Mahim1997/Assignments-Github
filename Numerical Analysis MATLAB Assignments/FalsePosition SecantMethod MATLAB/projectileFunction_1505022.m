function out = projectileFunction_1505022(x)
y0 = 1.8 ;
y = 1 ;
g = 9.8;
hor = 90 ;
v = 30 ;
%out = cos(x) ;
out = y0 +( hor*tan(x)) - (g*hor*hor)/(2*v*v*cos(x)*cos(x)) - y;
end