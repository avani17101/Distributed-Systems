# Distributed Systems Assignment 3

Roll number: 2019121004
Name: Avani Gupta

## Framework/Languages
* Pyro4
* Python3

## Structure
* server.py: Server
* client.pyL Client
* krushkals.py: Krushkals Algorithm implementation

## Requirements
```bash
 pip install Pyro4
 pip install numpy
```
## Steps to run
* install requirements(Pyro4)
* run server on one terminal
```bash
 python server.py
```
* run clients on other terminal
```bash
 python client.py <infile> <outfile>

```
Note: For multiple clients run client.py with different input output files in different terminal windows.

Example: 
```bash
 python client.py in.txt out.txt
```
## Features
* All features mentioned in assignment
* Multiple client support


## Algorithm: Krushkals
**Given** number of vertice:V
Weighted graph (undirected in our case), edges[u, v, w]: [source, vertex, weight]

**To Find** Finding sum of weights of minimum spanning tree 

**Steps**
Sort all the edges in non-decreasing order of their weight. 
while(size(mst) != V-1):
    find smallestEdge 
    if mst U smallestEdge doesnt form cycle:
        mst = mst U smallestEdge


**Time complexity: O(V^2)**

**Client: client.py** 
Multiple requests to server on funtionalities: add_graph, add_edge, get_mst. 

**Server: server.py**
Serves according to request by client, returns mst weight on get_mst.






