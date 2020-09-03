function [x, y] = Heun_022()
    [xNot, yNot, a, b, stepSize] = Info_022();
    format short ; format bank ;
    x = a : stepSize : b ;
    y = x ;
    for i = 1 : length(x)
        y(i) = 0 ;
    end
    y(1) = yNot ;
    n = length(x) ;
    h = stepSize ;
    for i = 1 : n-1
        k1 = f ( x(i), y(i) );
        k2 = f (  x(i) + h , y(i) + k1*h   ) ;
        y( i + 1 ) = y(i) + h * (  ( k1/2 ) + ( k2/2 )  ) ;
    end
%     for i = 1 : n-1
%        predictor=f ( x(i) , y(i) );
%        corrector=f( x(i+1), y(i)+ ( stepSize*predictor ) ); 
%        y(i+1)=y(i)+ (stepSize /2) * (predictor+corrector);
%     end
    
%     display(x) ;
%     display(y);
%     plot(x, y); grid on ;
end