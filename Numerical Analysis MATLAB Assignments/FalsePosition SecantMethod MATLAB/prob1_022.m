function prob1_022()
x = -1:.1:1;
y = log(1+x);
plot(x,y, x, appFun_1505022(x,1),'r',...
    x,appFun_1505022(x,3),'b' ,...
    x,appFun_1505022(x,5) ,'g' ,...
    x,appFun_1505022(x,20), 'm') ...
, xlabel('x') , ylabel('ln(1+x)'), title('Graph of ln(1+x) against x'), ...
grid on,...
legend('ln(1+x)', '1 term', '3 terms', '5 terms', '20 terms');
axis ([-1.2, 1.2, -2.5 1]);
set(gca,'xtick',[-1:.1:1]);
set(gca,'ytick',[-2.5:.1:1]);
figure ;

array = 2:1:50 ;
num = 0.5;

prevApp = appFun_1505022(num, 1);

errorArray = 1:1:49 ;

for i=1:1:49
   currentApp = appFun_1505022(num, array(i)); 
   appErr = currentApp - prevApp;
   relativeErr = abs(appErr/currentApp);
   prevApp = currentApp;
   errorArray(i) = relativeErr ;
   errorArray(i) = errorArray(i)*100 ;
   display(relativeErr)
end

%display(errorArray)
plot(array,errorArray,'r') , xlabel('Number of terms') ...
    , ylabel('Relative Approximation Error / %')...
    , title('Graph of Relative Approximation Error against Number of terms'), ...
grid on;
set(gca,'xtick',[2:2:50]);
set(gca,'ytick',[2:2:48]);

end

function myAns = appFun_1505022(x,n)
%x = 0 ;

sumOdd = 0;
sumEven = 0 ;


for i = 1:2:n     %odd terms
    sumOdd = sumOdd + (x.^i)/i ;
end

for i = 2:2:n     %even terms
    sumEven = sumEven + (x.^i)/i ;
end

myAns = sumOdd - sumEven ;

end