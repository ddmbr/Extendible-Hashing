./ehdb
# gprof ehdb |./gprof2dot.py | dot -Tpng -o profpic/proff_big_record.png
gprof ehdb |tee profoutput|./gprof2dot.py  | xdot
