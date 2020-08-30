/*
 * BCPOption.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Xin Feng
 */

#ifndef BCPOPTION_H_
#define BCPOPTION_H_

#include <boost/program_options.hpp>
#include <utility>
#include "option_parser/cmd_option_parser.h"

namespace options {
    namespace po = boost::program_options;

    class BCPOption {
    public:
        BCPOption(int argc, char **argv, const std::string &version);

        void parse();

        static int min_args;
        std::string version;
        po::variables_map mVM;

        bool needHtml() const;

        void report(std::ostream &os) const;

        std::string getFormat() const { return _format; };
        void setFormat(std::string format) { _format = std::move(format); };

        bool getVerboseRequested() const { return _verboseRequested; };
        void setVerboseRequested(bool verboseRequested) { _verboseRequested = verboseRequested; };

        std::string getOutputFile() const { return _output_file; };
        void setOutputFile(std::string output_file) { _output_file = output_file; };

        std::string getOutputDir() const { return _output_dir; };
        void setOutputDir(std::string output_dir) { _output_dir = output_dir; };

        double getCutOff() const { return _p_cut_off; };
        void setCutOff(double p_cut_off) { _p_cut_off = p_cut_off; };

        double getFdrCutOff() const { return _fdr_cut_off; };
        void setFdrCutOff(double fdr_cut_off) { _fdr_cut_off = fdr_cut_off; };

        uint32_t getSlidingWinSize() const { return _sliding_win_size; };
        void setSlidingWinSize(uint32_t sliding_win_size) { _sliding_win_size = sliding_win_size; };

        uint32_t getExtLength() const { return _ext_length; };
        void setExtLength(uint32_t ext_length) { _ext_length = ext_length; };

        uint32_t getHtmlRegionLength() const { return _html_region_length; };
        void setHtmlRegionLength(uint32_t html_region_length) { _html_region_length = html_region_length; };

        std::string getGeneAnnoFile() const { return _gene_anno_file; };
        void setGeneAnnoFile(std::string gene_anno_file) { _gene_anno_file = std::move(gene_anno_file); };

        const std::vector<std::string> &getTreatFiles() const { return _treat_files; };
        const std::vector<std::string> &getControlFiles() const { return _control_files; };
    private:
        void hasEnoughArgs(int argc);

        void verifyOptions();
        int _argc;
        char **_argv;

        bool _html;
        uint32_t _html_region_length;
        std::string _gene_anno_file;

        std::string _format;
        bool _verboseRequested = false;

        double _p_cut_off;
        double _fdr_cut_off;
        uint32_t _sliding_win_size;
        uint32_t _ext_length;

        std::vector<std::string> _treat_files;
        std::vector<std::string> _control_files;
        std::string _output_file;
        std::string _output_dir;

        po::opt all;
        po::p_opt popt;
    };

} /* namespace options */
#endif /* BCPOPTION_H_ */
