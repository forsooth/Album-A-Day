/*
 * Filename: album.h
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Contains the data for a single album, such as the title, artist,
 * and year of release, as well as functions for manipulating and retrieving the 
 * data.  
 *
 */

#ifndef ALBUM_H
#define ALBUM_H
#include <iostream> /* Error messages */
#include <string> /* Data members */

class Album {
        public:
                /* METHODS */

                /* 
                 * An album can be either created empty or initialized with all
                 * of its data filled. Empty albums have blank strings for
                 * their title and artist fields, and a year of 0.
                 */
                Album();
                Album(std::string title, std::string artist, short year);

                /* Copy constructor. */
                Album(const Album &album);

                /* Returns the title of the album. */
                std::string get_title() { return title; }

                /* Returns the name of the artist for the album. */
                std::string get_artist() { return artist; }

                /* Returns the year of release for the album. */
                short get_year() { return year; }

                /* Sets the album title to the desired string. */
                void set_title(std::string title);

                /* Sets the album artist to the desired string. */
                void set_artist(std::string artist);

                /* Sets the album release year to the desired integer. */
                void set_year(short year);

        private:

                /* DATA MEMBERS */

                std::string title;
                std::string artist;
                short year;

};

#endif