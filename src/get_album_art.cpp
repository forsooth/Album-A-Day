/*
 * Filename: get_album_art.cpp
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Given data about one or more albums, retrieves and downloads 
 * album art as CImg objects. 
 *
 */

#include "get_album_art.h"

#define GOOGLE_IMAGES_URL "http://www.google.com/search?site=&tbm=isch&q="


/* PUBLIC */

Image_Retriever::Image_Retriever() 
{
        last_skip = 0;
        last_artist = "";
        last_title = "";
        goog = NULL;
}

Image_Retriever::~Image_Retriever()
{
        if (goog != NULL)
                delete goog;
}

/* 
 * The method loops through google image results, and for each one runs a 
 * gauntlet of error checking methods to maximize the chance that the image
 * is as promised. The method first checks that there is a file extension on
 * the url of the file. If there is, it replaces it with an all lowercase
 * version, and removes all trailing characters. The file is written to disk,
 * and checked for the magic number identifier for the given image type. This
 * also catches any files which were written incorrectly and have a size of 0
 * bytes. 
 */
std::string Image_Retriever::art_from_strings(std::string title, 
                                                  std::string artist, uint skip)
{

        /* If skip is 0, we need to refresh the album information. */
        if (skip == 0) {

#ifdef DEBUG_ART        
                std::cerr << std::endl << std::endl << "FETCHING ALBUM ART FOR:" 
                          << std::endl << "Artist: " << artist 
                          << std::endl << "Album: " << title 
                          << std::endl;
#endif

                last_artist = artist;
                last_title = title;
                if (goog != NULL) 
                        delete goog;
                
                std::string url = url_from_strings(title, artist);
                std::string html = retrieve_html(url);
                
                /* Load HTML and regex to find and store URLs. */
                goog = new Google_Images(html);
                goog->parse_loaded();
        }

        /* Strings are used to store the name of the downloaded file on disk. */
        std::string filename = std::string();
        std::string extension = std::string();
        
        /* Loop conditional for finding a valid image. */
        bool is_valid = false;

#ifdef DEBUG_ART
        std::cerr << goog->num_matches() << " images found." << std::endl;
#endif

        std::string url = std::string();


        /* 
         * Main loop, fetches images while we haven't found a valid one and 
         * there are still images to be fetched.
         */
        for (uint i = last_skip; i < goog->num_matches() && !is_valid; i++) {
                
                /* 
                 * Set last skip so that if this image is found to be valid by
                 * the error checking here but cannot be opened by another 
                 * module, and a new image is requested, the loop can pick
                 * up where it left off.
                 */
                last_skip = i;

                /* Innocent until proven guilty. */
                is_valid = true;

#ifdef DEBUG_ART
                std::cerr << "Testing image " << i << "." << std::endl;
#endif
                /* fetch url of current image */
                url = goog->get_img(i);

                is_valid = has_extension(url);
                
                if (!is_valid)
                        continue;

                uint last_dot_index = url.find_last_of('.');
                std::string ext = url.substr(last_dot_index);

                /* Find file extension of file and assert it is JPG or PNG */
                bool is_jpg = true;

                if (boost::iequals(ext.substr(0, 4), ".jpg")) 
                        ext = ".jpg";
                else if (boost::iequals(ext.substr(0, 5), ".jpeg")) 
                        ext = ".jpeg";
                else if (boost::iequals(ext.substr(0, 4), ".png")) {
                        ext = ".png";
                        is_jpg = false;
                } else {
                        std::cerr << "ERROR 012: Image was not a valid "
                                  << "JPG or PNG image file." 
                                  << std::endl;
                        is_valid = false;
                        continue;
                }

                if(!is_valid)
                        continue;

                url = url.substr(0, last_dot_index);
                url += ext;

                /* Filenames are simple randomized integers */
                srand(time(NULL));
                filename = std::to_string(rand()) + ext;

#ifdef DEBUG_ART
                std::cerr << "Image URL: " << url << std::endl;
                std::cerr << "Filename to write: " << filename << std::endl;
                std::cerr << "Entering write_bytes to write image to disk." 
                          << std::endl;
#endif

                write_bytes(url, filename);
                
#ifdef DEBUG_ART
                std::cerr << "Validating image integrity." << std::endl;
#endif
                is_valid = is_valid_image(filename, is_jpg);
                if(!is_valid)
                        continue;
                 
        }


        if(!is_valid) {
                std::cerr << "ERROR 014: All images expended." << std::endl;
                return "";
        }

        return filename;

}

std::string Image_Retriever::art_from_last()
{
        last_skip++;
        return art_from_strings(last_title, last_artist, last_skip);
}

std::string Image_Retriever::art_from_album(Album* album)
{
        std::string title = album->get_title();
        std::string artist = album->get_artist();
        return art_from_strings(title, artist);
}


/* PRIVATE */

std::string Image_Retriever::url_from_strings(std::string title, 
                                                     std::string artist)
{
        std::string url = GOOGLE_IMAGES_URL;
        url += replace(title, ' ', '+');
        url += "+";
        url += replace(artist, ' ', '+');
#ifdef QUERY_EXTRA
        url += "+album+cover";
#endif
        return url;
}

bool Image_Retriever::has_extension(std::string url)
{
        uint last_dot_index = url.find_last_of('.');
        if(last_dot_index == std::string::npos) {
                std::cerr << "ERROR 011: No file extension found in "
                          << "image link. Fetching next image." 
                          << std::endl;
                return false;
        }

        return true;
}

bool Image_Retriever::is_valid_image(const std::string filename, bool is_jpg)
{
        /* Create a binary file input stream called f */
        std::ifstream f(filename, std::ios::binary | std::ios::in);
        char byte;
        const char *magic_num = is_jpg ? jpg_seq : png_seq;
        short len = is_jpg ? jpg_len : png_len;
                
        for(short i = 0; i < len; i++) {
                f.get(byte);
                if(f.eof() || byte != magic_num[i]) {
                        std::cerr << "ERROR 013: First bytes of downloaded "
                                  << "file did not match expected magic number "
                                  << "for the image type." << std::endl;
                        delete_img(filename.c_str());
                        return false;
                }
        }        

        f.close();
        return true;  
}