function myPrint_022(table)
    n = length(table);
    for i = 1 : n 
        for j = 1 : n
            fprintf('%5.6f   ', table(i, j)) ;
        end
        fprintf('\n');
    end
    fprintf('\n');
end