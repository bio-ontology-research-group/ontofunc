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




#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../../common/idmap.h"
#include "../../common/transitions.h"
#include "go_graph.h"
#include "global.h"

using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

int main( int argc, char *argv[] )
{


	if ( argc != 12 ) {
		cerr << "Usage: " << argv[0] << " term.txt graph_path.txt "
			"GO_ID term2term.txt changed detected lpval rpval"
			" lpval_refinement rpval_refinement cutoff" << endl ;
		exit( 0 ) ;
	}

	leftpval = atof( argv[7] ) ;
	rightpval = atof( argv[8] ) ;

	refin_leftpval = atof( argv[9] ) ;
	refin_rightpval = atof( argv[10] ) ;

	cerr << "leftpval: " << leftpval << endl ;
	cerr << "rightpval: " << rightpval << endl ;
	cerr << "leftpval refinement: " << refin_leftpval << endl ;
	cerr << "rightpval refinement: " << refin_rightpval << endl ;
	
	// argv[1] == term.txt from go_date_termdb-tables.tar.gz
	ifstream terms( argv[1] ) ;
	if ( ! terms ) {
		cerr << "Cannot open " << argv[1] << "." << endl ;
		exit( 1 ) ;
	}
	idmap id_to_go( terms ) ;
	terms.close(  ) ;
	cerr << "Read " << id_to_go.size() << " terms." << endl ;

	ifstream transition_graph( argv[2] ) ;
	if ( ! transition_graph ) {
		cerr << "Cannot open " << argv[1] << "." << endl ;
		exit( 1 ) ;
	}
	string parent_go( argv[3] ) ;
	string parent_id = id_to_go.get_id_for_go( parent_go ) ;
	transitions trans( parent_id, transition_graph ) ;
	transition_graph.close(  ) ;
	cerr << "Found " << trans.size() << " nodes." << endl ;

	ifstream term2term( argv[4] ) ;
	if ( ! term2term ) {
		cerr << "Cannot open " << argv[4] << "." << endl ;
		exit( 1 ) ;
	}
	go_graph graph( trans, term2term, id_to_go ) ;
	term2term.close(  ) ;
	cerr << "Graph created." << endl ;

	ifstream changed( argv[5] ) ;
	if ( ! changed ) {
		cerr << "Cannot open " << argv[5] << "." << endl ;
		exit( 1 ) ;
	}
	graph.add_changed_file( changed ) ;
	changed.close(  ) ;
	cerr << "Added changed." << endl ;
	
	ifstream detected( argv[6] ) ;
	if ( ! detected ) {
		cerr << "Cannot open " << argv[6] << "." << endl ;
		exit( 1 ) ;
	}
	graph.add_detected_file( detected ) ;
	detected.close(  ) ;
	cerr << "Added detected." << endl ;

	int cutoff = atoi( argv[11] ) ;

	graph.do_refine( parent_go, cutoff ) ;
	cerr << "Refinement done, printing results." << endl ;
	graph.print_results( cout ) ;
	
}
