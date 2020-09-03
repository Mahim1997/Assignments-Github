function displayArray(arr)
[row, col] = size(arr) ;
for i = 1 : row
    for j = 1 :col
        fprintf('%9.5f  ' , arr(i, j) ) ;
    end
    fprintf('\n');
end
fprintf('\n');

end