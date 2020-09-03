/*
 * Reads.cpp
 *
 *  Created on: May 5, 2011
 *      Author: xin
 */

#include "reads.h"

#include "utils/assert_helpers.h"
#include "utils/exceptions.h"
#include <algorithm>

using namespace std;

/*
 * return the number of pos and neg reads
 */
uint32_t Reads::getReadlength() const {
    return _readlength;
}

void Reads::setReadlength(uint32_t _readlength) {
    this->_readlength = _readlength;
}

void Reads::remove(const string &chr) {
    pos_reads.remove(chr);
    neg_reads.remove(chr);
}

/* Normal result:
 * Every chr must have reads in both the pos and neg strands,
 * otherwise it will be removed
 *
 * Extreme result:
 * The reads are empty if reads reside in only one strand
 *
 */
void Reads::removeUnequalChrs() {
    pos_reads.finalize();
    neg_reads.finalize();

    vector<string>::iterator it;
    vector<string> chrs_to_remove(
            neg_reads.chrs().size() + pos_reads.chrs().size());
    // todo: test this module
    if (neg_reads.chrs().size() < 1 && pos_reads.chrs().size() > 1) {
        chrs_to_remove = pos_reads.chrs();
        for (auto &chr: chrs_to_remove) {
            cerr << "Warning: " << chr
                 << " only contains reads in the positive strand."
                 << " The chromosome is removed.\n";
            pos_reads.remove(chr);
        }
        return;
    }
    if (pos_reads.chrs().size() < 1 && neg_reads.chrs().size() > 1) {
        chrs_to_remove = neg_reads.chrs();
        for (auto &chr: chrs_to_remove) {
            cerr << "Warning: " << chr
                 << " only contains reads in the negative strand."
                 << " The chromosome is removed.\n";
            neg_reads.remove(chr);
        }
        return;
    }
    //I dont trust the implementation of set_difference
    //so I have to make sure .begin() and .end() is not 0x0
    it = std::set_difference(pos_reads._chrs.begin(), pos_reads._chrs.end(),
                             neg_reads._chrs.begin(), neg_reads._chrs.end(),
                             chrs_to_remove.begin());
    chrs_to_remove.resize(it - chrs_to_remove.begin());
    for (auto &chr: chrs_to_remove) {
        cerr << "Warning: " << chr
             << " only contains reads in the positive strand."
             << " The chromosome is removed.\n";
        pos_reads.remove(chr);
    }
    vector<string> chrs_to_remove2(
            neg_reads.chrs().size() + pos_reads.chrs().size());

    it = std::set_difference(neg_reads._chrs.begin(), neg_reads._chrs.end(),
                             pos_reads._chrs.begin(), pos_reads._chrs.end(),
                             chrs_to_remove2.begin());
    chrs_to_remove2.resize(it - chrs_to_remove2.begin());
    for (auto &chr: chrs_to_remove2) {
        cerr << "Warning: " << chr
             << " only contains reads in the negative strand."
             << " The chromosome is removed.\n";
        neg_reads.remove(chr);
    }
}

void StrandReads::insertRead(string &chr, uint32_t &read) {
    if (_finalized)
        throw std::runtime_error("StrandReads is finalized, can't add/remove reads");

    rt_assert_neq_msg(chr, "", "The name of chr is empty.")
    rt_assert_neq_msg(chr, "*", "The name of chr is *.")

    auto it = _reads.find(chr);
    if (it == _reads.end())
        it = _reads.insert({chr, vector<uint32_t>()}).first;

    it->second.push_back(read);
}

size_t StrandReads::size() const {
    size_t size = 0;
    if (_finalized) {
        for (auto &reads: _sorted_reads_size)
            size += reads.second;
    }
    else {
        for (auto &reads: _reads)
            size += reads.second.size();
    }
    return size;
}

const vector<string> &StrandReads::chrs() const {
    if (!_finalized)
        throw runtime_error("");

    return _chrs;
}

bool StrandReads::hasReadsOnChr(const string &chr) const {
    if (!_finalized)
        throw runtime_error("");

    if (binary_search(_chrs.begin(), _chrs.end(), chr)) {
        return true;
    }
    return false;
}

void StrandReads::finalize() {
    _chrs.clear();
    for (auto & it : _reads) {
        _chrs.push_back(it.first);
    }
    sort(_chrs.begin(), _chrs.end());

    for(auto& it: _reads) {
        LOG_DEBUG3("Sorted " << itr->first);
        sort(it.second.begin(), it.second.end());
        _sorted_reads[it.first] = it.second.data();
        _sorted_reads_size[it.first] = it.second.size();
    }
    _finalized = true;
}

void StrandReads::remove(const string &chr) {
    _reads.erase(chr);
    if (_finalized) {
        _sorted_reads.erase(chr);
        _sorted_reads_size.erase(chr);
        auto it = find(_chrs.begin(), _chrs.end(), chr);
        if (it != _chrs.end())
            _chrs.erase(it);
    }
}

ritr StrandReads::begin_of(const string &chr) const {
    if (!_finalized)
        throw runtime_error("");
    if (hasReadsOnChr(chr)) {
        return _sorted_reads.at(chr);
    }
    throw ChrNotFound("Chromosome " + chr + " was not found in parsed positive/negative reads.");
}

ritr StrandReads::end_of(const string &chr) const {
    if (!_finalized)
        throw runtime_error("");
    if (hasReadsOnChr(chr)) {
        return _sorted_reads.at(chr) + _sorted_reads_size.at(chr);
    }
    throw ChrNotFound("Chromosome " + chr + " was not found in parsed positive/negative reads.");
}

StrandReads::StrandReads(std::map<std::string, uint32_t*> reads, std::map<std::string, size_t> sizes,
                         std::function<void()> destructor_callback) {
    _finalized = true;
    _sorted_reads = std::move(reads);
    _sorted_reads_size = std::move(sizes);
    _destructor_callback = destructor_callback;

    for (auto & it : _sorted_reads_size) {
        _chrs.push_back(it.first);
    }
    sort(_chrs.begin(), _chrs.end());
}
