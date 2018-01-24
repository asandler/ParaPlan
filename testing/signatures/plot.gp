set key autotitle columnhead
set term png truecolor rounded size 800,600 lw 2
set logscale y
set key left top
set xlabel "Number of regions"
set ylabel "Edge signatures cut-off, times"

set output 'images/misc.png'
plot './results/misc' using 2:3 with lines, './results/misc' using 2:5 with lines, './results/misc' using 2:7 with lines
set output 'images/misc-top.png'
plot './results/misc' using 2:4 with lines, './results/misc' using 2:6 with lines, './results/misc' using 2:8 with lines

set output 'images/non-linear.png'
plot './results/non_linear' using 2:3 with lines, './results/non_linear' using 2:4 with lines

set output 'images/random.png'
plot './results/random' using 1:2 with lines, './results/random' using 1:3 with lines

set output 'images/spiral.png'
plot './results/spiral' using 2:3 with lines, './results/spiral' using 2:4 with lines

set output 'images/pendulum.png'
plot './results/pendulum' using 2:3 with lines, './results/pendulum' using 2:4 with lines
