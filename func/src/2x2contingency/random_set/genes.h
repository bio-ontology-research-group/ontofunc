
#include <fstream>
#include <map>
#include <sstream>
#include "go_graph.h"
#include "gene.h"
#include <list>

using namespace std ;

/********************
 * handles gene-objects and shuffle gene_data
 *********************/
class genes
{
	public:
		// annotationfile = Gene \t GO_1\ GO_2 ...
		// datafile = Gene \t leftvalue \t rightvalue
		genes( go_graph &graph, istream &annotation, istream &data ) ;

		// shuffle genes and exchange data of genes.
		void create_random_set(  ) ;
		~genes(  ) ;
	private:
		map<string, gene*> genemap ;
		vector<gene*> genevec ;

} ;
