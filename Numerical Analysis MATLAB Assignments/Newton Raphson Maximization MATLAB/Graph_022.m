function Graph_022() 

    x = 0 : .5 : 20 ;
    
    y1 = ( 77 - x.*7) / 11 ;
    y2 = (80 - x.*10) /8 ;
    x3 = -15 : 1 : 10 ;
%     for i = 1 : 1 : 41 
%         x3(i) = myFun(i) ;
%     end
%     disp(x3) ;
    y3 = ones(41).*6  ;  % y = 6
    y4 = zeros(41) ;   % y = 0 
    y5 = ones(26) * 9 ;
    y6 = zeros(26) ;
    xmin = 0 ; xmax = 10 ; ymin = -5; ymax = 10 ;
    plot(x,y1,'r', ...
        x, y2, 'b' , ...
        x, y3, 'm', ...
        x, y4, 'g', ...
        y5, x3, 'b' , ...
        y6, x3, 'g') , ... 
            axis ( [xmin , xmax , ymin , ymax] ),grid on ;
        xlabel('x'); ylabel('y') ;
    set(gca,'xtick',[0:0.25:60]);
    set(gca,'ytick',[-50:1:20]);

end

function out = myFun(x) 
    out = 0 ;
    if(x == 9)
        out = 9 ;
    end
end