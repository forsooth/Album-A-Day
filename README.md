# Album A Day
Calendar creation tool for those listening to one album per day, on a schedule.

#### Introduction
My friend began a musical adventure several months ago, during which he has listened to one full album every day. He writes out the album lists and creates calendars for himself by hand in MS Paint for quick reference. I also like music, and began to follow in his footsteps a few days ago. However I ran into the snag that I really dislike doing rote tasks when I know they could be done programatically. With this, it should only take a few seconds to make a new calendar each month, for the two of us and also for any people interested by the music or the code.

#### Usage
The program is run from the command line, with one expected argument. If the program is compiled to the executable "`aad`" (album a day), on a file "`album_list.aad`", the expected command is `./aad album_list.aad`

This will save the images to the directory the executable is in. Support for custom save locations, as well as a simple GUI, may be implemented in future updates. 

#### Compiling

Compilation was tested on Ubuntu 14.04.2 LTS with g++, should be fine on other systems. 
Testing on Windows will be done soon, and this section will be updated with the results. I don't own a Mac, but any results would be appreciated.

The actual command used to compile on my system was:
```
g++ -Wall -L /lib64 -std=c++11 -O2 -g \
-o aad album.h album.cpp month.h \
month.cpp read_aad.h read_aad.cpp main.cpp retrieve_html.h retrieve_html.cpp \
google_images_html.h google_images_html.cpp get_album_art.h get_album_art.cpp \
write_imgs.h write_imgs.cpp sys_utils.h sys_utils.cpp str_utils.h \
str_utils.cpp CImg.h \
-lcurlpp -lcurl -lboost_regex -lpthread -lX11 \
`Magick++-config --cppflags --cxxflags --ldflags --libs`
```

This can be found in the `compile` file. Yes, this should be a Makefile, and I might make one eventually. Haha, get it, `make` one?

#### Dependencies
- curlpp
  * libcurl
- CImg
  * pthread
  * X11
  * libmagick++
- boost
  * boost_regex

#### Architecture
The program is split into two main phases: reading and writing. When reading, 
the file is parsed for album data, which is stored in a convenient structure. 
Once this is complete, a calendar is created for each month and for each day an
album cover is fetched from Google Images. This appears to produce fairly good 
results with some simple error checking. 

#### Input
As stated above, the file name for parsing is to be passed as a command line 
argument. The input is a simple text file, but the *.aad file extension is
the standard I have made. Read `aad_file_specification.txt` for a detailed
outline of what information the parser reads, and how.

#### Output
The images are output as .jpg files in the same directory where the program is 
found. The filenames follow the pattern: Month-Year-RandomNumer.jpg. The images
are 2000 by 2000 pixels. One image is created for every month of input.

Note that temporary files are created as the program runs, so if on the off 
chance someone has vital information stored in files with names in the random 
number range 0 to 2147483647, and with a .jpg extension, they have a 
1 in 2147483647 chance of being overwritten.  Be warned!

##### TODO
- GUI
- Support for UTF-8, etc
- Labels for days of the week
- Testing on other platforms
- User-defined calendar dimensions
- Support repetition of month-year pairs
- Finish month album sorting methods
- Support better/custom typefaces
- Draw a gauge or timeline for the album years
- Make error messages break at 80 columns
- Make images showing calendar measurement variables
- Write copy constructor/=overload for some objects
- makefile?

