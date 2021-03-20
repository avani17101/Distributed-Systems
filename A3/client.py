import Pyro4
import sys

f_u = open('uri.txt')
uri = f_u.read()
uri = uri.replace('\n','')

Server = Pyro4.Proxy(uri)  # get a Pyro proxy to the greeting object

infile  = sys.argv[1]
outfile = sys.argv[2]
f = open(infile)
f2 = open(outfile,'w+')
f2.close()

for i,line in enumerate(f.readlines()):
    lis = line.split(' ')
    if(lis[0]=='add_graph'):
        graphid = lis[1]
        V = int(lis[2].replace('\n',''))
        Server.add_graph(graphid,V)

    if(lis[0]=='add_edge'):
        graphid = lis[1]
        u, v, w = lis[2:]
        Server.add_edge(graphid, int(u) ,int(v), int(w))

    if(lis[0]=='get_mst'):
        graphid = lis[1].replace('\n','')
        weight = Server.get_mst(graphid)
        f2 = open(outfile,'a')
        print(str(weight),file=f2)
           
f2.close()
f.close()


       
    




