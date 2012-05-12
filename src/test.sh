num=$1
./cutlines.py $num
time ./ehdb 2>out
