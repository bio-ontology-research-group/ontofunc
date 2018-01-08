
#ifndef GENE_H
#define GENE_H

#include "go_obj.h"


using namespace std ;

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
  		 * save data to hka, cka
		 ************/
		void add_hka_cka( int hka, int cka ) ;
		string name ;
		int hka, cka ;
	private:
} ;

#endif
