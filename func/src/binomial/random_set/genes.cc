
#include "genes.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>
#define MAX_LINE_LENGTH 10000

genes::genes( go_graph &graph, istream &annotation, istream &data ) 
{
	srand( time(NULL) ) ;
	string line ;
	while ( annotation ) {
		getline( annotation, line ) ;
		istringstream is( line.c_str() ) ;
		string gene_name ;
		is >> gene_name ;

		if ( gene_name != "" ) {
			set<go_obj*> parents ;
			string go_name ;
			while ( is >> go_name ) {
				graph.get_parents( go_name, &parents ) ;
			}
			if ( parents.size() > 0 ) {
				genemap[gene_name] = new gene( gene_name, parents ) ;
			} else {
				cerr << gene_name << " not mapped.\n" ;
			}
		}
	}
	cerr << "Annotated " << genemap.size() << " genes." << endl ;
	
	while( data ) {
		getline( data, line ) ;
		istringstream is( line.c_str() ) ;
		string gene_name ;
		is >> gene_name ;

		if ( genemap.find( gene_name ) != genemap.end() ) {
		
			double hka ;
			is >> hka ;

			double cka ;
			is >> cka ;

			if ( cka ==0. && hka == 0. ) {
				delete genemap[gene_name] ;
				genemap.erase( gene_name ) ;
			} else {
				genemap[gene_name]->add_hka_cka( static_cast<int>(hka), static_cast<int>(cka)) ;
				gene_vec.push_back( genemap[gene_name] ) ;
			}
		}
	}
}

genes::~genes(  ) 
{
	for ( vector<gene*>::iterator it = gene_vec.begin() ; 
								it != gene_vec.end() ; ++it )  
	{
		delete *it ;
	}
}

#include <algorithm>

class c_prng {
	public:
		int operator()( int n ) { return( rand() % n ); } ;
} ;
void genes::create_random_set(  ) 
{
	c_prng rng ;
	random_shuffle( gene_vec.begin(), gene_vec.end(), rng ) ;

	int i = 0 ;

	for ( map<string,gene*>::const_iterator it = genemap.begin() ;
			it != genemap.end() ; ++it ) 
	{
		it->second->write_data_gos( gene_vec[i]->get_gos() ) ;
		i++ ;
	}
}
