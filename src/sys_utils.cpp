/*
 * Filename: sys_utils.cpp
 * Author: Matt Asnes
 * Date: June 19, 2015
 * Description: Various system utilities used throughout the program.
 *
 */

#include "sys_utils.h"

bool delete_img(const char *file)
{
        if(std::remove(file) != 0) {
                std::cerr << "ERROR 015: Encountered error deleting "
                          << "file " << file << "." << std::endl;
                return false;
        }
#ifdef DEBUG_REMOVE
        std::cerr << "File " << file << " successfully deleted." << std::endl;
#endif
        return true;
}