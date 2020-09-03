function [Ans, Iter] =  Newton_022(fPrime, fDoublePrime)
initialValue = 0.1 ;
maxErr = 0.0001 ;
err = 100 ;
itr = 0 ;
xOld = initialValue ;
format long ;
while err > maxErr
    xNew = xOld - fPrime(xOld)/fDoublePrime(xOld) ;
    itr  = itr+1 ;
    err = abs(xOld - xNew) ;
%     err = err*100;
    disp( sprintf('Iteration is %d and Ans is %f and err is %f',itr, xNew,err) ) ;
    xOld = xNew ;
end
% disp( fprintf('Finally,  Iteration is %d and Ans is %d',itr, xNew) ) ;

Ans = xNew ;
Iter = itr ;
% x = fPrime(0.9169);
% disp(x);
end
% 
% function out = f(x)
%     out = -1.5*x^6 - 2*x^4 + 12*x ;  
% end
% function out = fPrime(x)
%     out = -9*x^5 - 8*x^3 + 12 ;
% end
% function out = fDoublePrime(x)
%     out = -45*x^4 - 24*x^2 ;
% end