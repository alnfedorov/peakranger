/*
 * bamParser.h
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#ifndef BAMPARSER_H_
#define BAMPARSER_H_

#include "readsParser.h"
#include "bamtools/BamAux.h"

class bamParser : public readsParser {

public:
    void parse(Reads &reads, const std::string &filename) override;

    void parse(Reads &reads, std::istream &is) override;

    void parse(Reads &reads, std::istream &is,
               std::vector<std::string> &chrs_to_parse) override;

    void parse(Reads &reads, const std::string &filename,
               std::vector<std::string> &chrs_to_parse) override;

private:

    void insertRead(const BamTools::BamAlignment &read, Reads &reads, std::string &chr);

    void updateAvgReadLength(const BamTools::BamAlignment &read);

    uint32_t meanReadLen = 0;
    uint64_t readCnt = 0;

};

#endif /* BAMPARSER_H_ */
