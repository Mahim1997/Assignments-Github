function [out, finalOrder, finalError, table] = romberg_022(fun, a, b,  ea, inputFinalOrder, trueVal)
flag = 1000 ;
rowNum = inputFinalOrder / 2 ;
table = zeros(rowNum, rowNum);
n = length(table);
step = 1 ;
finalOrder = inputFinalOrder;
% for i = 1 : n
%     table(i, 1) = trapezium_022(@f_For_Prob1, a, b, step) ;
%     step = step * 2 ;
% end
% myPrint_022(table) ;
step = 1 ;
table(1, 1) =  trapezium_022(fun,  a, b, step) ;
out = table(1, 1) ;
step = step * 2 ;
for row = 2 : n 
    table(row, 1) = trapezium_022(fun, a, b, step) ;
%     fprintf('==>table ( %d, 1 ) = %f \n' ,row , table(row, 1) ) ;
    for col = 2 : row 
        moreAccurateTerm = table(row , col  - 1);
        lessAccurateTerm = table(row - 1  , col - 1);
        k = col ;
        result = (4 ^ (k - 1 ) * moreAccurateTerm ) - lessAccurateTerm ;
        result = result / ( ( 4 ^ (k - 1 )) - 1 ) ;
        table( row, col ) = result ;
        err = ( abs(result - trueVal) / trueVal ) * 100 ;
%         if err <= ea
%             finalError = err ;
%             out = result ;
%             finalOrder = k ;
%             flag = -1000 ;
%             break ;
%         end
%         fprintf(' ( %d , %d) , result = %f   and err = %f \n' , row , col , result ,  err ) ;
%         myPrint_022(table) ;
%         fprintf(' moreAccurate : table( %d, %d ) = %f , lessAccurate , tab(%d ,%d ) = %f ' ,row , col - 1 , moreAccurateTerm ,...
%             row - 1, col -1 , lessAccurateTerm) ;
%         fprintf(' ( %d , %d ) , k = %d , result = %f  , err = %f percent' , row , col, k, result , err ) ;
    end
%     if flag == -1000
%         break ;
%     end
    if err <= ea
        finalError = err ;
        out = result ;
        finalOrder = k ;
        flag = -1000 ;
        break ;
    end
%     fprintf('\n') ;
    step = step * 2 ;
end


% myPrint_022(table) ;
% fprintf('Final result is : %4.6f and final order is O(h^%d) and final error is %4.6f percent \n' ,...
%     out , 2*finalOrder, finalError);
end