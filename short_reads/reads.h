/*
 * reads.h
 *
 *  Created on: Apr 28, 2011
 *      Author: xin
 */

#ifndef READS_H_
#define READS_H_

#include "utils/exceptions.h"
#include "utils/logger.h"
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdint>


typedef std::vector<uint32_t> reads_vec;
typedef std::map<std::string, reads_vec> reads_t;
typedef std::map<std::string, reads_vec>::const_iterator pritr;
typedef std::vector<uint32_t>::const_iterator ritr;

/*
 * Reads are categorized based on strands and chromosomes.
 * No assumptions are posed toward strands and chromosomes.
 * It is possible that a chr contains only pos or neg reads.
 */
class Reads;

class StrandReads {
public:
    friend class Reads;
    /*
     * access to all reads;
     */
    pritr begin() const;

    pritr end() const;

    /*
     * access to reads of a chr
     */
    ritr begin_of(const std::string &chr) const;

    ritr end_of(const std::string &chr) const;

    /*
     * reads insertion
     */
    void insertRead(std::string &chr, uint32_t &read);

    /*
     * properties
     */
    size_t size() const;

    const std::vector<std::string> & chrs() const;

    bool hasReadsOnChr(const std::string &chr) const;

    void finalize();
private:
    std::map<std::string, std::vector<uint32_t> > _reads;
    std::vector<std::string> _chrs;
    bool _finalized = false;

    /*
     * modification
     */
    void remove(const std::string &chr);
};


class Reads {
public:
    Reads() = default;

    /*
     * return the number of pos and neg reads
     */
    size_t size() const { return pos_reads.size() + neg_reads.size(); };

    /*
     * Properties
     */
    uint32_t getReadlength() const;

    void setReadlength(uint32_t _readlength);

    /*
     * modification
     */
    void remove(const std::string &chr);

    /*
     * Quality control
     */
    void removeUnequalChrs();

    void finalize() { pos_reads.finalize(); neg_reads.finalize(); }

    StrandReads pos_reads;
    StrandReads neg_reads;
protected:
    uint32_t _readlength;
};

#endif /* READS_H_ */
