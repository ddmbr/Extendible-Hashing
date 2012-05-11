num=$1
./cutlines.py $num
time ./ehdb 2>/dev/null |tee out
