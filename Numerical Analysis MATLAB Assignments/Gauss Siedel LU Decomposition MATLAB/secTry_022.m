% Input has to be taken
dim1 = input('Enter first dimension (i.e. row)  ') ;
dim2 = input('Enter second dimension (i.e. column)  ') ;
disp('Row is ');
disp(dim1);
disp('Col is ');
disp(dim2);
if(dim1 ~= dim2)
    disp('A is NOT SQUARE MATRIX ');
    return 
end
A = ones(dim1, dim2) ;
for i = 1 : 1 : dim1 
    for j =1 : 1 : dim2
        A(i, j) = input('Enter element :  '  ) ;
    end
end
rowB = input('Enter row size of B matrix ');
colB = input('Enter Col size of B matrix ');
if(rowB ~= dim1)
    disp('Cannot solve , not enough parameters as B row doesnt match A row');
    return 
end
B = ones(rowB, colB) ;
if colB ~=1 
    disp('Not an equaition form as column is not 1 ') ;
    return 
end
for i = 1 : 1 : rowB 
    for j = 1 : 1 : colB
        B(i, j) = input('Enter elements: ') ;
    end
end

err = 0.5 ; maxItr = 150 ;
choice = -100 ;
choice = input('1- Gauss Seidel , 2 - LU , 3 - Both  ') ;
if choice == 1
    GaussSeidel_022(A,B, err, maxItr) ;
end
if choice ==2 
    LU_Decomposition_022(A,B);
end
if choice == 3
    GaussSeidel_022(A, B, err, maxItr);
    LU_Decomposition_022(A, B) ;
end
disp('Ans after inverse is : ') ;
disp(inv(A) * B ) ;
