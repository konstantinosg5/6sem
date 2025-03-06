fun parse file =
    let
        fun readInt input = 
        Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

        fun twodim input = 
        Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

        val inStream = TextIO.openIn file

        val n = (readInt inStream)

        val _ = TextIO.inputLine inStream
          
        (* readints left nextitem acc *)
        fun readInts 0 acc = rev acc 
          | readInts i acc = readInts (i - 1)  (readInt inStream :: acc)
          
        fun twodim 0 acc = rev acc
          | twodim i acc = twodim (i-1) (readInts n []  :: acc)
          

    in
        (n, twodim n [])
    end

fun get (grid, y, x) = 
    let 
       val row = List.nth(grid, y)
       val element = List.nth(row, x)    
    in
       element
    end


fun test file =
let
val (n,grid)=parse file
val goal = n-1

fun solve (y, x, moves) = 
    let
        fun safemove (y, x) =
            let
                fun nextmove (y, x) =
                    let
                       val currentvalue = get (grid, y, x)
                       val possible = ["E", "W", "S", "N", "SE", "SW", "NE", "NW"]
                       fun next y x "E" = (y, x+1, currentvalue)
                         | next y x "W" = (y, x-1, currentvalue)
                         | next y x "S" = (y+1, x, currentvalue)
                         | next y x "N" = (y-1, x, currentvalue)
                         | next y x "SE" = (y+1, x+1, currentvalue)
                         | next y x "SW" = (y+1, x-1, currentvalue)
                         | next y x "NE" = (y-1, x+1, currentvalue)
                         | next y x "NW" = (y-1, x-1, currentvalue)
                         | next y x _ = (~1,~1,~1) 
                
                        fun nextaux direction =  next y x direction
                    in
                       map nextaux possible
                    end
        
                fun safe x y n currentvalue grid =
                    if x>=0 andalso y>=0 andalso x<n andalso y<n andalso currentvalue>get(grid,y,x) then [y, x]
                    else []
                
                fun safeaux (x, y, currentvalue) = safe x y n currentvalue grid
        
                val listpossible=nextmove(y,x)
            in
                map safeaux listpossible
            end
    
            val safe = safemove(y, x)
    
            fun keep [[yn,xn], _, _, _, _, _, _, _]= solve(yn,xn,"E"::moves) 
              | keep [ _, [yn,xn], _, _, _, _, _, _]= solve(yn,xn,"W"::moves) 
              | keep [ _, _, [yn,xn], _, _, _, _, _]= solve(yn,xn,"S"::moves) 
              | keep [ _, _, _, [yn,xn], _, _, _, _]= solve(yn,xn,"N"::moves) 
              | keep [ _, _, _, _, [yn,xn], _, _, _]= solve(yn,xn,"SE"::moves) 
              | keep [ _, _, _, _, _, [yn,xn], _, _]= solve(yn,xn,"SW"::moves) 
              | keep [ _, _, _, _, _, _, [yn,xn], _]= solve(yn,xn,"NE"::moves) 
              | keep [ _, _, _, _, _, _, _, [yn,xn]]= solve(yn,xn,"NW"::moves)
              | keep [ _, _, _, _, _, _, _, _]= [~1]
              | keep [[], [], [], [], [], [], [], []] = []
    in        
      if keep safe=[~1] then ["IMPOSSIBLE"]
      else rev moves

    end


in
    solve (0,0,["moves"])
end


(*
fun solve grid y x acc = 
    let

    in
    end
  | solve grid n n acc = rev acc
*) 