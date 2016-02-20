#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <sstream>
#include <list>
#include <string>
#include <cstring>

using namespace std;

// a class to get more accurate time

class stopwatch{
	
private:
	double elapsedTime;
	double startedTime;
	bool timing;
	//returns current time in seconds
	double current_time( ) 
	{
		timeval tv;
		gettimeofday(&tv, NULL);
		double rtn_value = (double) tv.tv_usec;
		rtn_value /= 1e6;
		rtn_value += (double) tv.tv_sec;
		return rtn_value;
	}
	
public:
	stopwatch( ): elapsedTime( 0 ), startedTime( 0 ), timing( false )
	{
		
	}
	
	void start( )
	{
		if( !timing )
		{
			timing = true;
			startedTime = current_time( );
		}
	}
	
	void stop( )
	{
		if( timing )
		{
			elapsedTime +=  current_time( )-startedTime;
			timing = false;
		}
	}
	
	void resume( )
	{
		start( );
	}
	
	void reset( )
	{
		elapsedTime = 0;
		startedTime = 0;
		timing = false;
	}
	
	double getTime( )
	{
		return elapsedTime;
	}
};



// function takes an array pointer, and the number of rows and cols in the array, and 
// allocates and intializes the two dimensional array to a bunch of random numbers

void makeRandArray( unsigned int **& data, unsigned int rows, unsigned int cols, unsigned int seed )
{
	// allocate the array
	data = new unsigned int*[ rows ];
	for( unsigned int i = 0; i < rows; i++ )
	{
		data[i] = new unsigned int[ cols ];
	}
	
	// seed the number generator
	// you should change the seed to get different values
	srand( seed );
	
	// populate the array
	
	for( unsigned int i = 0; i < rows; i++ )
		for( unsigned int j = 0; j < cols; j++ )
		{
			data[i][j] = rand() % 10000 + 1; // number between 1 and 10000
		}
	
}

void getDataFromFile( unsigned int **& data, char fileName[], unsigned int &rows, unsigned int &cols )
{
	ifstream in;
	in.open( fileName );
	if( !in )
	{
		cerr << "error opening file: " << fileName << endl;
		exit( -1 );
	}
	
	in >> rows >> cols;
	data = new unsigned int*[ rows ];
	for( unsigned int i = 0; i < rows; i++ )
	{
		data[i] = new unsigned int[ cols ];
	}
	
	// now read in the data
	
	for( unsigned int i = 0; i < rows; i++ )
		for( unsigned int j = 0; j < cols; j++ )
		{
			in >> data[i][j];
		}
	
}


int main( int argc, char* argv[] ) 
{
	if( argc < 3 )
	{
		cerr<<" usage: exe [input data file] [num of threads to use] " << endl;
		
		cerr<<"or usage: exe rand [num of threads to use] [num rows] [num cols] [seed value]" << endl;
	}
	
	// read in the file
	unsigned int rows, cols, seed;
	unsigned int numThreads;
	unsigned int ** data;

	float ** data_avg;
	// convert numThreads to int
	{
		stringstream ss1;
		ss1 << argv[2];
		ss1 >> numThreads;
	}
	
	string fName( argv[1] );
	if( fName == "rand" )
	{
		{
			stringstream ss1;
			ss1 << argv[3];
			ss1 >> rows;
		}
		{
			stringstream ss1;
			ss1 << argv[4];
			ss1 >> cols;
		}
		{
			stringstream ss1;
			ss1 << argv[5];
			ss1 >> seed;
		}
		makeRandArray( data, rows, cols, seed );
	}
	else
	{
		getDataFromFile( data,  argv[1], rows, cols );
	}
	
		// cerr << "data: " << endl;
	 // for( unsigned int i = 0; i < rows; i++ )
	 // {
	 // for( unsigned int j = 0; j < cols; j++ )
	 // {
	 // cerr << "i,j,data " << i << ", " << j << ", ";
	 // cerr << data[i][j] << " ";
	 // }
	 // cerr << endl;
	 // }
	 // cerr<< endl;
	
	// tell omp how many threads to use
	omp_set_num_threads( numThreads );
	
#pragma omp parallel
	{
	}
	
	stopwatch S1;
	S1.start();

	/////////////////////////////////////////////////////////////////////
	///////////////////////  YOUR CODE HERE       ///////////////////////
	/////////////////////////////////////////////////////////////////////
	int count=0;
	//assuming array is an nxn matrix of numbers

	//OUTPUT ARRAY FOR DEBUGGING/DEV REASONS
	// for( unsigned int i = 0; i < rows; i++ ) 
	// {
	//  	for( unsigned int j = 0; j < cols; j++ )
	// 	{
	// 		cerr << data[i][j] << " ";
	// 	}
	// 	cerr << endl;
	// }
	// cerr << endl;

	// for( unsigned int i = 0; i < rows; i++) 
	// {
	//  	for( unsigned int j = i+1; j < rows; j++)
	// 	{
	// 		for( unsigned int k = j+1; k < rows; k++)
	// 		{
	// 		cerr << data[i][i] << " " << data[i][j] << " " << data[i][k] << " " << endl;
	// 		cerr << data[j][i] << " " << data[j][j] << " " << data[j][k] << " " << endl;
	// 		cerr << data[k][i] << " " << data[k][j] << " " << data[k][k] << " "	<< endl;
	// 	    cerr << endl;
	// 		}
	// 	}
	// 	cerr << endl;
	// }


	// This successfully breaks array into smaller 3x3 arrays
	// for( unsigned int i = 0; i < rows-2; i++ ) 
	// {
	//  	for( unsigned int j = 0; j < cols-2; j++ )
	// 	{
	// 		cerr << data[i][j] << " " << data[i][j+1] << " " << data[i][j+2] << " " << endl;
	// 		cerr << data[i+1][j] << " " << data[i+1][j+1] << " " << data[i+1][j+2] << " " << endl;
	// 		cerr << data[i+2][j] << " " << data[i+2][j+1] << " " << data[i+2][j+2] << " "	<< endl;
	// 	cerr << endl;
	//     }
	// }


	// create a new array to hold the average values for applicable cells
	data_avg = new float*[ rows ];
	for( unsigned int i = 0; i < rows; i++ )
	{
		data_avg[i] = new float[ cols ];
	}
	
	// Set the original values of the arry to 0 because they were sometimes preexsting memory address locations...
	for( unsigned int i = 0; i < rows; i++ ) 
	{
	 	for( unsigned int j = 0; j < cols; j++ )
		{
			data_avg[i][j]=0;
		//	cerr << data_avg[i][j] << " ";
		}
		//cerr << endl;
	}
	//cerr << endl;

	// Find the averages and save to a new array
	float max_avg=0;
	unsigned int max_row=0;
	unsigned int max_col=0;
	// Realized that we did not need to do this part...
	// unsigned int max_row_tie=0;
	// unsigned int max_col_tie=0;

	for( unsigned int i = 0; i < rows-2; i++ ) 
	{
	 	for( unsigned int j = 0; j < cols-2; j++ )
		{
			data_avg[i+1][j+1] = (data[i][j] + data[i][j+1] + data[i][j+2] 
								+ data[i+1][j] + data[i+1][j+1] + data[i+1][j+2]
			   					+ data[i+2][j] + data[i+2][j+1] + data[i+2][j+2])/9.0;

			// cerr << "Avg calculated at: " << i+1 << "," << j+1 << " : " << data_avg[i+1][j+1] << endl;

			if(data_avg[i+1][j+1] >= data_avg[max_row][max_col])
			{
				//realized that we did not need to keep track of duplicates...
				// if(data_avg[i+1][j+1] == max_avg)
				// {
				// 	max_row_tie = max_row;
				// 	max_col_tie = max_col;	
				// }

				max_avg=data_avg[i+1][j+1];
				max_row=i+1;
				max_col=j+1;
			}
			//cerr << "max_avg=" << max_avg << " at (" << max_row << "," << max_col << ")" << endl << endl;
		}
	}
	

	// for( unsigned int i = 0; i < rows; i++ ) 
	// {
	//  	for( unsigned int j = 0; j < cols; j++ )
	// 	{
	// 		cerr << data_avg[i][j] << " ";
	// 	}
	// 	cerr << endl;
	// }
	// cerr << endl;

	S1.stop();
	
	// print out the max value here
	cerr << "largest average: " << max_avg << endl;
	cerr << "found at cell: (" << max_row << "," << max_col << ")" << endl;
	
	cerr << "elapsed time: " << S1.getTime( ) << endl;
}


