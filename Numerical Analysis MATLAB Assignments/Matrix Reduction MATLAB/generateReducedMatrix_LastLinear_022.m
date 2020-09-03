function [newCoeffMatrix, newRHS] = generateReducedMatrix_LastLinear(x, y)
% function generateReducedMatrix()
% clc ;
% x = [3 4.5 7 9 10.5];
% y = [2.5 1 4.5 .5 .1]; 
[squareMatrix, rhsMatrix] = generateBigMatrix(x, y) ;
% myDisplay(coeffMatrix, rhsMatrix) ;
[row , col] = size(squareMatrix) ;

newCoeffMatrix = zeros(row-1 , col-1) ;
newCoeffMatrix(: , 1: (col - 3) ) = squareMatrix(1:(row-1) , 1:(col - 3 )) ;
%copied until last minus 3rd

newCoeffMatrix(: , (col - 2) : (col-1) ) = squareMatrix(1:(row-1) ,  col-1: col ) ;  %last two columns

newRHS = zeros( length(rhsMatrix) - 1 ) ;
newRHS(1:length(newRHS)) = rhsMatrix(1: length(newRHS) ) ;
% displayArray(newCoeffMatrix) ;
% myDisplay(newCoeffMatrix, newRHS) ;
end