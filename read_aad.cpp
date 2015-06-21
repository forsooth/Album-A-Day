/*
 * Filename: read_aad.cpp
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Takes as input the location of a locally stored *.aad file,
 * and reads the file into a convenient data structure. 
 *
 */

#include "read_aad.h"

/* PUBLIC */

AAD_Reader::AAD_Reader(std::string file_path)
{
        this->file_path = file_path;
        month_index = 0;
}

AAD_Reader::~AAD_Reader()
{
        for (uint i = 0; i < months.size(); i++) 
                delete months[i];
}

std::vector<Month*> AAD_Reader::extract_months()
{
        std::string line;
        std::ifstream in(file_path);
        
        if (!in.is_open()) {
                std::cerr << "ERROR 005: Failed to open selected file. "
                          << "Returning month list as it is." 
                          << std::endl;
                return months;
        } 
        
        Month *curr_month = NULL;
        while (std::getline(in, line)) {

                if (line.length() >= 3 && line.compare(0, 3, "END") == 0) {
                        if (curr_month != NULL) 
                                months.push_back(curr_month);
                        break;
                }

                if(line.length() == 0 || line[0] == ' ' || line[0] == '=')
                        continue;

                /* Day lines begin with an int for the day of the month */
                if (!std::isdigit(line[0]) && line[0] != delim) {
                        std::cerr << "ERROR 007: Bad line formatting. "
                                  << "Expected digit to define a day. "
                                  << "Discarding line: " << line
                                  << std::endl;
                        continue;
                }

                if (line[0] == delim) {
                        /* 
                         * A line beginning with the delimiter specifies the 
                         * end of a month, so we push the current month to 
                         * storage and fetch a new line to begin a new month.
                         */
                        if (curr_month) 
                                months.push_back(curr_month);

                        std::getline(in, line);
                        std::vector<std::string> month_data;

                        /* Extract month and year from formatting. */
                        boost::split(month_data, line, boost::is_any_of(space));
                        std::string month_name = "";
                        short year = 0;
                        for (uint i = 0; i < month_data.size(); i++) {

                                /*
                                 * month_dats should only contain a name and a
                                 * year, in that order, or the file is invalid.
                                 * So if name is empty, fill it, otherwise if
                                 * year is empty, fill it. This has the added 
                                 * benefit of allowing garbage after the year.
                                 */
                                if (!month_data[i].empty() && 
                                                month_name.empty()) {
                                        month_name = month_data[i];

                                } else if (!month_data[i].empty() 
                                                        && year == 0) {
                                        year = std::atoi(month_data[i].c_str());
                                }        
                        }

                        curr_month = new Month(month_name, year);

#ifdef DEBUG_READ
                        std::cerr << std::endl << "NEW MONTH: " << month_name
                                  << ", " << year 
                                  << std::endl;
#endif
                        
                        /* Month declarations are followed by an unused line */
                        std::getline(in, line);
                        continue;

                }

                if (curr_month == NULL) {
                        std::cerr << "ERROR 006: Cannot have a day " 
                                  << "entry before encountering a "
                                  << "month. Skipping line."
                                  << std::endl;
                        continue;
                }

                short day = 0, year = 0;
                std::string title = std::string(), artist = std::string();

                /* One line declares the data for one album */
                std::vector<std::string> album_data; 
                /* Split line by delimiter */
                boost::split(album_data, line, boost::is_any_of(delim_str));
                        
                /* Remove extra whitespace */
                for (uint i = 0; i < album_data.size(); i++) {
                        album_data[i] = boost::trim_copy(album_data[i]);
                }

                /* Error if not enough delims to match standard */
                if (album_data.size() < 4) {
                        std::cerr << "ERROR 007: Bad line formatting. "
                                  << "Not enough delimiters. "
                                  << "Discarding line: " << line
                                  << std::endl;
                        continue;
                }

                /* 
                 * Can't take substring if year is less than 1 digit or if year
                 * is too large to be a year. */
                if (album_data[3].length() < 3 || album_data[3].length() > 6) {
                        std::cerr << "ERROR 007: Bad line formatting. "
                                  << "Year not in range 10 to 9999. "
                                  << "Discarding line: " << line
                                  << std::endl;
                        continue;
                }

                /*
                 * NOTE: atoi causes undefined behavior if the integer is out
                 * of bounds of the int type, 0 if the text begins with a 
                 * non-whitespace, non-digit, non-sign character, and the number
                 * if the array begins with a number and ends with a string. */
                day = std::atoi(album_data[0].c_str());

                if (day > curr_month->get_num_days()) {
                        std::cerr << "ERROR 007: Bad line formatting. "
                                  << "Day number not valid for this month. "
                                  << "Discarding line: " << line
                                  << std::endl;
                        continue;
                }

                artist = album_data[1];
                title = album_data[2];

                /* Year is surrounded by parentheses in file. Remove them. */
                album_data[3] = album_data[3].substr(1, 
                                                    album_data[3].length() - 2);
                
                year = std::atoi(album_data[3].c_str());

#ifdef DEBUG_READ
                std::cerr << "Day: " << day 
                          << ", Album: " << title 
                          << " (" << year 
                          << "), artist: " << artist 
                          << std::endl;
#endif

                curr_month->add_album(day, title, artist, year);
                        
        }

        in.close();
        return months;
}

Month* AAD_Reader::next_month()
{
        if (month_index < months.size()) 
                return months.at(month_index++);
        return NULL;
}


