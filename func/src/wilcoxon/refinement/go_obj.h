
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
		 *   sumnties == factor for wilcoxon rank test
		 * Number of genes in this root node is needed to calculate wilcoxon rank test
		 * results are saved to prob_less_b..., print() prints the results
		 ********/
		void start_refinement( double pval_b, double pval_a, double sumnties, int cutoff ) ;

		// N == number of genes in root node. see above
		set<gene*>* refine( double pval_b, double pval_a, double N, double sum_nties, int cutoff ) ;
		void print( ostream &os ) ;

		// debugging...
		void set_ignore( ) ;

	private:
		string name ;
		vector<go_obj*> parents ;
		vector<go_obj*> childs ;
		set<gene*> gens ;

		// pvals before/after
		double prob_less_b, prob_greater_b, prob_less_a, prob_greater_a ; 
		bool sig ;
		bool ignore ;
} ;

#endif
