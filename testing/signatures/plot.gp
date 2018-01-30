set key autotitle columnhead
set term png truecolor rounded size 800,600 lw 2
set logscale y
set key left top
set xlabel "Number of regions"
set ylabel "Edge signatures cut-off, times"

set output 'images/misc.png'
plot './results/misc' using 2:3 with linespoints pt 7, './results/misc' using 2:5 with linespoints pt 7, './results/misc' using 2:7 with linespoints pt 7
set output 'images/misc-top.png'
plot './results/misc' using 2:4 with linespoints pt 7, './results/misc' using 2:6 with linespoints pt 7, './results/misc' using 2:8 with linespoints pt 7

set output 'images/non-linear.png'
plot './results/non_linear' using 2:3 with linespoints pt 7, './results/non_linear' using 2:4 with linespoints pt 7

set output 'images/random.png'
plot './results/random' using 1:2 with linespoints pt 7, './results/random' using 1:3 with linespoints pt 7

set output 'images/spiral.png'
plot './results/spiral' using 2:3 with linespoints pt 7, './results/spiral' using 2:4 with linespoints pt 7

set output 'images/pendulum.png'
plot './results/pendulum' using 2:3 with linespoints pt 7, './results/pendulum' using 2:4 with linespoints pt 7
