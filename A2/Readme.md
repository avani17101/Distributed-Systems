# Distributed Systems: Assingment 2

Roll: 2019121004
Name: Avani Gupta


## Problem 1
Write a program to pass an integer token value around all processes in a ring-like fashion, and make sure that it does not have a deadlock.

### Algorithm

1) Create P processes and link them together like a ring(Link adjacent processes together and link last process to first process).

2) Send and Recieve message between those processes using message id.
Send Stop message and stop the message passing afterwards.


## Problem 2
Given a weighted graph and a source vertex in the graph, find the shortest paths from source to all vertices in the graph. You can use any algorithm to solve this problem.

### Algorithm: Bellman-Ford Parallised

* Reference for Bellman-Ford simple: https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/

* Steps
1) Distribute the E/P edges to each process where E is number of edges in graph,
P is number of Processes (Subgraph used for distributing E/P edges and edge case handled, 
eg: if there are 5 edges and 3 processes, Process 1 gets E1, E2; Process 2 gets E3, E4 and Process 3 gets E5.

2) Initialize distances from the source to all vertices as infinite and distance to the source itself as 0. 

3) Calculate shortest distances. 
Do following |V|-1 times where |V| is the number of vertices in given graph.
	a) For each edge u-v
		If dist[v] > dist[u] + weight of edge uv, then update dist[v]
			dist[v] = dist[u] + weight of edge uv
	b) Merge the output of all processes.
		FinalDistance = Min(Dist_1, Dist_2, Dist_p)
		
4) Final output is the Distance after V-1 iterations.			
		
		
#### Time-Complexity: O(V*E/P)


## Running code

* Question 1
```
 erlc 2019121004_1.erl
 erl -noshell -s '2019121004_1' main 'in1.txt' 'out1.txt' -s init stop
 
```

* Question 2
```
 erlc 2019121004_2.erl
 erl -noshell -s '2019121004_2' main 'in2.txt' 'out2.txt' -s init stop
 ```
