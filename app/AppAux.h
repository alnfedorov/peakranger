/*
 * AppAux.h
 *
 *  Created on: Jun 27, 2012
 *      Author: xfeng
 */

#ifndef APPAUX_H_
#define APPAUX_H_

#include <string>

#include "common/boost_header.h"
#include "parser/readsParser.h"
#include "utils/exceptions.h"
#include "parser/bowtieParser.h"
#include "parser/samParser.h"
#include "parser/bamParser.h"
#include "parser/bedParser.h"
#include "option_parser/cmd_option_parser.h"
#include "option_parser/cmd_option_parser.h"
#include "region_detector/region_detector.h"
#include "region_detector/calledpeak.h"
#include "region_detector/fdr_based_thresholder.h"
#include "result_reporter/result_reporter.h"
#include "result_reporter/bed6_result_reporter.h"
#include "wiggle/wiggle_reporter.h"
#include "wiggle/JTwigglefile.h"

#include "utils/logger.h"
#include "utils/stringutil.h"
#include "utils/util_print.h"
#include "utils/Tracer.h"
#include "short_reads/readstools.h"

#include "ggplay/chipseqhtmlreporter.h"
#include "app/AppAux.h"

namespace app {
    namespace aux {
        void filterByFDR(const std::map<std::string, std::vector<called_peak> > &toFilter,
                         std::map<std::string, std::vector<called_peak> > &results, double fdr);

    }
} /* namespace app */
#endif /* APPAUX_H_ */
