function runner_022()
    clc ;
    [xCords, yCords] = arrays_022() ;
%     table = dividedDiffTable(xCords, yCords) ;
    x = -1 : .1 : 1 ; y = x ;
    [xTest, yTest] = testPoints_022( ) ;
    error = zeros(length(xTest) ) ;
    meanError = zeros(length(yCords) - 1  ) ;  %15
    yEstimated = xTest ;
    for i = 1 : (  length(xCords) - 1 )
        for k = 1 : length(xTest)
            yEstimated(k) = fun_022( xTest(k), i , xCords, yCords) ;
        end
        for k = 1 : 1 :length(xTest)
%             fprintf('k is %d =>x is %f , Estimation is %f and actual is %f FOR ORDER %d \n', k , xTest(k), yEstimated(k), yTest(k),i );
            val = yEstimated(k) - yTest(k) ;
            val = abs(val) ;
            val = val /  yTest(k) ;
            val = abs(val)  * 100 ;
            error(k) = val ;
        fprintf('x is %f , Estimation is %f and actual is %f FOR ORDER %d , TRUE error is %f\n',...
                     xTest(k), yEstimated(k), yTest(k),i , error(k));
        end
        sum = 0 ;
        for k1 = 1 : length(error) 
            sum = sum + error(k1) ;
        end
        meanError(i)  = sum/length(error)  ;
        str2 = sprintf( ' Average Error with %dth polynomial is %f percent \n' , i, meanError(i) ) ;
        disp(str2) ;
        for j =1 : length(y)
            y(j) = fun_022( x(j) , i , xCords, yCords) ;
        end
%         scatter(xTest, yTest) ;
        scatter(xCords, yCords) ;
        hold on ;
        plot(x, y, 'r') ,    axis ([-1,1, -2, 6]) ...
            , grid on  ,xlabel('x') , ylabel('f(x)') , title('Graph of interpolated function') ;
%         hold off ;
        set(gca,'xtick',[-2:  .05  : 2]);
        set(gca,'ytick',[-50: .25 :20]);
        str = sprintf(' %d(th) order polynomial ' , i ) ;
        legend(str) ;
        pause(2) ;
    end
    
%     for i = 1 :  length(meanError)
%         fprintf('=>Avg error with %d polynomial is %f percent\n ' , i , meanError(i) ) ;
%     end
    
end