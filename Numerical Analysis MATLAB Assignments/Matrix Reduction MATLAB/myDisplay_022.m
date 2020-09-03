function myDisplay( table ) 
    [ row, col ] = size(table) ;
    for i = 1 : 1 : row
        for j = 1 : 1 : col 
                fprintf(  '  %11.5f   ' , table(i, j) )  ;
        end
        fprintf('\n') ;
    end
    fprintf('\n\n') ;
end