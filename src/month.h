/*
 * Filename: month.h
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Contains the data for a month of albums, as well as information
 * about this month. 
 *
 */

#ifndef MONTH_H
#define MONTH_H

#include <iostream> /* Error messages */
#include <string> /* Data members */
#include <algorithm> /* transform function */
#include <cctype> /* tolower function */
#include "album.h" /* Data members */

/* Number of months (length of month arrays) */
static const short nm = 13;

/* Length of shortest */
static const short shortest_month = 3;

/* Arrays for month conversions. */
static const std::string month_names[nm] = {"", "January", "February", "March",
                                            "April", "May", "June", "July", 
                                            "August", "September", "October",
                                            "November", "December"};
                                            
static const short month_lengths[nm] = {29, 31, 28, 31, 30, 31, 30, 31, 31, 
                                                                30, 31, 30, 31};


class Month {
        public:

                /* METHODS */

                /* 
                 * A month is defined by a month name, and a year. An integer 
                 * 1 to 12 can alternatively be used to identify the month of
                 * the year. Case is irrelevant for month name. 
                 */
                Month(std::string name, short year);
                Month(short month_num, short year);

                Month(const Month &month);
                Month& operator=(const Month &source);
                ~Month();

                /* 
                 * Returns the month name. Month name is returned with only
                 * the first letter capitalized.
                 */
                std::string get_month_name() { return month_name; }

                /* 
                 * Returns the month number 1-12 (1 = January ... 12 = December)
                 * for the month.
                 */
                short get_month_num() { return month_num; }

                /* Returns the year of this month. */
                short get_year() {return year; }

                /*
                 * Given the data to create an album and a day at which to place
                 * the album, this will add a new album to the month's 
                 * representation of the album-a-day layout.
                 */
                void add_album(short day, std::string title, std::string artist,
                               short year);

                /* 
                 * Returns the number of albums in the month so far. Guarranteed 
                 * to be less than or equal to the number of days in the month.
                 */
                short get_num_albums() { return num_albums; }
                
                /* Returns the number of days in the month, in range 28-31. */
                short get_num_days() { return num_days; }

                /* Returns true if a given day is filled with an album. */
                bool has_album_for_day(short day);

                /* Returns the title of the album for a given day. */
                std::string get_album_title(short day);

                /* Returns the name of the album artist for a given day. */
                std::string get_album_artist(short day);

                /* Returns the year of release of the album for a given day. */
                short get_album_year(short day);

                void shuffle_albums();
                void sort_albums_alphabetically();
                void sort_albums_chronologically();

        private:

                /* METHODS */

                /* Conversion function between month name and month number. */
                short get_num_from_name(std::string name);

                /* DATA MEMBERS */

                /* Month data. */
                std::string month_name;
                short year;
                short month_num;
                short num_days;
                
                /* Album list data. */
                short num_albums;
                Album **albums;

};

#endif