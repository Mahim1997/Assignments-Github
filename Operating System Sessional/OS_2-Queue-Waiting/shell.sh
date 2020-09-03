echo "INSIDE shell.sh"
g++ -pthread main.cpp -o main_obj
echo "Main obj is created... and executing..."
./main_obj
