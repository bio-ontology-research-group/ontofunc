/*
 * FUNC - Functional Analysis of Gene Expression Data
 * Copyright (C) 2002  Bjoern Muetzel, Kay Pruefer
 * 
 * This program is modifiable/redistributable under the terms of the
 * GNU General Public License.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */




#include "go_obj.h" 
#include <cmath>
#define MATHLIB_STANDALONE
#include "../../include/Rmath.h"

using std::cerr;
using std::cout;
using std::endl;

go_obj::go_obj( string &name_ ) : name( name_ ), significant( 0 ), 
		lsig1( -1. ), rsig1( -1. ), lsig2( -1. ), rsig2( -1. )
{  }

void go_obj::add_parent( go_obj* p )
{ parents.push_back( p ) ; }

void go_obj::add_child( go_obj* p )
{ childs.push_back( p ) ; }

void go_obj::add_changed( string &s ) 
{
	changed.insert( changed.begin(), s ) ;
	for ( vector<go_obj*>::const_iterator it=parents.begin() ; 
				it != parents.end() ; ++it ) 
	{
		(*it)->add_changed( s ) ;
	}
}

void go_obj::add_detected( string &s ) 
{
	detected.insert( detected.begin(), s ) ;
	for ( vector<go_obj*>::const_iterator it=parents.begin() ; 
				it != parents.end() ; ++it ) 
	{
		(*it)->add_detected( s ) ;
	}
}

void go_obj::print( ostream &os ) 
{
	if ( significant ) os << name << "\t+\t"
		<< lsig1 << "\t" << rsig1 << "\t" << lsig2 << "\t" << rsig2 << endl ;
	else os << name << "\t-\t"
		<< lsig1 << "\t" << rsig1 << "\t" << lsig2 << "\t" << rsig2 << endl ;

}

// #include "pval_bjoern.h"
extern double leftpval ;
extern double rightpval ;
extern double refin_leftpval ;
extern double refin_rightpval ;

// n = changed in root-go
// N = detected in root-go
go_obj::return_sets go_obj::refine( int n, int N, int cutoff ) 
{

	// cutoff: detected genes = all genes in group 
	if ( detected.size() < cutoff ) {
		significant = 0 ;
		return_set_changed.clear() ;
		return_set_detected.clear() ;
		return_sets ret ;
		ret.detected = &return_set_detected ;
		ret.changed = &return_set_changed ;
		return ret ;
	}


	// check whether i am significant...

	lsig1 = phyper( changed.size(), n, N-n, detected.size(), 1, 0 ) ;
	rsig1 = phyper( static_cast<double>(changed.size())-1., n, N-n, detected.size(), 0, 0 ) ;

	significance_1 = (lsig1<=leftpval) ? lsig1 : rsig1 ;
	
	// if i am significant
	if ( (rsig1 <= rightpval) || (lsig1 <= leftpval) )
	{
		significant = 1 ;
		// i have no children: return my sets
		if ( childs.empty() ) {
			rsig2 = rsig1 ;
			lsig2 = lsig1 ;
			return_sets ret ;
			ret.detected = &detected ;
			ret.changed = &changed ;
			return ret ;
			
		// i have children
		} else {

			vector<return_sets> childsets ;
			// ask all childrens for their sets
			for ( vector<go_obj*>::const_iterator it = childs.begin() ;
					it != childs.end() ; ++it )
				childsets.push_back( (*it)->refine(n, N, cutoff) ) ;
				
			// difference from my sets
			set<string> det = detected ;
			set<string> chang = changed ;
			for ( vector<return_sets>::const_iterator it = childsets.begin() ;
				it != childsets.end() ; ++it ) 
			{
				for ( set<string>::const_iterator it2 =
					it->changed->begin() ; it2 != it->changed->end() ; ++it2 )
				{
					set<string>::iterator del = chang.find( *it2 ) ;
					if ( del != chang.end() ) chang.erase( del ) ;
				}
				for ( set<string>::const_iterator it2 =
					it->detected->begin() ; it2 != it->detected->end() ; ++it2 )
				{
					set<string>::iterator del = det.find( *it2 ) ;
					if ( del != det.end() ) det.erase( del ) ;
				}
			}

			lsig2 = phyper( chang.size(), n, N-n, det.size(), 1, 0 ) ;
			rsig2 = phyper( static_cast<double>(chang.size())-1., n, N-n, det.size(), 0, 0 ) ;
			significance_2 = (lsig2<=refin_leftpval) ? lsig2 : rsig2 ;
			
			// still significant?
			if ( (rsig2 <= refin_rightpval) || (lsig2 <= refin_leftpval) )
			{
				// yes: return my sets
				return_sets ret ;
				ret.detected = &detected ;
				ret.changed = &changed ;
				return ret ;
			} else {
				// no: return children sets
				significant = 0 ;
				return_set_changed.clear() ;
				return_set_detected.clear() ;
				for ( vector<return_sets>::const_iterator it = childsets.begin() ;
					it != childsets.end() ; ++it ) 
				{
					return_set_changed.insert( // return_set_changed.begin(), 
											it->changed->begin(),
											it->changed->end() ) ;
					return_set_detected.insert( // return_set_detected.begin(), 
											it->detected->begin(),
											it->detected->end() ) ;
				}
				return_sets ret ;
				ret.detected = &return_set_detected ;
				ret.changed = &return_set_changed ;
				return ret ;
			}
		}
	// i am not significant
	} else {
		significant = 0 ;
		// i have children: return sets of children
		if ( ! childs.empty() ) 
		{
			vector<return_sets> childsets ;
			for ( vector<go_obj*>::const_iterator it = childs.begin() ;
					it != childs.end() ; ++it )
				childsets.push_back( (*it)->refine(n, N, cutoff) ) ;

			return_set_changed.clear() ;
			return_set_detected.clear() ;
			for ( vector<return_sets>::const_iterator it = childsets.begin() ;
				it != childsets.end() ; ++it ) 
			{
				return_set_changed.insert( // return_set_changed.begin(), 
										it->changed->begin(),
										it->changed->end() ) ;
				return_set_detected.insert( // return_set_detected.begin(), 
										it->detected->begin(),
										it->detected->end() ) ;
			}
			return_sets ret ;
			ret.detected = &return_set_detected ;
			ret.changed = &return_set_changed ;
			return ret ;
		
		// i dont have children: return empty sets
		} else {
			return_set_changed.clear() ;
			return_set_detected.clear() ;
			return_sets ret ;
			ret.detected = &return_set_detected ;
			ret.changed = &return_set_changed ;
			return ret ;
		}
	}
}

void go_obj::do_refine( int cutoff ) 
{
	cout << "N = " << detected.size() << "\tK = " << changed.size() << endl ;
	refine( changed.size(), detected.size(), cutoff ) ;
}

