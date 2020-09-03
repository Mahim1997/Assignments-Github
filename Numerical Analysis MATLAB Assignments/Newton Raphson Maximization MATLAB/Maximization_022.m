function Maximization_022 (inputEqualityMatrix,toMaximise)
% inputEqualityMatrix = [ 4 5 1500 ; 5 3  1575; 1 2 420 ; ] ;
% toMaximise = [ 13 11 ];
% inputEqualityMatrix = [ 7 11 77 ; 10 8 80 ; 1 0 9 ; 0 1 6 ; ] ;
% toMaximise = [ 150 175 ];
bigMatrix = bigMatrix_022(inputEqualityMatrix, toMaximise);
[row, col] = size(bigMatrix) ;
disp('Z then x variables , then slack , then condition , then upperbound ');
disp(bigMatrix) ;

itr = 1 ;
while 1 
    disp( sprintf( ' Now iteration num %d \n' , itr ) ) ;
    
    biggestNeg = -row ; biggestNegCol = -row ;
    for i = 2 : 1 : col 
        if i==2
            biggestNeg = bigMatrix(1, i) ;
            biggestNegCol = i ;
        end
        if bigMatrix(1, i) < biggestNeg
            biggestNeg = bigMatrix(1, i) ;
            biggestNegCol = i ;
        end
    end
    % disp(biggestNegCol);
    %divide RHS with biggestNegElements to get upper bound
    for i = 2 : 1 : row
        bigMatrix(i, col) = bigMatrix(i, col-1) / bigMatrix(i, biggestNegCol) ;
        display(bigMatrix);
    end
    %upperbound done


    upperBoundLowestRow = 2 ; upperBLowestEl = bigMatrix(2, col );
%     counter = 2 ;
    firstPositiveMin = 1200000 ;
    for i = 2 : 1: row 
        if bigMatrix(i, col)  > 0 
            firstPositiveMin = bigMatrix(i, col) ; 
            firstPositiveMinRowIndex = i ;
            break ;
         end
    end
    upperBLowestEl = firstPositiveMin ;
    upperBoundLowestRow = firstPositiveMinRowIndex ;
    
%     upperBoundLowestRow = 2 ; upperBLowestEl = bigMatrix(2, col) ; ////TO DO 
    oldVal = upperBLowestEl ; oldIndex = upperBoundLowestRow ;
    for i = upperBoundLowestRow : 1: row 
            if ((bigMatrix(i, col)  < upperBLowestEl) && (bigMatrix(i, col) > 0))
                    upperBLowestEl = bigMatrix(i, col) ;
                    upperBoundLowestRow = i ;
            end
    end
%         if bigMatrix(i, col) > 0 && counter < 2 
%             upperBoundLowestRow  = i ;
%             counter = counter + 1 ;
%         end
%         if counter >= 2 
%             if bigMatrix(i, col) < upperBLowestEl 
%                 upperBLowestEl = bigMatrix(i, col) ;
%                 upperBoundLowestRow = i ;       
%             end
%         end
%         display( i  );
    display( biggestNegCol ) ;
    display( upperBoundLowestRow ) ;
    % display( upperBoundLowestRow);
    format shortG ;
    %make that row 1 col 1 and that col all zeroes
    disp(bigMatrix);
    x = bigMatrix(upperBoundLowestRow, biggestNegCol) ;
    % display(biggestNegCol) ;
    bigMatrix(upperBoundLowestRow, :) = bigMatrix(upperBoundLowestRow, :)  / x ;
    %done
    disp(bigMatrix);
    %oita baade baki shob zero
    val = bigMatrix(1:1:row, col) ;
    for i = 1 : row
        if i==upperBoundLowestRow
            continue ;
        end
        factor = bigMatrix(i, biggestNegCol)  ;
        bigMatrix(i, :) = bigMatrix(i, :) - bigMatrix(upperBoundLowestRow, :) .*factor ;
    %     disp(factor) ;
    end
    % colChanger = 5 ;
    % for i = 1 : row
    %     bigMatrix(i, col) = bigMatrix(i, col-1) / bigMatrix(i, col-colChanger) ;
    % end
    %done

    % display(x) ;
    bigMatrix(1:1:row, col) = val ;
    disp('Iteration ends') ;
    disp(bigMatrix);
    itr = itr+1 ;
    
    %breaking condition Z er row cannot have negative values
    negativesNumber = 0 ;
    for i = 1 : 1 : col 
        if bigMatrix(1, i) < 0 
            negativesNumber = negativesNumber  +  1 ;
        end
    end
    if(negativesNumber == 0 )
        break ;
    end
end
x = zeros(length(toMaximise )) ;
var1 = length(inputEqualityMatrix) ;%- 1 ;
% display(var1) ;
S = zeros(var1 ) ;
% display(length(S)) ;
% display(length(S)) ;
for j = 2 : length(x) +2    %Because first is Z
    for i = 2 : row 
        noOfones = 0 ;
        if(bigMatrix(i, j) == 1)
%             disp( sprintf( ' Inside %d , %d is no of ones %d \n', i, j, noOfones ) ) ;
            noOfones = noOfones + 1 ;
            indexI = i ;
            indexJ = j ;
        end
%         disp( sprintf( ' Inside %d , %d is no of ones %d \n', i, j, noOfones ) ) ;
        if noOfones == 1 
    %         disp(indexI) ;disp( indexJ ) ; 
            x(j-1) = bigMatrix(indexI , col -1 ) / bigMatrix(indexI, indexJ) ;
        end
    end
end

initialColumnStarter = length(x) + 2  ; %Start from after x terms are finished
%Now this initialCol is 2+1 = 3
% THIS IS FOR S[ ] 
for colItr = initialColumnStarter : 1 : length(S) + initialColumnStarter - 1
    noOfones = 0 ;  %Every column noOfOnes is initially zero
    for rowIter = 2 : 1 : row
        if bigMatrix(rowIter, colItr) == 1 
            rowIndex = rowIter;
            colIndex = colItr ;
            noOfones = noOfones + 1 ;
        end
    end
    if noOfones == 1
%         display ( sprintf( 'Row : %d , Col : %d , noOfones = %d ', rowIndex, colIndex, noOfones ) ) ;
%         v1 = rowIndex ; v2 = bigMatrix( rowIndex, col - 1 ) ;
%         display( sprintf( ' Switching S (..%d) with ... %d ',v1 , v2 ) ) ;
        S( rowIndex) = bigMatrix( rowIndex, col - 1 ) / 1  ;
    end
    
    
end


%S is done

format shortG ; 
disp(sprintf(' *****FINALLY ... \n ')) ;
for i = 1 : length(x)
    disp(sprintf(' x [%d] = %d ' , i , x(i) ) );
end
% display(length(inputEqualityMatrix)) ;
for i = 1 : length(S)
    disp(sprintf(' S [%d] = %d ' , i , S(i) ) );
end
Z = bigMatrix(1, col-1) ;
disp( sprintf( ' Z = %d \n' , Z ) ) ;
disp( sprintf('No of iterations = %d ' , itr)) ;



end