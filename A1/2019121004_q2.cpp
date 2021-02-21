#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

/* quick sort */
int pivotfunc(int *arr,int left,int right){
    int i=left;
    int j=right;
    int pivot=arr[left];
    
    while(i<j)
    {
        while(arr[j]>pivot)j--;
        while(arr[i]<=pivot)i++;
        if(i<j)
        {
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[j],arr[left]);
    return j;
}
int quick_sort(int *arr,int left,int right){
    if(left<right){
        int j;
        j=pivotfunc(arr,left,right);
        quick_sort(arr,left,j-1);
        quick_sort(arr,j+1,right);
    }
    return 1;
}


int main(int argc, char** argv) {
    int rank, numprocs;
    
    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */
    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

    if(rank==0){
       
        //reading input
        ifstream f_input;
        string line;

        f_input.open (argv[1]);
        int n = 0;
        f_input >> n;
       
        int arr[n];
        for(int a = 0; a < n; a++)
        {
            f_input >> arr[a];
        }
     
        f_input.close();
                
        int j;
        j=pivotfunc(arr,0,n-1);
        quick_sort(arr,0,j-1);
        int x = n-j-1;
        int y = x;
        MPI_Send(&y,1, MPI_INT,1, 0, MPI_COMM_WORLD);
        MPI_Send(arr+j+1,n-j-1, MPI_INT,1, 0, MPI_COMM_WORLD);
        int n1;
        int from = 1;
        MPI_Recv(&n1, 1, MPI_INT,from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int brr[n1];
        MPI_Recv(brr, n1, MPI_INT,1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //write array to file
        ofstream f_output;
        f_output.open(argv[2]);
        for(int i=0;i<j+1;i++)
        {
            f_output<< arr[i] << " ";
        }
        for(int i=0;i<n1;i++)
        {
            f_output<< brr[i] << " ";
        }
        f_output.close();
        printf( "Total time (s): %f\n", maxTime );
    }
    if(rank==1){
        int n;
        int from = 0;
        MPI_Recv(&n, 1, MPI_INT,from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int arr[n];
        MPI_Recv(arr, n, MPI_INT,0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        quick_sort(arr,0,n-1);
        int x = n;
        int y = x;
        MPI_Send(&y,1, MPI_INT,0, 0, MPI_COMM_WORLD);
        MPI_Send(arr,n, MPI_INT,0, 0, MPI_COMM_WORLD);
    }   

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}