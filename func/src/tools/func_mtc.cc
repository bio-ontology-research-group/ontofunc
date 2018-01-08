#include <iostream>
#include <fstream> 
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include "../common/overall_sign.h" 


using namespace std ;

void usage( char *cmd, int err ) {
	cerr << "Usage: " << cmd << " [tsv-file]" << endl ;
	exit( err ) ;
}

int main( int argc, char *argv[] )
{
	if ( argc != 2 ) {
		usage( argv[0],  2 ) ;
	}

	ifstream tsvfile( argv[1] ) ;
	if ( ! tsvfile ) {
		cerr << "Cannot open file `" << argv[1] << "'" << endl ;
		usage( argv[0], 3 ) ;
	}

	// check #columns by counting through descr line ;
	int nr_fields = 0 ;
	{
		string descr ;
		getline( tsvfile, descr ) ;
		istringstream desc_iss( descr ) ;
		while( !desc_iss.eof() && desc_iss.ignore( 1000000, '\t' ) ) 
			nr_fields++ ;
	}

	vector<double> smallest( nr_fields-6, 1. ) ;
	vector<double> data_pval_vec ;
	vector<vector<double> > rand_pval_vec( nr_fields-6, vector<double>() ) ;
	
	int l=1 ;
	try {
	    while ( tsvfile ) {
		l++ ;
		string isss ;
		getline( tsvfile, isss ) ;
		if ( tsvfile.eof() ) break ;
		istringstream iss( isss ) ;
		
		string go_id ;
		getline( iss, go_id, '\t' ) ;

		string taxonomy ;
		getline( iss, taxonomy, '\t' ) ;

		string desc ;
		getline( iss, desc, '\t' ) ;

		// nr genes in root node, nr genes in group
		string dummy ;
		getline( iss, dummy, '\t' ) ;
		getline( iss, dummy, '\t' ) ;
		
		double data_pval ;
		iss >> data_pval ;
		data_pval_vec.push_back( data_pval ) ;

		for ( int i= 0 ; i < nr_fields-6 ; ++i ) {
			double rand_pval ;
			iss >> rand_pval ;
			rand_pval_vec[i].push_back( rand_pval ) ;
			if ( rand_pval < smallest[i] ) {
				smallest[i] = rand_pval ;
			}
		}
		if ( iss.fail() ) { cerr << isss << endl ; throw( int(0) ) ; }
	    }
	} catch(...) {
		cerr << "Error in line " << l <<  endl ;
		exit( 256 ) ;
	}

	// FWER 
	multiset<double> smallest_rand_pvals ;
	smallest_rand_pvals.insert( smallest.begin(), smallest.end() ) ;
	vector<double> fwer ;
	for ( vector<double>::const_iterator it = data_pval_vec.begin() ;
			it != data_pval_vec.end() ; ++it ) {
		int nr_le = 0 ;
		multiset<double>::const_iterator it2 = smallest_rand_pvals.begin() ;
		while( it2 != smallest_rand_pvals.end() && (*it2) <= (*it) ) 
			nr_le++, it2++ ;
		fwer.push_back( static_cast<double>(nr_le)/
				static_cast<double>(nr_fields-6) ) ;
	}

	// global test statistics
	overall_significance os ;
	multiset<double> datams ; 
	datams.insert( data_pval_vec.begin(), data_pval_vec.end() ) ;
	os.add_set( datams ) ;
	for ( vector<vector<double> >::iterator it=rand_pval_vec.begin() ;
			it != rand_pval_vec.end() ; ++it ) {
		datams.clear() ;
		datams.insert( it->begin(), it->end() ) ;
		os.add_set( datams ) ;
		it->clear() ;
	}
	double global_p = os.significance( 0, 0.05 ) ;
//	cerr << "Global P = " << global_p << endl ;

	// FDR
	map<double,double> *fdr_qvals = os.fdr_qvals( 0 ) ;
	vector<double> fdr ;
	for ( vector<double>::const_iterator it = data_pval_vec.begin() ;
			it != data_pval_vec.end() ; ++it ) {
		fdr.push_back( (*fdr_qvals)[*it] ) ;
	}
	tsvfile.close(  ) ;
	ifstream tsvfile2( argv[1] ) ;
	string s ;
	getline( tsvfile2, s ) ;
	cout << s << "\tFWER\tFDR\tglobal-p" << endl ;
	
	int i = 0 ;
	while( tsvfile2 ) {
		string s ;
		getline( tsvfile2, s ) ;
		if ( s == "" ) break ;
		cout << s << '\t' << fwer[i] << '\t' << fdr[i] << '\t' << global_p << endl ;
		i++ ;
	}
	tsvfile2.close(  ) ;
}
