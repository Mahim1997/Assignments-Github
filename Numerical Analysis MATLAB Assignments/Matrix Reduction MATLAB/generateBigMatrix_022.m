function [squareMatrix, rhsMatrix ] = generateBigMatrix (x, y)
% function generateBigMatrix ( ) 
% clc ;
% [ x , y ] = trainingPoints() ;
% x = [3 4.5 7 9 10.5];
% y = [2.5 1 4.5 .5 .1]; %NOT EXACTLY BOOK's
% x = [ 3 4] ; y = [ 2.1 0.69 ]; 
noOfPoints = length(x) ;
order = noOfPoints - 1 ; %taken from the function 
n = order ;
%copying rhsMatrix
rhsMatrix = zeros(order*3, 1) ;
% display(length(rhsMatrix) );
for i = 1 : order
    rhsMatrix(i) = y(i) ;
end
for i = 1 : order
    rhsMatrix(order+i) = y(i+1) ;
end
% displayArray(rhsMatrix) ;
%rhsMatrix done

%coefficient Matrix
squareMatrix = zeros(3*order , 3*order) ;

%from x1^2 to xn^2
delY = 0;
for i = 1 : 1 : n 
    squareMatrix(i, 1+delY) = x(i)^2 ;
    squareMatrix(i, 2+delY ) = x(i) ;
    squareMatrix(i, 3+delY) = 1 ;
    delY = delY + 3 ;
end
%from x1^2 to xn^2 done

%from x2^2 to x(n+1)^2
delY = 0 ; delX = n ;
for i = 2 : 1 : n+1 
    squareMatrix(i-1 + delX, 1+delY) = x(i)^2 ;
    squareMatrix(i-1 + delX, 2+delY ) = x(i) ;
    squareMatrix(i-1 + delX, 3+delY) = 1 ;
    delY = delY + 3 ;
end
%from x2^2 to x(n+1)^2 done


%derivatives
delY = 0 ; delX = 0;  j = 2 ;
for i = 2*n+1 : 1 : length(squareMatrix) -1 
    squareMatrix(i+delX, delY+1) =  2*x(j) ; %+69;
    squareMatrix(i+delX, delY+2) = +1;
    squareMatrix(i+delX, delY+4) =  -2*x(j) ; %-69 ;
    squareMatrix(i+delX, delY+5) = -1;
    delY = delY + 3 ;
    j = j + 1 ;
end
%derivatives done ..
% displayArray(coeffMatrix);
% myDisplay(coeffMatrix, rhsMatrix) ;
% 
% [newCoeffMatrix, newRHS] = generateReducedMatrix(coeffMatrix, rhsMatrix) ;
% myDisplay(newCoeffMatrix, newRHS) ;
%coefficient Matrix done

end