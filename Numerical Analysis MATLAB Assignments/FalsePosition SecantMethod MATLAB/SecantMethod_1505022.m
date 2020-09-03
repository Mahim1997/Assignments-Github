function [out,noOfIterations] = SecantMethod_1505022(f,xInitial,xFinal, expectedError, maxIteration)
    %x = 1:1:30;
    projectileGraph_1505022();
    x1 = xInitial*pi/180;
    x2 = xFinal*pi/180;
    display(x1);
    display(x2);
    %error = x(2) - x(1);
    error = x2 - x1 ;
    error = abs(error);
    error = error*100 ;
    iTerm = x2;
    iMinus1Term = x1;
    %iPlus1Term = x2;
    i = 0 ;
    while (error > expectedError) && (i < maxIteration) 
        %x(i+1) = x(i) - f(x(i)) * abs((x(i) - x(i-1))/ (f(x(i)) - f(x(i-1))) ); 
        valueFunction = f(iTerm);%%SO that f(x(i)) is not calculated multiple times
        iPlus1Term = iTerm - (valueFunction * abs(((iTerm - iMinus1Term)...
            / (valueFunction - f(iMinus1Term))))) ; 
        %currentTerm = x(i+1);
        %prevTerm = x(i) ;
        display(iTerm);
        %error = iPlus1Term - iTerm ;
        error = iTerm - iMinus1Term;
        error = error/iTerm;
        error = error*100; %PERCENTAGE
        error = abs(error);
        %var = iTerm ;
        iMinus1Term = iTerm ;
        iTerm = iPlus1Term ;
        i = i + 1 ;
       
    end
    out = iTerm *180/pi ;
    noOfIterations = i ;
end


%%plot the graph

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
