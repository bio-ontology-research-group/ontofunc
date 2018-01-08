
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
		
			double ch_s ;
			is >> ch_s ;

			double ch_ns ;
			is >> ch_ns ;

			double hh_s ;
			is >> hh_s ;

			double hh_ns ;
			is >> hh_ns ;

			genemap[gene_name]->add( static_cast<int>(ch_s),
			 			static_cast<int>(ch_ns), 
						static_cast<int>(hh_s),
					 	static_cast<int>(hh_ns) ) ;
			genevec.push_back( genemap[gene_name] ) ;
		}
	}
}

genes::~genes(  ) 
{
	for ( map<string, gene*>::iterator it = genemap.begin() ; 
								it != genemap.end() ; ++it )  
		delete it->second ;
}

#include <algorithm>

class c_prng {
	public:
		int operator()( int n ) { return( rand() % n ); } ;
} ;

void genes::create_random_set(  ) 
{
	c_prng rng ;
        random_shuffle( genevec.begin(), genevec.end(), rng ) ;

        int i = 0 ;

        for ( map<string,gene*>::const_iterator it = genemap.begin() ;
                        it != genemap.end() ; ++it )
        {
                it->second->write_data_gos( genevec[i]->get_gos() ) ;
                i++ ;
        }
}

