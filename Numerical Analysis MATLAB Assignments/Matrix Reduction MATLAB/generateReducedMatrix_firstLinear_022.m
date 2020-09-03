function [reducedSquare, reducedRHS] = generateReducedMatrix_firstLinear ( x, y) 

[squareMatrix, rhsMatrix] = generateBigMatrix(x, y) ;
% myDisplay(coeffMatrix, rhsMatrix) ;
[row , col] = size(squareMatrix) ;

reducedSquare = zeros(row-1 , col-1) ;
reducedSquare ( 1 :length(reducedSquare) , 1 :length(reducedSquare)  ) = ...
    squareMatrix ( 1 :length(reducedSquare) , 2 : length(squareMatrix) ) ;

reducedRHS = zeros( length(rhsMatrix) - 1 ) ;
reducedRHS(1 :length(reducedSquare)  ) = rhsMatrix( 1 : length(reducedSquare) ) ;
% reducedRHS(1:length(reducedRHS)) = rhsMatrix(1: length(reducedRHS) ) ;



end