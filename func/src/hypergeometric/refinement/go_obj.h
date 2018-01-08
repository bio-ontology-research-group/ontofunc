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




#ifndef GO_OBJ_H
#define GO_OBJ_H

#include <string>
#include <vector>
#include <set>
#include <iostream>

using std::vector ;
using std::set ;
using std::ostream ;
using std::string ;

/* 
 * Each object of this class will represent one GO-Term. 
 */
class go_obj
{
	public:
		// name could be any string, preferable the GO-ID
		go_obj( string &name_ ) ; 		

		// let object know about siblings and parents.
		void add_parent( go_obj* p ) ;
		void add_child( go_obj* p ) ;

		// Adds a changed or detected gene, s is a identifier for the gene
		// genes with the same name will only be added once.
		void add_changed( string &s ) ;
		void add_detected( string &s ) ;

		// prints GO-ID (name) and left and right p-values before and after refinement
		void print( ostream &os ) ;
		struct return_sets_ {
			set<string>* detected ;
			set<string>* changed ;
		} ;
		typedef struct return_sets_ return_sets ;

		// recursive function. Will call method refine() for all children.
		// p-value without the GO-Ids of the return_sets of the children
		// will be calculated. 
		return_sets refine( int n, int N, int cutoff ) ;

		// start refinement with this go_obj
		void do_refine( int cutoff ) ;
		

	private:
		string name ;
		vector<go_obj*> parents ;
		vector<go_obj*> childs ;
		set<string> changed ;
		set<string> detected ;
		// only used if not significant to return merged sets of
		// children (if more than 1 children)
		set<string> return_set_changed ;
		set<string> return_set_detected ;
		bool significant ;
		double significance_1, significance_2 ;
		double lsig1, rsig1, lsig2, rsig2 ;
} ;

#endif
