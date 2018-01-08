
RELEASE= 0.4.8

install = /usr/bin/install


prefix = /home/leechuck
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib

all:
	@cd common && make
	@cd wilcoxon && make
	@cd hypergeometric && make
	@cd binomial && make
	@cd 2x2contingency && make
	@cd tools && make
	@cd doc && make

clean:
	@cd common && make clean
	@cd wilcoxon && make clean
	@cd hypergeometric && make clean
	@cd binomial && make clean
	@cd 2x2contingency && make clean
	@cd tools && make clean
	@cd doc && make clean

dist-clean:
	rm -rf Makefile config.* *.cache
	@cd common && make dist-clean
	@cd wilcoxon && make dist-clean
	@cd hypergeometric && make dist-clean
	@cd binomial && make dist-clean
	@cd 2x2contingency && make dist-clean
	@cd tools && make dist-clean
	@cd doc && make dist-clean
	
install:
	$(install) -d $(bindir)
	@cd wilcoxon && make install
	@cd hypergeometric && make install
	@cd binomial && make install
	@cd 2x2contingency && make install
	@cd tools && make install
	$(install) -d $(prefix)/doc
	$(install) -d $(prefix)/doc/func-$(RELEASE)
	@cd doc && make install
