#!/usr/bin/perl
#
# reads term.txt, graph_path.txt and an standard formatted func-inputfile and returns
# a list of all go-categories with annotated genes.
#



my %goid_to_dbid ;
my %dbid_to_goid ;

open TERM,"<$ARGV[0]" ;
while ( <TERM> ) {
	chomp ;
	@r = split /\t/ ;
	$goid_to_dbid{$r[3]} = $r[0] ;
	$dbid_to_goid{$r[0]} = $r[3] ;
}
close TERM ;

my %id_to_ids ;

open GRAPH,"<$ARGV[1]" ;
while (<GRAPH>) {
	chomp ;
	@r = split /\t/ ;
	push @{$id_to_ids{$r[2]}}, $r[1] ;

}
close GRAPH ;

my %genes ;
open IN, "<$ARGV[2]" ;
while (<IN>) {
	chomp ;
	@r = split /\t/ ;
	unless (defined $genes{$r[0]} ) {
		$genes{$r[0]} = {} ;
	}
	foreach $i (@{$id_to_ids{$goid_to_dbid{$r[1]}}}) {
		$genes{$r[0]}->{$dbid_to_goid{$i}} = 1 ;
	}
}

foreach $g (keys %genes) {
	print $g, "\t" ;
	print join "\t", keys %{$genes{$g}} ;
	print "\n" ;
}

