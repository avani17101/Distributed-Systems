-module('2019121004_2').
-compile(export_all).
-export([main/1]).

main(Args) ->
    [Infile, Outfile] = Args,
    {ok ,File} = file:open(Infile, [read]),
    Graph = [],
    P = 0, %num process
    V = 0,
    E = 0,
    S = 0, %source vertex
    [Graph2| Params] = read_file(File, Outfile, Graph, P, V, E, S),
    %io:format("~p",[Graph2]),
    [P1,V1,E1,S1] = Params,
    MainPid = self(),
    Dist = lists:duplicate(V1,100000),
    [H|T] = Dist,
    Distances = [0|T],
    Disn = lists:duplicate(V1+2,100000),
    Pids = createProcs(P1,1,Graph2,P1,V1,2*E1,S1,[]),
    while(Disn, Graph,Distances,Graph2,P1,V1,2*E1,S1,Pids,MainPid,Outfile).


for3(N,Distances,S,Outfile,V) ->
    if 
        N == 0 -> 
            ok;
        true -> 
            [H|T] = Distances,
            Yo = V-N+1,
            io:format(S,"~p ~p~n",[Yo,H]),
            {ok ,S1} = file:open(Outfile, [append]),
            for3(N-1,T,S1, Outfile,V)
            
    end.

write(Outfile, Distances) ->
    {ok ,S} = file:open(Outfile, [write]),
    for3(length(Distances),Distances,S, Outfile,length(Distances)).

while(L, Graph,Dist3,Graph2,P1,V1,E1,S1,Pids,MainPid,Outfile) -> while(L,0, Graph,Dist3,Graph2,P1,V1,E1,S1,Pids,MainPid,Outfile). 
while([], Acc, Graph, Dist3,Graph2,P1,V1,E1,S1,Pids,MainPid,Outfile) ->
     write(Outfile, Dist3);

while([_|Dist], Acc, Graph, Dist2,Graph2,P1,V1,E1,S1,Pids,MainPid,Outfile) ->
    %io:format("iter ~p~n",[Acc]),
    spawn(?MODULE, distributeWork, [P1,Graph2,P1,V1,E1,S1,Pids,self(),Dist2]),
    DistNew = lop(P1,Dist2,Dist2),
    %io:format("Insideeeee ~p~n",[DistNew]),
    while(Dist,Acc+1, Graph, DistNew,Graph2,P1,V1,E1,S1,Pids,self(),Outfile). 

lop(0,Dist,DistOld) ->
    Dist;
lop(N,Dist1,DistOld) ->
    receive
        {Dist} -> 
        %io:format("~n Done ~p~n",[Dist]),
        Dist3 = merge(DistOld,Dist,[]),
        lop(N-1,Dist3,Dist)
    end.

merge(Dist1, Dist2,Dist) ->
    if 
        Dist1 == [] -> 
            Dist;
        true -> 
            [H1|T1] = Dist1,
            [H2|T2] = Dist2,
            Dist_merged = lists:append(Dist, [min(H1,H2)]),
            merge(T1,T2,Dist_merged)
            
    end.


getSubgraph([],Graph,SubGraph) -> SubGraph;
    getSubgraph([H|T],Graph,SubGraph) ->
        SubGraph_edge = lists:nth(H, Graph),
        {E1, E2, W} = SubGraph_edge,
        SubGraph2 = SubGraph ++ [{E1, E2, W}],
        getSubgraph(T,Graph,SubGraph2).

createProcs(0,_,Graph,P,V,E,S,Pids) ->  Pids; 
   createProcs(N,Term,Graph,P,V,E,S,Pids) when N > 0 -> 
   Pid = spawn(fun subProcsloop/0),
   createProcs(N-1,Term,Graph,P,V,E,S,lists:append(Pids, [Pid])). 

distributeWork(0,Graph,P,V,E,S,Pids,MainPid,Dist) ->
      ok; 
distributeWork(N,Graph,P,V,E,S,Pids,MainPid,Dist) when N > 0 -> 
    E2 = E/P,
    E3 = list_to_integer(erlang:float_to_list(E2,[{decimals,0}])),
    
    Start = max(N*E3-E3,1),
    End = min(N*E3,E),
    Index = lists:seq(Start,End),
    SubGraph = getSubgraph(Index,Graph,[]), 
    %io:format("~p~n",[SubGraph]),
    Pid = lists:nth(N, Pids),
    Pid ! {MainPid,SubGraph,Dist},
    distributeWork(N-1,Graph,P,V,E,S,Pids,MainPid,Dist). 

check(Res,Res3,U,V,W,Dist2)->
    case Res and Res3 of
        true ->
            Dist3 = lists:sublist(Dist2,V-1) ++ [lists:nth(U, Dist2) + W] ++ lists:nthtail(V, Dist2),
            Dist3;
        false ->
            Dist2
    end.

check3(Edge, Dist2) ->
    {U,V,W} = Edge,
    Res = lists:nth(U, Dist2) /= 100000,
    Res2 = lists:nth(U, Dist2) + W,
    Res3 = Res2 < lists:nth(V, Dist2),
    Dist3 = check(Res,Res3,U,V,W,Dist2),
    %io:format("~p~n",[Dist3]),
    Dist3.

for(Graph, Dist,[]) ->
    if 
        Graph == [] -> 
            Dist;
        true -> 
            [H|T] = Graph,
            Dist2 = check3(H,Dist),
            for(T,Dist2,[])
            
    end.


subProcsloop() ->
    receive
        {Pid,Graph, Dist} ->
            Dist2 = for(Graph, Dist,[]),
            Pid ! {Dist2},
            subProcsloop()
    end.



read_file(Device, Outfile, Graph,P, N, M, S) ->
    case file:read_line(Device) of
    eof ->
        [Graph,P,N,M,S];
    {ok, Line} ->
        List = lists:map(fun erlang:list_to_integer/1, 
                     lists:map(fun erlang:binary_to_list/1, 
                     re:split(string:strip(Line, right, 10), "\s+", [notempty]))), 
        if
            length(List)==3 -> 
                [E1, E2, W] = List,
                Graph2 = Graph ++ [{E1, E2, W}],
                Graph3 = Graph2 ++ [{E2, E1, W}],

                read_file(Device, Outfile, Graph3,P, N, M, S);
            true ->
                case length(List) of
                1 -> 
                    if P == 0 ->
                        [P1] = List,
                        read_file(Device, Outfile, Graph,P1, N, M, S);
                    true ->
                        [S1] = List,
                        read_file(Device, Outfile, Graph,P, N, M, S1)
                    end;

                2 -> 
                    [N1, M1] = List,
                    read_file(Device, Outfile, Graph,P, N1, M1, S)
                end
        end
    end.

