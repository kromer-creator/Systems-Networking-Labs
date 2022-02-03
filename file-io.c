#include <stdio.h>
#include <stdlib.h>

/*
Author: Kelly Romer

Date: January 29th, 2020

This program reads input values from a file. The values are then processed as data for the amount of
time a system spends idle for a given amount of days. The performance rate, average daily time idle,
and days with maximum and minimum time idle are then written to an output file.

*/

void timeIdle(int timeData[], int numDays, FILE *out){

    //initialize loop variable
    int i;
    //initialize total time variable
    int totalTimeIdle = 0;

    //add all time idle values together
    for (i = 1; i <= numDays; i++){
        totalTimeIdle += timeData[i];
    }

    //write weekly total time idle to output file
    fprintf(out, "Weekly total idle time: %d minutes\n", totalTimeIdle);

    //calculate performance rate
    double performanceRate = ((double) totalTimeIdle / (numDays * 1440)) * 100;

    //write performance rate to output file
    fprintf(out, "Performance Rate: %f percent\n", performanceRate);

    //calculate average daily time idle
    double avgDailyIdleTime = ((double) totalTimeIdle / numDays);

    //write average daily time idle to output file
    fprintf(out, "Average daily idle time: %f minutes\n", avgDailyIdleTime);

    //calculate minimum time idle
    int minimum = timeData[0];
    for (i = 0; i < numDays; i++){
        if (timeData[i] <= minimum){
            minimum = timeData[i];
        }
    }
    //write minimum time idle to output file
    fprintf(out, "Lowest time idle over period: %d minutes\n", minimum);

    //calculate day(s) with minimum time idle and write to output file
    fprintf(out, "Day(s) with lowest time idle: \n");
    for (i = 0; i < numDays; i++){
        if (timeData[i] == minimum){
            fprintf(out, "%d\n", (i+1));
        }
    }
    //calculate maximum time idle
    int maximum = timeData[0];
    for (i = 0; i < numDays; i++){
        if (timeData[i] > maximum){
            maximum = timeData[i];
        }
    }
    //write maximum time idle to output file
    fprintf(out, "Highest time idle over period: %d minutes\n", maximum);

    //calculate day(s) with maximum time idle and write to output file
    fprintf(out, "Day(s) with highest time idle: \n");
    for (i = 0; i < numDays; i++){
        if (timeData[i] == maximum){
            fprintf(out, "%d\n", (i+1));
        }
    }
}

int main(int argc, char *argv[]){

    //create pointer to input file
    FILE *in;
    //open file from command line argument for reading
    in = fopen(argv[1], "r");

    //create pointer to output file
    FILE *out;
    //open file from command line argument for writing
    out = fopen(argv[2], "w");

    //initialize loop variable
    int i;

    //if either file null, print error message and exit
    if (in == NULL || out == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    //initialize variable for number of days file contains
    int numDays;
    //scan first line of file for amount of days' worth of data
    fscanf(in, "%d", &numDays);

    //initialize array to store values from file
    int timeData[numDays];

    //scan time idle values from input file to array
    for (i = 0; i < numDays; i++){
        fscanf(in, "%d", &timeData[i]);
    }

    //call timeIdle function
    timeIdle(timeData, numDays, out);

    return 0;
}
