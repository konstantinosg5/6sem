
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 

#include <fcntl.h>  // flags library
#include <stdlib.h> // exit
#include <sys/wait.h> //wait
#include "signal.h" //signals
#include <time.h> 

//global variables
int  pid, ppid, state ;
time_t start_t; 

char  *id;

long int getRunTime(){
 time_t running;
  
 return   time(&running)-start_t;
}

//==============
void AlarmHandler (){// Alarm every 15 sec  
  if (state==1) 
     printf("\033[32m [ID=%s/PID=%d/TIME=%lds] The gates are open! \033[0m \n",id,pid, getRunTime());
     else printf("\033[31m [ID=%s/PID=%d/TIME=%lds] The gates are closed! \033[0m \n",id,pid, getRunTime());

  alarm(15);
}

//==============
void USR1_Handler (){ // user want info 
if (state==1) 
    printf("\033[33m [ID=%s/PID=%d/TIME=%lds] The gates are open! \033[0m \n",id,pid, getRunTime());
    else printf("\033[33m [ID=%s/PID=%d/TIME=%lds] The gates are closed! \033[0m\n",id,pid, getRunTime());

}

//================
void USR2_Handler (){  // state changer
   if (state == 0 )state = 1;
       else state = 0;
}

 // ==================================   MAIN   ========================================== //
int main(int argc, char **argv){

//initializing ids
ppid = getppid();
pid = getpid() ;
id = argv[2];//stored as char

time(&start_t);

if (argv[1][0]=='t')  state=1;// open;
    else state = 0; // closed



//starting with signal handling 
signal(SIGALRM, AlarmHandler);// sigAlarm
signal(SIGUSR1, USR1_Handler );// give runtime and state
signal(SIGUSR2, USR2_Handler );// state changer

AlarmHandler();


 while (1){pause();}// just waiting for signals

return 0;

}


 /*     
struct sigaction action = { 0 }; //δήλωση δομής struct sigaction
action.sa_handler =AlarmHandler(id,pid,timer,state); //ορισμός ονόματος συνάρτησης χειρισμού

sigemptyset(&action.sa_mask);
action.sa_flags = 0; //ορισμός ονόματος συνάρτησης χειρισμού

*/
