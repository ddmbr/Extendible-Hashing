perf record -g ./ehdb
# perf script | ./gprof2dot.py -f perf | xdot
perf script | ./gprof2dot.py -f perf | dot -Tpng -o profpic/proff_big_record_perf.png
