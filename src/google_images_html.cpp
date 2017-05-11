/*
 * Filename: google_images_html.cpp
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Finds image URLs from the source HTML of a google images
 * webpage. 
 *
 */

#include "google_images_html.h"

/* PUBLIC */

Google_Images::Google_Images()
{
        html = std::string();
}

Google_Images::Google_Images(const std::string html)
{
        this->html = html;
}

void Google_Images::parse_new(const std::string html)
{
        this->html = html;
        parse_loaded();
}

void Google_Images::parse_loaded()
{
        /* Create boost object from constant */
        boost::regex expr(regex_string);

        /* Need iterators for search */
        std::string::const_iterator start, end;
        start = html.begin();
        end = html.end();   

        /* 
         * Match result object will be filled with a result each time the
         * loop iterates 
         */
        boost::match_results<std::string::const_iterator> m;
        
        /* 
         * Default flags are the automatic flags, but we will be modifying
         * them as we go so we need to store a copy separately.
         */
        boost::match_flag_type flags = boost::match_default;

        while (boost::regex_search(start, end, m, expr, flags)) {
                std::string url = m[0];

                /* Removes the google images */
                url = url.substr(substr_b, url.length() - substr_e);

                /* Fixes Google's mangling of the '%' character. */
                url = replace_all(url, "%25", "%");

                urls.push_back(url);
                start = m[0].second; 
                flags |= boost::match_prev_avail;
                flags |= boost::match_not_bob;
        }
}

std::string Google_Images::first_img()
{
        if (urls.empty()) {
                std::cerr << "ERROR 010: Not enough matches to retrieve "
                          << "the requested URL. Number of matches: " 
                          << urls.size() << std::endl;
                return std::string();
        }

        return urls[0];
}

std::vector<std::string> Google_Images::all_imgs()
{
        return urls;
}

std::string Google_Images::get_img(uint i)
{
        if (urls.size() <= i) {
                std::cerr << "ERROR 010: Not enough matches to retrieve "
                          << "the requested URL. Number of matches: " 
                          << urls.size() << std::endl;
                return std::string();
        }

        return urls[i];
}

uint Google_Images::num_matches()
{
        return urls.size();
}