/*
 * Filename: write_imgs.h
 * Author: Matt Asnes
 * Date: June 10, 2015
 * Description: Takes a Month, with its associated album data, and outputs
 * a PNG or JPG image file based on the data.
 *
 */

#ifndef WRITE_IMGS_H
#define WRITE_IMGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cassert>
#include "CImg.h"
#include "month.h"
#include "get_album_art.h"
#include "sys_utils.h"

using namespace cimg_library;

static const uint days_per_week = 7;

/* RGB values */
static const unsigned char black[3] = {0, 0, 0}; 
static const unsigned char white[3] = {255, 255, 255};

class AAD_Writer {
        public:
                AAD_Writer();
                ~AAD_Writer();
                
                /* 
                 * Runs the routine for initializing the canvas image to
                 * be a basic, empty calendar. Needs to be run only once, at
                 * the beginning of the writer's execution.
                 */
                void init_canvas(uint x = 2000, uint y = 2000);

                /* Load the data for a given month into the calendar. */
                void load_month(Month* month);
                
                /* Displays the contents of the calendar in a window. */
                void display_image();

                /* Saves the contents of the calendar to disk as filename. */
                void save_image(std::string filename);

                /* Returns the raw image data for further manipulation. */
                CImg<unsigned char>* get_image();

#ifdef DEBUG_WRITE
                /* Displays the test image "modest_mouse.jpg" in a window. */
                void display_test();
#endif

        private:
                /* METHODS */

                /* Draws four lines in a square of side length s at (x, y) */
                void draw_square(uint x, uint y, uint s, 
                                         CImg<unsigned char> *img, 
                                         const unsigned char *color);

                /* 
                 * Draws a horizontal or vertical line with the given thickness.
                 * Note that this method has unchecked, undefined behavior if
                 * the line is at an angle not 0 or 90 degrees. 
                 */
                void draw_line_with_thickness(uint x0, uint y0, uint x1, 
                                              uint y1, short thickness, 
                                              CImg<unsigned char>* img,
                                              const unsigned char* color);
                
                /* Draws a square with the given thickness. See draw_square. */
                void draw_square_with_thickness(uint x, uint y, uint s, 
                                                short thickness, 
                                                CImg<unsigned char> *img,
                                                const unsigned char *color);

                /* 
                 * Given a date, returns the day of the week the day is on.
                 * Uses Sakamoto's algorithm. */
                uint day_of_week(int d, int m, int y);

                /* 
                 * If the calendar length exceeds the original number of weeks
                 * allotted to it, this extends the calendar by drawing another
                 * row of weeks to it. Note that this deletes the old calendar
                 * and creates a new one from it. This function is untested for
                 * unrealistic numbers of weeks.
                 */
                void extend_calendar(uint week);

                std::string get_cal_title(std::string month_name, 
                                                        std::string year);

                bool draw_day_box(uint day, uint dow, uint week);

                std::string get_cut_string(std::string str, uint font_size);

                void init_measurements(uint x, uint y);


                /* DATA MEMBERS */

                Month *month;
                CImg<unsigned char> *canvas;
                CImg<unsigned char> *calendar;

                uint weeks_in_month = 5;

                /* Calendar measurements for use in drawing the calendar */
                
                /* Image global variables */
                uint width, /* Calendar width */
                     height, /* Calendar height */
                     center, /* Center of calendar */
                     o,   /* Default opacity */
                     c,   /* Default number of channels */
                     intrpl; /* Default interpolation method */

                /* Calendar body measurements */
                uint ml,  /* Left (and right) margin of the calendar */
                     mt,  /* Top (and bottom) margin of the calendar */
                     fw,  /* Width of calendar rectangle, width - margins */
                     fh,  /* Height of calendar rectangle, height - margins */
                     mrl, /* Margin right location, width - ml */
                     mbl, /* Margin bottom location, height - mr */
                     nld; /* Name line drop, distance from top to line between
                           * month name box and the calendar */

                /* Line thickness */
                uint t;   

                /* Individual box measurements */
                uint bw,  /* Box width for one calendar day */
                     bh,  /* Box height for one calendar day */
                     dld, /* Day line drop, bh - bw */
                     p,   /* Text box padding */
                     biw; /* Box inner-width (exclude border line) */
                     
                /* Type measurements & day label box */
                uint tfs, /* Title font size */
                     tfm, /* Title font margin */
                     ttd, /* Title text drop */
                     dbw, /* Day box width */
                     dfs, /* Day font size */
                     dfm, /* Day font margin */
                     dtd, /* Day text drop */
                     afs; /* Album artist/title font size */

                /* Error checking values */
                int min_aw; /* Minimum allowed album width/height */
                            /* Is an int because of values it's compared to */
                float ar_tol; /* Aspect ratio tolerance */

};

#endif