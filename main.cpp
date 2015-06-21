/*
 * Filename: main.cpp
 * Author: Matt Asnes
 * Date: June 9, 2015
 * Description: Runs the program.
 *
 */

#include "read_aad.h"
#include "write_imgs.h"
#include "month.h"

 int main(int argc, char* argv[])
 {
        if(argc < 2) {
                std::cerr << "ERROR 000: Not enough arguments. "
                          << "Re-run the program as ./aad [filename]"
                          <<std::endl;
                exit(1);
        }

        std::string file_in = std::string(argv[1]);

        AAD_Reader *reader = 
        //              new AAD_Reader("/home/matt/Dropbox/AlbumADay/test.aad");
        //              new AAD_Reader("test.aad");
                        new AAD_Reader(file_in);
        std::vector<Month*> months = reader->extract_months();
        std::cerr << "Got " << months.size() << " months." << std::endl;
        AAD_Writer *writer = new AAD_Writer();
        writer->init_canvas();
       
        int i = 1;
        srand (time(NULL));
        
        Month *m = reader->next_month();        
        
        while(m != NULL) {
                std::cerr << "Reading month " << i++ << "." << std::endl;
                writer->load_month(m);
                std::string filename = std::string();
                filename += m->get_month_name();
                filename += "-" + std::to_string(m->get_year());
                filename += "-" + std::to_string(rand() % 65535);
                filename += ".jpg";
                writer->save_image(filename);
                m = reader->next_month();
        }

        delete reader;
        delete writer;

        return 0;
 }