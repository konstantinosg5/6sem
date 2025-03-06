import sys


with open(sys.argv[1], 'r') as file:
    N = int(file.readline().strip())
    seq = list(map(int,file.readline().strip().split()))

TotalSum=sum(seq,0)
best=TotalSum
SubSum=0

for i in range(0,N):
    for j in range(i,N):
        SubSum=seq[j]+SubSum        
         
        cache=abs(TotalSum - 2*SubSum )  
        if( cache < best): best = cache 
        if(best==0):break; break

    SubSum=0

print(best)