
#include "go_groups.h"
#include <cmath>
#include <map> 
#include <fstream>
#define MATHLIB_STANDALONE
#include "../../include/Rmath.h"


using namespace std ;

// groups = names of groups, space seperated in string
// co = cutoff in number of genes
// in = number genes per go group
go_groups::go_groups( string &groups, istream *in, int co, string root_go ) 
{
	map<string, bool> groups_to_check ;
	if ( in ) {
		while ( *in ) {
			string go_gr ;
			*in >> go_gr ;
			int num_genes ;
			*in >> num_genes ;
			groups_to_check[go_gr] = (num_genes<co)?0:1 ;
		}	
	}

	istringstream is( groups.c_str() ) ;
	string name ;
	while ( is >> name ) {
		names.push_back( name ) ;
		check.push_back( groups_to_check[name] ) ;
		if ( name == root_go ) root_idx = names.size() - 1 ; 
	}
	cerr << "GOs: " << names.size() << endl ;
}

// data = left and right values whitespace separated for each group
int* go_groups::calculate_data( string &data, ostream *os ) 
{
	istringstream is( data.c_str() ) ;
	int i = -1 ;
	int *ret = new int[10] ;
	for ( int i=0 ; i<10 ; ++i ) {
		ret[i] = 0 ;
	}

	vector<int> human_ka ;
	vector<int> chimp_ka ;

	while ( is ) {
		i++ ;
		int c, h ;
		is >> c ;
		is >> h ;

		human_ka.push_back( h ) ;
		chimp_ka.push_back( c ) ;

	}

	// average for binomial test
	double p_binom = static_cast<double>(chimp_ka[root_idx])/
				static_cast<double>(chimp_ka[root_idx]+human_ka[root_idx]) ;

	// for overall_significance
	data_pvals_c.resize( names.size() ) ;
	data_pvals_h.resize( names.size() ) ;

	multiset<double> pvals_c, pvals_h ;

	for( unsigned int i = 0 ; i < names.size() ; ++i ) {

		data_pvals_c[i] = -1. ;
		data_pvals_h[i] = -1. ;

		int c = chimp_ka[i] ;
		int h = human_ka[i] ;

		if ( (c == 0) && (h == 0) ) continue ;
		if ( check[i] == 0 ) continue ;
		
		double pro_c = pbinom( c-1, c+h, p_binom, 0, 0 ) ;
		double pro_h = pbinom( h-1, c+h, (1.-p_binom), 0, 0 ) ;

		data_pvals_c[i] = pro_c ;
		data_pvals_h[i] = pro_h ;

		pvals_c.insert( pro_c ) ;
		pvals_h.insert( pro_h ) ;


		if ( os ) {
			*os << names[i] << "\t" 
			   << c << "\t"
			   << h << "\t" << endl ;
		}

		if ( pro_c < 0.1 ) {
			ret[0]++ ;
			if ( pro_c < 0.05 ) {
				ret[1]++ ;
				if ( pro_c < 0.01 ) {
					ret[2]++ ;
					if ( pro_c < 0.001 ) {
						ret[3]++ ;
						if ( pro_c < 0.0001 ) {
							ret[4]++ ;
						}
					}
				}
			}
		}
		if ( pro_h < 0.1 ) {
			ret[5]++ ;
			if ( pro_h < 0.05 ) {
				ret[6]++ ;
				if ( pro_h < 0.01 ) {
					ret[7]++ ;
					if ( pro_h < 0.001 ) {
						ret[8]++ ;
						if ( pro_h < 0.0001 ) {
							ret[9]++ ;
						}
					}
				}
			}
		}
	}
	c_sig.add_set( pvals_c ) ;
	h_sig.add_set( pvals_h ) ;
	return ret ;
}
int* go_groups::calculate_rand( string &data, ostream *os ) 
{
	istringstream is( data.c_str() ) ;
	int i = -1 ;
	int *ret = new int[10] ;
	for ( int i=0 ; i<10 ; ++i ) {
		ret[i] = 0 ;
	}

	vector<int> human_ka ;
	vector<int> chimp_ka ;

	while ( is ) {
		i++ ;
		int c, h ;
		is >> c ;
		is >> h ;

		human_ka.push_back( h ) ;
		chimp_ka.push_back( c ) ;

	}

	// average for binomial test
	double p_binom = static_cast<double>(chimp_ka[root_idx])/
				static_cast<double>(chimp_ka[root_idx]+human_ka[root_idx]) ;

	multiset<double> pvals_c, pvals_h ;

	for( unsigned int i = 0 ; i < names.size() ; ++i ) {

		int c = chimp_ka[i] ;
		int h = human_ka[i] ;

		if ( (c == 0) && (h == 0) ) continue ;
		if ( check[i] == 0 ) continue ;
		
		double pro_c = pbinom( c-1, c+h, p_binom, 0, 0 ) ;
		if ( ISNAN(pro_c) ) pro_c = 1. ; // happens if c < 0 
		double pro_h = pbinom( h-1, c+h, (1.-p_binom), 0, 0 ) ;
		if ( ISNAN(pro_h) ) pro_h = 1. ;

		pvals_c.insert( pro_c ) ;
		pvals_h.insert( pro_h ) ;

		if ( os ) {
			*os << names[i] << "\t" 
			   << c << "\t"
			   << h << "\t"
			   << pro_c << "\t"
			   << pro_h << endl ;
		}

		if ( pro_c < 0.1 ) {
			ret[0]++ ;
			if ( pro_c < 0.05 ) {
				ret[1]++ ;
				if ( pro_c < 0.01 ) {
					ret[2]++ ;
					if ( pro_c < 0.001 ) {
						ret[3]++ ;
						if ( pro_c < 0.0001 ) {
							ret[4]++ ;
						}
					}
				}
			}
		}
		if ( pro_h < 0.1 ) {
			ret[5]++ ;
			if ( pro_h < 0.05 ) {
				ret[6]++ ;
				if ( pro_h < 0.01 ) {
					ret[7]++ ;
					if ( pro_h < 0.001 ) {
						ret[8]++ ;
						if ( pro_h < 0.0001 ) {
							ret[9]++ ;
						}
					}
				}
			}
		}
	}
	
	// for family wise error rate
	smallest_rand_p_c.insert( *(pvals_c.begin()) ) ;
	smallest_rand_p_h.insert( *(pvals_h.begin()) ) ;

	c_sig.add_set( pvals_c ) ;
	h_sig.add_set( pvals_h ) ;
	return ret ;
}


void go_groups::print_pvals( int nr_randsets, ostream &os ) {
	
	// vector<double> *fdr_q_c = c_sig.fdr_qvals( 0 ) ; 
	// vector<double> *fdr_q_h = h_sig.fdr_qvals( 0 ) ; 
	map<double,double> *fdr_q_c = c_sig.fdr_qvals( 0 ) ; 
	map<double,double> *fdr_q_h = h_sig.fdr_qvals( 0 ) ; 

	for( unsigned int i = 0 ; i < names.size() ; ++i ) {
		if ( check[i] == 1 ) { 
			int n_c = 0 ; 
			multiset<double>::const_iterator it = smallest_rand_p_c.begin() ;
			while ( it != smallest_rand_p_c.end() && 
				*it <= data_pvals_c[i] ) 
					n_c++, it++ ;
			int n_h = 0 ;
			it = smallest_rand_p_h.begin() ;
			while ( it != smallest_rand_p_h.end() && 
				*it <= data_pvals_h[i] ) 
					n_h++, it++ ;
			os << names[i] << "\t" << data_pvals_c[i] << "\t"
				<< data_pvals_h[i] << "\t" 
				<< static_cast<double>(n_c)/
				   static_cast<double>(nr_randsets) << "\t" 
				<< static_cast<double>(n_h)/
				   static_cast<double>(nr_randsets) << "\t" 
				<< (*fdr_q_c)[data_pvals_c[i]] << "\t" 
				<< (*fdr_q_h)[data_pvals_h[i]] << endl ;
				// << (*fdr_q_c)[c_sig.index_for_pval(data_pvals_c[i])] << "\t" 
				// << (*fdr_q_h)[h_sig.index_for_pval(data_pvals_h[i])] << endl ;
		}
	} 

	delete fdr_q_c ;
	delete fdr_q_h ;

	os << endl << endl 
           << "global-test-statistics (0 - 0.05): " << endl 
	   << c_sig.significance( 0, 0.05 ) << "\t" << h_sig.significance( 0, 0.05 ) << endl ;
	os << endl ;

	vector<double> *fdr_c = c_sig.fdr( 0 ) ; 
	vector<double> *fdr_h = h_sig.fdr( 0 ) ; 

	os << "FDR" << endl ;
	os << "0.1\t0.05\t0.01\t0.001\t0.0001\t0.1\t0.05\t0.01\t0.001\t0.0001" <<  endl ;
	for ( int i = 0 ; i < 5 ; i++ ) os << (*fdr_c)[i] << "\t" ;
	for ( int i = 0 ; i < 5 ; i++ ) os << (*fdr_h)[i] << "\t" ;
	os << endl ;
	os << endl ;

}
