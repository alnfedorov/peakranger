/*
 * bowtieParser.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: xin
 */

#include "bowtieParser.h"
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include "utils/logger.h"
#include "utils/exceptions.h"
#include "short_reads/reads.h"

using namespace std;

void bowtieParser::parse(Reads &outputreads, const string &filename) {

    ifstream ifs(filename.c_str());

    if (!ifs.good()) {
        throw FileNotGood(filename);
    }

    parse(outputreads, ifs);

    ifs.close();
}

void bowtieParser::parse(Reads &outputreads, istream &ifs) {

    char dir;
    size_t ploc;
    string chr, seq, line;
    uint32_t loc;

    if (!ifs.good()) {
        throw FileNotGood("input stream");
    }

    while (getline(ifs, line)) {

        ploc = line.find("\t+\t");
        if (ploc == string::npos) {
            ploc = line.find("\t-\t");
            if (ploc == string::npos) {
                cout << "Warning: Didnt find direction character (+/-)"
                        " in the line: \n" << line << "\nThis line was skipped"
                     << endl;
                continue;
            }
        }
        string nameTrimmed(line.begin() + ploc,
                           line.end());
        istringstream iss(nameTrimmed);
        iss >> dir >> chr >> loc >> seq;
        if (dir == '-') {
            outputreads.neg_reads.insertRead(chr, loc);
        } else {
            outputreads.pos_reads.insertRead(chr, loc);
        }
    }
    outputreads.setReadlength(seq.size());
}

void bowtieParser::parse(Reads &outputreads,
                         const string &filename,
                         vector<string> &chrs_to_parse) {
    ifstream ifs(filename.c_str());

    if (!(ifs.good())) {
        throw FileNotGood(filename);
    }

    parse(outputreads, ifs, chrs_to_parse);

    ifs.close();
}

void bowtieParser::parse(Reads &outputreads,
                         istream &ifs,
                         vector<string> &chrs_to_parse) {
    char dir;
    size_t ploc;
    string chr, seq, line;
    uint32_t loc;

    if (!(ifs.good())) {
        throw FileNotGood("The specified bowtie input file");
    }

    while (getline(ifs,
                   line)) {

        ploc = line.find("\t+\t");
        if (ploc == string::npos) {
            ploc = line.find("\t-\t");
            if (ploc == string::npos) {
                cout << "Warning: Didnt find direction character (+/-)"
                        " in the line: \n" << line << "\nThis line was skipped"
                     << endl;
                continue;
            }
        }
        string nameTrimmed(line.begin() + ploc,
                           line.end());
        istringstream iss(nameTrimmed);
        iss >> dir >> chr >> loc >> seq;
        if (std::find(chrs_to_parse.begin(),
                      chrs_to_parse.end(),
                      chr) != chrs_to_parse.end()) {
            LOG_DEBUG4("In bowtie parser, Inserted read: " << loc);
            if (dir == '-') {
                outputreads.neg_reads.insertRead(chr, loc);
            } else {
                outputreads.pos_reads.insertRead(chr, loc);
            }
        }
    }
    outputreads.setReadlength(seq.size());
}

