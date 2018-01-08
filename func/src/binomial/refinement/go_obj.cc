
#include "go_obj.h" 
#include <math.h>
#define MATHLIB_STANDALONE
#include "../../include/Rmath.h"

using std::cerr;
using std::cout;
using std::endl;

#include "gene.h"

go_obj::go_obj( string &name_ ) : name( name_ ), cp_b(-1.), cp_a(-1.), hp_b(-1.), hp_a(-1.), sig(0)
{  
}

void go_obj::add_parent( go_obj* p )
{ 
	parents.push_back( p ) ; 
}

void go_obj::add_child( go_obj* p )
{ 
	childs.push_back( p ) ; 
}

void go_obj::get_parents( set<go_obj*> *parentset ) 
{
	parentset->insert( parentset->begin(), this ) ;
	for ( vector<go_obj*>::const_iterator it = parents.begin() ;
			it != parents.end() ; ++it ) 
	{
		(*it)->get_parents( parentset ) ;
	}
}

void go_obj::add_gene( gene* g ) 
{
	gens.insert( gens.begin(), g ) ;
}

void go_obj::start_refinement( double pval_b, double pval_a, int cutoff )
{
	
	int cka=0 ;
	int hka=0 ;
	for ( set<gene*>::const_iterator it = gens.begin() ; it !=
		gens.end() ; ++it ) 
	{
		cka += (*it)->cka ;
		hka += (*it)->hka ;
	}

	double p_binom = static_cast<double>(cka)/
				static_cast<double>(cka+hka) ;

	delete this->refine( pval_b, pval_a, p_binom, cutoff ) ;
}

set<gene*>* go_obj::refine( double pval_b, double pval_a, double p_binom, int cutoff )
{

	if ( gens.size() < cutoff ) {
		sig=0 ;
		// this->print( cerr ) ;
		return new set<gene*> ;
	}

	int cka=0 ;
	int hka=0 ;
	for ( set<gene*>::const_iterator it = gens.begin() ; it !=
		gens.end() ; ++it ) 
	{
		cka += (*it)->cka ;
		hka += (*it)->hka ;

	}

	if ( cka == 0 && hka == 0 ) {
		sig=0 ;
		return new set<gene*> ;
	}

//	cerr << cka << " " << hka << endl ;

 
	cp_b = pbinom( cka-1, cka+hka, p_binom, 0, 0 ) ;
	hp_b = pbinom( hka-1, cka+hka, (1.-p_binom), 0, 0 ) ;

// cerr << name << " " << cka << " " << hka << " " << cp_b << " " << hp_b << endl ;


	// i am significant
	if ( cp_b < pval_b || hp_b < pval_b ) {
		// return my sets.
		if ( childs.empty() ) {
			sig = 1 ;
			cp_a = cp_b ;
			hp_a = hp_b ;
			// this->print( cerr ) ;
			return new set<gene*>( gens ) ;
		// i have childs, ask them for their sets.
		} else {
			set<gene*>* child_sets = new set<gene*>;
			set<gene*>* my_set = new set<gene*>( gens ) ;
			for ( vector<go_obj*>::const_iterator it = childs.begin() ; 
				it != childs.end() ; ++it ) 
			{
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, p_binom, cutoff ) ;
				for ( set<gene*>::const_iterator it2 = cs->begin() ; 
					it2 != cs->end() ; ++it2 ) 
				{
					// kill child entrys from my set
					set<gene*>::iterator del = my_set->find( *it2 ) ;
					if ( del != my_set->end() ) my_set->erase( del ) ;
					// insert to child sets 
					child_sets->insert( child_sets->begin(), *it2 ) ;
				}
				delete cs ;
			}
			// calculate my values
			int cka=0 ;
			int hka=0 ;
			for ( set<gene*>::const_iterator it = my_set->begin() ; it !=
				my_set->end() ; ++it ) 
			{
				cka += (*it)->cka ;
				hka += (*it)->hka ;
			}
			cp_a = pbinom( cka-1, cka+hka, p_binom, 0, 0 ) ;
			hp_a = pbinom( hka-1, cka+hka, (1.-p_binom), 0, 0 ) ;
			
			if ( cp_a < pval_a || hp_a < pval_a ) { // i am still significant, so return my values
				sig = 1 ;
				delete my_set ;
				delete child_sets ;
				// this->print( cerr ) ;
				return new set<gene*>( gens ) ;
			} else { // i am not significant, so return childsets
				sig = 0 ;
				delete my_set ;
				// this->print( cerr ) ;
				return child_sets ;
			}
		}
	} else { // not significant
		if ( childs.empty() ) { // no childs -> return empty set
			sig=0 ;
			// this->print( cerr ) ;
			return new set<gene*> ;
		} else { // return child_sets
			sig=0 ;
			set<gene*>* child_sets = new set<gene*>;
			for ( vector<go_obj*>::const_iterator it = childs.begin() ; 
				it != childs.end() ; ++it ) 
			{
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, p_binom, cutoff ) ;
				for ( set<gene*>::const_iterator it2 = cs->begin() ; 
					it2 != cs->end() ; ++it2 ) 
				{
					// insert to child sets 
					child_sets->insert( child_sets->begin(), *it2 ) ;
				}
				delete cs ;
			}
			return child_sets ;
		}
	}
}

void go_obj::print( ostream &os ) {
	os << name << "\t" ;
	if ( sig ) os << "+" << "\t" ;
	else os << "-" << "\t" ;
	os << cp_b << "\t" << hp_b << "\t" << cp_a << "\t" << hp_a << endl ;
}

