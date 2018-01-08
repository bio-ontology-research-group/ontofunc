
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "go_groups.h"

int main( int argc, char *argv[] )
{
	if ( argc != 7 ) {
		cerr << "Usage " << argv[0] << " randset outfile genes_per_go_file cutoff GO:ID profile" << endl ;
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

	ofstream *profile = new ofstream( argv[6] ) ;
	if ( ! *profile ) profile = 0 ;

	string root_go ;
	{ 
		istringstream ppp( argv[5] ) ;
		ppp >> root_go ;
	}

	/*************
         * start reading from randomset-file  
         ************/
	string groups ;
	getline( *in, groups ) ; // GO IDs
	if ( groups == "" ) {
		cerr << "Error reading random sets" << endl ;
		exit( 1 ) ;
	}

	ifstream in_genespergroup( argv[3] ) ;
	if ( ! in_genespergroup ) {
		cerr << "Cannot open " << argv[3] << endl ;
	}

	int co_genes_per_group ;
	{
		istringstream cutoff( argv[4] ) ;
		cutoff >> co_genes_per_group ;
	}

	/*************
         * go_groups handles parsing and analysis of dataset and randset lines
         ************/
	go_groups gos( groups, &in_genespergroup, co_genes_per_group, root_go ) ;

	string data ;
	getline( *in, data ) ; // real data 

	// returns number of significant groups for 0.1, 0.05, 0.01, 0.001
	int *realdata = gos.calculate_data( data, profile ) ;

	int sum_randdata[10] ;
	for ( int i=0 ; i < 10 ; ++i ) sum_randdata[i] = 0 ;
	int nr_groups_ge[10] ;
	for ( int i=0 ; i < 10 ; ++i ) nr_groups_ge[i] = 0 ;
	int num_randdata = 0 ;
	// randomsets
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
	cout << "Randomsets: " << num_randdata << endl ;
	cout << "chimp\t\t\t\thuman" << endl ;
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

//        gos.print_pvals( num_randdata, cout ) ;
        gos.print_pvals( num_randdata, out ) ;
	
	// write outfile
	out << "Randomsets: " << num_randdata << endl ;
	out << "chimp\t\t\t\thuman" << endl ;
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
