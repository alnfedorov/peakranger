/*
 * bedParser.h
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#ifndef BEDPARSER_H_
#define BEDPARSER_H_

#include "readsParser.h"

class bedParser : public readsParser {
public:
    bedParser() :
            readsParser() {
    }

    ~bedParser() override = default;

    void parse(Reads &reads, const std::string &filename) override;

    void parse(Reads &reads, std::istream &is) override;

    void parse(Reads &reads, std::istream &is,
               std::vector<std::string> &chrs_to_parse) override;

    void parse(Reads &reads, const std::string &filename,
               std::vector<std::string> &chrs_to_parse) override;

};

#endif /* BEDPARSER_H_ */
