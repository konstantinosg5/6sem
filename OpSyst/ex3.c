#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h> 
#include <fcntl.h>  // flags library
#include <stdlib.h> // exit
#include <sys/wait.h> //wait
#include <ctype.h>
#include <stdbool.h>
#include <poll.h>
#include <signal.h>
#include <time.h> //rand() 
#define BUFFER_SIZE 1024

int main (int argc, char **argv) {
   
    int round_robin_count = 0; //starts from first child
    bool random = false;
    bool round_robin = false; 

    if (argc == 2) {
        bool valid_integer = true; 
        for (int i=0; i<strlen(argv[1]); i++) {
            if (isdigit(argv[1][i]) == 0) {
                valid_integer = false;
                break;
            }
        }
        if (valid_integer == false) {
            printf("Usage: %s <nChildren> [--random] [--round-robin]\n", argv[0]);
            return 1;
        }
        else {
            round_robin = true;
        }  
    }
    else if (argc == 3) {
        bool valid_integer = true; 
        for (int i=0; i<strlen(argv[1]); i++) {
            if (isdigit(argv[1][i]) == 0) {
                valid_integer = false;
                break;
            }
        }
        if (valid_integer == true && strcmp(argv[2], "--round-robin")==0) {
            round_robin = true;
        }  
        else if (valid_integer == true && strcmp(argv[2], "--random")==0) {
            random = true;
        }
        else {
            printf("Usage: %s <nChildren> [--random] [--round-robin]\n", argv[0]);
            return 1; 
        }
    }
    else {                
        printf("Usage: %s <nChildren> [--random] [--round-robin]\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    int **parent_to_child;
    int **child_to_parent;

    parent_to_child = (int**) malloc(N * sizeof(int*));
    child_to_parent = (int**) malloc(N * sizeof(int*));

    if (parent_to_child == NULL||child_to_parent == NULL) {
        printf("Memory allocation failed!");
        return -1;
    }

    for (int i=0; i<N; i++) {
        parent_to_child[i] = (int*) malloc(2 * sizeof(int));
        child_to_parent[i] = (int*) malloc(2 * sizeof(int));

        if (parent_to_child[i] == NULL || child_to_parent[i] == NULL) {
            printf("Memory allocation failed!");
            return -1;
        }
    }
    for (int i=0; i<N; i++) {
        if (pipe(parent_to_child[i]) == -1 || pipe(child_to_parent[i]) == -1) {
            perror("Pipe failed!");
            exit(1);
        }
    }    
    
    pid_t child[N];
    int value;
    for(int i=0; i<N; i++) {
        child[i] = fork();
        if (child[i] < 0) { // error
            perror("Fork failed!");
            exit(1);
        }  
        if (child[i] == 0) {
            close(parent_to_child[i][1]); //child can only read, not write
            close(child_to_parent[i][0]); //child can only write, not read
            while(1) {
                if (read(parent_to_child[i][0], &value, sizeof(int)) == -1) { //size of integer is 4 bytes
                    perror("Read failed!");
                    exit(1);
                }
                printf("[Child %d] [%d] Child received %d!\n", i, getpid(), value);
                value--;
                sleep(10); 
                if (write(child_to_parent[i][1], &value, sizeof(int)) == -1) {
                    perror("Write failed!");
                    exit(1);
                }
                printf("[Child %d] [%d] Child finished hard work, writing back %d!\n", i, getpid(), value);
            }
            close(parent_to_child[i][0]); 
            close(child_to_parent[i][1]);
            return 0;
        }
        printf("[PARENT/PID=%d] Created child %d (PID=%d)!\n", getpid(), i, child[i] );
    }
    //parent's code
    for (int i=0; i<N; i++) {
        close(parent_to_child[i][0]); //parent can only write, not read
        close(child_to_parent[i][1]); //parent can only read, not write
    }

    struct pollfd fds[N+1];      // N+1 cause N children plus the terminal
    fds[N].fd = STDIN_FILENO;    // file descriptor for terminal ,its just 0, at the last place N
    fds[N].events = POLLIN;      // events , POLLIN = data is ready to read
    
    for(int i=0; i<N; i++){
    fds[i].fd = child_to_parent[i][0]; // Read end of the pipe
    fds[i].events = POLLIN;
    }

    char buffer[BUFFER_SIZE];

    while (1) {
    
        if ( poll(fds, N+1, -1) == -1) {
            perror("Poll failed!");
            exit(1);
        }

        if (fds[N].revents & POLLIN) { // Data available on terminal
            // Read from terminal (stdin)
            int terminal_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (terminal_read == -1) {
                perror("Read failed!");
                exit(1);
            }

            buffer[terminal_read] = '\0'; //null terminate buffer
            bool number_in_terminal = true;
            int j = 0;

            while (buffer[j] != '\n') {
                if (isdigit(buffer[j]) == 0) {
                    number_in_terminal = false;
                    break;
                }
                else {
                    j = j + 1;
                }
            }
            if (strcmp(buffer, "help\n") == 0 ) {
                printf("Type a number to send a job to a child!\n");
            }
            else if (strcmp(buffer, "exit\n") == 0) { //terminate all children & self
                int status;
                for(int i=0; i<N; i++) { //loop for each child  
                    printf("[PARENT/PID=%d] waiting for %d children to terminate!\n", getpid(), N-i);
                    if (kill(child[i], SIGTERM)==-1)
                        printf("TERM FAILED on child %d!", i); 
                    if (waitpid(child[i], &status, 0) == -1) { //suspends execution of the calling process until a specified child has changed state
                        perror("waitpid");
                        exit(EXIT_FAILURE);
                    }
                    else if (WIFEXITED(status) == 0) { //option of waitpid which can be used to know the exit status of the child, returns true if child terminated normally
                        printf("[PARENT/PID=%d] Child with PID=%d terminated successfully, with exit code %d.\n", getpid(), child[i], WEXITSTATUS(status));
                    }
                    else {
                        printf("[PARENT/PID=%d] Child with PID=%d terminated with problems! with exit code %d.\n", getpid(), child[i], WEXITSTATUS(status));
                    }
                }
                //Father terminating as well
                printf("[PARENT/PID=%d] All children terminated successfully! Terminating as well.\n", getpid());
                for (int i=0; i<N; i++) {
                    close(parent_to_child[i][1]); 
                    close(child_to_parent[i][0]); 
                }
                for (int i=0; i<N; i++){// free memory
                    free(parent_to_child[i]);
                    free(child_to_parent[i]);
                    }     

                    free(child_to_parent);
                    free(parent_to_child);
                
                exit(0);
            }
            else if (number_in_terminal == true) { //user has entered an integer
                value = atoi(buffer);
                if (random == true) {
                    srand(time(0)); 
                    int random_child = rand() % N; //0 < random_child < N-1
                    if (write(parent_to_child[random_child][1], &value, sizeof(int)) == -1) {
                        perror("Write failed!");
                        return 1;
                    }
                    printf("[Parent] [%d] Assigned %d to child %d\n", getpid(), value, random_child);
                }
                else if (round_robin == true) {
                    if (round_robin_count < N) {
                        if (write(parent_to_child[round_robin_count][1], &value, sizeof(int)) == -1) {
                            perror("Write failed!");
                            return 1;
                        }
                        printf("[Parent] [%d] Assigned %d to child %d\n", getpid(), value, round_robin_count);
                        
                        round_robin_count = round_robin_count + 1;
                    }
                    if (round_robin_count == N) { //circle reset
                        round_robin_count = 0;
                    }
                }
            }
            else {
                printf("Type a number to send job to a child!\n");
            }
        }

        for(int i=0; i<N; i++){// checking for data in any children's pipe
            if (fds[i].revents & POLLIN){
                if (read(fds[i].fd, &value, sizeof(int)) == -1) {
                    perror("Read failed!");
                    exit(1);
                } 

                printf("[Parent] [%d] Received %d from child %d\n", getpid(), value, i);
               
            }

        }
    }
    return 0;
}
 
