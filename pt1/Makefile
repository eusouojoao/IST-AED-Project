FILES1 = $(shell ls ../testes/*.in1)
VALG = valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s --log-file="valgrind_$$(date +"%Y_%m_%d_%T").log" 
testes1:
	rm -f ../testes/*.sol1
	for F in ${FILES1}; do sleep 1 ; ${VALG} ./roap -s $${F} ; done;

FILES2 = $(shell ls ../testes/*.sol1)
DIFF = diff -s
diff1:
	mkdir -p diffs
	for F in ${FILES2} ; do ${DIFF} $${F} $$(echo $${F} | sed 's/testes/sol/g') > diffs/$$(echo $${F} | sed "s/\.\.\/testes\/*/DIFF_/g") ; done ;
