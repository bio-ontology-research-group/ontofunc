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




using namespace std ;

#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <memory>
#include "go.h"
#include "go_graph.h"
#include "../../common/idmap.h"
#include "../../common/transitions.h"


#define MAX_LINE_LENGTH 20000

int main( int argc, char *argv[] ) 
{

	if (  argc != 9 ) {
		cerr << "Usage: " << argv[0] << " detected "
				"changed number_of_sets outfile term.txt" << endl
				<< "       graph_path.txt term2term.txt GO_ID" << endl ;
		exit( 1 ) ;
	}
	
	// Build GO-Graph using different files from go_date_termdb-tables.tar.gz
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
	
	// gos-object will be used to get one int* per GO and to print 
	// the results.
	go gos ;
	
	ostream *out ;
	if ( string( argv[4] ) == "-" ) {
		out = &cout ;
	} else {
		out = new ofstream( argv[4] ) ;
	}
	if ( !*out ) {
		cerr << "Cannot open output file " << argv[4] << endl ;
		exit( 1 ) ;
	}

	
	ifstream in( argv[1] ) ;


	cerr << "Reading detectedfile... " 
			<< endl ;
	
	// gens == genes. This vector is a simple representation of the go tree.
	// every gene is 1 vector of int*, where the int represents one go-node.
	vector<vector<int*> > gens;


	map<string,int> genename_to_index ;
	int index = 0 ;

//	char line[MAX_LINE_LENGTH] ;
	string line ;
	while ( in ) {
		getline( in, line ) ;
//		in.getline(line, MAX_LINE_LENGTH, '\n' ) ;
		istringstream is( line.c_str() ) ; 
		string gen_name ;
		is >> gen_name ; 
#ifdef DEBUG
		cout << "gen_name: " << gen_name << endl ;
#endif

		vector<int*> *gen_vec = new vector<int*> ;
		string go_name ;
		set<string> parents ;
		while ( is >> go_name ) {
			// Get the names of all nodes that are parents of go_name
			graph.get_parents( go_name, &parents ) ;
		}
		for ( set<string>::const_iterator it = parents.begin() ; 
				it != parents.end() ; ++it ) 
		{
			// gos.add returns a unique int* for every string.
			gen_vec->push_back( gos.add( *it ) ) ; 
#ifdef DEBUG
			cout << "go: " << *it << endl ;
#endif
		}
		// if the gene is annotated, add it to the genes-vector
		if ( gen_vec->size() > 0 ) {
			gens.push_back( *gen_vec ) ;
			genename_to_index[gen_name] = index ;
			index++ ;
		} else 
			delete gen_vec ;
	}

	cerr << "Found " << gens.size() << " usable entrys in " << argv[1]
		<< " with " << gos.size() << " GOs" << endl ;
	
	*out << "Genes:\t" << gens.size() << endl ;
	*out << "GOs:\t" << gos.size() << endl ;
	
	gos.print_names( *out ) ;

	// add changedfile...
	ifstream changed_in( argv[2] ) ;
	if ( ! changed_in ) {
		cerr << "Cannot open " << argv[2] << endl ;
		exit( 1 ) ;
	}

	string line_s ;

	int size_of_random_sets = 0 ;

	while( changed_in ) {
		getline( changed_in, line_s ) ;
		istringstream is( line_s.c_str() ) ;
		string gen_name ;
		is >> gen_name ;
		if ( genename_to_index.find( gen_name ) != genename_to_index.end() ) {
			for ( vector<int*>::iterator it = gens[genename_to_index[gen_name]].begin() ;
					it != gens[genename_to_index[gen_name]].end() ; ++it ) {
				(*(*it))++ ;
			}
			size_of_random_sets++ ;
		}
	}
	gos.print_sum( *out ) ;
	gos.clear() ;

	int number_of_randomsets ;
	{
		istringstream is( argv[3] ) ;
		is >> number_of_randomsets ;
	}
/*	int size_of_random_sets ;
	{
		istringstream is( argv[2] ) ;
		is >> size_of_random_sets;
	}
*/
	cerr << "Creating " << number_of_randomsets << " randomsets with "
			"size " << size_of_random_sets << endl ;

/*	*out << "Randomsets:\t" << number_of_randomsets << endl ;
	*out << "Genes per randomset:\t" << size_of_random_sets << endl ;
*/




	// forall randomsets
	for ( int i = 1 ; i <= number_of_randomsets ; ++i ) {
		// create randomset
		set<int> random_numbers ;
		double max = gens.size() ;
		for ( int randi = 1 ; randi <= size_of_random_sets ; ++randi ) {
		
			int rand_num = static_cast<int>((max*rand())/(RAND_MAX+1.0)) ;
			while ( random_numbers.find( rand_num ) !=
												random_numbers.end() ) {
				rand_num = static_cast<int>((max*rand())/(RAND_MAX+1.0)) ;
			}
			random_numbers.insert( random_numbers.begin(), rand_num ) ;
		}
		// reset all go-nodes
		gos.clear(  ) ;

		// add 1 to every GO that a randomly choosen gene is part of
		for ( set<int>::const_iterator it = random_numbers.begin() ;
				it != random_numbers.end() ; ++it ) {
			for ( vector<int*>::iterator it2 = gens[*it].begin() ;
					it2 != gens[*it].end() ; ++it2 ) {
				(*(*it2))++ ;
			}
		}
		random_numbers.clear() ;
		
		// print a line with the values for every go
		gos.print_sum( *out ) ;

	}
	cerr << "\rFinished" << endl ;

}
