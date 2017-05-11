/*
 * Filename: write_imgs.cpp
 * Author: Matt Asnes
 * Date: June 10, 2015
 * Description: Takes a Month, with its associated album data, and outputs
 * a PNG or JPG image file based on the data.
 *
 */

#include "write_imgs.h"

/* PUBLIC */

AAD_Writer::AAD_Writer()
{
        calendar = NULL;
        canvas = NULL;
        month = NULL;
        /* By default, CImg pops up user-closed windows on exception. */
        cimg::exception_mode(0);
}


AAD_Writer::~AAD_Writer()
{
        if (calendar != NULL) delete calendar;
        if (canvas != NULL) delete canvas;
}

void AAD_Writer::init_canvas(uint x, uint y)
{        
        init_measurements(x, y);
        uint z = 1;
        uint init_value = 255;
        //TODO: try-catch
        canvas = new CImg<unsigned char>(x, y, z, c, init_value);
        
        /* square for frame */
        draw_square_with_thickness(ml, mt, fw, t, canvas, black);

        /* line to make title box */
        draw_line_with_thickness(ml, nld, mrl, nld, t, canvas, black);

        /* vertical grid bars */
        for (uint i = ml + bw; i <= mrl; i += bw) {
                draw_line_with_thickness(i, nld, i, mbl, t, canvas, black);
        }

        /* horizontal grid bars */
        for (uint i = nld + bh; i <= mbl; i += bh) {
                draw_line_with_thickness(ml, i, mrl, i, t, canvas, black);                
        }

        /* horizontal lines to make artist info boxes */
        for (uint i = nld + dld; i <= mbl; i += bh) {
                draw_line_with_thickness(ml, i, mrl, i, t, canvas, black);                
        }

}

void AAD_Writer::load_month(Month *month)
{
#ifdef DEBUG_WRITE
        std::cerr << std::endl 
                  << "In load_month function. Making calendar from canvas." 
                  << std::endl;
#endif

        this->month = month;

        if (calendar != NULL) delete calendar;
        calendar = new CImg<unsigned char>(*canvas, false);

        std::string month_name = month->get_month_name();
        
        /* Convert month name to all uppercase for printing */
        std::transform(month_name.begin(), month_name.end(),
                                month_name.begin(), ::toupper);
        
        std::string year = std::to_string(month->get_year());

        std::string cal_title = get_cal_title(month_name, year);

#ifdef DEBUX_WRITE_HEX
        hex_print(example);
        hex_print(cal_title);
#endif

        /* Draw calendar title in the form "MONTH_NAME YEAR" */
        calendar->draw_text(center - tfm * cal_title.length(), ttd, 
                                cal_title.c_str(), black, white, o, 
                                        CImgList<>::font(tfs, false));

#ifdef DEBUG_WRITE
        std::cerr << "Beginning album art loop." << std::endl;
#endif
        /* Needed for math. */
        uint first_day = day_of_week(1, month->get_month_num(),
                                                 month->get_year());

        /* Set up album art image retriever. */
        Image_Retriever *r = new Image_Retriever();

        /* 
         * Keep track of the last day for which an album was retrieved, in case
         * the program encounters an error and needs to fetch another for the
         * same day.
          */
        short last_index = 0;

        /* Main calendar drawing loop. Pastes album art, draws text. */
        for (short i = 1; i <= month->get_num_days(); i++) {

                /* Get the day of the week and week of the month for day i */
                uint day = day_of_week(i, month->get_month_num(),
                                                 month->get_year());

                /* 
                 * Algorithm for finding week of the month. See reference in 
                 * source (week_algorithm.txt) if curious.
                 */
                uint week = (i - 1 + first_day) / 7;

                /* 
                 * If the week will cause drawing past image borders (canvas 
                 * initializes with five weeks, some months have 6, if they 
                 * start on a Friday or Saturday), this adds additional weeks.
                 */
                if (week >= weeks_in_month) {
                        extend_calendar(week);
                        weeks_in_month++;

                }

                if (!month->has_album_for_day(i)) {
                        /* 
                         * Attempt to draw a day box for the day. May fail, but
                         * there is no recourse for an uncooperative CImg.
                         */
                        draw_day_box(i, day, week);
                        continue;
                }
                        
                std::string title = month->get_album_title(i);
                std::string artist = month->get_album_artist(i);
                std::string fn;
                
                /* If we are repeating an image-fetch instead of restarting */
                if (i == last_index) {
                        fn = r->art_from_last();
                } else {
                        r->reset_skip();
                        last_index = i;
                        fn = r->art_from_strings(title, artist);
                }

                /* The Image Retriever object returns "" upon error. */
                while (fn.empty()) {
                        std::cerr << "ERROR 017: Found empty filename string "
                                  << "when attempting to draw album art." 
                                  << std::endl;
                        fn = r->art_from_last();
                }
                

                const char *art_filename = fn.c_str();
#ifdef DEBUG_WRITE
                std::cerr << "drawing art from filename " <<art_filename 
                          << std::endl; 
#endif      

                try {
                        /* Make a little image for just the album art. */
                        CImg<unsigned char> art(art_filename);

#ifdef DEBUG_WRITE         
                        std::cerr << "Image found, dimensions of " 
                                  << art.width() << "x" << art.height() 
                                  << "pixels." << std::endl;
#endif
                        /* Very small images will appear grainy, discard them */
                        if (art.width() < min_aw || art.height() < min_aw) {
                                std::cerr << "ERROR 018: Image height or width "
                                          << "too small (less than " 
                                          << min_aw << "pixels)." 
                                          << std::endl;
                                delete_img(art_filename);
                                i--;
                                continue;
                        }

                        /* Album art is square, and images are everywhere. */
                        double aspect_ratio = art.width() / art.height();
                        if (aspect_ratio > 1 + ar_tol || 
                                        aspect_ratio < 1 - ar_tol) {
                                std::cerr << "ERROR 019: Image aspect ratio "
                                          << "too far from 1 (greater than "
                                          << 1 + ar_tol 
                                          << " or less than "
                                          << 1 - ar_tol << ")." 
                                          << std::endl;
                                delete_img(art_filename);
                                i--;
                                continue;
                        }

                        /* Resize the image to be square. */
                        aspect_ratio > 1 ? 
                                art.resize(biw, biw/aspect_ratio, intrpl, c, o)  
                              : art.resize(biw/aspect_ratio, biw, intrpl, c, o);

#ifdef DEBUG_WRITE
                        std::cerr << "day: " << day << ", week: " 
                                  << week << std::endl;
                        
                        std::cerr << "Drawing image at: (" 
                                  << ml + day * bw + p << ", " 
                                  << nld + dld + bh * week + p << ")." 
                        << std::endl;
#endif
                        /* Paste the album art where it belongs. */
                        calendar->draw_image(ml + day * bw + p, 
                                             nld + dld + bh * week + p, art, o);

                        /* Make album title and artist strings fit in box. */
                        std::string cut_artist = artist, cut_title = title;
                        cut_artist = get_cut_string(cut_artist, afs);
                        cut_title = get_cut_string(cut_title, afs);  

                        calendar->draw_text(ml + day * bw + 2*p, 
                                            nld + bh * week + 4*p, 
                                            cut_title.c_str(), black, white, o,
                                            CImgList<>::font(afs, true));
                        

                        calendar->draw_text(ml + day * bw + 2*p, 
                                            nld + bh * week + afs + 8*p, 
                                            cut_artist.c_str(), black, white, o,
                                            CImgList<>::font(afs, true));

#ifdef SAVE_OFTEN
                        calendar->save("cal.jpg");
#endif

#ifdef DISPLAY_OFTEN
                        display_image();
#endif
                        } catch (CImgInstanceException &e) {
                                std::cerr << "ERROR 016: Image could not "
                                          << "be read. CImg library gave "
                                          << "error: " << e.what() 
                                          << std::endl;
                                delete_img(art_filename);
                                i--;
                                continue;
                        } catch (CImgIOException &e) {
                                std::cerr << "ERROR 016: Image could not "
                                          << "be read. CImg library gave "
                                          << "error: " << e.what() 
                                          << std::endl;
                                delete_img(art_filename);
                                i--;
                                continue;
                        } catch (CImgArgumentException &e) {
                                std::cerr << "ERROR 016: Argument exception. "
                                          << "CImg library gave error: " 
                                          << e.what() << std::endl;
                                delete_img(art_filename);
                                i--;
                                continue;
                        }
#ifdef DEBUG_WRITE
                        std::cerr << "Deleting image " << art_filename << "." 
                                  << std::endl;
#endif
                        delete_img(art_filename);
                        /* Album days need day labels too! */
                        draw_day_box(i, day, week);
        }

        delete r;

#ifdef SAVE_OFTEN
        calendar->save("cal.jpg");
#endif

}


void AAD_Writer::save_image(std::string filename) {
        calendar->save(filename.c_str());
}


CImg<unsigned char>* AAD_Writer::get_image() {
        return calendar;
}

void AAD_Writer::display_image()
{
        CImgDisplay main_disp(*calendar, "Calendar Image Display");
        while (!main_disp.is_closed()) {
                main_disp.wait();
        }
}


#ifdef DEBUG_WRITE
void AAD_Writer::display_test()
{
        canvas = new CImg<unsigned char>("modest_mouse.jpg");
        CImgDisplay main_disp(*canvas, "Image Display");
        while (!main_disp.is_closed()) {
                main_disp.wait();
        }
        delete canvas;
}
#endif

/* PRIVATE */

void AAD_Writer::init_measurements(uint x, uint y)
{
        width = x;
        height = y;
        center = width / 2;

        o = 1;
        c = 3;
        intrpl = 1;

        ml = width / 40;
        mt = height / 40;
        fw = width - (2 * ml);
        fh = height - (2 * mt);
        mrl = width - ml;
        mbl = height - ml;
        nld = height / 10;

        t = (width / 500) + 1;
        
        bw = fw / days_per_week;
        bh = (fh - (nld - mt)) / weeks_in_month;
        dld = bh > bw ? bh - bw : bw - bh;
        p = t/2 + 1;
        biw = bw - t;

        tfs = width / 20;
        tfm = width / 50;
        ttd = (nld - mt) / 2;

        dbw = bw / 3.75;
        dfm = dbw / 12;
        dfs = width / 40;
        dtd = dbw / 5;

        afs = width / 91;

        min_aw = bw > 100 ? 100 : bw;
        ar_tol = 0.3;
   
}



void AAD_Writer::extend_calendar(uint week) 
{     
        CImg<unsigned char> *extended;   

        try {
#ifdef DEBUG_WRITE
                std::cerr << "Extending calendar by one week." << std::endl;
#endif
                extended = new CImg<unsigned char>(calendar->width(), 
                                                   calendar->height() + bh,
                                                   calendar->depth(), 
                                                   calendar->spectrum(), 255); 
                height += bh;       
                fh += bh;
                mbl = height - ml;
        
                extended->draw_image(0, 0, *calendar, 1);

                /* vertical grid bars */
                for (uint i = ml; i < mrl; i += bw) {
                        draw_line_with_thickness(i, nld, i, mbl, 
                                                         t, extended, black);
                }

                draw_line_with_thickness(mrl, nld, mrl, mbl, 
                                                        t, extended, black);

                /* horizontal grid bars */
                for (uint i = nld; i <= mbl; i += bh) {
                        draw_line_with_thickness(ml, i, mrl, i, t, 
                                                        extended, black);                
                }

                /* horizontal lines to make artist info boxes */
                for (uint i = nld + dld; i <= mbl; i += bh) {
                        draw_line_with_thickness(ml, i, mrl, i, t, 
                                                        extended, black);                
                }   

#ifdef DEBUG_WRITE
                std::cerr << "Deleting old calendar." << std::endl;
#endif

                delete calendar;

#ifdef DEBUG_WRITE
                std::cerr << "Setting pointers." << std::endl;
#endif

                calendar = extended;

#ifdef DEBUG_WRITE
                std::cerr << "Returning." << std::endl;   
#endif

        } catch (CImgInstanceException &e) {
                std::cerr << "ERROR 016: Image could not be read. "
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
        } catch (CImgIOException &e) {
                std::cerr << "ERROR 016: Image could not be read. "
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
        } catch (CImgArgumentException &e) {
                std::cerr << "ERROR 016: Argument exception. "
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
        }


}


uint AAD_Writer::day_of_week(int d, int m, int y)
{
    static uint t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

std::string AAD_Writer::get_cal_title(std::string month_name, std::string year)
{
        std::string cal_title = std::string();
        uint chars = 0;

        for (; chars < month_name.length(); chars++) {
                if (month_name[chars] == '\0') 
                        continue;
                cal_title += month_name[chars];
        }

        cal_title += " ";
        
        for (; chars <= month_name.length() + year.length(); chars++) {
                if (year[chars - month_name.length() - 1] == '\0') 
                        continue;
                cal_title += year[chars - month_name.length() - 1];
                
        }

        return cal_title;
}

bool AAD_Writer::draw_day_box(uint day, uint dow, uint week)
{
        try {
                /* Create a new, small image for the day box. */
                /* Zeros are for z and fill color. */
                CImg<unsigned char> day_box(dbw, dbw, 1, c, 0);
                day_box.draw_rectangle(0, 0, dbw - t, dbw - t, white, o);

                /* Math to center the text in the box. */
                short digit_shift = (day >= 10) ? dfm : 2*dfm;

                /* Write the text. */
                const char *day_num = std::to_string(day).c_str();
                day_box.draw_text(dfm + digit_shift, dtd, day_num, 
                                  black, white, o, CImgList<>::font(dfs, true));

                /* Paste the completed day box into the calendar. */
                calendar->draw_image(ml + dow * bw + p, 
                                         nld + dld + bh * week + p, day_box, o);

        } catch (CImgInstanceException &e) {
                std::cerr << "ERROR 016: Image could not be read. " 
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
                return true;
        } catch (CImgIOException &e) {
                std::cerr << "ERROR 016: Image could not be read. " 
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
                return true;
        } catch (CImgArgumentException &e) {
                std::cerr << "ERROR 016: Argument exception. " 
                          << "CImg library gave error: " << e.what() 
                          << std::endl;
                return true;
        }

        return false;
}

std::string AAD_Writer::get_cut_string(std::string str, uint font_size) 
{
        /* Create empty 0x0 image. */
        CImg<unsigned char> boundaries;

        /* Draw text in it. This will auto-scale the image to the text. */
        boundaries.draw_text(0,0, str.c_str(), black, white, o, 
                                         CImgList<>::font(font_size, true));

        uint curr_width = boundaries.width();
        uint str_len = str.length();

        if (curr_width >= bw) { 
                while (curr_width >= bw - 7 * p) {
                        CImg<unsigned char> cut_boundaries;
                        str = str.substr(0, --str_len);
                        cut_boundaries.draw_text(0, 0, str.c_str(), black, 
                                white, o, CImgList<>::font(font_size, true));
                        curr_width = cut_boundaries.width();
                }

                str += "...";
        }

        return str;

}


/* Draw the four lines for a square, given a side length. */
void AAD_Writer::draw_square(uint x, uint y, uint s, CImg<unsigned char> *img, 
                                                     const unsigned char *color)
{
        img->draw_line(x, y, x + s, y, color);
        img->draw_line(x, y, x, y + s, color);
        img->draw_line(x, y + s, x + s, y + s, color);
        img->draw_line(x + s, y, x + s, y + s, color);
}


/* Draws the four lines for a square repeatedly */
void AAD_Writer::draw_square_with_thickness(uint x, uint y, uint s, 
                                            short thickness, 
                                            CImg<unsigned char> *img, 
                                            const unsigned char *color)
{
        /* int, not uint, because thickness is small and i < 0 to start */
        for (int i = 0 - thickness / 2; i <= thickness / 2; i++) {
                uint x_copy = x + i;
                uint y_copy = y + i;
                img->draw_line(x_copy, y_copy, x_copy + s, y_copy, color);
                img->draw_line(x_copy, y, x_copy, y_copy + s, color);
                img->draw_line(x_copy, y_copy + s, x_copy + s, 
                                                   y_copy + s, color);
                img->draw_line(x_copy + s, y_copy, x_copy + s, 
                                                   y_copy + s, color);
        }
}

/* 
 * Draws lines around the original points. Note that this does not account
 * for angles, and so only works well for straight lines. Also, the ends of the
 * lines are not guaranteed to be squared off, and will differ by one pixel
 * diagonally from the previous line.
 */
void AAD_Writer::draw_line_with_thickness(uint x0, uint y0, uint x1, uint y1,
                                          short thickness, 
                                          CImg<unsigned char> *img,
                                          const unsigned char *color)
{
        for (int i = 0 - thickness / 2; i <= thickness / 2; i++) {
                img->draw_line(x0 + i, y0 + i, x1 + i, y1 + i, color);
        }
}


