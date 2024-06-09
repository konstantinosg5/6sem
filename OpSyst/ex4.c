#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h> 

#include <fcntl.h>  // flags library
#include <stdlib.h> // exit
#include <sys/wait.h> //wait
#include <signal.h> //signals

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <time.h>

#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

//================================    MAIN    ========================================//

int main(int argc, char **argv){
// Defaults
int PORT=20241; 
char HOST[]="os4.iot.dslab.ds.open-cloud.xyz";
int helpFlag=0,DebugFlag=0;

//=======   proccess to check and send error messages on input
for (int i=1; i<argc; i++){
    if (!strcmp(argv[i],"--help")) {helpFlag=1; break;}
    if (!strcmp(argv[i],"--host")) {strcpy(HOST,argv[i+1]);continue; continue; }
    if (!strcmp(argv[i],"--port")) {PORT=atoi(argv[i+1]);continue; continue; }
    if (!strcmp(argv[i],"--debug")) {DebugFlag=1;continue; }
}

if ((helpFlag) || (argc>6)){
    printf("Usage: %s [--host HOST] [--port PORT] [--debug]\n",argv[0]);
    return 0;
}

int pid = getpid();

// ====================== connecting to server
int sd = socket(AF_INET,SOCK_STREAM,0);
if (sd < 0) {
    perror("socket");
    return 1;
}

struct sockaddr_in sin; 
struct hostent *hostp;

hostp = gethostbyname(HOST); 
sin.sin_family = AF_INET;
sin.sin_port = htons(PORT);
sin.sin_addr.s_addr =htonl (INADDR_ANY);

bcopy(hostp->h_addr_list[0] , &sin.sin_addr, hostp->h_length);
//bind(sd,(struct sockaddr *) &sin, sizeof(sin));

if (connect(sd, (struct sockaddr *) &sin, sizeof(sin)) < 0){
   perror("connect");
   return 1;
}

//============================= CONNECTED
fd_set read_set;
char buf[1024];

while(1){ 
    FD_ZERO(&read_set);  //We initialize 
    FD_SET(0,&read_set); //Terminal
    FD_SET(sd,&read_set);//socket
     
    if (select((sd + 1), &read_set, NULL, NULL, NULL) < 0){
        perror("select");
        return 1;
    }

    if (FD_ISSET(0, &read_set)){ // data from terminal
        if (fgets(buf, sizeof(buf), stdin) == NULL){
            perror("fgets");
            return 1;
        }

        if(!strcmp(buf,"exit\n")){
            printf(MAGENTA "Ending connection..." WHITE "\n");
            close(sd);
            exit(0);
        }   

        

        if(!strcmp(buf,"help\n")){
            printf(MAGENTA "Type:'exit' to exit \n     'get' to recover server data\n     'N name surname reason' permission to leave home" WHITE "\n");
            continue;
        }        

        if(write(sd,&buf,strlen(buf) + 1) == -1) {// sending to server
            perror("write");                      // get data
            return 1;                             // N name surname reason
        }                                         // verification code
                                              
        
        if(DebugFlag){    
            int i = 0;
            while(buf[i] != '\n') {++i;}//We remove \n to print '%s' without changing line
            
            buf[i] = '\0';
            printf(RED "[DEBUG] Sent '%s'" WHITE "\n",buf); 
                  
        }
    

    }
    else if (FD_ISSET(sd, &read_set)){ //data from server
        int nbytes=read(sd,&buf,1023);

        if( nbytes == -1 ){
            perror("read");
            return 1;
        }

        buf[nbytes-1] = '\0';  // last

        if(DebugFlag) printf(RED "[DEBUG] read '%s'" WHITE "\n",buf);

        if(!strcmp(buf,"try again")){
            printf(GREEN "%s" WHITE "\n",buf);
            continue;
        }

        if(buf[1] == ' '){  // X YYY ZZZZ WWWWWWWWWW     
            int  light, temp, flag, timecache;
            char* token;

            token = strtok(buf, " ");  //Here we separate the X YYY ZZZZ and WWWWWWWWWW strings to print them
            flag = atoi(token);

            token = strtok(NULL, " ");
            light = atoi(token);

            token = strtok(NULL, " ");
            temp = atoi(token);
            double temperature = ((double)temp)/100;

            token = strtok(NULL, " ");
            timecache = atoi(token);
                    
            time_t senttime = timecache;
            struct tm *info;
            time( &senttime );
            info = localtime( &senttime );


            char message[12];
            switch (flag){
                case 0:
                    strcpy(message,"boot");
                    break;
                case 1:
                    strcpy(message,"setup");
                    break;
                case 2:
                    strcpy(message,"interval");
                    break;
                case 3:
                    strcpy(message,"button");
                    break;
                case 4:
                    strcpy(message,"motion");
                    break;
                default:
                    strcpy(message,"what?");
                    break;
            }

            printf(GREEN "---------------------------\n");
            printf("Latest event:\n");
            printf("%s (%d)\n", message, flag);
            printf("Temperature is: %.2f\n",temperature);
            printf("Light level is: %d\n",light);
            printf("Timestamp is: %s", asctime(info));
            printf("---------------------------" WHITE"\n");
        }
        else if(buf[0] == 'A' && buf[1] == 'C' && buf[2] == 'K' ){ // 'ACK' reply 
            printf(GREEN "Response: %s" WHITE "\n",buf);
        }
        else
            printf(GREEN "Send verification code : %s" WHITE "\n",buf);


    }

}

}//end of main
