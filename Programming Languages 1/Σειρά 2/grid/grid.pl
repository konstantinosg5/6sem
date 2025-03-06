initial(config(0,0)).
final(config(X,Y),N):-
    X=N, Y=N.


/* get_element(+Array, +RowIndex, +ColIndex, -Element) */
get_element(Array, RowIndex, ColIndex, Element) :-
    nth0(RowIndex, Array, Row),
    nth0(ColIndex, Row, Element).

/* moves(input, move ,output  ) */
nextmove(config(X,Y), e,config(Xout,Yout)):- Xout is X ,Yout is Y +1. 
nextmove(config(X,Y), w,config(Xout,Yout)):- Xout is X ,Yout is Y-1.

nextmove(config(X,Y), s,config(Xout,Yout)):- Xout is X+1 ,Yout is Y.
nextmove(config(X,Y), n,config(Xout,Yout)):- Xout is X-1 ,Yout is Y.

nextmove(config(X,Y), se,config(Xout,Yout)):- Xout is X+1 ,Yout is Y +1.
nextmove(config(X,Y), sw,config(Xout,Yout)):- Xout is X+1 ,Yout is Y -1.

nextmove(config(X,Y), ne,config(Xout,Yout)):- Xout is X-1 ,Yout is Y +1.
nextmove(config(X,Y), nw,config(Xout,Yout)):- Xout is X-1,Yout is Y -1.

safe(config(X,Y),config(X2,Y2),N,Grid):-
    X2>=0 , Y2>=0 , 
    X2=<N , Y2=<N ,
    get_element(Grid,X,Y,Previous),
    get_element(Grid,X2,Y2,Next),
    Previous>Next.

solve(Conf, N, _, []):-final(Conf,N).
solve(Conf, N, Grid, [Move|Moves]):-
    nextmove(Conf, Move, Next),
    safe(Conf, Next, N, Grid),
    solve(Next, N, Grid, Moves).

solve(File, Moves):-
    read_and_return(File, N, Grid),
    N1 is N-1,
    
    list_length(Moves,N1),  
    Max is (N*N),

     (length(Moves,Length),
    (Length >= Max)->!; true )  , 

    solve(config(0,0), N1, Grid, Moves)  
    .

moves(File, Moves):-once(solve(File, Moves))->true;Moves = 'IMPOSSIBLE'.

list_length(List, N) :-    
    length(List, Length), 
    Min is N,
    Length >= Min.

       

   
/*
 * The first predicate is not so interesting. It just shows you how you can
 * write in Prolog a predicate that contains a loop with *only* side-effects.
 * Use as:
 *
 *   ?- read_and_just_print_codes('grid1.txt').
 */
read_and_just_print_codes(File) :-
    open(File, read, Stream),
    repeat,
    read_line_to_codes(Stream, X),
    ( X \== end_of_file -> writeln(X), fail ; close(Stream), ! ).

/*
 * The second predicate reads the information of an input file and returns
 * it in the next two arguments: the N and the grid represented as a list of
 * integer lists (the values in the rows of the grid).
 * 
 *   ?- read_and_return('grid1.txt', N, Grid).
 *   N = 5,
 *   Grid = [[15, 42, 3, 4, 7], [17, 13, 67, 4, 32], ...].
 *
 * To read the information of each of the segments, it uses the auxiliary
 * predicates read_grid/3.
 */

read_and_return(File, N, Grid) :-
    open(File, read, Stream),
    read_line(Stream, [N]),
    read_grid(Stream, N, Grid),
    close(Stream).

read_grid(Stream, N, Grid) :-
    ( N > 0 ->
	Grid = [Row|Rows],
        read_line(Stream, Row),
        N1 is N - 1,
        read_grid(Stream, N1, Rows)
    ; N =:= 0 ->
	Grid = []
    ).
 
/*
 * An auxiliary predicate that reads a line and returns the list of
 * integers that the line contains.
 */
read_line(Stream, List) :-
    read_line_to_codes(Stream, Line),
    atom_codes(A, Line),
    atomic_list_concat(As, ' ', A),
    maplist(atom_number, As, List).