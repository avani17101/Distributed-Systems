
/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;


int countDistinct(int a[], int size){
   int i, j, count = 1;
   for (i = 1; i < size; i++){
      for (j = 0; j < i; j++){
         if (a[i] == a[j]){
            break;
         }
      }
      if (i == j){
         count++;
      }
   }
   return count;
}
vector <vector <int>> get_adjacency_mat(int N, int M, vector <int> from_edge_list, vector <int> to_edge_list){
        vector <vector <int>> AdjacencyList(M+1);
        vector <vector <int>> Nodes(N+1);
        
        int i= 1;
        while(i<=M)
        {
            Nodes[to_edge_list[i]].push_back(i);
            Nodes[from_edge_list[i]].push_back(i);
            i ++;
        }
           
            
        for(int i=1; i<=N; i++){
           for(int a: Nodes[i]){
               for(int b: Nodes[i]){
                    if(a==b) 
                    {
                        continue;
                    }
                    AdjacencyList[a].push_back(b);
               }
           }
        }
        return AdjacencyList;
}



vector <int> Add_colour(vector <vector <int>> &adjacency_list, vector <int> color_cur, int l, int r){

    for(int i=l; i<r; i++){
        int toColor = 1;
        if(color_cur[i] != 0) 
        {
            toColor = 0;
        }
        for(int x: adjacency_list[i]){
            if(color_cur[x] != 0) continue;
            if(x > i)
            {
                toColor = 0;
            }
        }

        if(toColor){
            vector <int> vec(502, 0);
            for(int x : adjacency_list[i]){
                if(color_cur[x]) vec[color_cur[x]] = 1;
            }
            int color;
            for(color = 1; vec[color] != 0; color++) ;
            color_cur[i] = color;
        }
    }
    return color_cur; 
}



int main( int argc, char **argv ) {
    int rank, numprocess;

    MPI_Status stat;
    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocess );
    
    /*synchronize all procesesses*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */
    int N, M;

    if(rank == 0){
       
        ifstream f_input;
        f_input.open(argv[1]);
        f_input >> N;
        int temp;
        f_input >> M;

        vector <int> from_edge_list(M+1);
        int k;
        vector <int> to_edge_list(M+1);

        for(int i=1; i<=M; i++){
    
            f_input >>from_edge_list[i];
            f_input >>to_edge_list[i];

        }
        f_input.close();

        for(int proces=1; proces<numprocess; proces++){
            int k;
            MPI_Send(&N, 1, MPI_INT, proces, 0, MPI_COMM_WORLD);
            MPI_Send(&M, 1, MPI_INT, proces, 0, MPI_COMM_WORLD);

            k = M+1;
            MPI_Send(&k, 1, MPI_INT, proces, 0, MPI_COMM_WORLD);
            MPI_Send(&from_edge_list[0], k, MPI_INT, proces, 0, MPI_COMM_WORLD);
    
            MPI_Send(&k, 1, MPI_INT, proces, 0, MPI_COMM_WORLD);
            MPI_Send(&to_edge_list[0], k, MPI_INT, proces, 0, MPI_COMM_WORLD);
        }
      
        vector <vector <int>> AdjacencyList = get_adjacency_mat(N, M, from_edge_list, to_edge_list);
        int num = (M/numprocess);
        vector <int> color_cur(M+1, 0);

    
        if(rank == numprocess-1)
        {
            num = M - (M/numprocess) * rank;
        }

        int num_t;
        vector <vector <int>> Colours(numprocess);
        int flag = 0;
        while(1){
            int flag = 1;
            for(int i=1; i<=M; i++)
            {
                if(color_cur[i] == 0)
                {
                    flag=0;
                } 
            }
            int N = M+1;
            for(int i=1; i<numprocess; i++){
                MPI_Send(&flag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            if(flag != 1)
            {
                for(int i=1; i<numprocess; i++){
                    MPI_Send(&N, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    int ar = color_cur[0];
                    MPI_Send(&color_cur[0], N, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
            int k;
            
            if(flag)
            {
                break;
            } 
            flag = 0;
            int temp = (M/numprocess)*rank;
            Colours[0] = Add_colour(AdjacencyList, color_cur, temp + 1, temp + 1 + num);
            k = 1;
            for(int i=0; i<numprocess; i++){
                k += 1;
                if(i) 
                {
                    int l;
                    MPI_Recv(&l, 1, MPI_INT,i, 0, MPI_COMM_WORLD, &stat);
                    k ++;
                    vector <int> array = vector<int> (l,0);
                    
                    MPI_Recv(&array[0], l, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
                    k ++;
                    Colours[i] = array;
                }

               
                int num_t = (M/numprocess);
                int temp = (M/numprocess) * i ;
                if(i == numprocess-1)
                {
                    num_t = M - temp;
                } 
                temp = temp + 1;
            
                for(int j = temp; j < temp + num_t; j++){
                    color_cur[j] = Colours[i][j];
                }
            }

       
        }
        ofstream f_output;
        f_output.open(argv[2]);
        int len = sizeof(color_cur)/sizeof(color_cur[0]);
        // int uniqu = countDistinct(color_cur,len);
           int i, j, count = 1;
            for (i = 1; i < len; i++){
                for (j = 0; j < i; j++){
                    if (color_cur[i] == color_cur[j]){
                        break;
                    }
                }
                if (i == j){
                    count++;
                }
            }
        f_output<<count <<"\n";
        for(int i=1; i<= M; i++){
            f_output<< color_cur[i] << " ";
        }
        f_output.close();
    }
    else {

        int N, M;
        int toStop;

        MPI_Recv(&N, 1,MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        toStop = 0;
        MPI_Recv(&M, 1,MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);


        int l;
        MPI_Recv(&l, 1, MPI_INT,0, 0, MPI_COMM_WORLD, &stat);
        vector <int> array = vector<int> (l,0);
        MPI_Recv(&array[0], l, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        vector <int> from_edge_list = array;

        MPI_Recv(&l, 1, MPI_INT,0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(&array[0], l, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        vector <int> to_edge_list = array;
    
        

        auto AdjacencyList = get_adjacency_mat(N, M, from_edge_list, to_edge_list);

        int num = (M/numprocess);
        if(rank == numprocess-1)
        {
            num = M - (M/numprocess) * rank;

        } 
        if(toStop==0)
        {
            MPI_Recv(&toStop, 1, MPI_INT, 0, 0 ,MPI_COMM_WORLD, &stat);
        } 
        int k = 0;
        MPI_Recv(&l, 1, MPI_INT,0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(&array[0], l, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        vector <int> color_cur = array;


        while(toStop != 1){
            color_cur = Add_colour(AdjacencyList, color_cur, (M/numprocess)*rank + 1, (M/numprocess)*rank + 1 + num);
            int k = M+1;
            MPI_Send(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&color_cur[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&toStop, 1, MPI_INT, 0, 0 ,MPI_COMM_WORLD, &stat);
            if(toStop)
            {
                break;
            }
            int l;
            MPI_Recv(&l, 1, MPI_INT,0, 0, MPI_COMM_WORLD, &stat);
            vector <int> array = vector<int> (l,0);
            MPI_Recv(&array[0], l, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
           
             
            color_cur = array;
        }
    }


    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}