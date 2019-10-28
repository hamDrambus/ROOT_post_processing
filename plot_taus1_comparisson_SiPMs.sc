set terminal qt size 900, 550 enhanced font 'Helvetica,13'
set datafile missing "--"
set xlabel "E/N [Td]" offset 0, 0.6
set ylabel "{/Symbol t} [{/Symbol m}s]" rotate by 90 offset 2, 0
set key top center at 5.85, 9.9 spacing 1.6 box
set yrange [0:10]
set xrange [3:9]
set lmargin 7
set rmargin 2
set ytics mirror
set format x "%-1.1f"
plot "190404/results/taus_v0.txt" u 2:7:8 w yerrorbars linewidth 2 ps 2 pt 7 lt 1 title "190404 Slow component {/Symbol t} ver. 0 by SiPMs"
replot "190404/results/taus_v2.txt" u 2:7:8 w yerrorbars linewidth 2 ps 2 pt 6 lt 2 title "190404 {/Symbol t} version 2 by SiPMs"
replot "190307/results/taus_v2.txt" u 2:7:8 w yerrorbars linewidth 2 ps 2 pt 17 lt 7 title "190307 {/Symbol t} version 2 by SiPMs"
replot "190404/results/taus_v2.txt" u 2:5:6 w errorbars linewidth 2 ps 0 pt 10 lc rgb "#000000" notitle
replot "190404/results/taus_v2.txt" u 2:5 w linespoints linewidth 2 ps 2 pt 11 lc rgb "#000000" title "190404 {/Symbol t} version 2 by fPMTs"
