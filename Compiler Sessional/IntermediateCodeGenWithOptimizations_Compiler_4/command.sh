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


g++ -o outOpt.out IntermediateCodeStuffs/OptimizerRunner.cpp 
./outOpt.out

#echo '=>Trying to execute Optmizer.l\n'
#flex -o output2.cpp IntermediateCodeStuffs/OptimizedCodes/1505022_Optimizer.l
#echo '=>1st Step Done\n'
#g++ output2.cpp -lfl -o out2.out
#echo '=>2nd Step Done\n'
#./out2.out IntermediateCodeStuffs/ICode.asm
#echo '=>3rd Step Done\n'

#g++ -o output foo.c