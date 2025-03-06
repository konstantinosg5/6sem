minbases([],[]).
minbases([H|T] , [Hr|Tr]):-    
    solve(H,Hr,2),
    minbases(T,Tr).


solve(Input, Result, Base):-
     checkbase(Input, Base, _)  ->  Result = Base;
        NextB is Base + 1,
        solve(Input, Result, NextB ).

checkbase(0,_,_).
checkbase(Input, Base, Oldremain):-
   /* write(Input),write(Base), writeln(Oldremain) ,*/

    Remain is Input mod Base ,
    Result is Input//Base ,
    Remain=Oldremain,

    /*  write(Result),writeln(Remain),*/

    (checkbase(Result, Base, Remain)).
