#!/bin/bash
echo "Renomear soluções originais para .original?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) rename .sol .sol.original *.sol ;break;;
        No ) break;;
    esac
done

VALG () {
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s --log-file=valgrind_$(date +"%Y_%m_%d_%T").log $1 $2 
}

DIFF="diff -s"

input=$(ls *.in)

for F in ${input}; do 
    sleep 1;
    VALG "./roap" "${F}";
done;

FILES=$(ls *.sol)

mkdir -p diffs
rm -f diffs/*

for F in ${FILES}; do
    ${DIFF} ${F} $( echo ${F}.original ) > diffs/$( echo ${F}.diff ) ;
done;

echo "Apagar soluções do programa (as originais mantêm-se)?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) rm -f *.sol ;break;;
        No ) exit;;
    esac
done
