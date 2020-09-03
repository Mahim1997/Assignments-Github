% function [row, col] = dividedDiffTable(x) 
function table = dividedDiffTable(x, fx) 
    format bank ;
        %f(x0) is the value of 'y' when x is x0
%     [x, fx] = arrays( ) ;
    row = length(x) ;
    col = length(fx) ;
    table = zeros ( row, col ) ;
%     display(table) ;
    for j =1 : 1 : row
                table(j, 1) = fx( j) ;
    end
%     myDisplay(table) ;
    
    for j = 2 : col   %should fill up column first
        fxLowerLimit = 1 ; 
        for i = j : row  %start from that column
%                 fxLowerLimit = 1 ;  %for the x2 - x0 eg.
                val= table(i , j-1) - table(i-1 , j-1) ;
                val = val / ( x(i) - x ( fxLowerLimit ) ) ;
%                 fprintf( ' At col %d ==> %.2f - %.2f... divided by  ...  %d - %d = %.2f  placed at table(%d, %d)\n' ,...
%                     j, table(i , j-1) , table(i-1 , j-1) ,...
%                      x(i), x(fxLowerLimit) ,val ,i , j) ;
                table(i, j)  = val ;
                fxLowerLimit = fxLowerLimit + 1 ;
        end
    end
%     myDisplay(table) ;
end