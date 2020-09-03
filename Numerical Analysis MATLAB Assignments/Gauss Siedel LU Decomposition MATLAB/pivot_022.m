function [out, out2] =  pivot_022(A,B)

sizeRow = length(A);
disp('Size is :');
disp(sizeRow);
sizeCol = length(transpose(A));
if(sizeRow ~= sizeCol)
    disp('Not a Square Matrix, Cannot Pivot ! ');
    return
end
% i know it is a square matrix
% first condition
% nb = sizeRow + 1 ;
n = sizeRow ;
Intermediate = [A B] ;
for k = 1 : 1 : sizeRow -1 
    [x, i] = max ( abs(Intermediate(k:n, k) )) ;
    iter = i + k - 1 ; 
    if iter ~= k
        Intermediate ( [k, iter] , :) = Intermediate([iter, k], : ) ;
        
    end
end
disp(Intermediate);
out = A ;
for i = 1 : 1 : length(A) 
    for j = 1 : 1 :length(A)
            out(i,j) = Intermediate(i,j) ;
    end
end
out2 = B ;
for i = 1 : 1: length(B)
    out2(i, 1) = Intermediate(i, length(A) + 1) ;
end
end