/*
 * bcp.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Xin Feng
 */

#ifndef BCP_H_
#define BCP_H_

#include <string>
#include "option_parser/BCPOption.h"
#include "short_reads/reads.h"
#include "region_detector/calledpeak.h"

namespace app::bcp {
    typedef std::map<std::string, std::vector<called_peak> > enriched_regions;

    options::BCPOption parse_options(int argc, char **argv, const std::string &version);

    std::pair<Reads, Reads> parse_data(options::BCPOption &option);

    enriched_regions predict(const Reads &treads, const Reads &creads, const options::BCPOption &option);

    void report_regions(const enriched_regions &regions, const options::BCPOption &option);

    void report_details(const enriched_regions &regions, const options::BCPOption &option);

    void report_html(const Reads &treads, const Reads &creads,
                     const enriched_regions &regions, const options::BCPOption &option);

    void run(int args, char **argv, const std::string &version);
}
#endif /* BCP_H_ */

