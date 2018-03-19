#include <stdlib.h>
#include <stdio.h>
#define NUM_POINTS 4
#define NUM_COMMANDS 4

int main()
{
    char * commandsForGnuplot[] = {"set title \"TITLEEEEE\"", "set ylabel 'ACCURACY'","set xlabel 'NUMBER OF HIDDEN UNITS'","plot 'data.temp' with linespoints title 'varying accuracy with number of hidden neurons'"};
    double xvals[NUM_POINTS] = {5 , 6 , 7 , 8};
    double yvals[NUM_POINTS] = {10.220441, 11.523046, 10.521042, 9.919840};
    FILE * temp = fopen("data.temp", "w");
    /*Opens an interface that one can use to send commands as if they were typing into the
     *     gnuplot command line.  "The -persistent" keeps the plot open even after your
     *     C program terminates.
     */
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i;
    for (i=0; i < NUM_POINTS; i++)
    {
    fprintf(temp, "%lf %lf \n", xvals[i], yvals[i]); //Write the data to a temporary file
    }

    for (i=0; i < NUM_COMMANDS; i++)
    {
    fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
    return 0;
}