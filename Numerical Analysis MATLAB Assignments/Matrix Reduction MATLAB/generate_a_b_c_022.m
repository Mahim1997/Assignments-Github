function [ a, b, c] = generate_a_b_c(x, y)
% clc ;
% x = [ 3 ,  4.5 ,  7 , 9 ];
% y = [ 2.5 , 1  , 2.5,  .5] ;
% [squareMat ,rhsMat] = generateReducedMatrix_firstLinear(x, y) ;
% myDisplay(squareMat, rhsMat) ;
coeff = generateCoefficients(x, y) ;
% displayArray(coeff) ;


lengthOfCoeff = length(coeff) / 3 ;
a = zeros( 1, lengthOfCoeff  ) ;
b = zeros(1,  lengthOfCoeff ) ;
c = zeros( 1, lengthOfCoeff  ) ;

for i = 1 : length(coeff)
    if(mod(i, 3) == 1)
        a(((i-1)/3)+1) = coeff(i) ;
    end
    if(mod(i, 3) == 2)
        b(((i-2)/3)+1) = coeff(i) ;
    end
    if(mod(i, 3) == 0)
        c(((i-0)/3)) = coeff(i) ;
    end
end

% displayArray(a) ; displayArray(b) ; displayArray(c) ;

end