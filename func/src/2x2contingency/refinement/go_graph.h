
#ifndef GO_GRAPH_H
#define GO_GRAPH_H

#include "gene.h"
#include "../../common/idmap.h"
#include <map>
#include <set>
#include <iostream>

using std::ostream;

/******
 * Reads GO DAG and handles go_objs. 
 *******/
class go_graph
{
	public:
		/*********
 		 * Reads DAG of GO and creates go_objs. 
		 * idmap: map database id of termdb-tables to GO_Id
		 * nodes: all nodes below the root node
		 * term2term: file with term2term associations
		 ***********/
		go_graph( set<string> &nodes, istream &term2term, idmap &idm_, int co=1 ) ;
		~go_graph(  ) ;

		/*********
 		 * returns parent go_objs of node with name "go", 
		 * including the node itself
		 ***********/
		void get_parents( string &go, set<go_obj*> *parents ) ;

		// search graph for "go", run refinement below "go" 
		// with the 2 pvals. see start_refinement() in go_obj.h
		void do_refin( string &go, double pval_before, double pval_after, int cutoff ) ;

		// go thru all nodes and print results
		void print_results( ostream &os ) ;

	private:
		idmap &idm ;
		map<string, go_obj*> graph ;
		int cutoff ;
} ;


#endif
