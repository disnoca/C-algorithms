.PHONY : all
all:
	cd data_structures && make $@
	cd hashing && make $@
	cd searching && make $@
	cd sorting && make $@

.PHONY : clean
clean:
	cd data_structures && make $@
	cd hashing && make $@
	cd searching && make $@
	cd sorting && make $@
