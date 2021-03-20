import Pyro4
from krushkals import Graph

@Pyro4.expose
class Server(object):
    def __init__(self):
        self.g = {}
        self.Edges = {}
        
    def add_graph(self, graphId, n):
        self.g[graphId]  = Graph(n)
        self.Edges[graphId] = []
    
    def add_edge(self, graphId,u,v,w ):
        self.Edges[graphId].append([u-1,v-1, w])

    def get_mst(self, graphId):
        if(len(self.Edges[graphId])==0):
            return -1
        else:
            try:
                st = self.g[graphId].Kruskal(self.Edges[graphId])
                return st
            except:
                return -1
            

daemon = Pyro4.Daemon()         # make a Pyro daemon
uri = daemon.register(Server)   # register the greeting maker as a Pyro object

f = open('uri.txt','w+')
print(uri,file=f)
f.close()

print("Ready. Object uri =", uri)      # print the uri so we can use it in the client later
daemon.requestLoop()                   # start the event loop of the server to wait for calls