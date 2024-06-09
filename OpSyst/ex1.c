#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 

//for checking if file input is ok
#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type

#include <fcntl.h>  // flags library
#include <stdlib.h> // exit
#include <sys/wait.h> //wait


//funcion that tells if file exists
bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

//================================    MAIN    ========================================//
int main(int argc, char **argv){

//=======   proccess to check and send error messages
if (argc != 2){                //only one output program
    printf("Usage: %s filename \n ",argv[0]);
    return 1;
    }

if (file_exists(argv[1])) {    // already existing file
    printf("Error: %s already exists \n", argv[1]);
    return 1;
    }

if (strcmp(argv[1],"--help")==0 ) {// asks for help
    printf("Usage: %s filename \n", argv[0]);
    return 0;
    }

int fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY, 0644);//File Descriptor

if (fd == -1){//doesnt open
        perror("open");
        return 1;
    }

// Creating variables
int status;
pid_t child;
   
//======================      Child Birth (fork)     ============================//
child = fork();

if (child < 0){ // error
    printf("Child failed to open!");
    return 1;
    }

if (child == 0){// ====================== child’s code  
     pid_t pid = getpid() ;
     pid_t ppid = getppid() ;
     char buf[64];

     sprintf(buf,"[CHILD] getpid()= %d, getppid()= %d\n",pid, ppid);
     

    if (write(fd, buf, strlen(buf)) < strlen(buf)){//ελεγχος buf
            perror("write error");
            return 1;
            }  
    exit(0);
    }
    else{ //============================= father’s code
        wait(&status);//περιμενει το παιδι να πεθανει 
        
        pid_t pid = getpid() ;
        pid_t ppid = getppid() ;
        char buf[64];
       
        sprintf(buf,"[PARENT] getpid()= %d, getppid()= %d\n",pid, ppid);


        if (write(fd, buf, strlen(buf)) < strlen(buf)){//ελεγχος buf
            perror("write error");
            return 1;
            }

        close(fd);
        if(close<0){
            printf("closing fie failed");
        }

        exit(0);
        }

return 0;
}
