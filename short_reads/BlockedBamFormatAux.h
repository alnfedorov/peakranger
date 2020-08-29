/*
 * BlockedBamFormatAux.h
 *
 *  Created on: May 10, 2012
 *      Author: xin
 */

#ifndef BLOCKEDBAMFORMATAUX_H_
#define BLOCKEDBAMFORMATAUX_H_

#include "bamtools/BamAux.h"
#include "BlockedBamHit.h"
#include "common/stl_header.h"
#include "Strand.h"
#include "CigarString.h"
#include "CigarStringFactory.h"
#include "concepts/RegionInt32.h"
#include "Read.h"
#include <boost/shared_ptr.hpp>

namespace BamTools {
    std::ostream &operator<<(std::ostream &os, const BamAlignment &bam);
}

namespace reads {

    void getBlockedR1StartEnd(const BamTools::BamAlignment &read, int32_t &start,
                              int32_t &end);

    void getBlockedR2StartEnd(const BamTools::BamAlignment &read, int32_t &start,
                              int32_t &end);

    BlockedBamHit getBlockedRead1FromPEBamRead(const BamTools::BamAlignment &read,
                                               const BamTools::RefVector &ref);

    BlockedBamHit getBlockedRead2FromPEBamRead(const BamTools::BamAlignment &read,
                                               const BamTools::RefVector &ref);

    void getBlockedBamHitsFromPEBamRead(const BamTools::BamAlignment &read,
                                        const BamTools::RefVector &ref, BlockedBamHit &r1, BlockedBamHit &r2);

    ranger::concepts::RegionInt32 regionFromCigar(const CigarString &cigar);

    bool isAdditiveCigar(const CigarString &c);

    Read readFromCigar(const CigarString &cigar, const std::string &chr,
                       const Strand &strand);

    void getCigarString(const BamTools::BamAlignment &bam,
                        std::vector<boost::shared_ptr<CigarString> > &lica);

    void parseBamBlocks(std::vector<Read> &resBlocks,
                        const BamTools::BamAlignment &bam, const BamTools::RefVector &ref);

    void lessGoFirst(BamTools::BamAlignment &mread, BamTools::BamAlignment &read);

    void extendRead(BamTools::BamAlignment &read, int32_t length);

    void extendBamRead(const BamTools::BamAlignment &read,
                       std::vector<reads::Read> &rds, uint32_t length);

}

#endif /* BLOCKEDBAMFORMATAUX_H_ */
