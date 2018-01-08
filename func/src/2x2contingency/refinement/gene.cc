
#include "gene.h"

void gene::add( int ch_s_, int ch_ns_, int hh_s_, int hh_ns_ ) {
	ch_s = ch_s_ ;
	ch_ns = ch_ns_ ;
	hh_s = hh_s_ ;
	hh_ns = hh_ns_ ;
}

gene::gene( string name_, set<go_obj*> &gos ) : name( name_ ), ch_s(0), ch_ns(0), hh_s(0), hh_ns(0) 
{
	for ( set<go_obj*>::const_iterator it = gos.begin() ;
		it != gos.end() ; ++it )
	{
		(*it)->add_gene( this ) ;
	}
}

