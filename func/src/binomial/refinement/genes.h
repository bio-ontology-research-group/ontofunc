
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
		// datafile = Gene \t leftvalue \t rightvalue
		// gene objects are created, gene objects register to the 
		//  go_objs they are annotated to
		genes( go_graph &graph, istream &annotation, istream &data ) ;
		~genes(  ) ;
	private:
		vector<gene*> gene_vec ;

} ;
