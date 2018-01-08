
#ifndef GENE_H
#define GENE_H

#include <string>
#include <set>
#include "go_obj.h"

using std::string ;

/***************
 * encapsulates gene information
 ****************/
class gene {
	public:
		
		/*********
  		 * name == name of gene 
		 * gos == GO objects 
		 * registers this gene to all gos it is annotated to
		 ************/
		gene( string name_, set<go_obj*> &gos ) ;

		/*********
  		 * save data 
		 ************/
		void add( int ch_s_, int ch_ns_, int hh_s_, int hh_ns_ ) ;
		string name ;
		int ch_s, ch_ns, hh_s, hh_ns ;
	private:
} ;

#endif

