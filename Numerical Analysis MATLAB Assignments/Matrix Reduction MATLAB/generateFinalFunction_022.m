function out = generateFinalFunction(xArray, yArray, x)
% clc ;
% xArray = [ 3 ,  4.5 ,  7 , 9 ];
% yArray = [ 2.5 , 1  , 2.5,  .5] ;
    out = 699999 ;
    [a, b, c] = generate_a_b_c(xArray, yArray) ;
%     displayArray(a) ; displayArray(b) ; displayArray(c) ;
    for i = 1 :length(xArray)-1
        if( x < xArray(1) )
            out = a(1)*x*x + b(1)*x + c( 1 );
%             fprintf(' .. %f (x)  generating %f x^2 + %f x  + %f \n' , x , a(1 ) , b(1) , c(1) ) ;
        end
        if( x>xArray(length(xArray) ) ) 
            out = a(length(xArray)-1)*x*x + b(length(xArray)-1)*x + c(length(xArray)-1) ;
%             fprintf(' .. generating %f x^2 + %f x  + %f \n' , a(length(xArray)-1 ) , b(length(xArray)-1) , c(length(xArray)-1) ) ;
        end
        if ( ( ( xArray(i) <= x )  &&  ( x <= xArray(i+1) ) ) || (x == xArray(i) ) ) 
%             fprintf(' %f (x) is in between %f and %f .. ' , x, xArray(i) , xArray(i+1) ) ;
%             disp( ' ' ) ;
            out = a(i) *x*x + b(i) * x + c(i) ;
%             fprintf(' .. generating %f x^2 + %f x  + %f \n' , a(i ) , b(i) , c(i) ) ;
        end
    end
end
