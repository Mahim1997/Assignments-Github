#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<sstream>
#include<unistd.h>

#define SIZE_QUEUE 5
#define CHOCOLATE 2
#define VANILLA 1
using namespace std;

//Semaphores and mutexes and queues...

//Semaphores for queues 1, 2, 3 respectively ...
sem_t empty_q1;
sem_t full_q1;

sem_t empty_q2;
sem_t full_q2;

sem_t empty_q3;
sem_t full_q3;

//QUEUES 
queue<string> q1;	//Just Cakes
queue<string> q2;	//Vanilla Cakes
queue<string> q3;	//Chocolate cakes

//MUTEXES For locking on queues respectively

pthread_mutex_t lock_queue_1;
pthread_mutex_t lock_queue_2;
pthread_mutex_t lock_queue_3;	
//FOR PRINTING 

pthread_mutex_t print_lock;	



void init_mutex(){
	pthread_mutex_init(&lock_queue_1, 0);
	pthread_mutex_init(&lock_queue_2, 0);
	pthread_mutex_init(&lock_queue_3, 0);
	pthread_mutex_init(&print_lock, 0);
}
void init_semaphore(){
	sem_init(&empty_q1, 0, SIZE_QUEUE);
	sem_init(&empty_q2, 0, SIZE_QUEUE);
	sem_init(&empty_q3, 0, SIZE_QUEUE);

	sem_init(&full_q1, 0, 0);
	sem_init(&full_q2, 0, 0);
	sem_init(&full_q3, 0, 0);
}

//------------------Print using mutex lock--------------------------
void print(string s){
	pthread_mutex_lock(&print_lock);
	cout << s << endl ;
	sleep(2);	//fOR TEST
	pthread_mutex_unlock(&print_lock);
}
void print(string s, int time){
	pthread_mutex_lock(&print_lock);
	//printf("%s\n", s);
	cout << s << endl ;
	sleep(time);
	pthread_mutex_unlock(&print_lock);
}
string integerToString(int a){
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}
string getTypeName(int a){
	if(a==CHOCOLATE){
		return "CHOCOLATE";
	}else if(a == VANILLA){
		return "VANILLA";
	}
	return "NULL_TYPE";
}
string formCake(int type, int id){
	string typeName;
	if(type == CHOCOLATE){
		typeName = "C";
	}else if(type == VANILLA){
		typeName = "V";
	}
	string name = typeName + "_" + integerToString(id);
	return name ;
}
int getTypeFromName(string name){
	if(name.at(0) == 'V'){
		return VANILLA;
	}else if(name.at(0) == 'C'){
		return CHOCOLATE;
	}
	return -1;
}

//---------------------------------------LEFT CHEFs X and Y Function----------------------------------------
void *leftChefsFunc(void *arg){
	int *ptr = (int*)arg;
	int val = *ptr;
	string cake;
//val == 2 then chocolate, val == 1 then vanilla ....
	string type;
	if(val == CHOCOLATE){
		type = "Chocolate";
	}else{
		type = "Vanilla";
	}
	string str ;
	int iter = 1;
	while(1){
		string str1 = "<START>Inside Chef " + type + " , " ; 
		string str2 = "trying to call down(empty_q1)";
		str = str1 + str2; 
		print(str);	//WITH LOCK PRINT...
		
		
		string str3 = "-->>>Inside CR <empty_q1_sem> of Chef " + type + " , tryin to access mutex for q1 ... " ;
		
		//Form cake
		cake = formCake(val, iter);

//------------------------------------------------CRITICAL REGION BEGINS---------------------------------------
		sem_wait(&empty_q1);
				
			print("--->>>>Inside Critical Reg of Chef " + type + " , tryin to lock q1.mutex");

			pthread_mutex_lock(&lock_queue_1);
				q1.push(cake);		//Push to queue1
				queue<string> copy = q1;	//Copy queue1 to a temporary queue..
			//pthread_mutex_unlock(&lock_queue_1);	//EIKHANE RAAKHLE Multiple access er karone inconsistency aashe

		//PORE PRINT KORLE INCONSISTENCY HOBE...
			string s = "Chef " + type + ", Q1 -> adding cake " 
					+ type + " ID = " + integerToString(iter);
			s += " printing q1 ... \nQ1: ";
		  	while (!copy.empty())
		  	{
		    	s += copy.front() + " , ";
		    	copy.pop();
		  	}
			print(s); 

			pthread_mutex_unlock(&lock_queue_1);

		sem_post(&full_q1);
//---------------------------------------------------------CR ENDS------------------------------------------------

		print("<NON CR>Chef " + type + " going to sleep for 6 sec TEST");
		sleep(6);
		iter++;
		//other non critical activities ....
	}






	
	return 0;
}


//---------------------------------------------NOW INTERMEDIATE Chef Z FUNCTION------------------------------------------
void *intermediateChefFunc(void *arg){

	int iter = 1;
	while(1){
		string str = "++--->>>Inside CHEF Z ";
		if(iter==1){
			str = "==>>>Chef Z starts now .. ";
		}
		
		str = str + " , Check<NOT REMOVE> top q1 (Try to unlock Q1.mutex ... ) ";
		string cake;
		int element;
  		sem_wait(&full_q1);

			print(str);

			pthread_mutex_lock(&lock_queue_1);
				cake = q1.front();
			pthread_mutex_unlock(&lock_queue_1);
			
			print("-->>After checking, Chef Z, top cake is " + cake + " , call sem_wait(Q3./Q2.empty)...");
			element = getTypeFromName(cake);


			if(element == CHOCOLATE){
				//Check Queue 3 ...
				sem_wait(&empty_q3);

					print("--->>>>CHEF Z ,Check Chocolate cake queue is accessable... 1st trying mutex Q1..");
					pthread_mutex_lock(&lock_queue_1);
					pthread_mutex_lock(&lock_queue_3);
						//RETRIEVE FROM QUEUE1 ...
						q1.pop();
						queue<string> copy = q1;
					//pthread_mutex_unlock(&lock_queue_1);	//PRINTING E INCONSISTENCY ASHE EIKHANE RAKHLE
					
					print("--->>>>Q1.mutex success, Z pops cake " + cake + " from Q1 ,call SEM_UP(Q1.empty) and try to lock Q3.mutex");
		//sem_post(&empty_q1);	//EIKHANE DILE INCONSISTENCY HOY PRINTING ER
					//pthread_mutex_lock(&lock_queue_3);	//PRINTING E INCONSISTENCY ASHE EIKHANE RAKHLE
						//PUSH TO QUEUE CHOCOLATE
						q3.push(cake);
						queue<string>copy3 = q3;
					pthread_mutex_unlock(&lock_queue_3);
					pthread_mutex_unlock(&lock_queue_1);
				//IF THIS PRINTING DONE AFTER sem_post(q3.full) then inconsistency would arise...
					string s = "AFTER CHEF Z push CHOCOLATE, print Queues 1 and 3\nQ1: ";
				  	while (!copy.empty())
				  	{
				    	s += copy.front() + " , ";
				    	copy.pop();
				  	}
			  		s += "\nQ3: ";
			  	 	while (!copy3.empty())
				  	{
				    	s += copy3.front()+ " , ";
				    	copy3.pop();
				  	}
				  	print(s);
	  	sem_post(&empty_q1);
				sem_post(&full_q3);



				

			}else if(element == VANILLA){
				//Check Queue 2 ...
				sem_wait(&empty_q2);

					print("++++--->>>>CHEF Z ,Check Vanilla cake queue is accessable... 1st trying mutex Q1..");
					pthread_mutex_lock(&lock_queue_1);
						//RETRIEVE FROM QUEUE1 ...
						q1.pop();
						queue<string>copy = q1;
					pthread_mutex_unlock(&lock_queue_1);
					
					print("++++--->>>>Q1.mutex success, Z pops cake " + cake + " from Q1 ,call SEM_UP(Q1.empty) and try to lock Q2.mutex");
		//sem_post(&empty_q1);//EIKHANE DILE INCONSISTENCY HOY PRINTING ER
					pthread_mutex_lock(&lock_queue_2);
						//PUSH TO QUEUE Vanilla
						q2.push(cake);
						queue<string>copy3 = q2;
					pthread_mutex_unlock(&lock_queue_2);
				
				//IF THIS PRINTING DONE AFTER sem_post(q2.full) then inconsistency would arise...
					string s = "AFTER CHEF Z push Vanilla, print Queues 1 and 3\nQ1: ";
				  	while (!copy.empty())
				  	{
				    	s += copy.front() + " , ";
				    	copy.pop();
				  	}
			  		s += "\nQ2: ";
			  	 	while (!copy3.empty())
				  	{
				    	s += copy3.front()  + " , ";
				    	copy3.pop();
				  	}
				  	print(s);
	  	sem_post(&empty_q1);
				sem_post(&full_q2);

		 

			}

		print("+++++>>>>>>CHEF Z GOING TO SLEEP FOR 4sec");
		sleep(4);
		iter++;
	}	

	return 0;
}

//-------------------------------------------NOW WAITER CHOCOLATE FUNCTION-------------------------------------
void *waiterChocolateFunc(void *arg){
	int iter = 1;
	while(1){

		string str; 
		str = "Inside Waiter Chocolate ... ";
		if(iter==1){
			str = "Chocolate Waiter starting...";
		}


		string s = "--++->>AFTER Waiter retreives Choc cake, printing queue 3 ..";
	  	s += "\nQ3: ";
	  	string cake; 
		sem_wait(&full_q3);
			print(str);
			pthread_mutex_lock(&lock_queue_3);
				cake = q3.front();
				q3.pop();
				queue<string>copy2 = q3;
				str += " , popping Chocolate Cake ... ";
			pthread_mutex_unlock(&lock_queue_3);
	  	 	
	  	 	while (!copy2.empty())
		  	{
		    	s += copy2.front() + " , ";
		    	copy2.pop();
		  	}
		  	print(s);
		  	//IF DONE LATER THEN INCONSISTENCY WOULD ARISE....
		sem_post(&empty_q3);
		
		


	  	


		iter++;
		print("<TEST>WAITER CHOCOLATE going to sleep for 1000sec");
		sleep(1000);
	}
	return 0;
}

//-------------------------------------------NOW WAITER VANILLA FUNCTION-------------------------------------
void *waiterVanillaFunc(void *arg){
	int iter = 1;
	while(1){

		string str; 
		str = "Inside Waiter Vanilla ... ";
		if(iter==1){
			str = "Vanilla Waiter starting...";
		}
		string s = "-->>AFTER Waiter retreives Van cake, printing queue 2 ..";
	  	s += "\nQ2: ";

		
	  	string cake;
		sem_wait(&full_q2);
			print(str);
			pthread_mutex_lock(&lock_queue_2);
				cake = q2.front();
				q2.pop();
				queue<string>copy2 = q2;
				str += " , popping Vanilla Cake ... ";
			pthread_mutex_unlock(&lock_queue_2);
	  	 	while (!copy2.empty())
		  	{
		    	s += copy2.front()  + " , ";
		    	copy2.pop();
		  	}
		  	print(s);
		  	//IF PRINTED LATER THEN INCONSISTENCY WOULD ARISE ... aage/pore/onek pore print hoito

		sem_post(&empty_q2);


	  	

		iter++;
		print("<TEST>WAITER Vanilla going to sleep for 2000sec");
		sleep(2000);
	}

	return 0;
}


int main(void)
{	
	pthread_t chefChocolate;	//Chef X
	pthread_t chefVanilla;		//Chef Y
	pthread_t intermediateChef;	//Chef Z
	
	pthread_t waiterChocolate;	//Waiter 1
	pthread_t waiterVanilla;	//Waiter 2
	
	init_semaphore();
	init_mutex();
	
    int chocolate = CHOCOLATE;
    int vanilla = VANILLA;


//Creating Chefs X and Y
	pthread_create(&chefChocolate,NULL,leftChefsFunc,&chocolate);
	pthread_create(&chefVanilla,NULL,leftChefsFunc,&vanilla);	


//Now Creating CHEF Z
	pthread_create(&intermediateChef, NULL, intermediateChefFunc, (void*)0);



//Finally creating waiters 1 and 2
	pthread_create(&waiterChocolate,NULL,waiterChocolateFunc,(void*)0);
	pthread_create(&waiterVanilla,NULL,waiterVanillaFunc,(void*)0);
	
	

	
	while(1); //JOIN CAN BE USED AS WELL BUT EITHER WAY, INFINITE LOOP WILL OCCUR SINCE EACH THREAD HAS INFINITE LOOP
	return 0;
}

















