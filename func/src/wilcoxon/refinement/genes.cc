
#include "genes.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
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
	
	multimap<double, gene*> genes_ranked ;

	// skip header 
//	getline( data, line ) ;
	while( data ) {
		getline( data, line ) ;
		istringstream is( line.c_str() ) ;
		string gene_name ;
		is >> gene_name ;

		if ( genemap.find( gene_name ) != genemap.end() ) {
		
			double data ;
			is >> data ;

			gene_vec.push_back( genemap[gene_name] ) ;
			genes_ranked.insert(
				pair<double, gene*>(data,genemap[gene_name]) ) ;
		}
	}
	// rank
	int i = 1 ;
	sum_nties = 0. ;
	for ( multimap< double, gene* >::iterator it = genes_ranked.begin() ;
			it != genes_ranked.end() ;  ) 
	{
		int equal = genes_ranked.count( it->first ) ;
		if ( equal > 1 ) {
			sum_nties += pow(static_cast<double>(equal), 3.)
					-static_cast<double>(equal) ;
			double rank = static_cast<double>(i) + 
					static_cast<double>(equal-1)/2. ;
			for ( int i2 = 0 ; i2 < equal ; i2++ ) {
				it->second->set_rank( rank ) ;
				i++ ;
				++it ;
			}
		} else {
			it->second->set_rank( i ) ;
			i++ ;
			++it ;
		}
	}
	for ( map<string,gene*>::const_iterator it = genemap.begin() ;
			it != genemap.end() ; ++it ) 
	{
		it->second->write_to_gos( ) ;
	}
}

genes::~genes(  ) 
{
	for ( map<string,gene*>::const_iterator it = genemap.begin() ;
			it != genemap.end() ; ++it ) 
	{
		delete it->second ;
	}
}

