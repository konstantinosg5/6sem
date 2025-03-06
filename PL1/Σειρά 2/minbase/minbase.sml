fun minbases input =
    let
        fun base x =(*AΠΟΤΥΧΙΑ ,δουλευει για μικρα test cases, αλλα σκαει επικα στα μεγαλα*)
            let          
                fun checkbase (x, b, k, d,p) = (*αριθμος εισοδου, βαση, ψηφια, τιμη ψηφιων, δυναμη που θα χρησιμοποιησω *)
                    let
                        val n=Real.trunc(Real.fromInt(d)*(Real.fromInt( p-1)/Real.fromInt(b-1)))                                      
                    in
                       (* print(Int.toString(n)^" "); *)
                        if x=n then b 
                        else if(x>n andalso d<b) then checkbase (x, b, k, d+1, p) (*Aυξανω τιμες*)
                            else if(x>n) then checkbase (x, b, k+1, 1, p*b) (*Aυξανω ψηφια*)
                                 else checkbase ( x, b+1, 1, 1, b+1)  (*αλλαζω base*)                   
                    end
            in
                checkbase (x, 2, 1, 1, 2)
            end

        fun base2 x = (*PLAN B , που θα επρεπε μαλλον να ειναι Α*)
            let          
                fun checkbase (x, ~1, b, ~1) =(*αριθμος εισοδου,result, βαση, oldremain | αυτος ο κλαδος ειναι για εκκινηση*)
                    let
                        val remain = x mod b 
                        val result = Real.trunc(Real.fromInt(x)/Real.fromInt(b))                                    
                    in
                       (* print(Int.toString(x)^" "^Int.toString(result)^" "^Int.toString(b)^" "^Int.toString(remain)^"$");*) 
                        if result=0 andalso remain=0 then b
                        else checkbase (x,result, b, remain) (*Aυξανω τιμες*)
                    end
                  | checkbase (x, res, b, oldremain) = 
                    let
                        val remain = res mod b 
                        val result = Real.trunc(Real.fromInt(res)/Real.fromInt(b))                                    
                    in
                       (* print(Int.toString(x)^" "^Int.toString(result)^" "^Int.toString(b)^" "^Int.toString(remain)^"#");*) 
                        if result=0 andalso remain=oldremain then b
                        else if remain=oldremain then checkbase (x,result, b, remain) (*Aυξανω τιμες*)
                            else checkbase (x, ~1, b+1, ~1)  (*αλλαζω base*)                   
                    end
            in
                checkbase (x, ~1, 2,~1)
            end        

    in 
        map base2 input
    end
    