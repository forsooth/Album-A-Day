/*
 * Filename: retrieve_html.h
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Makes an HTTP request to the given URL and returns the resulting
 * HTML as a C++ string. 
 * Note: Adapted from the example cURLpp program created by Chris Laplante at
 * http://www.mostthingsweb.com/2014/11/parsing-html-c-revisited/
 *
 */

#ifndef RETRIEVE_HTML_H
#define RETRIEVE_HTML_H

#include <curlpp/cURLpp.hpp> /* Network requests */
#include <curlpp/Easy.hpp> /* Network requests */
#include <curlpp/Options.hpp> /* Network request options */
#include <sstream> /* String stream, for writing text files to disk */
#include <iostream> /* Standard IO, for printing files to standart out */
#include <fstream> /* File stream, for writing binary files to disk */
#include <string> /* For parameters */
 
/* 
 * Given a URL string, retrieves the html for the given page as strings. Note
 * that this does not check for errors in the URL format. If cURLpp throws
 * an exception, this function will return an empty string. */
std::string retrieve_html(std::string url);

/*
 * Given a URL string, this function will write the file as binary to 
 * standard out. Note that this does not check for errors in the URL format.
 */
void print_bytes(std::string url);

/* 
 * Given a URL string, this writes the file as binary data to disk as a file
 * named 'filename'. If no argument is supplied the string 'test_file' is used. 
 */
void write_bytes(std::string url, std::string filename = "test_file");

/* 
 * TODO: write a function for downloading binary data to a data structure, 
 * like with HTML into strings.
 */

#endif