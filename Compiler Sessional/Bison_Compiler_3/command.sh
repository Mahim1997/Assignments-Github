clear
clear
clear
clear
 
bison -d -y 1505022.y
echo '===>>> 1st step bison -d -y 1505022_Bison.y done\n'
g++ -fpermissive -w -c -o y.o y.tab.c
echo '===>>> 2nd step g++ -fpermissive -w -c -o y.o y.tab.c done \n'
flex 1505022.l
echo '====>> 3rd step flex 1505022_Flex.l done \n'
g++ -fpermissive -w -c -o l.o lex.yy.c
echo '====>> 4th step g++ -fpermissive -w -c -o l.o lex.yy.c done\n'
g++ -o a.out y.o l.o -lfl -ly
echo '====>> 5th step g++ -o a.out y.o l.o -lfl -ly done\n\n\n'
./a.out 1505022_Input.txt 

