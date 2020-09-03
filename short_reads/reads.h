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
#include <functional>


typedef std::vector<uint32_t> reads_vec;
typedef std::map<std::string, reads_vec> reads_t;
typedef const uint32_t *ritr;

/*
 * Reads are categorized based on strands and chromosomes.
 * No assumptions are posed toward strands and chromosomes.
 * It is possible that a chr contains only pos or neg reads.
 */
class Reads;

// Actually there are must be 2 classes for reads. Finalized and not.
class StrandReads {
public:
    friend class Reads;

    StrandReads() = default;

    ~StrandReads() { if (_destructor_callback) _destructor_callback(); };

    StrandReads(std::map<std::string, uint32_t *> reads, std::map<std::string, size_t> sizes,
                std::function<void()> destructor_callback);

    ritr begin_of(const std::string &chr) const;

    ritr end_of(const std::string &chr) const;

    void insertRead(std::string &chr, uint32_t &read);

    size_t size() const;

    const std::vector<std::string> &chrs() const;

    bool hasReadsOnChr(const std::string &chr) const;

    bool finalized() const { return _finalized; };

    void finalize();

private:
    std::map<std::string, std::vector<uint32_t> > _reads;

    // That is very hacky, but allows copy-free memory
    // managment during pickling/unpickling.
    std::map<std::string, uint32_t *> _sorted_reads;
    std::map<std::string, size_t> _sorted_reads_size;
    std::function<void()> _destructor_callback;

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

    void finalize() {
        pos_reads.finalize();
        neg_reads.finalize();
    }

    StrandReads pos_reads;
    StrandReads neg_reads;
protected:
    uint32_t _readlength;
};

#endif /* READS_H_ */
