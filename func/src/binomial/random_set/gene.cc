
#include "gene.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

void gene::add_hka_cka( int hka_, int cka_ ) {
	
	hka = hka_ ;
	cka = cka_ ;

	for ( set<go_obj*>::const_iterator it = gos.begin() ;
			it != gos.end() ; ++it ) 
	{
		(*it)->add_cka( cka ) ;
		(*it)->add_hka( hka ) ;
		(*it)->add_gene(  ) ;
	}
}

void gene::write_data_gos( set<go_obj*>* gos_ ) 
{
	for ( set<go_obj*>::const_iterator it = gos_->begin() ;
			it != gos_->end() ; ++it ) 
	{
		(*it)->add_cka( cka ) ;
		(*it)->add_hka( hka ) ;
	}
	
}

set<go_obj*>* gene::get_gos( ) 
{
	return &gos ;
}

