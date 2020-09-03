function out = LU_Decomposition_022(A,B)
%Pivotting
if length(A) ~= length(B)
    disp('Row of A not equal to row of B! ');
    return 
end
[A,B] = pivot_022(A,B) ;
disp('After pivot: ');
disp('A is : ');
disp(A);
disp('B is : ');
disp(B);
%Now finding L and U
size = length(A) ; 
L =zeros(size, size) ;
L(size , size )  = 1; 
m = length(A) ;
U = zeros ( size, size ) ;

for col = 1 : 1 : size 
    for row = col+1  : 1 : size
            factor = A(row, col) / A(col, col) ;
            for k = 1: 1: size
%                 A(row, :) = A(col, :) * factor - A(row,:) ;
                    A(row, k) = A(col, k) * factor - A(row,k) ;
                    A(row,k) = - A(row, k) ;
            end
            L(row, col) = factor ;
%                 disp(A);
            
%                 disp('Printing B at row : ');
%                 disp(row) ;
%                 B(row, 1) = B(col,1) * factor - B(row,1) ;
%                 disp(B) ;
            
    end
    L(col, col) = 1 ;
%     disp(A);
end
U = A ;


disp('Upper matrix is : ') ;
disp(U) ;
disp('Lower matrix is : ' );
disp(L) ;
disp('After Forward Elimination , B is : ') ;
disp(B) ;
d = B ;
%Forward substitution
 d(1) = B(1)/L(1,1);
 for i = 2: 1: size,
     d(i) = ( B(i)-  L(i, 1: i-1)* d(1 :i-1) )/  L(i, i) ; %Direct Triangular product
 end
disp('After Forward substitution , d is : ') ;
disp(d) ;

%Back substitution
x = d ;
for i = size : -1 : 1 
    x(i) = d(i  ) / U(i, i ) ;
    for j = 1 : 1 : i-1 
        d(j ) = d(j, 1) - ( x(i) * U(j, i) );
    end
end
disp(' Finally x is : ') ;
disp(x) ;
out = x ;
end