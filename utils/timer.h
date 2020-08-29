#ifndef TIMER_H_
#define TIMER_H_

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

/*
 * Adpoted from Bowtie's source codes.
 */
namespace utils {

    void logDate(std::ostream &os, bool nl = true);

    void getDate(std::string &date);

    void logTime(std::ostream &os, bool nl = true);

}
#endif /*TIMER_H_*/
