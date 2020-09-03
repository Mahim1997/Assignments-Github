function [x, y] = Ralston_022()
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
       k1 = f ( x(i) , y(i) );
       k2 = f( x(i) + (3*h/4)  ,  y(i)+ ( (3*k1*h/4) ) ); 
       y(i+1) = y(i)+ ( ( k1/3 + 2*k2/3 ) * h ) ;
    end
end