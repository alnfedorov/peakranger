/*
 * bamFormatAux.h
 *
 *  Created on: May 4, 2012
 *      Author: xin
 */

#ifndef BAMFORMATAUX_H_
#define BAMFORMATAUX_H_

#include "bamtools/BamAux.h"
#include "common/stl_header.h"
#include "Strand.h"
#include "reads.h"

namespace reads {

    std::string getR1Chr(const BamTools::BamAlignment &read, const BamTools::RefVector &refvec);

    std::string getRChr(const int32_t &ref_id, const BamTools::RefVector &refvec);

    uint32_t getReadLength(const BamTools::BamAlignment &read);

    int32_t getR1End(const BamTools::BamAlignment &read);

    int32_t getR2Start(const BamTools::BamAlignment &read);

    int32_t getR2End(const BamTools::BamAlignment &read);

    int32_t guessMateEnd(const BamTools::BamAlignment &read);

    bool isFirstPEGoodRead(const BamTools::BamAlignment &read);

    bool isGoodPERead(const BamTools::BamAlignment &read);

    bool isPERead(const BamTools::BamAlignment &read);

    bool isSERead(const BamTools::BamAlignment &read);

    bool isGoodRead(const BamTools::BamAlignment &read);

    bool isGoodSERead(const BamTools::BamAlignment &read);

    void processAbnormalRead(const BamTools::BamAlignment &read, const BamTools::RefVector &ref);

    bool isChrToParse(std::vector<std::string> &chrs_to_parse, std::string &chr);

    bool isSameReadName(const BamTools::BamAlignment &lhs,
                        const BamTools::BamAlignment &rhs);

    bool isChrToParse(std::vector<std::string> &chrs_to_parse, std::string &chr);

    bool isDifferentReadName(const BamTools::BamAlignment &lhs,
                             const BamTools::BamAlignment &rhs);

    enum {
        MULTI_FRAG = 1,
        EACH_FRAG_MAPPED = 2,
        UNMAPPED = 4,
        NEXT_FRAG_UNMAPPED = 8,
        FRAG_REV_COM = 16,
        NEXT_FRAG_REV = 32,
        IS_HEAD_FRAG = 64,
        IS_TAIL_FRAG = 128,
        SECONDARY = 256,
        BAD_QC = 512,
        PCR_OPT_DUP = 1024
    };

} /* namespace reads */
#endif /* BAMFORMATAUX_H_ */
