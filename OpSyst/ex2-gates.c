#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 

#include <fcntl.h>  // flags library
#include <stdlib.h> // exit
#include <sys/wait.h> //wait
#include <signal.h> //signals

int N,*childs, pid, termination=0;

void USR1_Handler(){// send SIGUSR1 to all childs  // ===================================

printf("\033[35m [PARENT/PID=%d] sending USR1 to all childs! \033[0m \n",pid);
for(int i=0; i<N; i++){
  if(kill(childs[i],SIGUSR1)==-1) printf("USR1 FAILED !");
  }
}


void USR2_Handler(){// send SIGUSR2 to all childs  // ===================================

printf("\033[35m [PARENT/PID=%d] sending USR2 to all childs! \033[0m \n",pid);
for(int i=0; i<N; i++){
  if(kill(childs[i],SIGUSR2)==-1) printf("USR2 FAILED !");

  }
}


void terminator (){// kill every child and suicide // =========================================
int status;
termination=1;

for(int i=0; i<N; i++){// loop for each child  
printf("\033[35m [PARENT/PID=%d] waiting for %d children to terminate! \033[0m \n",pid,N-i);//remaining children

if(kill(childs[i],SIGTERM)==-1)
     printf("TERM FAILED on child %d !",i);//give signal

if (waitpid(childs[i],&status,0)==-1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }else if(WIFEXITED(status)==0){
        printf("\033[35m [PARENT/PID=%d] Child with PID=%d terminated successfully, with exit code %d. \033[0m \n",pid,childs[i],WEXITSTATUS(status));
        }else  printf("\033[35m [PARENT/PID=%d] Child with PID=%d terminated with problems! with exit code %d. \033[0m \n",pid,childs[i],WEXITSTATUS(status));
    
}

// Father terminating as well
printf ( "\033[35m [PARENT/PID=%d] All children terminated successfully! Terminating as well.\033[0m \n",pid);
exit(0);
}


//================================    MAIN    ========================================//
int main(int argc, char **argv){

//=======   proccess to check and send error messages
if (argc != 2){                //only one output program
    printf("Usage: %s series of N t & f \n ",argv[0]);
    return 1;
    }

if (strcmp(argv[1],"--help")==0 ) {// asks for help
    printf("Usage: %s series of N t & f \n", argv[0]);
    return 0;
    }

N = strlen(argv[1]);//Number of children
int state[N];

for (int i=0; i<N; i++){// checking if correct input series of t f
    if (argv[1][i]=='f' || argv[1][i]=='t'){ 
        if (argv[1][i]=='f') state[i]=0;
            else state[i]=1;
    }
        else { printf("ERROR:only t or f can be used as states, not %c!\n",argv[1][i]);
               return 1;
             }
}

// Creating variables
int status ;
pid_t child[N];
pid=getpid();
   
//======================      Child Birth (fork)     ============================//

for(int i=0; i<N; i++){
    child[i] = fork();

    if (child[i] < 0){ // error
        printf("Child %d failed to open!\n",i);
        return 1;
    }  
     
    if (child[i] == 0){// ====================== child’s code          
       char buf1[5], buf2[5];      

       sprintf(buf1,"%c", argv[1][i]); // state
       sprintf(buf2,"%d", i);  // id
       
       char *args[]={"./child",buf1, buf2 ,NULL} ;// USAGE:./child t/f id

        if(execv(args[0],args)== -1) {
            printf("ERROR: EXECV failed!");
            return 1;
        }

    }
//first anouncement of birth   
printf("\033[35m [PARENT/PID=%d] Created child %d (PID=%d) and initial state ",pid, i, child[i] );
    if(state[i]==1) printf("open! \033[0m \n");//35m Μοβ                   
        else printf("closed! \033[0m \n");//35m Μοβ

}
childs=child;

//============================= father’s code
signal(SIGUSR1,USR1_Handler);
signal(SIGUSR2,USR2_Handler);
signal(SIGTERM,terminator);

int w;
while(!termination){
w = waitpid(-1, &status, WUNTRACED);
if (w == -1) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }

if ((WIFEXITED(status)||WIFSIGNALED(status))) {
      printf("\033[35m [PARENT/PID=%d] OH! My child (PID=%d) died! (exit status %d)  \n ", pid, w, WEXITSTATUS(status));
      printf("Let's have another one...\033[0m \n");
         
         int id;// find which id
         for(int i=0; i<N; i++){
             if (child[i]==w){
                 id=i; 
                 break;
             }
         }
         child[id] = fork();
     
         if (child[id] < 0){ // error
             printf("Child %d failed to open!\n",id);
             return 1;
         }  
          
         if (child[id] == 0){// ====================== child’s code 
          char buf1[5], buf2[5];
          sprintf(buf1,"%c", state[id]); 
          sprintf(buf2,"%d", id); 
            
            char *args[]={"./child",buf1, buf2 ,NULL} ;// USAGE:./child t/f id
     
             if(execv(args[0],args)== -1) {
                 printf("ERROR: EXECV failed!");
                 return 1;
             }
     
          }
          //first anouncement of birth   
          printf("\033[35m [PARENT/PID=%d] Created child %d (PID=%d) and initial state ",pid, id, child[id] );
              if(state[id]==1) printf("open! \033[0m \n");//35m Μοβ                   
                  else printf("closed! \033[0m \n");//35m Μοβ
           
     
     
            
    } else if (WIFSTOPPED(status)) {
        printf("\033[35m [PARENT/PID=%d] Someone stopped my child (PID=%d)! (signal %d)\n",pid,w, WSTOPSIG(status));
        printf(" Let's resume it...\033[0m \n");
        kill(w,SIGCONT);
      } 
    
}


}
