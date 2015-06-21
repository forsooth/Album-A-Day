/*
 * Filename: google_images_html.h
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Finds image URLs from the source HTML of a Google Images
 * webpage. 
 *
 */

#ifndef GOOGLE_IMAGES_H
#define GOOGLE_IMAGES_H

#include <iostream> /* Error messages */
#include <string> /* Data members */
#include <vector> /* Data structure */
#include <boost/regex.hpp> /* Finding URLs in Google Images HTML */
#include "str_utils.h" /* Recursive replace all method */
 
class Google_Images {

        public: 
                Google_Images();
                Google_Images(const std::string html);

                /*
                 * Creates the list of image URLs by parsing the HTML.
                 * Either takes in new HTML or uses one already loaded from a
                 * constructor. If a new HTML string is supplied, the old will
                 * be discarded if it exists.
                 */
                void parse_new(const std::string html);
                void parse_loaded();

                /*
                 * Retrieves the first image link from a Google Images HTML
                 * page in the form of an HTTP or HTTPS URL.
                 */
                std::string first_img();

                /* Returns a vector of image URLs instead. */
                std::vector<std::string> all_imgs();

                /*
                 * Accesses the url list and returns the url at the specified
                 * index. Returns an empty string if the index is out of bounds.
                 */
                std::string get_img(uint i);

                /* Returns the number of matches found. */
                uint num_matches();

        private:
                /* DATA MEMBERS */
                /*
                 * These constants control the regex operation. The string is
                 * passed as Emacs regex to boost, as is default, and the 
                 * substring constants determine how much will be cut off. 
                 * substr_b is the index of the first character to keep, and
                 * substr_e is substr_b + the number of characters to remove 
                 * from the end. 
                 */
                const std::string regex_string = "imgurl=.+?&amp;";
                const uint substr_b = 7; 
                const uint substr_e = 12;
                
                std::string html; /* Complete webpage HTML */
                std::vector<std::string> urls; /* List of URLs in html */

};

#endif
