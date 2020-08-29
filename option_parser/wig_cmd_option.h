/*
 * wig_cmd_option.h
 *
 *  Created on: May 28, 2011
 *      Author: xin
 */

#ifndef WIGCMDOPTION_H_
#define WIGCMDOPTION_H_

#include "cmd_option_parser.h"
#include "option_parser/OptionAux.h"
#include <boost/program_options.hpp>
#include <iostream>


class wig_cmd_option : public cmd_option_parser {
public:
    wig_cmd_option(int argc, char **argv, const std::string &version);

    ~wig_cmd_option();

    void parse();

    void print_option(std::ostream &os);

    bool isSplit() const;

    void setSplit(bool _split);

    bool isGz() const;

    bool isStranded() const;

    void setGz(bool _gz);

    void setStranded(bool _stranded);

    void report(std::ostream &os) const;

    std::string version;
private:

    po::variables_map vm;
    po::opt all;
    po::p_opt popt;


    void verify();

    bool _split;
    bool _gz;
    bool _stranded;
};

#endif /* WIGTCMDOPTION_H_ */
