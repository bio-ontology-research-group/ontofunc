
#include "gene.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

void gene::add_hka_cka( int hka_, int cka_ ) {
	hka = hka_ ;
	cka = cka_ ;
}

gene::gene( string name_, set<go_obj*> &gos ): name(name_) 
{
	for ( set<go_obj*>::const_iterator it = gos.begin() ; it != gos.end() ;
			++it ) 
	{
		(*it)->add_gene( this ) ;
	}
}
