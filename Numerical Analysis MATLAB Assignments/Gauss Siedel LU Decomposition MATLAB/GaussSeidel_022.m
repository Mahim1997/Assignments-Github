function out = GaussSeidel_022(A,B,relativeApproxError, maxIteration)
%A is the square matrix
[A, B] = pivot_022(A,B) ;
% out = 0 ;
disp('After pivoting : ');
disp('A is '); disp(A); disp('B is ') ; disp(B);
rowLength = length(A);
xNew = zeros(rowLength,1); 
xOld =  zeros(rowLength,1); %row X col initial set at zeroes
%xUpdate = zeroes(rowLength, 1);
%Iteration occurs in column major
itr = 1 ; 
err = 100 ; 
error = zeros(rowLength) ;
%maxIteration = 20 ;
prevTerm = 110 ; currentTerm = 0 ; %&& err>relativeApproxError
while (itr<=maxIteration) 
    for i = 1 : 1: rowLength
        %prevTerm = xOld(i,1);
        sum1 = 0 ; sum2 = 0 ;
%         disp('<><><><> When i is ');
%         disp(i);
        for j = 1 : 1: i-1
            %COlumn comes first
%             disp('first loop j is ');
%             disp(j);
%             disp(sum1);
            sum1 = sum1 + A(i,j) * xNew(j,1) ;
        end
        sum1 = -sum1 ;
        for j = i+1 : 1 : rowLength
%             disp('second loop j is ');
%             disp(j);
%             disp(sum2);
            sum2 = sum2 + A(i,j) * xOld(j,1);
        end
        sum2 = -sum2 ;
        currentTerm = sum1 + sum2 + B(i,1);
%         disp('Current term ');
%         disp(currentTerm);
        currentTerm = currentTerm / A(i,i) ;
%         disp('Current Term again');
%         disp(currentTerm);
        xNew(i,1) = currentTerm; 
        error(i) = xNew(i,1) - xOld(i,1) ;
        error(i) = abs(error(i)) ;
        error(i) = error(i) / xOld(i) * 100 ;
    end
    err = max(error) ;
%     err = abs(max(xNew) - max(xOld));
%     err = err / currentTerm * 100 ;
%     disp('***ON THE INSIDE');
%     disp(xNew);
%     disp('***DONE');
    xOld = xNew ;
    itr = itr + 1;  
    
    if err <= relativeApproxError
        disp('No of iterations is : ');
        disp(itr);
        out = xNew ;
        disp(out) ;
        return 
    end
end
disp('No of iterations is : ');
disp(itr);


out = xNew ;
disp(out) ;
% disp('Xold is ');
% disp(xOld);
% disp('xNew is ');
% disp(xNew);
%out = 0 ;
end 