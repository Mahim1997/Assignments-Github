function out = bigMatrix_022(inputEqualityMatrix, toMaximise)
% inputEqualityMatrix = [ 4 5 1500 ; 5 3  1575; 1 2 420 ; ] ;
% toMaximise = [ 13 11 ];
disp(inputEqualityMatrix);
disp('Coefficients start with Z and S i , and then x i from left to right') ;
disp(toMaximise);
[row , col] = size(inputEqualityMatrix);
% justInEqual = zeros(row, col-1) ;
% justInEqual( 1:1:row, 1:1:col-1) = inputEqualityMatrix( 1:1:row, 1:1:col-1 );
disp('*****DEBUG');
disp(row) ;
disp(col);
noOfSlack = row ;
matrixRowSize = row + 1 ; %Inequalities + the to maximise term
matrixColSize = col + 1 + noOfSlack ; %no of slacks plus 1 for Z and the no of col of inequalities
matrixColSize = matrixColSize + 1 ;
fullMatrix = zeros(matrixRowSize, matrixColSize)  ;
disp(fullMatrix);
fullMatrix(1, :) = 0 ;  %first row full 1 
% fullMatrix(1,1) = 1 ; fullMatrix(1,2) = -toMaximise(1) ; fullMatrix(1,3) = -toMaximise(2) ;%first row done
fullMatrix(1, 1) =1 ;
for i = 1 : 1 : length(toMaximise)
    fullMatrix(1 ,i+1 ) = toMaximise(i)*-1 ;
end
%last column
fullMatrix(2: 1 : matrixRowSize, matrixColSize-1) = inputEqualityMatrix(1 : 1: row, col) ;
%last column done
%mid parts
fullMatrix( 2 : 1 : row+1 , 2 : 1 : col) = inputEqualityMatrix(1:1:row, 1:1:col-1) ;
%mid parts done

%add slack variables
for i = 2 : 1 : matrixRowSize
    fullMatrix( i, i+col-1) = 1 ;
end
%add slack variables
disp(fullMatrix);
% disp(justInEqual);
out  = fullMatrix ;
return 
