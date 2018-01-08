
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "go_groups.h"

int main( int argc, char *argv[] )
{
	if ( argc != 6 ) {
		cerr << "Usage " << argv[0] << " randset outfile profile cutoff GO:ID" << endl ;
		exit( 1 ) ;
	}

	/*************
         * parsing arguments, creating in and outstreams
         ************/
	istream *in ;
	if ( string( argv[1] ) == "-" ) {
		in = &cin ;
	} else {
		in = new ifstream( argv[1] ) ;
	}
	if ( ! *in ) {
		cerr << "Cannot open " << argv[1] << endl ;
	}

	ofstream out( argv[2] ) ;
	if ( ! out ) {
		cerr << "Cannot open " << argv[2] << endl ;
	}

	int cutoff=1 ;
	{
		istringstream cutoff_s( argv[4] ) ;
		cutoff_s >> cutoff ;
	}

	string root_go ;
	{ 
		istringstream ppp( argv[5] ) ;
		ppp >> root_go ;
	}


	/*************
         * start reading from randomset-file  
         ************/
	// ignore header-lines
	string dummy ;
	getline( *in, dummy ) ; 
	getline( *in, dummy ) ; 

	string groups, sites ;
	getline( *in, groups ) ; // GO IDs
	if ( groups == "" ) {
		cerr << "Error reading randomsets" << endl ;
		exit( 1 ) ;
	}
	
	string detected, changed ;
	// ignore detected-values (reading it from profile instead)
	getline( *in, detected ) ; 
	getline( *in, changed ) ; 

	/*************
         * go_groups handles parsing and analysis of dataset and randset lines
         ************/
	go_groups gos( groups, detected, changed, root_go, cutoff ) ;

	ofstream profile( argv[3] ) ;

	// returns number of significant groups for 0.1, 0.05, 0.01, 0.001
	int *realdata = gos.calculate_data( &profile ) ;
	out << endl << endl ;

	int sum_randdata[10] ;
	for ( int i=0 ; i < 10 ; ++i ) sum_randdata[i] = 0 ;
	int nr_groups_ge[10] ;
	for ( int i=0 ; i < 10 ; ++i ) nr_groups_ge[i] = 0 ;
	int num_randdata = 0 ;
	// randomsets
	string data ;
	while ( *in ) {
		getline( *in, data ) ;
		if ( data == "" ) { break ; } 
		int *randdata = gos.calculate_rand( data ) ;
		for ( int i=0 ; i<10 ; ++i ) {
			sum_randdata[i] += randdata[i] ;
			if ( randdata[i] >= realdata[i] ) {
				nr_groups_ge[i]++ ;
			}
		}
		for ( int i=0 ; i < 10 ; ++i ) {
			cout << randdata[i] << "\t" ;
		}
		cout << "\n" ;
		delete randdata ;
		num_randdata++ ;
	}
	// gos.print_pvals( num_randdata, cout ) ;
	gos.print_pvals( num_randdata, out ) ;

	// write outfile
	out << "Randomsets: " << num_randdata << endl ;
	out << "conserved\t\t\t\tchanged" << endl ;
	out << "# sig. groups dataset" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		out << realdata[i] << "\t" ;
	out << endl ;
	out << "# sig. groups mean randomsets" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		out << sum_randdata[i]/static_cast<double>(num_randdata) << "\t" ;
	out << endl ;
	out << "# p value" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		out << nr_groups_ge[i]/static_cast<double>(num_randdata) << "\t" ;
	out << endl ;
}
