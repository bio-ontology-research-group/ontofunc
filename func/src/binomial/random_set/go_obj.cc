
#include "go_obj.h" 
#include <math.h>

using std::cerr;
using std::cout;
using std::endl;

go_obj::go_obj( string &name_ ) : name( name_ ), hka(0), cka(0), nr_genes(0)
{  }

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


void go_obj::add_hka( int x=1 ) 
{ 
	hka+=x ; 
} 

void go_obj::add_cka( int x=1 ) 
{ 
	cka+=x ; 
}

void go_obj::add_gene() 
{
	nr_genes++ ;
}

void go_obj::clear_ka(  ) 
{ 
	hka = 0 ; 
	cka = 0 ; 
} 

void go_obj::print_nr_genes( ostream &os ) 
{
	os << name << '\t' << nr_genes << endl ;
}

void go_obj::print_ka( ostream &os ) 
{ 
	os << hka << ' ' << cka << '\t' ; 
}

