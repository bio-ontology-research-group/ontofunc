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




#ifndef GO_GRAPH_H
#define GO_GRAPH_H

#include "go_obj.h"
#include "../../common/idmap.h"
#include <map>

using std::ostream;

/* Represents the go-graph. 
 */
class go_graph
{
	public:
		// "nodes" are the nodes that should be used to build the graph.
		// term2term is the term2term.txt file from go_termdb_tables,
		// idmap see idmap.h
		go_graph( set<string> &nodes, istream &term2term, idmap &idm_ ) ;
		~go_graph(  ) ;

		// Add a one detected/changed gene to go_obj with the name go_name.
		// every gene will also be added to any parent of go_name.
		void add_changed( string go_name, string gene_name ) ;
		void add_detected( string go_name, string gene_name ) ;

		// Add a whole file with detected or changed genes.
		// Format: 
		// Gene-Identifier[Space_or_Tab]GO_1[Space_or_Tab]GO_2...\n
		void add_changed_file( istream &is ) ;
		void add_detected_file( istream &is ) ;

		// start refinement with GO-ID go as root-node
		void do_refine( string go, int cutoff=1 ) ;
		
		// print all GO-Ids with their p-values
		void print_results( ostream &os ) ;
	private:
		idmap &idm ;
		map<string, go_obj*> graph ;
} ;


#endif
