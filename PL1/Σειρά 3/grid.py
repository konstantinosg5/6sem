import sys
from collections import deque

with open(sys.argv[1], 'r') as file:
    N = int(file.readline().strip())
    grid =[0]*N
    for i in range(0,N):
        grid[i] = list(map(int,file.readline().strip().split()))

def next(y,x,d):
    value=grid[y][x]
    if (y-1>=0 and value>grid[y-1][x] ):yield y-1, x, 'N'
    if (y+1<N and value>grid[y+1][x] ):yield y+1, x, 'S'
    if (x-1>=0 and value>grid[y][x-1]):yield y, x-1, 'W'
    if (x+1<N and value>grid[y][x+1]):yield y, x+1, 'E'
  
    if (y+1<N and x+1<N and value>grid[y+1][x+1]):yield y+1, x+1, 'SE'
    if (y-1>=0 and x+1<N and value>grid[y-1][x+1]):yield y-1, x+1, 'NE'
    if (y+1<N and x-1>=0 and value>grid[y+1][x-1]):yield y+1, x-1, 'SW'
    if (y-1>=0 and x-1>=0 and value>grid[y-1][x-1]):yield y-1, x-1, 'NW'



moves = deque ([(0, 0, None)])
prev = {(0, 0, None): (None,None, None)}
solved=False

while moves:
    s = moves.popleft()
    #print("/",*s)
    if s[0]==N-1 and s[1]==N-1:
       # print("here")
        solved=True
        break
   
    for m in next(*s):
       # print('%',*s,end=' ')
        if m not in prev:
            #print('#',m)
            moves.append(m)
            prev[m] = s

result=deque()
if solved:
    while s[2]:
        result.append(s[2])
        s=prev[s]
        
    print('[',end='')
    while result:
        print(result.pop(), end = '')
        if result:print(',',end='')
    print(']')
else: print("IMPOSSIBLE")
