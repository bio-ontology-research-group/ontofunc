
#include "go_obj.h" 
#include <math.h>

using std::cerr;
using std::cout;
using std::endl;

go_obj::go_obj( string &name_ ) : name( name_ ), nr_genes(0)
{  
	ch_s = 0 ;
	ch_ns = 0 ;
	hh_s = 0 ;
	hh_ns = 0 ;
}

void go_obj::add_parent( go_obj* p )
{ parents.push_back( p ) ; }

void go_obj::get_parents( set<go_obj*> *parentset ) {
	parentset->insert( parentset->begin(), this ) ;
	for ( vector<go_obj*>::const_iterator it = parents.begin() ;
			it != parents.end() ; ++it ) 
	{
		(*it)->get_parents( parentset ) ;
	}
}

void go_obj::add( int ch_s_, int ch_ns_, int hh_s_, int hh_ns_ ) {
	ch_s += ch_s_ ;
	ch_ns += ch_ns_ ;
	hh_s += hh_s_ ;
	hh_ns += hh_ns_ ;
}


void go_obj::add_gene() 
{
	nr_genes++ ;
}


void go_obj::clear(  ) 
{ 
	ch_s = 0 ;
	ch_ns = 0 ;
	hh_s = 0 ;
	hh_ns = 0 ;
} 

void go_obj::print_nr_genes( ostream &os ) 
{
	os << name << '\t' << nr_genes << endl ;
}

void go_obj::print( ostream &os ) 
{ 
	os << ch_s << ' ' << ch_ns << ' ' << hh_s << ' ' << hh_ns << '\t' ; 
}

