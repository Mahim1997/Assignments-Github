function runProb1_022()
% format short ; format bank ;
%either error % is less than 1% or h^16 i.e. Two h^14 are used
clc; clear; 
pi = 3.141592654 ;
a = 0 ; b = pi ; 
inputFinalOrder = 16 ; %In the question it is stated to h^16 or less than 1%
ea =1 ;    trueVal = 2 ; 
[ out , finalOrder, finalError, table ] = romberg_022( @f_For_Prob1, a, b, ea,inputFinalOrder, trueVal ) ;
myPrint_022(table) ;
fprintf('Final result is : %4.6f and final order is O(h^%d) and final error is %4.6f percent \n' ,...
    out , 2*finalOrder, finalError);

end

function out = f(x)
    out =  sin(x)  ;
end