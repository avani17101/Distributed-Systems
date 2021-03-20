from collections import defaultdict
import numpy as np
 
class Graph:

    def __init__(self, V):
        self.graph = [] 
        self.V = V

    def findNode(self, p, i):
        diff = p[i] -i
        if diff==0:
            return i
        return self.findNode(p, p[i])

    def Kruskal(self, graph):
        self.graph = graph
        self.graph = sorted(self.graph, key=lambda edge: edge[2])
 
        p = np.arange(self.V)  #parent
        rank = np.zeros(self.V)
    
        numV = 0
        minCost = 0

        i = 0
        while numV < self.V - 1:
            u, v, w = self.graph[i]  #iterate through each edge
            left = self.findNode(p, u)
            right = self.findNode(p, v)

            if left != right:
                minCost += w  #the edge u, v, w is in MST hence, add minCost

                l_root = self.findNode(p, left)
                r_root = self.findNode(p, right)

                if rank[l_root] == rank[r_root]:
                    p[r_root] = l_root
                    rank[l_root] += 1

                if rank[l_root] < rank[r_root]:
                    p[l_root] = r_root

                if rank[l_root] > rank[r_root]:
                    p[r_root] = l_root

                numV = numV + 1
            i = i + 1
 
        if(minCost==0):
            return -1
        return minCost
