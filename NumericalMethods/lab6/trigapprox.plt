set terminal png
set output 'tryg_050_024.png'
set title 'blad sr. 2.708128E+00   max. 18.000000'
set xrange [0:6.28319]
g(x) = 1.63836601266245978081E+01+1.607850e+00 * cos(1 * x) -6.274915e+00 * sin(1 * x)-9.892144e+00 * cos(2 * x) -3.125039e+00 * sin(2 * x)-1.699113e-01 * cos(3 * x) -2.069531e+00 * sin(3 * x)-2.671187e-01 * cos(4 * x) -1.537583e+00 * sin(4 * x)-3.120995e-01 * cos(5 * x) -1.215021e+00 * sin(5 * x)-3.365202e-01 * cos(6 * x) -9.971110e-01 * sin(6 * x)-3.512309e-01 * cos(7 * x) -8.389591e-01 * sin(7 * x)-3.607638e-01 * cos(8 * x) -7.181098e-01 * sin(8 * x)-3.672837e-01 * cos(9 * x) -6.220803e-01 * sin(9 * x)-3.719307e-01 * cos(10 * x) -5.433738e-01 * sin(10 * x)-3.753515e-01 * cos(11 * x) -4.772121e-01 * sin(11 * x)-3.779348e-01 * cos(12 * x) -4.204024e-01 * sin(12 * x)-3.799258e-01 * cos(13 * x) -3.707270e-01 * sin(13 * x)-3.814849e-01 * cos(14 * x) -3.265939e-01 * sin(14 * x)-3.827206e-01 * cos(15 * x) -2.868275e-01 * sin(15 * x)-3.837086e-01 * cos(16 * x) -2.505377e-01 * sin(16 * x)-3.845022e-01 * cos(17 * x) -2.170344e-01 * sin(17 * x)-3.851401e-01 * cos(18 * x) -1.857713e-01 * sin(18 * x)-3.856508e-01 * cos(19 * x) -1.563061e-01 * sin(19 * x)-3.860549e-01 * cos(20 * x) -1.282732e-01 * sin(20 * x)-3.863680e-01 * cos(21 * x) -1.013633e-01 * sin(21 * x)-3.866012e-01 * cos(22 * x) -7.530911e-02 * sin(22 * x)-3.867625e-01 * cos(23 * x) -4.987284e-02 * sin(23 * x)-3.868572e-01 * cos(24 * x) -2.483772e-02 * sin(24 * x)

plot 10 + x * x / 2 - 10 * cos(2 * x), g(x) title 'przyblizenie', '-' title 'wezly'
0.000000 0.000000
0.125664 0.322064
0.251327 1.268516
0.376991 2.781375
0.502655 4.768063
0.628319 7.107222
0.753982 9.656339
0.879646 12.260702
1.005310 14.763117
1.130973 17.013790
1.256637 18.879738
1.382301 20.253143
1.507964 21.058125
1.633628 21.255518
1.759292 20.845319
1.884956 19.866699
2.010619 18.395535
2.136283 16.539645
2.261947 14.432015
2.387610 12.222437
2.513274 10.068103
2.638938 8.123728
2.764602 6.531825
2.890265 5.413750
3.015929 4.862082
3.141593 4.934802
3.267256 5.651650
3.392920 6.992886
3.518584 8.900530
3.644247 11.282002
3.769911 14.015945
3.895575 16.959847
4.021239 19.958993
4.146902 22.856192
4.272566 25.501650
4.398230 27.762382
4.523893 29.530571
4.649557 30.730338
4.775221 31.322514
4.900885 31.307099
5.026548 30.723264
5.152212 29.646884
5.277876 28.185779
5.403539 26.472932
5.529203 24.658138
5.654867 22.898589
5.780530 21.348998
5.906194 20.151879
6.031858 19.428588
6.157522 19.271705
e
