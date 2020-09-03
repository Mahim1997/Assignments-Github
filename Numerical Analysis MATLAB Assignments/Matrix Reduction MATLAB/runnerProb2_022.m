function runner()
% x = [-3.1415,3.1415] ;
clc;
%     x = -3.1415: .5 :3.1415 ;
    [x, fx] = trainingPoints() ;
    
    order = 1;
    %with two points
    diff = length(x) - 1 ;
    l = 1 ; u = length(x) ;
    xArray = zeros(order + 1 ); indices = zeros(order + 1 ) ; yArray = xArray ;
%     display(length(xArray) ) ;
    for i = 1 : length(xArray)
        indices(i) = l + round( ( i-1) * diff / (order) ) ;
    end
    for i = 1 : length(xArray)
        xArray(i) = x( indices(i ) ) ;
        yArray(i) = fx(indices(i) ) ;
%         fprintf('Inside xArray : at %dth is %f \n ' , i , xArray(i) ) ;
    end
    aCoeffs = zeros(order ) ;
    bCoeffs = zeros(order) ;
    cCoeffs = zeros(order - 1 ) ;
    if(order == 1 ) 
        squareMat = zeros(order+1, order+1)  ;
        rhsMat = zeros(length(squareMat) ) ;
        for i = 1 : 1 : length(squareMat)
            squareMat(1, i) = 2* xArray(i) ;
            squareMat(2, i) = 1 ;
            rhsMat(i) = yArray(i) ;
        end
        myDisplay(squareMat, rhsMat) ;
    end
end