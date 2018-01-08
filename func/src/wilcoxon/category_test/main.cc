
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
		cerr << "Usage " << argv[0] << " randset outfile cutoff GO:ID profile" << endl ;
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

	ofstream *profile = new ofstream( argv[5] ) ;
	if ( ! *profile ) profile = 0 ;

	string root_go ;
	{ 
		istringstream ppp( argv[4] ) ;
		ppp >> root_go ;
	}

	/*************
         * start reading from randomset-file  
         ************/
	string s_sum_nties ;
	double sum_nties ;
	getline( *in, s_sum_nties ) ; // sum(NTIES^3 - NTIES)
	istringstream ntss( s_sum_nties.c_str() ) ;
	ntss >> sum_nties ;

	string groups, sites ;
	getline( *in, groups ) ; // GO IDs
	getline( *in, sites ) ; // sites

	if ( groups == "" || sites == "" ) {
		cerr << "Cant read Randomsets" << endl ;
		exit( 1 ) ;
	}

	int co_genes_per_group ;
	{
		istringstream cutoff( argv[3] ) ;
		cutoff >> co_genes_per_group ;
	}

	/*************
         * go_groups handles parsing and analysis of dataset and randset lines
         ************/
	go_groups gos( groups, sites, co_genes_per_group, root_go ) ;

	string data ;
	getline( *in, data ) ; // real data 

	// returns number of significant groups for 0.1, 0.05, 0.01, 0.001, 0.0001
	int *realdata = gos.calculate_data( data, sum_nties, profile ) ;
	out << endl << endl ;

	int sum_randdata[10] ;
	for ( int i=0 ; i < 10 ; ++i ) sum_randdata[i] = 0 ;
	int nr_groups_ge[10] ;
	for ( int i=0 ; i < 10 ; ++i ) nr_groups_ge[i] = 0 ;
	int num_randdata = 0 ;
	// randomsets
	while ( *in ) {
		getline( *in, data ) ;
		if ( data == "" ) { break ; } 
		int *randdata = gos.calculate_rand( data, sum_nties ) ;
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

	cout << "Randomsets: " << num_randdata << endl ;
	cout << "less\t\t\t\t\tgreater" << endl ;
	cout << "# sig. groups dataset" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		cout << realdata[i] << "\t" ;
	cout << endl ;
	cout << "# sig. groups mean randomsets" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		cout << sum_randdata[i]/static_cast<double>(num_randdata) << "\t" ;
	cout << endl ;
	cout << "# p value" << endl ;
	for ( int i = 0 ; i < 10 ; ++i ) 
		cout << nr_groups_ge[i]/static_cast<double>(num_randdata) << "\t" ;
	cout << endl ;
	
	// write outfile
	out << "Randomsets: " << num_randdata << endl ;
	out << "less\t\t\t\t\tgreater" << endl ;
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
