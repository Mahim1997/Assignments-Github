function myDisplay(squareMatrix, rhsMat)
    for i = 1 : length(squareMatrix)
        for j = 1 : length(squareMatrix)
            fprintf('%9.5f  ', squareMatrix(i, j) ) ;
        end
        fprintf(' => %9.5f ' , rhsMat(i) ) ;
        fprintf('\n') ;
    end
    fprintf('\n') ;

end