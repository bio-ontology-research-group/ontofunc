
#include "overall_sign.h"

int main()
{
	overall_significance os ;

	multiset<double> bla ;
	for ( int i = 0 ; i <= 10 ; i++ ) {
		bla.insert( (double)i/10. ) ;
	}
	os.add_set( bla ) ;
	os.add_set( bla ) ;
	os.add_set( bla ) ;
	os.add_set( bla ) ;

	cerr << "bla!" << endl ;

	map<double,double> *ptr = os.fdr_qvals( 0 ) ;

	for ( map<double, double>::const_iterator it = ptr->begin() ; 
			it != ptr->end() ; ++it ) {
		cout << it->first << "\t" << it->second << endl ;
	}

}
