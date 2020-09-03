function runProb2_022()
[xEuler, yEuler] = euler_022() ;    
plot(xEuler, yEuler ); hold on ; plot(xEuler, yEuler, 'or') ; grid on ;  %hold off ;
[xHeun, yHeun] = Heun_022();
plot(xHeun, yHeun ); hold on ; plot(xHeun, yHeun, '>g') ; grid on ; 
[xMid, yMid] = MidPoint_022();
plot(xMid, yMid ); hold on ; plot(xMid, yMid, '+') ; grid on ; 
[xRal, yRal] = Ralston_022();
plot(xRal, yRal ); hold on ; plot(xRal, yRal, '<b' ) ; grid on ; 
[xNot, yNot, a, b, stepSize] = Info_022() ;
xActualFunc = linspace(a, b, 100);
yActualFunc = xActualFunc ;
for i = 1 : length(xActualFunc)
%         x = xActualFunc(i) ;
%         yActualFunc(i) =  (-2*x^4)/4 + 4*x^3 - 10*x^2 + 8.5*x + 2 ;
        yActualFunc(i) = (xActualFunc(i)^5)/5 - (xActualFunc(i)^3)/3 + 3*xActualFunc(i) + 2 ;
end
plot(xActualFunc, yActualFunc , 'g'); hold on ; grid on ; 
s1 = 'EULER' ;
s2= ' HEUN' ;
s3  = 'MIDPOINT' ;
s4 = 'RALSTON'; 
s5 = 'ACTUAL' ;
legend(s1 , '' , s2, '' , s3, '' ,  s4 , '', s5);
end