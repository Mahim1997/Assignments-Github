function [fullAnsMatrix] = generateCoefficients(xArray,yArray)
% clc ;
% x = [3 4.5 7 9 10.5];
% y = [2.5 1 4.5 .5 .1]; 
% x = [ 3 4.5 7 9 ];
% y = [ 2.5 1 2.5 .5] ;
[sq , a] = generateBigMatrix(xArray, yArray) ;
% myDisplay(sq, a) ;
[squareMat ,rhsMat] = generateReducedMatrix_firstLinear(xArray, yArray) ;
% myDisplay(squareMat, rhsMat) ;

myCoefficientsMatrix =squareMat \ rhsMat ;
% displayArray(myCoefficientsMatrix) ;

ansMatrix = zeros( length(rhsMat) , 1 ) ;
ansMatrix(: , 1) = myCoefficientsMatrix(: , 1 ) ;


fullAnsMatrix = zeros(length(ansMatrix) + 1 , 1 );
fullAnsMatrix(2:length(fullAnsMatrix) , 1 ) = ansMatrix(1:length(ansMatrix) ,1 ) ;
% displayArray(fullAnsMatrix) ;
% val = squareMat * ansMatrix ;
% displayArray(val) ;
end
