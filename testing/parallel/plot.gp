set key autotitle columnhead
set term png truecolor rounded size 830,600 lw 2
set key left top
set xrange [1:12]
set xlabel "Number of threads"

set yrange [0:4]
set label "Heavy tests, reachable, absolute time" at 4.8,3.556 font "Arial,20"
set ylabel "Time, sec"
set output 'images/abs-big-reach.png'
plot for [i=1:3] './results/abs_big_reach' using ($0+1):i with linespoints pt 7
unset label

set yrange [0:0.8]
set label "Heavy tests, unreachable, absolute time" at 4.55,0.71 font "Arial,20"
set output 'images/abs-big-unreach.png'
plot for [i=1:3] './results/abs_big_unreach' using ($0+1):i with linespoints pt 7
unset label

set yrange [0:0.1]
set label "Light tests, reachable, absolute time" at 5,0.09 font "Arial,20"
set output 'images/abs-little-reach.png'
plot for [i=1:10] './results/abs_little_reach' using ($0+1):i with linespoints pt 7
unset label

set yrange [0:0.1]
set label "Light tests, unreachable, absolute time" at 4.5,0.09 font "Arial,20"
set output 'images/abs-little-unreach.png'
plot for [i=1:10] './results/abs_little_unreach' using ($0+1):i with linespoints pt 7
unset label

set ylabel "Speed-up, times"
set logscale y
set yrange [1:400]
set label "Heavy tests, reachable, relative speed-up" at 4,200 font "Arial,20"
set output 'images/rel-big-reach.png'
plot for [i=1:3] './results/rel_big_reach' using ($0+1):i with linespoints pt 7
unset label
unset logscale

set yrange [1:2.5]
set label "Heavy tests, unreachable, relative speed-up" at 3.9,2.33 font "Arial,20"
set output 'images/rel-big-unreach.png'
plot for [i=1:3] './results/rel_big_unreach' using ($0+1):i with linespoints pt 7
unset label

set yrange [0.5:4]
set label "Light tests, reachable, relative speed-up" at 4.4,3.65 font "Arial,20"
set output 'images/rel-little-reach.png'
plot for [i=1:10] './results/rel_little_reach' using ($0+1):i with linespoints pt 7
unset label

set yrange [0.5:4]
set output 'images/rel-little-unreach.png'
set label "Light tests, unreachable, relative speed-up" at 4.2,3.65 font "Arial,20"
plot for [i=1:10] './results/rel_little_unreach' using ($0+1):i with linespoints pt 7
unset label
