clc ;
x = 1 : 1 : 5 ; y = x ;
for i =2 : 10 
    for j = 1 : length(x)
        y(j) = x(j)^i ;
    end
    scatter(x, y);
    hold on ;
    plot(x, y, 'r' ), grid on  ;
    hold off ;
    pause ( 2) ;
end