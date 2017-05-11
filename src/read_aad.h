/*
 * Filename: read_aad.h
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Takes as input the location of a locally stored *.aad file,
 * and reads the file into a convenient data structure. 
 *
 */

#ifndef READ_AAD_H
#define READ_AAD_H

#include <iostream> /* Error messages */
#include <fstream> /* File reading */
#include <vector> /* Data structures */
#include <boost/algorithm/string.hpp> /* String manipulation functions */
#include <string> /* Data members */
#include "month.h" /* Data members */

static const std::string delim_str = "-";
static const std::string space = " ";
static const char delim = '-';

class AAD_Reader {
        public:
                AAD_Reader(std::string file_path);
                ~AAD_Reader();
                
                /* 
                 * Reads the file specified by filename from the disk and 
                 * parses it for data. Must be run before any data access can
                 * be requested of the reader. Returns a vector of the extracted
                 * month data, though months may also be accessed one by one.
                 * Returns empty vector on error.
                 */
                std::vector<Month*> extract_months();
                
                /* 
                 * Retrieves months after reading one by one using an
                 * internal counter. Returns NULL if there are no more months.
                 */
                Month* next_month();

        private:
                /* DATA MEMBERS */
                std::vector<Month*> months;
                std::string file_path;
                uint month_index;
};

#endif