
#include "go_obj.h" 
#include <math.h>
#include <cmath>
#define MATHLIB_STANDALONE
#include "../../include/Rmath.h"


using std::cerr;
using std::cout;
using std::endl;

#include "gene.h"

go_obj::go_obj( string &name_ ) : name( name_ ), prob_less_b(-1.), prob_less_a(-1.), prob_greater_b(-1.), prob_greater_a(-1.), sig(0), ignore(0)
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

void go_obj::start_refinement( double pval_b, double pval_a, double sumnties, int cutoff )
{
	
	double N = static_cast<double>(gens.size()) ;

	delete this->refine( pval_b, pval_a, N, sumnties, cutoff ) ;
}

void go_obj::set_ignore(  ) 
{
	ignore=1 ;
}

set<gene*>* go_obj::refine( double pval_b, double pval_a, double N, double sum_nties, int cutoff )
{
	
	double n = this->gens.size() ;

	double N2 = N - n ;

	// this group has less than cutoff genes
	if ( n < cutoff ) {
		sig=0 ;
		// this->print( cerr ) ;
		return new set<gene*> ;
	}
	
	double ranksum = 0. ;

	for ( set<gene*>::const_iterator it = gens.begin() ; it !=
		gens.end() ; ++it ) 
	{
		ranksum += (*it)->get_rank(  ) ;

//		cerr << (*it)->name << endl ;

	}
 
	double C = ranksum - ((n*(n+1.))/2.) ;
	double z = C - (n*N2*0.5) ;
	double sigma = sqrt( (n*N2/12.)*
			     ( (n+N2+1.) - sum_nties / 
				((n+N2)*(N2+n-1.)) 
			     )
			   ) ; 
	double corr = -0.5 ;
	prob_less_b = pnorm( (z-corr) / sigma, 0., 1., 1, 0 ) ;
	corr = 0.5 ;
	prob_greater_b  = 1. - pnorm( (z-corr) / sigma, 0., 1., 1, 0 ) ;


	// i am significant
	if ( prob_less_b < pval_b || prob_greater_b < pval_b ) {
		// return my sets.
		if ( childs.empty() ) {
			sig = 1 ;
			prob_less_a = prob_less_b ;
			prob_greater_a = prob_greater_b ;
			// this->print( cerr ) ;
			return new set<gene*>( gens ) ;
		// i have childs, ask them for their sets.
		} else {
			set<gene*>* child_sets = new set<gene*>;
			set<gene*>* my_set = new set<gene*>( gens ) ;
			for ( vector<go_obj*>::const_iterator it = childs.begin() ; 
				it != childs.end() ; ++it ) 
			{
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, N, sum_nties, cutoff ) ;
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
			double n = my_set->size() ;
			double N2 = N - n ;
			double ranksum = 0. ;
			for ( set<gene*>::const_iterator it = my_set->begin() ; it !=
				my_set->end() ; ++it ) 
			{
				ranksum += (*it)->get_rank() ;
			}
			double C = ranksum - ((n*(n+1.))/2.) ;
			double z = C - (n*N2*0.5) ;
			double sigma = sqrt( (n*N2/12.)*
					     ( (n+N2+1.) - sum_nties / 
						((n+N2)*(N2+n-1.)) 
					     )
					   ) ; 
			double corr = -0.5 ;
			prob_less_a = pnorm( (z-corr) / sigma, 0., 1., 1, 0 ) ;
			corr = 0.5 ;
			prob_greater_a  = 1. - pnorm( (z-corr) / sigma, 0., 1., 1, 0 ) ;

			if ( prob_less_a < pval_a || prob_greater_a < pval_a ) { // i am still significant, so return my values
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
				set<gene*>* cs = (*it)->refine( pval_b, pval_a, N, sum_nties, cutoff ) ;
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
	os << prob_less_b << "\t" << prob_greater_b << "\t" <<  prob_less_a << "\t" << prob_greater_a << endl ;
}

