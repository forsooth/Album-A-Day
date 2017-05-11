/*
 * Filename: get_album_art.h
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Given data about one or more albums, retrieves and downloads 
 * album art as CImg objects. 
 *
 */

#ifndef GET_ALBUM_ART_H
#define GET_ALBUM_ART_H

#include <iostream> /* Error messages */
#include <fstream> /* Reading file from disk */
#include <string> /* Data members */
#include <vector>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include "google_images_html.h"
#include "retrieve_html.h"
#include "album.h"
#include "sys_utils.h"
#include "str_utils.h"

/* Magic number sequences for common image filetypes */
static const short jpg_len = 2;
static const char jpg_seq[jpg_len] = {'\xFF', '\xD8'};
static const short png_len = 8;
static const char png_seq[png_len] = {'\211', 'P', 'N', 'G', 
                                        '\r', '\n', '\032', '\n'};


class Image_Retriever {
        public:
                Image_Retriever();
                ~Image_Retriever();
                
                /* 
                 * Given identifying strings, retrieves the album art for an 
                 * album and passes on a filename, with prodigious error 
                 * checking. Optionally, a number of known bad image results
                 * to skip may be supplied.
                 */
                std::string art_from_strings(std::string title, 
                                             std::string artist, uint skip = 0);

                /* Downloads art as with art_from_strings, but from an Album. */
                std::string art_from_album(Album* album);
                
                /* 
                 * Retrieves the next image result for the last album 
                 * for which art was requested. 
                 */
                std::string art_from_last();

                /* Resets the number of albums to skip to zero. */
                void reset_skip() { last_skip = 0; }

        private:
                /* METHODS */

                /* Returns the google search URL for a given query. */
                std::string url_from_strings(std::string title, 
                                             std::string artist);

                /* Tests if a downloaded file has a file extension. */
                bool has_extension(std::string url);

                /* Tests if an image format is met (image magic number). */
                bool is_valid_image(const std::string filename, bool is_jpg);

                /* DATA MEMBERS */
                Google_Images *goog;
                std::string last_artist;
                std::string last_title;
                uint last_skip;
};

#endif