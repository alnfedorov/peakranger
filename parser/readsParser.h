/*
 * readsParser.h
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#ifndef READSPARSER_H_
#define READSPARSER_H_

#include <string>
#include <iostream>
#include <vector>

class Reads;

class readsParser {
public:
    readsParser() {

    }

    virtual ~readsParser() {
    }

public:
    virtual void parse(Reads &reads, std::string &filename) = 0;

    virtual void parse(Reads &reads, std::istream &is) = 0;

    virtual void parse(Reads &reads, std::istream &is,
                       std::vector<std::string> &chrs_to_parse) = 0;

    virtual void parse(Reads &reads, std::string &filename,
                       std::vector<std::string> &chrs_to_parse) = 0;
};

#endif /* READSPARSER_H_ */
