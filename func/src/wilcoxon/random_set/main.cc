
using namespace std ;

#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <memory>
#include "go_graph.h"
#include "../../common/idmap.h"
#include "../../common/transitions.h" 
#include "genes.h"

#define MAX_LINE_LENGTH 20000

int main( int argc, char *argv[] ) 
{

	if (  argc != 9 ) {
		cerr << "Usage: " << argv[0] << " infile_ann "
				"infile_data number_of_sets outfile term.txt" << endl
				<< "       graph_path.txt term2term.txt GO_ID" << endl ;
		exit( 1 ) ;
	}
	
	/*****************
         * read graph-structure and create graph
	 *******************/
	// argv[5] == term.txt from go_date_termdb-tables.tar.gz
	ifstream terms( argv[5] ) ;
	if ( ! terms ) {
		cerr << "Cannot open " << argv[5] << "." << endl ;
		exit( 1 ) ;
	}
	idmap id_to_go( terms ) ;
	terms.close(  ) ;
	cerr << "Read " << id_to_go.size() << " terms." << endl ;

	ifstream transition_graph( argv[6] ) ;
	if ( ! transition_graph ) {
		cerr << "Cannot open " << argv[6] << "." << endl ;
		exit( 1 ) ;
	}
	string parent_go( argv[8] ) ;
	string parent_id = id_to_go.get_id_for_go( parent_go ) ;
	transitions trans( parent_id, transition_graph ) ;
	transition_graph.close(  ) ;
	cerr << "Found " << trans.size() << " nodes." << endl ;

	ifstream term2term( argv[7] ) ;
	if ( ! term2term ) {
		cerr << "Cannot open " << argv[7] << "." << endl ;
		exit( 1 ) ;
	}
	go_graph graph( trans, term2term, id_to_go ) ;
	term2term.close(  ) ;
	cerr << "Graph created." << endl ;

	/*****************
         * read gene information and annotate to graph
	 *******************/
	ifstream annf( argv[1] ) ;
	if ( ! annf ) {
		cerr << "Cannot open " << argv[1] << "." << endl ;
		exit( 1 ) ;
	}
	ifstream dataf( argv[2] ) ;
	if ( ! dataf ) {
		cerr << "Cannot open " << argv[2] << "." << endl ;
		exit( 1 ) ;
	}

	genes gns( graph, annf, dataf ) ;
	cerr << "Data and annotation file parsed." << endl ;

	int number_of_randomsets ;
	{
		istringstream is( argv[3] ) ;
		is >> number_of_randomsets ;
	}
	ostream *out ;
	if ( string(argv[4]) == "-" ) {
		out = &cout ;
	} else {
		out = new ofstream( argv[4] ) ;
		if ( !*out ) {
			cerr << "Cannot open output file " << argv[4] << endl ;
			exit( 1 ) ;
		}
	}
	// force full length of all written floats...
	out->precision( 100 ) ; 

	/*****************
         * save original data, create and save randdata to randomsetfile, 
	 *******************/
	*out << gns.sumnties() << endl ;
	graph.print_header( *out ) ;
	graph.print_sumranks( *out ) ;

	for ( int i=1 ; i <= number_of_randomsets ; ++i ) {
		graph.clear_genes(  ) ;
		gns.create_random_set(  ) ;
		graph.print_sumranks( *out ) ;
	}
	
}
