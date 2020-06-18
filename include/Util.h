#ifndef UTIL_H
#define UTIL_H
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>


#include <ostream>
class Util{
		public:
				static void Exit(const char msg[]);
				static std::string getTimeStr();
				static void getTimeStr(char timeStr[]);

				static std::vector<timeval> ticList;
				static std::vector<timeval> tocList;
				static std::vector<std::string>  strList;
				static void   tic(std::string str);
				static double toc(std::string str); // return seconds.

};

#ifdef NO_COLORED_OUTPUT
#define RST   ""
#define KRED  ""
#define KGRN  ""
#define KYEL  ""
#define KBLU  ""
#define KMAG  ""
#define KCYN  ""
#define KWHT  ""
#define BOLD(x) x 
#define BOLDRED(x) x 
#define BOLDGRN(x) x 
#define UNDL(x) x 
#define FSTARTBLU ""
#else
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BOLD(x) "\x1B[1m" x RST
#define BOLDRED(x) "\x1b[31m" "\x1b[1m" x RST
#define BOLDBLU(x) "\x1B[34m" "\x1b[1m" x RST
#define BOLDGRN(x) "\x1B[32m" "\x1b[1m" x RST
#define UNDL(x) "\x1B[4m" x RST
#define FSTARTBLU KBLU
#endif


#define FEND    RST

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST




#endif

