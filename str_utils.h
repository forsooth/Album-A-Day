/*
 * Filename: str_utils.h
 * Author: Matt Asnes
 * Date: June 19, 2015
 * Description: Various string utilities used throughout the program.
 *
 */

#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <iostream> /* Standard IO */
#include <string> /* Obviously */

/*
 * Given a string, prints the contents of the string's bytes as hexadecimal
 * to standard error. 
 * Adapted from a StackOverflow answer by user195488 on August 1st, 2010
 * at: 
 *      http://stackoverflow.com/questions/3381614/
 */
void hex_print(std::string str);

/*
 * Replaces all instances of ch1 in the string with ch2.
 * Adapted from:
 *       http://www.cplusplus.com/forum/beginner/33835/
 */
std::string replace(std::string str, char ch1, char ch2);


/*
 * Replaces all instances of the substring search in the string with the
 * string replace. Note that this is implemented in a way that the new string,
 * after a replacement is done, is scanned again. This has no overflow 
 * protection and will run infinitely if replace contains search, but is 
 * necessary behavior. 
 * Adapted from a stackoverflow answer by Czarek Tomczak on February 4th, 2013
 * at:
 *      http://stackoverflow.com/questions/5343190/ 
 */
std::string replace_all(std::string subject, const std::string& search,
                                                    const std::string& replace);


#endif