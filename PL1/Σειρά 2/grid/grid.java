import java . io .*;
import java.util.*;

class GridState {
// represent all four entities as booleans where false = west, true = eastprivate 
int x, y, N;
private int[][] grid = new int[N][N];
// link to the parent state
private GridState previous;

/*
 * Goat state constructor.
 */
public GridState(int xc ,int yc, GridState prev) {
    x=xc; y=yc;
    previous = prev;
    grid=prev.grid;
    N=prev.N;
    
}

/*
 * Goat state constructor for the initial state only, using constructor overloading.
 */
public GridState(int size,int[][] gr) {
    x=0; y=0; N=size;
    grid=gr;
   
    previous = null;
}

/*
 * A state is final if all four entities are on the east side.
 */
public boolean isFinal() {
    return (x==N-1) && (y==N-1) ;
}

/*
 * Return all possible next state. May contain invalid states.
 */
public Collection<GridState> next() {
    Collection<GridState> states = new ArrayList<>();
        
    // add all possible moves to the solution
    if(x-1>=0) states.add(new GridState(x-1,y, this));//left
    // System.out.print("#"+x+" "+y);
    if(y-1>=0) states.add(new GridState(x,y-1, this));//up
    
    if (x-1>=0 && y-1>=0) states.add(new GridState(x-1,y-1, this));//up left
 
    if(y+1<N) states.add(new GridState(x,y+1, this));//down
    if(x+1<N) states.add(new GridState(x+1,y, this));//right
    if((x+1<N) && (y+1<N)) states.add(new GridState(x+1,y+1, this));//down right
     
    if(x+1<N && y-1>=0) states.add(new GridState(x+1,y-1, this));//right up
    if(x-1>=0 && y+1<N) states.add(new GridState(x-1,y+1, this));//left down

    return states;
}

/*
 * Return the parent state.
 */
public GridState getPrevious() {
    return previous;
}

public int getTraffic() {
    return grid[x][y];
}

/*
 * A state is valid if the wold and the goat are not on the same side unattended (i.e. the man is not on the same side as them)
 * and if the goat and the cabbage are not on the same side unattended.
 */
public boolean isValid() {
   // System.out.print(getPrevious().getTraffic()+ ">"+grid[x][y]);
   return getPrevious().getTraffic() > grid[x][y];
}

/*
 * Override toSting to print a more informative representation.
 */
@Override
public String toString() {
   StringBuilder sb = new StringBuilder("");

    if(x<getPrevious().x && y<getPrevious().y) sb.append("NW");
    else if (x<getPrevious().x && y>getPrevious().y) sb.append("SW");
         else if (x<getPrevious().x ) sb.append("W");

    if(x>getPrevious().x && y>getPrevious().y) sb.append("SE");
    else if (x>getPrevious().x && y<getPrevious().y) sb.append("NE");
         else if (x>getPrevious().x ) sb.append("E");

    if(x==getPrevious().x && y>getPrevious().y) sb.append("S");
    else if (x==getPrevious().x && y<getPrevious().y) sb.append("N");
        
   return sb.toString();
}

/*
 * Override equals so that two states are equal iff the four entities have the same positions.
 */
@Override
public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    GridState s = (GridState) o;
    return (s.x == x && s.y == y && s.grid[s.x][s.y]==grid[x][y]);
}

/*
 * Override hashCode so that if s1.equals(s2) then s1 and s2 have the same hashes.
 */
@Override
public int hashCode() {
    return Objects.hash(x, y,grid[x][y]);
}

}
//==========================================================================================
class Solver {
    public GridState solve(GridState init) {
        Queue<GridState> remaining = new ArrayDeque<>();
        Set<GridState> visited = new HashSet<>();
        // add initial state to the queue
        remaining.add(init);
        // add initial state to the visited set
        visited.add(init);

        // while the queue is not empty ...
        while (!remaining.isEmpty()) {
            // ... explore the next state
            GridState s = remaining.remove();
            //System.out.println(s.x+" "+s.y);
            // if final state is found, return
            if (s.isFinal()) return s;
            for (GridState n : s.next()) {
               // System.out.print("|"+n.x+" "+n.y+"|");
                // if a next state is valid and not visited ...
                if (!visited.contains(n) && n.isValid()){
                   // System.out.print("|"+n.x+" "+n.y+" ");
                    /// ... add it to the queue and mark it as visitied
                    remaining.add(n);
                    visited.add(n);
                }
            }//System.out.println();
        }
        // no solution found
        return null;
    }
}


//==============================================================================================
public class grid {
public static void main(String args[]) {
try { 
    BufferedReader reader =
        new BufferedReader (new FileReader (args[0]));
    String line = null ;
    int N;
    line = reader.readLine () ;
    N=Integer.parseInt (line) ;
    
    int [][] grid = new int[N][N];

    for (int i=0; i<N; i++ ){
        line = reader.readLine () ; 
        for (int j=0; j<N; j++ ){
            String[] numbersStr = line.split("\\s+");
            grid[j][i] = Integer.parseInt(numbersStr[j]);
        }
    }

    reader.close();

/*     for (int i=N-1; i>=0; i-- ){
        for (int j=0; j<N; j++ ){
            System.out.print(grid[j][i] + " ");
        }
        System.out.println();
    }*/

   GridState initial = new GridState(N,grid);
   Solver solver = new Solver();
   GridState result = solver.solve(initial);


if (result == null) {
    System.out.println("IMPOSSIBLE");
} else {
    System.out.print("[");
    printSolution(result);
    System.out.println("]");
}

} catch ( Exception e ) { // Bad practice !
    System.out.println ( "Something went wrong: " + e ) ;
}
}//main  

/*
 * Follow the previous links to recursively print the solution
 */
private static void printSolution(GridState s) {
    if (s.getPrevious().getPrevious() != null) {
        
        printSolution(s.getPrevious());
        System.out.print(",");
    }
    System.out.print(s);
}


}//class main