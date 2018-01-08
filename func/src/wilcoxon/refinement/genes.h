#ifndef GENES_H
#define GENES_H

#include <fstream>
#include <map>
#include <sstream>
#include "go_graph.h"
#include "gene.h"

/********************
 * handles gene-objects 
 *********************/
class genes
{
	public:
		// annotationfile = Gene \t GO_1\ GO_2 ...
		// datafile = Gene \t float
		// rank is calculated 
		// gene objects are created, gene objects register to the 
		//  go_objs they are annotated to
		genes( go_graph &graph, istream &annotation, istream &data ) ;
		double sumnties( ) { return sum_nties ; } ;
		~genes(  ) ;
	private:
		double sum_nties ;
		map<string, gene*> genemap ;
		vector<gene*> gene_vec ;

} ;

#endif 
