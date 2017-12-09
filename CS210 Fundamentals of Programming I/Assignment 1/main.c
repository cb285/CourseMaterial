/* File: main.c
 * This program calculates sunrise and sunset times for any date and location.
 * Uses stdio.h, stdlib.h, and math.h library files.
 * Only main function is declared.
 * --------------------------------------------------------------
 * Class: CS 210                   Instructor: Dr. Deborah Hwang
 * Assignment: Assignment 1        Date assigned:  January 22, 2015
 * Programmer: Clayton Brutus      Date completed: January 24, 2015
 */

#include <stdio.h>       /* printf, scanf  */
#include <stdlib.h>
#include <math.h>        /* sin, cos, acos */
#define PI 3.1415926     /* defines the constant pi used in calculating sunset and sunrise times. */

/* Function: main
   Computes time of sunrise and sunset from latitude, longitude, timezone, and day of year,
   then prints the results.
 */

int main()
{
    printf("SUNRISE, SUNSET\n");  /* Program Header */
    printf("This program will calculate sunrise and sunset times for any date and\nlocation\n");

    /* Declare vars. */

    double latitude,   /* Latitude of the location in degrees (positive is north, negative is south) */
           longitude,  /* Longitude of the current location in degrees (positive is east, negative is west) */
           gamma,      /* Computational intermediate (greek letter gamma) */
           eqt,        /* Equation of time in minutes (number of minutes that the position of the sun
                          differs from the actual time) */
           d,          /* Declination of the sun in radians */
           ha,         /* Hour angle of the sun in degrees */
           sunrise,    /* Sunrise time in minutes */
           sunset;     /* Sunset time in minutes */

    int timezone,      /* The local timezone as an offset from UTC (e.g., CST is ­6) */
        day,           /* The numerical day of the year (i.e., January 1 is 1, January 2, is 2, etc) */
        sunrise_hour,  /* Hour component of sunrise time (military time) */
        sunrise_min,   /* Minute component of sunrise time */
        sunset_hour,   /* Hour component of sunset time (military time) */
        sunset_min;    /* Minute component of sunset time */

    /*  1. Asks user to input the latitude, longitude, timezone, and day of year. */

    printf("\nEnter latitude: ");
    scanf("%lf", &latitude);

    printf("Enter longitude: ");
    scanf("%lf", &longitude);

    printf("Enter timezone: ");
    scanf("%d", &timezone);

    printf("Enter day of year: ");
    scanf("%d", &day);

    /* 2. Compute sunrise and sunset times. */

    gamma = ((2 * PI)/365) * (day - 1);

    eqt = 229.18 * (0.000075+0.001868 * cos(gamma) - 0.032077 * sin(gamma)  /* Calculate the equation of time */
          - 0.014615 * cos(2 * gamma) - 0.040849 * sin(2 * gamma));

    d = (0.006918 - 0.399912 * cos(gamma) + 0.070257 * sin(gamma)           /* Calculate declination of the sun */
         - 0.006758 * cos(2* gamma) + 0.000907 * sin(2 * gamma)
         - 0.002697 * cos(3 * gamma) + 0.00148 * sin(3 * gamma));

    ha = acos ((cos(1.5853) / (cos(latitude*(PI/180)) * cos(d)))            /* Calculate hour angle of the sun */
               - tan(latitude * (PI / 180)) * tan(d)) * (180 / PI);

    sunrise = 720 + 4 * (longitude - ha) - eqt - 60 * timezone;             /* Calculate sunrise time in minutes */

    sunrise_hour = (sunrise / 60);                                          /* Calculate sunrise time hour component */

    sunrise_min = sunrise - (sunrise_hour * 60);                            /* Calculate sunrise time minute component */

    sunset = 720 + 4 * (longitude + ha) - eqt - (60 * timezone);            /* Calculate sunset time in minutes */

    sunset_hour = (sunset / 60);                                            /* Calculate sunset time hour component */

    sunset_min = (sunset - (sunset_hour * 60));                             /* Calculate sunset time minute component */

    /* 3. Print results in hour:minute military time format. */

    printf("\nSunrise: %d:%02d", sunrise_hour, sunrise_min);

    printf("\nSunset: %d:%02d\n", sunset_hour, sunset_min);

    return 0;
}
