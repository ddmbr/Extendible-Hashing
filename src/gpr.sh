exe=$1
if test -z $exe
then
    exit
fi
./$exe
# gprof ehdb |./gprof2dot.py | dot -Tpng -o profpic/proff_big_record.png
gprof $exe |tee profoutput|./gprof2dot.py  | xdot
