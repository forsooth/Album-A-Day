/*
 * Filename: month.cpp
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Contains the data for a month of albums, as well as information
 * about this month. 
 *
 */

#include "month.h"

/* PUBLIC */

Month::Month(std::string name, short year)
{
        /* Error if month name is shorter than "May" */
        if (name.length() < shortest_month) {
                std::cerr << "ERROR 002: Month name too short to be valid. "
                          << "Month string set to \"January\"."
                          << std::endl;
                name = "January";
        }

        /* Convert input month name to title case */
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        name[0] = toupper(name[0]);

        month_name = name;
        
        this->year = year;
        if (year < 0) {
                std::cerr << "ERROR 003: Invalid month year. "
                          << "Year set to zero." 
                          << std::endl;
                year = 0;
        }

        month_num = get_num_from_name(name);
        num_days = month_lengths[month_num];

        /* Handle leap years correctly. See Sakamoto's Algorithm */
        if (month_num == 2 && year % 4 == 0 && 
                        (year % 100 != 0 || year % 400 == 0)) {
                num_days++;
        }

        albums = new Album*[num_days + 1];
        
        for (short i = 0; i < num_days + 1; i++) {
                albums[i] = NULL;
        }
        
        num_albums = 0;

}

Month::Month(short month_num, short year)
{
        /* TODO: error checking for 1 <= month_num <= 12 */
        this->month_num = month_num;
        
        /*Retrieve name from list, using month_num as index */
        month_name = month_names[month_num];
        this->year = year;

        if (year < 0) {
                std::cerr << "ERROR 003: Invalid month year. "
                          << "Year set to zero." 
                          << std::endl;
                year = 0;
        }

        num_days = month_lengths[month_num];

        /* Handle leap years correctly. See Sakamoto's Algorithm. */
        if (month_num == 2 && year % 4 == 0 && 
                        (year % 100 != 0 || year % 400 == 0)) {
                num_days++; 
        }

        albums = new Album*[num_days + 1];
        
        for (short i = 0; i < num_days + 1; i++) {
                albums[i] = NULL;
        }

        num_albums = 0;

}

Month::Month(const Month &month)
{
        this->month_num = month.month_num;
        this->year = month.year;
        this->num_days = month.num_days;
        this->albums = new Album*[num_days + 1];

        for (short i = 0; i < month.num_albums; i++) {
                if (month.albums[i] != NULL) {
                        this->albums[i] = new Album(*month.albums[i]);
                } else {
                        this->albums[i] = NULL;
                }
        }
        
        this->num_albums = month.num_albums;

}

Month& Month::operator=(const Month &source)
{
        /* check for self-assignment */
        if (this == &source)
                return *this;

        this->month_num = source.month_num;
        this->year = source.year;
        this->num_days = source.num_days;
        this->albums = new Album*[num_days + 1];

        for (short i = 0; i < source.num_albums; i++) {
                if (source.albums[i] != NULL) {
                        this->albums[i] = new Album(*source.albums[i]);
                } else {
                        this->albums[i] = NULL;
                }
        }
        
        this->num_albums = source.num_albums;

 
        return *this;
}

Month::~Month() 
{
        for (short i = 0; i < num_days + 1; i++) {
                if (albums[i] != NULL) 
                        delete albums[i];
        }
        delete [] albums;
}

/* 
 * Add an album to the month, represented as a vector of albums where NULL in
 * a day's space represents no album.
 */
void Month::add_album(short day, std::string title, std::string artist, short year)
{

        if (year < 0) {
                std::cerr << "ERROR 003: Invalid month year. "
                          << "Year set to zero." 
                          << std::endl;
                year = 0;
        }

        Album *album = new Album(title, artist, year);

        if (day < 1 || day > num_days) {
                std::cerr << "ERROR 004: Day for album addition is larger "
                          << "than the number of days in the month. "
                          << "Day value is " << day << "." 
                          << std::endl;
                delete album;
                return;
        }

        if (albums[day] != NULL) {
                std::cerr << "NOTE: Album is being overwritten with new value "
                          << "for day " << day << "." 
                          << std::endl;
                delete albums[day];
                num_albums--;
        }

        albums[day] = album;
        num_albums++;
}

/* Generic get methods, taking a day as a parameter to pass on album info. */

bool Month::has_album_for_day(short day)
{ 
        return albums[day] != NULL; 
}

std::string Month::get_album_title(short day)
{ 
        return albums[day]->get_title(); 
}

std::string Month::get_album_artist(short day)
{
        return albums[day]->get_artist(); 
}

short Month::get_album_year(short day) { return albums[day]->get_year(); }


/* Not yet implemented. */
void Month::shuffle_albums() { /* TODO: This, maintain blank days */ }
void Month::sort_albums_alphabetically() { /* TODO: This */ }
void Month::sort_albums_chronologically() { /* TODO: This */ }


/* PRIVATE */

short Month::get_num_from_name(std::string name)
{
        for (short i = 1; i < nm; i++) {
                if (name.compare(month_names[i]) == 0) {
                        return i;
                }
        }
        return 0;
}

