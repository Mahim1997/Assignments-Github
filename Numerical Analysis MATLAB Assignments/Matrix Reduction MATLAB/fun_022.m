function out = fun_022(x, n, xCords, yCords)
    if(n> length(xCords))
        frpintf('Array doesnot have enough values! \n');
        return ;
    end
%     disp('FROM THE OTHER SIDE') ;
    d = length(xCords) - 1 ;
%     xArray = ones(n) ;
    l = 1 ; u = l + d ;
    indices = ones(n+1) ;
    indices(1) = l ;
    indices(length(indices) ) = u ;
    for i = 1 : length(indices)
        indices(i) = l + round((i-1)*d/n) ;
    end
%     display(indices) ;
    xArray = indices ; yArray = indices ;
    for i = 1 : length(indices)
        xArray(i) = xCords(indices(i)) ;
        yArray(i) = yCords(indices(i)) ;
    end
    table = dividedDiffTable_022(xArray, yArray) ;
%     myDisplay(table) ;
    out = table(1, 1) ;
%     prod1 = 1 :
    prod = 1;
    for i = 1 : 1 : n  %loop should run n times
%         fprintf(' initially prod is %f  , array val is %f \n' , prod , xArray(i) ) ;
        prod= prod * ( x - xArray(i)  ) ;
%         fprintf('===>>mid : x ( %f ) - x0 ( %f ) => table(i+1, i+1) is %f * prod or x - x0( %f ) + out ( %f ) \n' ,...
%             x , xArray(i) , table(i+1, i+1) , prod , out) ;
        out = out + prod * table(i+1, i+1) ;
%         fprintf(' finally prod is %f  , array val is %f , out is %f\n' , prod , xArray(i) , out) ;
    end
%     fprintf('FUFUUFUFUF\n') ;
end