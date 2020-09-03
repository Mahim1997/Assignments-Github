function actualRun()
clc ;
[xTrain, yTrain] = trainingPoints() ;
[xTest, yTest] = testPoints () ;
% displayArray(xTrain) ;
% displayArray(yTrain) ;
fprintf('*****************\n\n') ;
maxOrder = length(xTrain) - 1 ;
diff = length(xTrain) - 1 ; 
for currentOrder = 1 : maxOrder
    indices = zeros(1, currentOrder+1) ;
    for j = 1 : currentOrder
        indices(1, j) = 1 + round (    ( j - 1 )  * ( diff / currentOrder )  ) ;
        indices(1, length(indices) ) = length(xTrain) ;
    end
%     display(length(indices) ) ;
    xArray = indices ; yArray = indices ;
    for i = 1 : length(indices)
        xArray(i) = xTrain ( indices(i) ) ;
        yArray(i) = yTrain ( indices(i) ) ;
    end
%     displayArray(indices) ;
%     displayArray(xArray) ;
%     displayArray(yArray) ;
    init = -3.14159 ; 
    fin = 4.14159 ;
    x = linspace( init, fin, 20 ) ; y = x ; 
    for i = 1 : length(x)
        y(i) = generateFinalFunction(xArray, yArray, x(i));
    end
    
    error = xTest ;
    for i = 1 : length(error)
        val = yTest(i) - generateFinalFunction(xArray, yArray, xTest(i)) ;
        val = abs(val) ;
        error(i) = val ;
%         fprintf(' %d order : xTestCase ( %d ) is %f and from func is %f and yTest is %f and error abs is %f \n' , ...
%             currentOrder, i, xTest(i), generateFinalFunction(xArray, yArray, xTest(i)), yTest(i), val ) ;
    end
    fprintf('==>With %d order , mean TRUE error is %f \n ' , currentOrder, mean(error) ) ;
%     [ a1, b1 , c1] = generate_a_b_c(xArray, yArray) ;
%     displayArray(a1) ; displayArray(b1) ; displayArray(c1) ;
%     scatter( xTest, yTest) ;
    scatter(xTrain, yTrain); 
    hold on ;
    plot(x, y , 'r' ) ; title('Quadratic Spline') ; xlabel('x') ; ylabel('f(x)');
    set(gca,'xtick',[-20:  .25  : 20]);
%     set(gca,'ytick',[-60:  .25  : 60]);
    str = sprintf('Polynomial with %d order approx' , currentOrder ) ;
    legend(str) ;
    grid on ;
%     hold off ;
    pause(3) ;
%     displayArray(x) ;
%     displayArray(y) ;
end

% displayArray(xTest) ;
% displayArray(yTest) ;

% x = linspace( -3.14159, 3.14159, 20 ) ;
% for i = 1 : length(x)
%     y(i) = generateFinalFunction(xArray, yArray, x(i));
% end
% 
% displayArray(x) ;
% displayArray(y) ;
end