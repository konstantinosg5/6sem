(* Aπο το αρχειο countries στο site του μαθηματος, με καποιες ψιλοαλλαγες για να κανει
Input parse code by Stavros Aronis, modified by Nick Korasidis. *)
fun parse file =
  let
	
    fun readInt input = 
	 	Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

	(* Open input file. *)
    val inStream = TextIO.openIn file

  (* Read an integer (number of countries) and consume newline. *)
	 val n = readInt inStream
   val _ = TextIO.inputLine inStream

  (* A function to read N integers from the open file. *)
  	fun readInts 0 acc = acc (* Replace with 'rev acc' for proper order. *)
	    | readInts i acc = readInts (i - 1) (readInt inStream :: acc)
  in
   	(readInts n []) (*list of seq *)
  end
(*Η ιδεα ειναι η εξης , δεν με νοιαζουν τα στοιχεια τις σειρας μονο το αθροισμα τους. Οποτε, ξεκιανω να σαρωνω
  την σειρα και να προσθετω στοιχεια σε εναν accumulator sum, μεχρι να βρω το καλυτερο αποτελεσμα. Οταν φτασω 
  στο προτελευταιο στοιχειο σταματαω. Μετα ξανα σαρωνω απο το επομενο στοιχειο . Ολο μοιαζει καπως ετσι:
  1 17 5 2 42 (input)
  1        sum= (1)     to best =totalsum - 2*sum
    17          (18)   
       5        (23)
	     2      (25)
	17          (17)
	   5        (23)
	     2      (25)
	   5         (5)
	     2       (7)
		 2       (2)
    *)
  

fun solve (seq) = 
    let
       fun min (a,b) =
            if a<b then a
            else b;
   
       val totalSum = foldl (op +) 0 seq    (*total sum of starting seq*)
   
       fun subSeqbest ([t] , sum , best) = best      
        |  subSeqbest (prevseq, sum, best) = 
			let
			    val newsum = hd prevseq + sum 
			    val newbest = min(abs(totalSum - 2*newsum), best)
			    val nextseq = tl prevseq
			in
               (* print(Int.toString(newbest)^" "); *)
			   subSeqbest( nextseq , newsum , newbest) 
		    end
          
       fun nextSubSeq ([t], best) = best
	    |  nextSubSeq (subseq, 0 ) = 0
	    |  nextSubSeq (subseq, best) = min (nextSubSeq (tl subseq,subSeqbest(subseq,0,totalSum)),best)
		
		 
        
  in
    print(Int.toString(nextSubSeq(seq,totalSum))^"\n")
  end


fun fairseq fileName = solve (parse fileName) 

