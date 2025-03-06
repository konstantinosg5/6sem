// **************************************************************************
//   Project     : Programming Languages 1 - Assignment 1 - Exercise 1 (fairseq)
//   Author      : Gkavogiannis Konstantinos (konstantinosg5@gmail.com)
//   Date        : 18 april 2024
//   Description : kinda good solution O(3n)
//   -----------
//   School of ECE, National Technical University of Athens.
// 
#include "iostream"
#include <cstdlib>
#include <string.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv){
int N, *seq , TotalSum=0, SubSum=0 , best=0, cache;

//  Reading input
ifstream file;

file.open(argv[1]);

if (!file.is_open())
{
    std::cerr << "Error opening file" << std::endl;
    return 1;
}

file >> N;

seq = (int*) malloc(N * sizeof(int));

for (int i=0; i<N; i++){
  file >> seq[i] ; 
  TotalSum+=seq[i];
}

file.close();

best=TotalSum;
//  Starting proccesing

for (int i=0; i <N; i++){//  Pick starting point i
    
    for (int j=i; j<N; j++){// Pick ending point j
    // Print subarray between current starting and ending points

    //cout << seq[j] << " ";
    SubSum+=seq[j];          
  
    cache=abs(TotalSum - 2*SubSum );   
    if( cache < best) best = cache;  
    if(best==0){break;break;}//Δεν υπαρχει καλυτερο!
    //printf("(%d) ",cache);
           
  }
//printf("\n");
SubSum=0;
}
    
printf("%d\n",best);

free(seq);
return 0;
}
