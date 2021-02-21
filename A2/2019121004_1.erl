-module('2019121004_1').
-compile(export_all).
-export([main/1]).

main(Args) ->
    [Infile, Outfile] = Args,
    {ok ,File} = file:open(Infile, [read]),
    read_file(File, Outfile).

read_file(Device, Outfile) ->
    case file:read_line(Device) of
    eof ->
        ok;
    {ok, Line} ->
        [P, Token] = lists:map(fun erlang:list_to_integer/1, 
                     lists:map(fun erlang:binary_to_list/1, 
                     re:split(string:strip(Line, right, 10), "\s+", [notempty]))), 
        proc(P, Token, Outfile),
        {ok ,S} = file:open(Outfile, [write]),
        io:format(S, "~s",[""]),
        read_file(Device, Outfile)
    end.

proc(P, Token, Outfile) ->
    Ring_ = create_procs([], P),
    [H|T] = Ring_,
    Ring = lists:append(Ring_, [hd(Ring_)]),
    send_msg({Token}, Ring, Ring, Outfile).

create_procs(Pids, 0) ->
    Pids;
create_procs(Pids, Num) ->
    create_procs([spawn(?MODULE, loop, [])|Pids], Num-1).

send_msg({_, 0}, [H|T], Ring, Outfile) ->
    H ! {stop, T, Ring, Outfile};

send_msg({Token}, [H|T], Ring, Outfile) ->
    H ! {Token, T, Ring, Outfile},
    send_msg({Token, 0}, [H|T], Ring, Outfile).

loop() ->
    receive
        {stop, [Next|Pids], Ring, Outfile} ->
            Next ! {stop, Pids, Ring, Outfile};

        {Token, [Next|Pids], Ring, Outfile} ->
            {ok ,S} = file:open(Outfile, [append]),
            io:format(S, "Process ~p recieved token ~p from  process ~p~n", [string:str(Ring, [self()]), Token, string:str(Ring, [Next])]),
            Next ! {Token, Pids, Ring, Outfile},
            loop()
    end.