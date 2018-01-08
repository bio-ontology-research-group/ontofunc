
#include "go_obj.h" 
#include <math.h>

using std::cerr;
using std::cout;
using std::endl;

#include "gene.h"

go_obj::go_obj( string &name_ ) : name( name_ ), p_b_1(-1.), p_a_1(-1.), p_b_2(-1.), p_a_2(-1.), sig(0)
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
	delete this->refine( pval_b, pval_a, cutoff ) ;
}

#include "../../include/numeric.h"

set<gene*>* go_obj::refine( double pval_b, double pval_a, int cutoff )
{
	if ( gens.size() < static_cast<unsigned int>(cutoff) ) {
		sig=0 ; 
		return new set<gene*> ;
	}

	int ch_s=0 ;
	int ch_ns=0 ;
	int hh_s=0 ;
	int hh_ns=0 ;
	for ( set<gene*>::const_iterator it = gens.begin() ; it !=
		gens.end() ; ++it ) 
	{
		ch_s += (*it)->ch_s ;
		ch_ns += (*it)->ch_ns ;
		hh_s += (*it)->hh_s ;
		hh_ns += (*it)->hh_ns ;
	}
 
	if ( hh_s == 0 && ch_s == 0 ) {
		if ( hh_ns < ch_ns ) 
			p_b_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
		else p_b_1 = 1. ;
	} else if ( hh_s == 0 ) p_b_1 = 1. ;
	else if ( ch_s == 0 ) 
		p_b_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
	else if ( static_cast<double>(hh_ns)/static_cast<double>(hh_s) >=
		static_cast<double>(ch_ns)/static_cast<double>(ch_s) ) 
	  p_b_1 = 1. ;
	else 
	  p_b_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;

	if ( hh_s == 0 && ch_s == 0 ) {
		if ( hh_ns > ch_ns ) p_b_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
		else p_b_2 = 1. ;
	} else if ( ch_s == 0 ) p_b_2 = 1. ;	
	else if ( hh_s == 0 ) p_b_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
	else if ( static_cast<double>(ch_ns)/static_cast<double>(ch_s) 
		>= static_cast<double>(hh_ns)/static_cast<double>(hh_s) ) 
	   p_b_2 = 1. ;
	else 
	   p_b_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;


	// i am significant
	if ( p_b_1 < pval_b || p_b_2 < pval_b ) {
		// return my sets.
		if ( childs.empty() ) {
			sig = 1 ;
			p_a_1 = p_b_1 ;
			p_a_2 = p_b_2 ;
		//	this->print( cerr ) ;
			return new set<gene*>( gens ) ;
		// i have childs, ask them for their sets.
		} else {
			set<gene*>* child_sets = new set<gene*>;
			set<gene*>* my_set = new set<gene*>( gens ) ;
			for ( vector<go_obj*>::const_iterator it = childs.begin() ; 
				it != childs.end() ; ++it ) 
			{
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, cutoff ) ;
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
			// calculate my new values
			int ch_s=0 ;
			int ch_ns=0 ;
			int hh_s=0 ;
			int hh_ns=0 ;
			for ( set<gene*>::const_iterator it = my_set->begin() ; it !=
				my_set->end() ; ++it ) 
			{
				ch_s += (*it)->ch_s ;
				ch_ns += (*it)->ch_ns ;
				hh_s += (*it)->hh_s ;
				hh_ns += (*it)->hh_ns ;
			}
			if ( hh_s == 0 && ch_s == 0 ) {
				if ( hh_ns < ch_ns ) 
					p_a_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
				else p_a_1 = 1. ;
			} else if ( hh_s == 0 ) p_a_1 = 1. ;
			else if ( ch_s == 0 ) 
				p_a_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
			else if ( static_cast<double>(hh_ns)/static_cast<double>(hh_s) >=
				static_cast<double>(ch_ns)/static_cast<double>(ch_s) ) 
			  p_a_1 = 1. ;
			else 
			  p_a_1 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;

			if ( hh_s == 0 && ch_s == 0 ) {
				if ( hh_ns > ch_ns ) p_a_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
				else p_a_2 = 1. ;
			} else if ( ch_s == 0 ) p_a_2 = 1. ;	
			else if ( hh_s == 0 ) p_a_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;
			else if ( static_cast<double>(ch_ns)/static_cast<double>(ch_s) 
				>= static_cast<double>(hh_ns)/static_cast<double>(hh_s) ) 
			   p_a_2 = 1. ;
			else 
			   p_a_2 = fisher_chi2( ch_s, ch_ns, hh_s, hh_ns ) ;


			if ( p_a_1 < pval_a || p_a_2 < pval_a ) { // i am still significant, so return my values
				sig = 1 ;
				delete my_set ;
				delete child_sets ;
			//	this->print( cerr ) ;
				return new set<gene*>( gens ) ;
			} else { // i am not significant, so return childsets
				sig = 0 ;
				delete my_set ;
			//	this->print( cerr ) ;
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
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, cutoff ) ;
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
	os << p_b_1 << "\t" << p_b_2 << "\t" << p_a_1 << "\t" << p_a_2 << endl ;
}

