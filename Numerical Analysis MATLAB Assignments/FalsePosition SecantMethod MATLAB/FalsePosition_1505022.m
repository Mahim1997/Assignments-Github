function [out,noOfIterations] = FalsePosition_1505022(f,xL,xU, expectedError, maxIteration)
projectileGraph_1505022();
xL = xL * pi/180 ;
xU = xU * pi/180 ;
fU = f(xU);
fL = f(xL);
error = 100 ;
i = 0;
while (error>expectedError) && (i<maxIteration)
    i = i + 1;
    %fL = f(xL);
    %fU = f(xU);
    %xR = ( (xL*f(xU))-(xU*f(xL)) )/ (f(xU) - f(xL) )  ;
    xR = ( (xL*fU )-(xU*fL ) )/ (fU - fL )  ;
    display(xR);
    fR = f(xR);
    if i > 1
        error = xR - prev;
        error = error / xR * 100;
        error = abs(error);
    end
    if (fR * fL) > 0
        prev = xR ;
        xU = xR;
        fU = fR;
    else
        prev = xR ;
        xL = xR;
        fL = fR;
    end

end
out = xR * 180/pi ;
noOfIterations = i ;
end


function projectileGraph_1505022()
y0=1.8;
g=9.8;x=90;
max = 70 ;
v = 30;
arrayOfy = 0:1:max;
theta = 0*pi/180:1*pi/180:max*pi/180;
thetaInDegrees = 0:1:max ;
for i=1:1:70
    arrayOfy(i) = y0 +( x*tan(theta(i))) - (g*x*x)/(2*v*v*cos(theta(i))*cos(theta(i)));
    display(arrayOfy(i));
    display(thetaInDegrees(i));
end
y = ones(1,max+1);
plot(thetaInDegrees, arrayOfy,'r', thetaInDegrees, y, 'm') ...
    , xlabel('Angle of Elevation /deg') ...
    , ylabel('VerticalDisplacement of Ball/ m')...
    , title('Graph of Displacement against Angle of Elevation'), ...
    axis ([0, 60, -50 20]),...
    grid on; 
%%grid minor; 
%set(gca,'xtick',[0:5:60]);
%set(gca,'ytick',[-50:10:20]);
set(gca,'xtick',[0:2:60]);
set(gca,'ytick',[-50:5:20]);
%set(gca,'LineWidth',1.5);
end
