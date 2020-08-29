/*
 * bowtieParser.h
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#ifndef BOWTIEPARSER_H_
#define BOWTIEPARSER_H_

#include "readsParser.h"

class bowtieParser : public readsParser {
public:
    bowtieParser() :
            readsParser() {

    }

    void parse(Reads &reads, const std::string &filename) override;

    void parse(Reads &reads, std::istream &is) override;

    void parse(Reads &reads, std::istream &is,
               std::vector<std::string> &chrs_to_parse) override;

    void parse(Reads &reads, const std::string &filename,
               std::vector<std::string> &chrs_to_parse) override;
};

#endif /* BOWTIEPARSER_H_ */
