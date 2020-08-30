/*
 * cmd_option_parser.h
 *
 *  Created on: Jun 6, 2011
 *      Author: xin
 */

#ifndef CMD_OPTION_PARSER_H_
#define CMD_OPTION_PARSER_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

namespace po = boost::program_options;

namespace boost {
    namespace program_options {
        typedef options_description opt;
        typedef positional_options_description p_opt;
    }
}
//@to_be_depreciated
class cmd_option_parser {
public:
    cmd_option_parser() = default;

    cmd_option_parser(int argc, char **argv) :
            _pad(false), _verboseRequested(false), _ac(argc), _av(argv) {
        maxThreads = boost::thread::hardware_concurrency();
    }

    virtual ~cmd_option_parser() = default;

    virtual void report(std::ostream &) const = 0;

    virtual void parse() = 0;

public:
    uint32_t fragmentSize;
    uint32_t slidingWinSize;
    uint32_t movingStep;
    uint32_t minCount;
    uint32_t outputNum;
    int randomSeed;
    double minScore;
    uint32_t bootstrapPass;
    double smoothingFactor;
public:
    double getCutOff() const;

    double getDelta() const;

    uint32_t getExtLength() const;

    std::string getFormat() const;
    void setFormat(std::string _format);

    std::string getMode() const;

    uint32_t getNoOfThread() const;

    void setOutputDir(std::string _output_dir);
    std::string getOutputDir() const;

    void setOutputFile(std::string _output_file);
    std::string getOutputFile() const;

    bool getPad() const;

    const std::vector<std::string> &getTreatFiles() const;

    const std::vector<std::string> &getControlFiles() const;

    uint32_t getBandwidth() const;

    void setExtLength(uint32_t _ext_length);

    void setPad(bool _pad);

    bool getVerboseRequested() const;
    void setVerboseRequested(bool _verboseRequested);

    uint32_t getBinlength() const;

    double getFdrCutOff() const;

    uint32_t getHtmlRegionLength() const;

    std::string getGeneAnnoFile() const {
        return _gene_anno_file;
    }

    void setGeneAnnoFile(std::string geneAnnoFile) {
        _gene_anno_file = geneAnnoFile;
    }

    uint32_t getPeakHeightCutoff() const {
        return _threshold;
    }

    void setPeakHeightCutoff(uint32_t cut) {
        this->_threshold = cut;
    }

public:
    static const std::string format_bowtie;
    static const std::string format_bed;
    static const std::string format_sam;
    static const std::string format_bam;
    static const std::string format_eland;

protected:
    std::vector<std::string> _treat_files;
    std::vector<std::string> _control_files;
    std::string _output_file;
    std::string _output_dir;
    std::string _format;
    std::string _mode;
    std::string _gene_anno_file;

    double _p_cut_off{};
    double _delta{};
    double _fdr_cut_off{};
    uint32_t _no_of_thread{};
    uint32_t _ext_length{};
    uint32_t _bandwidth{};
    uint32_t _threshold{};
    uint32_t _binlength{};
    uint32_t _html_region_length{};
    bool _pad{};
    bool _verboseRequested{};
    int _ac{};
    char **_av{};
    uint32_t maxThreads{};
};

#endif /* CMD_OPTION_PARSER_H_ */
