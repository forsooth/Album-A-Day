/*
 * Filename: sys_utils.h
 * Author: Matt Asnes
 * Date: June 19, 2015
 * Description: Various system utilities used throughout the program.
 *
 */

#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include <iostream> /* Standard IO */
#include <cstdio> /* File deletion */

/* 
 * Given a character array as a file name, attempts to delete the file and 
 * returns true if the deletion was successful. Prints an error if file deletion
 * was unsucessful. 
 * Adapted from the example found at:
 *      http://www.cplusplus.com/reference/cstdio/remove/
 */
bool delete_img(const char *file);

#endif