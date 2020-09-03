function NewtonTest_022() 
[A, B] = Newton_022(@fPrime, @fDoublePrime);

% disp(A) ; disp(B) ;
% x = A ;
% var = -45*x^4 - 24*x^2 ; 
% disp(var) ;
disp(sprintf('\n\nIteration: %d\n',B) );
disp(sprintf('Ans is %f\n', A) ) ;
disp( sprintf( 'Since it is max, double derivative is %f i.e. less than zero\n', fDoublePrime(A) )  ) ;
disp(sprintf('\n\n')) ;
end

function out = f(x)
    out = -1.5*x^6 - 2*x^4 + 12*x ;  
end
function out = fPrime(x)
    out = -9*x^5 - 8*x^3 + 12 ;
end
function out = fDoublePrime(x)
    out = -45*x^4 - 24*x^2 ;
end