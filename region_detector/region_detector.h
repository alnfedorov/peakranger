/*
 * region_detector.h
 *
 *  Created on: Jun 6, 2011
 *      Author: xin
 */

#ifndef REGION_DETECTOR_H_
#define REGION_DETECTOR_H_

#include "short_reads/reads.h"
#include "option_parser/cmd_option_parser.h"
#include "result_reporter/result_reporter.h"
#include "region_detector/calledpeak.h"

#include <map>
#include <string>
#include <vector>
#include <stdint.h>
#include <ostream>

typedef std::map<std::string, std::vector<called_peak> > enriched_regions;
typedef std::vector<called_peak>::iterator ritrr;
typedef enriched_regions::iterator pritrr;
typedef double pvalue;

class region_detector {
public:

    region_detector() = default;

    virtual ~region_detector() = default;

    virtual void detectSummits(Reads &treatment_reads, Reads &control_reads, cmd_option_parser &option) = 0;

    virtual void detectSummits(Reads &treatment_reads, Reads &control_reads, cmd_option_parser &option, std::ostream &os) = 0;

public:
    enriched_regions _resultRegions;

};

#endif /* REGION_DETECTOR_H_ */
