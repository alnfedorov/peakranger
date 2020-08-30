/*
 * BCP.cpp
 *
 *  Created on: Sep 27, 2014
 *      Author: Xin Feng
 */
#include <stdexcept>
#include <fstream>
#include "parser/readsParser.h"
#include "utils/exceptions.h"
#include "parser/bowtieParser.h"
#include "parser/samParser.h"
#include "parser/bamParser.h"
#include "parser/bedParser.h"
#include "option_parser/cmd_option_parser.h"
#include "region_detector/bcp_algo.h"
#include "region_detector/calledpeak.h"
#include "result_reporter/result_reporter.h"
#include "wiggle/wiggle_reporter.h"
#include "utils/logger.h"
#include "utils/stringutil.h"
#include "utils/Stamp.h"
#include "short_reads/readstools.h"
#include "ggplay/chipseqhtmlreporter.h"
#include "app/bcp.h"
#include "option_parser/OptionAux.h"
#include "app/AppAux.h"
#include "tab_file/NearbyGeneFinder.h"
#include "concepts/RegionInt32.h"

using namespace tab_file;
using namespace std;
using namespace boost;
using namespace utils;
using namespace options;
using namespace options::aux;
using namespace ranger::concepts;

namespace app::bcp {
    options::BCPOption parse_options(int argc, char **argv, const std::string &version) {
        options::BCPOption option(argc, argv, version);
        try {
            option.parse();
        } catch (std::exception &e) {
            cout << "\n" << e.what() << "\n";
            cout << "\n" << "Provided args:\n" << printRawOpts(argc, argv)
                 << "\n";
            exit(0);
        } catch (FileNotGood &e) {
            e.debugPrint();
            cout << "\n" << "Provided args:\n" << printRawOpts(argc, argv)
                 << "\n";
            exit(0);
        }

        if (option.getVerboseRequested()) {
            option.report(cout);
        }

        return option;
    }

    std::pair<Reads, Reads> parse_data(BCPOption &option) {
        boost::shared_ptr<readsParser> parser;
        if (option.getFormat() == cmd_option_parser::format_bowtie) {
            parser = boost::make_shared<bowtieParser>();
        } else if (option.getFormat() == cmd_option_parser::format_sam) {
            parser = boost::make_shared<samParser>();
        } else if (option.getFormat() == cmd_option_parser::format_bed) {
            parser = boost::make_shared<bedParser>();
        } else if (option.getFormat() == cmd_option_parser::format_bam) {
            parser = boost::make_shared<bamParser>();
        } else {
            string str("The specified format ");
            str += option.getFormat();
            str += " has not been implemented yet.\n";
            throw not_in_range(str.c_str());
        }

        Reads treads, creads;

        for (auto &f: option.getTreatFiles())
            parser->parse(treads, f);
        treads.finalize();
        utils::Tracer tracer(cout, option.getVerboseRequested());
        tracer << "\nReads statistics:\n";
        tracer << "\n Treatment reads +:       " << treads.pos_reads.size();
        tracer << "\n Treatment reads -:       " << treads.neg_reads.size();
        tracer << "\n Average read length:     " << treads.getReadlength();

        for (auto &f: option.getControlFiles())
            parser->parse(creads, f);
        creads.finalize();
        tracer << "\n Control reads +:         " << creads.pos_reads.size();
        tracer << "\n Control reads -:         " << creads.neg_reads.size();
        tracer << "\n Average read length:     " << creads.getReadlength();
        tracer << "\n Verifying reads...\n";

        reads_tools::verify_and_correct_Reads_both_strands(treads, creads);
        tracer << "\nReads statistics after correction:\n";
        tracer << "\n Treatment reads +:       " << treads.pos_reads.size();
        tracer << "\n Treatment reads -:       " << treads.neg_reads.size();
        tracer << "\n Control reads +:         " << creads.pos_reads.size();
        tracer << "\n Control reads -:         " << creads.neg_reads.size();

        if (treads.size() < 1) {
            tracer << "\n\nNo reads were found in the treatment data. "
                   << "\nbcp must stop here.\n\n";
            exit(0);
        }
        if (creads.size() < 1) {
            tracer << "\n\nNo reads were found in the control data. "
                   << "\nbcp must stop here.\n\n";
            exit(0);
        }

        uint32_t treadslen = treads.getReadlength();
        uint32_t creadslen = creads.getReadlength();
        uint32_t optExtlen = option.getExtLength();
        uint32_t longerlen = treadslen > creadslen ? treadslen : creadslen;
        if (optExtlen < treadslen || optExtlen < creadslen) {
            tracer << "\nWarning: Specified read extension length " << optExtlen
                   << " is shorter than" << " the read length " << treadslen
                   << "(+)/" << creadslen << "(-). Forced to use " << longerlen
                   << " as the read extension length\n ";
            option.setExtLength(longerlen);
        }
        return std::make_pair(std::move(treads), std::move(creads));
    }

    enriched_regions predict(const Reads &treads, const Reads &creads, const BCPOption &option) {
        utils::Tracer tracer(cout, option.getVerboseRequested());
        tracer << "\n\n Calling peaks...\n\n";

        auto detector = bcp_algo(option.getExtLength(), option.getSlidingWinSize(), option.getCutOff());
        detector.detectSummits(treads, creads, option.getVerboseRequested());

        return std::move(detector._resultRegions);
    }

    void report_regions(const enriched_regions &regions, const BCPOption &option) {
        string file = option.getOutputFile() + "_region.bed";
        ofstream of(file.c_str());
        if (!(of.is_open())) {
            throw FileNotGood(file.c_str());
        }
        utils::Stamp::citationRangerBCPAndDate(of);
        option.report(of);

        of << "\n\n#region_chr\tregion_start\tregion_end"
              "\tregion_ID\tregion_pval\tregion_strand\n";

        for (auto &it: regions) {
            for (auto &pk: it.second) {
                of << it.first << "\t" << pk.first << "\t" << pk.second
                   << "\tbcp";
                of << "_pval_" << pk.p << "\t" << pk.q << "\t+\n";
            }
        }
    }

    void report_details(const enriched_regions &regions, const BCPOption &option) {
        std::string file = option.getOutputFile() + "_details";
        ofstream of_raw(file.c_str());
        if (!(of_raw.is_open())) {
            throw FileNotGood(file.c_str());
        }

        utils::Stamp::citationRangerBCPAndDate(of_raw);
        option.report(of_raw);


        of_raw << "\n#region_chr\tregion_start\t" << "region_end\t"
               << "nearby_genes(" << option.getHtmlRegionLength() / 1000
               << "kbp)" << "\tregion_ID\t" << "region_summits\t"
               << "region_fdr\tregion_strand\tregion_treads\tregion_creads\n";

        NearbyGeneFinder _nbgf;
        _nbgf.setAnnoFile(option.getGeneAnnoFile());
        _nbgf.setSearchSpan(option.getHtmlRegionLength());

        for (auto &it: regions) {
            for (auto &pk: it.second) {
                of_raw << it.first << "\t" << pk.first << "\t" << pk.second
                       << "\t";
                if (option.needHtml()) {
                    std::stringstream geneNamess;
                    vector<TabGene> genes;
                    _nbgf.getOverlappedGenes(it.first,
                                             RegionUint32(pk.first, pk.second), genes);
                    for (auto &g: genes) {
                        geneNamess << g.name << ",";
                    }
                    string geneNames(geneNamess.str());
                    boost::replace_last(geneNames, ",", "");
                    of_raw << geneNames;
                }
                of_raw << "\tbcp";
                of_raw << "\t" << utils::vector_to_string(pk.summits, ",");

                of_raw << "\t" << pk.q << "\t+\t" << pk.treads << "\t"
                       << pk.creads << "\n";
            }
        }
    }

    void report_html(const Reads &treads, const Reads &creads,
                     const enriched_regions &regions, const BCPOption &option) {
        utils::Tracer tracer(cout, option.getVerboseRequested());
        if (option.needHtml()) {
            throw runtime_error("HTML report is not implemented at the moment.");
//            tracer << "\n\n Generating reports:\n";
//            chipseq_html_reporter rptr;
//            utilprint::bcpCitation ct;
//            utilprint::citation ct2;
//            rptr.addCitation(ct2.tostring() + " and " + ct.tostring());
//            rptr.setRegionLength(option.getHtmlRegionLength());
//            map<string, vector<called_peak> > passFDR;
//            app::aux::filterByFDR(regions, passFDR, option.getCutOff());
//            rptr.generate_report(treads, creads, passFDR, option);
        }
    }

    void run(int argc, char **argv, const std::string &version) {
        auto options = parse_options(argc, argv, version);
        utils::Tracer tracer(cout, options.getVerboseRequested());

        SET_LOG_FILE(option.getOutputFile() + "_bcp.log");

        SET_LOG_LEVEL("DEBUG3");

        LOG_DEBUG1("\n\n************Starting an new round***********\n\n");

        // treads, creads
        auto reads = parse_data(options);
        for (auto& e: reads.first.pos_reads.chrs())
            std::cerr << e << std::endl;
        for (auto& e: reads.first.neg_reads.chrs())
            std::cerr << e << std::endl;
        for (auto& e: reads.second.pos_reads.chrs())
            std::cerr << e << std::endl;
        for (auto& e: reads.second.neg_reads.chrs())
            std::cerr << e << std::endl;
        auto regions = predict(reads.first, reads.second, options);

        size_t fdr_passed = 0;
        for (auto &it: regions)
            fdr_passed += it.second.size();
        tracer << "\n\nTotal regions discovered:\t" << fdr_passed;

        report_regions(regions, options);
        report_details(regions, options);
        report_html(reads.first, reads.second, regions, options);
        tracer << "\n\nProgram finished.\n\n";

        LOG_DONE();
    }
}
