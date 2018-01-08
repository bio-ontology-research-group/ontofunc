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
		gene( string name_, set<go_obj*> &gos_ ) : name( name_ ), gos(gos_)
		{  }
		set<go_obj*>* get_gos(  ) ;

		// write ranks to gos_
		void write_to_gos( set<go_obj*>* gos_ ) ;

		// write ranks to private: gos
		void write_to_gos(  ) ;


		void set_rank( double rank_ ) ;
		double get_rank(  ) ;
		
		string name ;
	private:
		set<go_obj*> gos ;
		double rank ;

} ;

#endif
