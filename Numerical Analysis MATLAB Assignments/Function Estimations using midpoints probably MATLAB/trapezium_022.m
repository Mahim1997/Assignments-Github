function I = trapezium_022(f, a, b, stepSize)
% format long ;
h = ( b - a ) / stepSize ;
n = stepSize + 1 ;
x = linspace(a, b, n) ;
% fprintf(' h = %d , n = %d\n', h, n ) ;
% display(x) ;
% x(1) = a ; x(n) = b ;
% display(x) ;
sum = 0 ;
for i = 2 : n-1 
    sum = sum + f_For_Prob1 ( x(i) ) ;
end
sum = sum * 2 ;
I = 0.5 * h * ( f_For_Prob1( a ) + f_For_Prob1 ( b ) + sum ) ;
% display(f(a));
% display(f(b));
% display(sum);
% display(I) ;
end