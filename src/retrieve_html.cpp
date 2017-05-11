/*
 * Filename: retrieve_html.cpp
 * Author: Matt Asnes
 * Date: June 11, 2015
 * Description: Makes an HTTP request to the given URL and returns the resulting
 * HTML as a C++ string. 
 * Note: Adapted from the example cURLpp program created by Chris Laplante at
 * http://www.mostthingsweb.com/2014/11/parsing-html-c-revisited/
 *
 */

#include "retrieve_html.h"
 
#define HEADER_ACCEPT "Accept:text/html,application/xhtml+xml,application/xml"
#define HEADER_USER_AGENT "User-Agent:Mozilla/5.0 (X11; Linux x86_64) \
AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.70 Safari/537.17"
 
std::string retrieve_html(std::string url)
{
        try {
                curlpp::Easy request;
 
                /* Specifies the URL */
                request.setOpt(curlpp::options::Url(url));
 
                /* Gives cURLpp the headers to send the request */
                std::list<std::string> headers;
                headers.push_back(HEADER_ACCEPT);
                headers.push_back(HEADER_USER_AGENT);
                request.setOpt(curlpp::options::HttpHeader(headers));
                request.setOpt(curlpp::options::FollowLocation(true));
 
                /* Configure curlpp to use the string stream to write */
                std::ostringstream responseStream;
                curlpp::options::WriteStream streamWriter(&responseStream);
                request.setOpt(streamWriter);
 
                /* Performs the request */
                request.perform();
                return responseStream.str();

        } catch (curlpp::RuntimeError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;
        } catch (curlpp::LogicError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;        
        }

        return std::string();
}

void print_bytes(std::string url)
{
        try {                
                /* 
                 * Set the writer callback to enable cURL to write result 
                 * in a memory area (i.e. std out)
                 */
                curlpp::Easy request;
                request.setOpt(curlpp::options::WriteStream(&std::cout));
        
                /* Setting the URL and fetching its data */
                request.setOpt(curlpp::options::Url(url));
                request.perform();

        } catch (curlpp::RuntimeError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;
        } catch (curlpp::LogicError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;        
        }
}


void write_bytes(std::string url, std::string filename)
{
        try {
                curlpp::Easy request;

                std::ofstream out(filename);

                if (out.fail()) {
                        std::cerr << "ERROR 009: Failed to open file "
                                  << filename << " while writng to disk. "
                                  << "No file will be written."
                                  << std::endl;
                        out.close();
                        return;
                }

                /* 
                 * Set the writer callback to enable cURL to write result 
                 * in a memory area (i.e. std out)
                 */
                request.setOpt(curlpp::options::WriteStream(&out));
                
                /* Setting the URL and downloading its contents */
                request.setOpt(curlpp::options::Url(url));
                request.perform();
                out.close();
                
        } catch (curlpp::RuntimeError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;
        } catch (curlpp::LogicError &e) {
                std::cout << "ERROR 008: cURLpp encountered an error and "
                          << "threw the exception: " << e.what() 
                          << std::endl;        
        }


}