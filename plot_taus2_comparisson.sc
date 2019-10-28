set terminal qt size 900, 550 enhanced font 'Helvetica,13'
set datafile missing "--"
set xlabel "E/N [Td]" offset 0, 0.6
set ylabel "{/Symbol t}2 [{/Symbol m}s]" rotate by 90 offset 2, 0
set key top left at 5.0, 400 spacing 1.6 box
set yrange [0:400]
set xrange [5:9]
set lmargin 7
set rmargin 2
set ytics mirror
set format x "%-1.1f"
plot "190404/results/taus_v0.txt" u 2:9:10 w yerrorbars linewidth 2 ps 2 pt 7 lt 1 title "190404 Long component {/Symbol t}2 ver. 0"
replot "190404/results/taus_v2.txt" u 2:9:10 w yerrorbars linewidth 2 ps 2 pt 6 lt 2 title "190404 {/Symbol t}2 version 2"
replot "190307/results/taus_v2.txt" u 2:9:10 w yerrorbars linewidth 2 ps 2 pt 17 lt 7 title "190307 {/Symbol t}2 version 2"

