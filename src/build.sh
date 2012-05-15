#! /bin/sh
# cp Makefile.template Makefile
if test $1 = H 
then
    sed  s/HashFunc=L_HASH/HashFunc=H_HASH/ Makefile > .mftemp
    make -B -f .mftemp && mv -v ehdb ehdb_h 
elif test $1 = L
then
    sed  s/HashFunc=H_HASH/HashFunc=L_HASH/ Makefile > .mftemp 
    make -B  -f .mftemp && mv -v ehdb ehdb_l
fi

