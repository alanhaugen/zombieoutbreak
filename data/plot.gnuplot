set title 'f(x) = x^3 − 6x^2 + 11x − 6'
use grid
set xrange [-1:5]
set yrange [-10:40]
plot '../intermediate/data.txt' using 1:2 with lines title 'f(x)', 'data.txt' using 1:3 with lines title 'df(x)'
