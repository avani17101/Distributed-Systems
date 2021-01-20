/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;
typedef long long int ll;

int main(int argc, char **argv)
{
    int rank, numprocs;

    /* start up MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*synchronize all processes*/
    MPI_Barrier(MPI_COMM_WORLD);
    double tbeg = MPI_Wtime();

    /* write your code here */
    //reading input
    ifstream f_input;
    string line;
    f_input.open (argv[1]);
    getline(f_input, line); 
    f_input.close();

    // convert input to string
    stringstream tostring(line);
    int n = 0;
    tostring >> n;
    // question solution
    
    float sum = 0;
    int each_process_sum = n / numprocs;
    float start_val = (rank * each_process_sum) + 1;

    for (float i = start_val; i <= (each_process_sum*(rank + 1)); i++)
    {
        float i_sq = i * i;
        sum += 1 / i_sq;
    }

    if (rank == numprocs - 1)
    {
        float start_val = ((rank + 1) * each_process_sum) + 1;
        for (float i = start_val; i <= n; i++)
        {
            float i_sq = i * i;
            sum += 1 / i_sq;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    float final_sum = 0;
    MPI_Reduce(&sum, &final_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    // output final sum val
    if(rank==0)
    {
        
        ofstream f_output;
        f_output.open(argv[2]);
        f_output<< "final sum: "<<fixed<<setprecision(6)<<final_sum << "\n";
        f_output.close();
    }
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce(&elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {

        printf("Total time (s): %f\n", maxTime);
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}