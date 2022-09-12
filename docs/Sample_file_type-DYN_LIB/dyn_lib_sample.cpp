/*
** https://radist-elvin.blogspot.com/2008/07/linux.html
** "Статические и динамические библиотеки в Linux"
**
** type == ET_DYN
**
** Итак, исходный код библиотеки (C++):
*/

#include <iostream>

extern "C" int hello();

using namespace std;

extern "C" int hello()
{
	std::cout << "Hello world!\n I'm function hello()" << endl;
	return 0;
}

/*
** Compile this:
** g++ -fPIC -c dyn_lib_sample.cpp -o dynamic.o
**
** Create the library:
** g++ -shared -olibdynamic.so dynamic.o
**
** Copy to standard destination:
** cp libdynamic.so /usr/lib
**
** Check properties:
** ls -la /usr/lib/libdynamic.so
** file /usr/lib/libdynamic.so
** readelf -h /usr/lib/libdynamic.so
**
** Compile c-file with this library:
** gcc sample_main_4dynlib.c  -ldynamic -L. -o res
*/