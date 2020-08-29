/*
 * samParser.h
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#ifndef SAMPARSER_H_
#define SAMPARSER_H_

#include "readsParser.h"
#include <iostream>
#include "short_reads/reads.h"


class samParser : public readsParser {
public:
    samParser() :
            readsParser() {

    }

    void parse(Reads &reads, const std::string &filename) override;

    void parse(Reads &reads, std::istream &is) override;

    void parse(Reads &reads, std::istream &is,
               std::vector<std::string> &chrs_to_parse) override;

    void parse(Reads &reads, const std::string &filename,
               std::vector<std::string> &chrs_to_parse) override;
};

#endif /* SAMPARSER_H_ */
