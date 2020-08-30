/*
 * BCPOption.cpp
 *
 *  Created on: Jun 27, 2012
 *      Author: xfeng
 */

#include "option_parser/BCPOption.h"
#include "option_parser/OptionAux.h"
#include "utils/stringutil.h"
#include <string>
#include <iostream>

namespace oa = options::aux;
using namespace std;
using namespace boost;
using namespace boost::program_options;
namespace options {

    int BCPOption::min_args = 2;

    BCPOption::BCPOption() = default;

    BCPOption::~BCPOption() = default;

    void BCPOption::parse() {
        hasEnoughArgs(_ac);

        po::store(
                po::command_line_parser(_ac, _av).options(all).positional(popt).run(),
                mVM);
        if (mVM.count("help")) {
            oa::printHelp(all, cout);
            exit(0); //todo: should we move this up?
        }
        if (mVM.count("version")) {
            cout << "\n" << VERSION << endl;
            exit(0); //todo: should we move this up?
        }

        po::notify(mVM);

        setFormat(to_lower_copy(trim_copy(getFormat())));

        if (mVM.count("help")) {
            options::aux::printHelp(all, cout);
            exit(0);
        }
        if (mVM.count("verbose")) {
            setVerboseRequested(true);
        }
        if (mVM.count("version")) {
            oa::printVersion(cout, version);
        }
        if (mVM.count("report")) {
            _html = true;
        } else {
            _html = false;
        }
        if (mVM.count("gene_annot_file")) {
            _html = true;
            oa::file_r_good(_gene_anno_file.c_str());
        }

        verifyOptions();

        string dir, file, file_ext;
        utils::stringutil::get_dir_file(_output_dir, dir, file, file_ext);
        //todo: linux only
        setOutput_file(_output_dir);
        setOutput_dir(dir);
    }

    void BCPOption::hasEnoughArgs(int argc) {
        if (argc < BCPOption::min_args) {
            oa::printHelp(all, cout);
            throw std::logic_error("Not enough command options.");
        }
    }

    bool BCPOption::needHtml() const {
        return this->_html;
    }

    void BCPOption::verifyOptions() {
        oa::require("data", mVM);
        oa::require("control", mVM);
        oa::require("output", mVM);
        oa::require("format", mVM);

        if (mVM.count("report")) {
            oa::require("gene_annot_file", mVM);
        }

        for (const auto &f: _treat_files)
            oa::file_r_good(f.c_str());

        for (const auto &f: _control_files)
            oa::file_r_good(f.c_str());

        oa::file_w_good(_output_dir.c_str());
    }

    void BCPOption::report(std::ostream &os) const {
        os << ("#program version:         ") << version << endl;
        os << ("#Data files:\n");
        os << ("# File format:             ") << getFormat() << endl;
        os << ("# Treatment file[s]:") << endl;
        for (const auto &f: getTreatFiles())
            os << "                          " << f << endl;
        os << ("# Control file[s]:") << endl;
        for (const auto &f: getControlFiles())
            os << "                          " << f << endl;
        os << ("#Qualities:\n");
        os << ("# P value cut off:         ") << getCutOff() << endl;
        os << ("# FDR cut off:             ") << getFdrCutOff() << endl;
        os << ("# sliding window size:     ") << slidingWinSize << endl;
        os << ("# Read extension length:   ") << _ext_length << endl;
        os << ("#Output:\n");
        os << ("# Regions:                ") << getOutput_file() + "_region.bed"
           << endl;
        os << ("#HTML reports:           ");
        if (needHtml()) {
            os << "Enabled" << endl;
            os << ("# Plot region length:     ") << getHtmlRegionLength() << endl;
            os << ("# Annotation file:        ") << getGeneAnnoFile() << endl;
        } else {
            os << "Disabled(--report not specified)\n";
        }
    }
} /* namespace report */


options::BCPOption::BCPOption(int argc, char **argv,
                              const std::string &version) : cmd_option_parser(argc, argv), version(version),
                                                            _html(false),
                                                            all("\nbcp " + version + "\n\nUsage"), popt() {

    opt other("Other");
    opt input("Input");
    opt output("Output");
    opt qualities("Qualities");
    opt running_modes("Running modes");

    other.add_options()

            ("help,h", "show the usage")

            ("verbose", "show progress")

            ("version", "output the version number");

    input.add_options()

            ("data,d", po::value<vector<string>>(&_treat_files)->multitoken()->required(), "data file[s]")

            ("control,c", po::value<vector<string>>(&_control_files)->multitoken()->required(), "control file[s]")

            ("format", po::value<string>(&_format),
             "the format of the data file, can be one of : "
             "bowtie, sam, bam and bed");

    output.add_options()

            ("output,o", po::value<string>(&_output_dir), "the output location")

            ("report", "generate html reports")

            ("plot_region",
             po::value<uint32_t>(&_html_region_length)->default_value(6000),
             "the length of the snapshort regions in the report")

            ("gene_annot_file", po::value<string>(&_gene_anno_file),
             "the gene annotation file");

    qualities.add_options()

            ("pval,p", po::value<double>(&_p_cut_off)->default_value(double(0.0001L)),
             "p value cut-off")

            ("win_size", po::value<uint32_t>(&slidingWinSize)->default_value(500),
             "sliding window size")

            ("ext_length,l", po::value<uint32_t>(&_ext_length)->default_value(200),
             "read extension length");


    popt.add("data", 1).add("control", 1).add("output", 1);
    all.add(input).add(output).add(qualities).add(other);

}
