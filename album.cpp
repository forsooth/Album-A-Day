/*
 * Filename: album.cpp
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Contains the data for a single album, such as the title, artist,
 * and year of release, as well as functions for manipulating and retrieving the 
 * data.  
 *
 */

#include "album.h"

Album::Album() 
{
        this->title = std::string();
        this->artist = std::string();
        this->year = 0;
}

Album::Album(std::string title, std::string artist, short year) 
{
        this->title = title;
        this->artist = artist;
        this->year = year;

        if (year < 0) {
                std::cerr << "ERROR 001: Invalid album year. "
                          << "Year set to zero." 
                          << std::endl;
                year = 0;
        }
}


Album::Album(const Album &album)
{
        if (&album != NULL) {
                this->title = album.title;
                this->artist = album.artist;
                this->year = album.year;
        } else {
                this->title = std::string();
                this->artist = std::string();
                this->year = 0;
        }
}

void Album::set_title(std::string title) { this->title = title; }

void Album::set_artist(std::string artist) { this->artist = artist; }

void Album::set_year(short year) 
{ 
        this->year = year; 
  
        if (year < 0) {
                std::cerr << "ERROR 001: Invalid album year. "
                          << "Year set to zero." 
                          << std::endl;
                year = 0;
        }
}

