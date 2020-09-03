function [x, y] = euler_022()
%     a = 0 ; b = 4 ; stepSize = 0.5 ; yNot = 1 ;
    [xNot, yNot, a, b, stepSize] = Info_022();
    format short ; format bank ;
    x = a : stepSize : b ;
%     display(x);
    y = x ;
    for i = 1 : length(x)
        y(i) = 0 ;
    end
    n = length(x) ;
    y(1) = yNot ;
    for i = 1 : n-1
        y( i+1 ) = y(i) + ( f( x(i), y(i) ) * stepSize ) ; 
%         fprintf('i+1 is %d  and x(i) is %f...  y(i+1) is .. %f + %f X %d and \n' ,(i+1), x(i),  y(i) , f(x(i)) , stepSize ) ;
    end
    fprintf('Euler : .. \n') ;
    display(x) ;
    display(y) ;
%     plot(x, y ); hold on ; plot(x, y, 'o') ; grid on ;  hold off ;
end