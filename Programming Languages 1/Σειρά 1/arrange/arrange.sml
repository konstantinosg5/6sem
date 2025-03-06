datatype 'a BinaryTree = Empty | Node of 'a * 'a BinaryTree * 'a BinaryTree;

fun insert [] = (Empty, [])
  | insert (0::xs) = (Empty, xs)
  | insert (x::xs) =
    let
        val (left, xs1) = insert xs
        val (right, xs2) = insert xs1
    in
        (Node(x, left, right), xs2)
    end

fun parse file =
    let
        fun readInt input = 
        Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

        val inStream = TextIO.openIn file

        val n = (2*readInt inStream+1 )

        val _ = TextIO.inputLine inStream
          
        (* readints left nextitem acc *)
        fun readInts 0 acc = rev acc 
          | readInts i acc = readInts (i - 1)  (readInt inStream :: acc)
         
    in
        (readInts n [])
    end


fun test file = 
    let
       val myTree = insert (parse file);
    in
       myTree
    end


fun traverse (Node(value, Empty , Empty)) = (Node(value, Empty, Empty), value)  
  | traverse (Node(value, left, Empty)) = 
        let
            val (treel, datal) = traverse(left)
        in
            if value > datal then (Node(value, treel,Empty), datal)
            else (Node(value, Empty, treel ),value) 
        end
      
  | traverse (Node(value, Empty , right)) = 
        let 
            val (treer,datar) = traverse(right)
        in
            if value < datar then (Node(value,Empty,treer), value)
            else (Node(value, treer, Empty ),datar) 
        end
  | traverse (Node(value, left, right)) = 
        let
            val (treel,datal) =traverse(left)
            val (treer,datar) =traverse(right)
        in
            if datal<datar then (Node(value,treel,treer),datal)
            else (Node(value, treer, treel),datar)
        end

fun printInOrder (Node(value, left, right)) = ( 
       printInOrder(left);
       print(Int.toString(value)^" ");
       printInOrder(right)
    ) 
  | printInOrder (Empty) =  () 
    

fun arrange (file) =
    let
       val (tree , waste) = insert(parse(file))
       val (Solution ,waste2) = traverse (tree)
    in
       printInOrder(Solution);
       print("\n")
    end

 
