
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

class gene ;

/**********
 * GO Node with information about parent-nodes, childnodes,
 * annotated genes. 
 ***********/
class go_obj
{
	public:
		go_obj( string &name_ ) ;
		void add_parent( go_obj* p ) ;
		void add_child( go_obj* p ) ;
		void add_gene( gene* ) ;
		void get_parents( set<go_obj*> *parentset ) ;

		/*******
		 * start the refinement with
		 *   pval_b == pvalue before genes are extracted from group
		 *   pval_a == pvalue are genes are extracted from group
		 *   cutoff == #genes the group has to have at least to be checked
		 * this node gives the average p needed for binomial test
		 * results are saved to cp_b...hp_a, print() prints the results
		 ********/
		void start_refinement( double pval_b, double pval_a, int cutoff ) ;

		/*******
		 * recursive function with 
		 *   p_binom calculated from root_node (start_refinement(..)) 
		 *   pval_b, pval_a, cutoff -> see above
		 *********/
		set<gene*>* refine( double pval_b, double pval_a, 
                                            double p_binom, int cutoff ) ;

		// prints results: one line with 4 pvals 
		// (before after and 2 directions)
		void print( ostream &os ) ;
		string name ;

	private:
		vector<go_obj*> parents ;
		vector<go_obj*> childs ;
		set<gene*> gens ;
		double cp_b, cp_a, hp_b, hp_a ;
		bool sig ;
} ;

#endif
