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
            _pad(false), _verboseRequested(false), _chrtableSpecified(false), _ac(argc), _av(argv) {
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
    bool isStrandSensitiveMode;
    uint32_t outputNum;
    int randomSeed;
    double minScore;
    uint32_t bootstrapPass;
    double smoothingFactor;
public:
    std::string getChr_table() const;

    double getCutOff() const;

    double getDelta() const;

    uint32_t getExt_length() const;

    std::string getFormat() const;

    std::string getMode() const;

    uint32_t getNo_of_thread() const;

    std::string getOutput_dir() const;

    std::string getOutput_file() const;

    bool getPad() const;

    const std::vector<std::string>& getTreatFiles() const;

    const std::vector<std::string>& getControlFiles() const;

    uint32_t getBandwidth() const;

    void setAc(int _ac);

    void setChr_table(std::string _chr_table);

    void setExt_length(uint32_t _ext_length);

    void setFormat(std::string _format);

    void setOutput_dir(std::string _output_dir);

    void setOutput_file(std::string _output_file);

    void setPad(bool _pad);

    bool getChrtableSpecified() const;

    void setChrtableSpecified(bool _chrtableSpecified);

    bool getVerboseRequested() const;

    void setVerboseRequested(bool _verboseRequested);

    std::vector<std::string> getChrs_to_parse() const;

    uint32_t getBinlength() const;

    void setConfigFile(std::string _config_file);

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
    std::string _config_file;
    std::string _chr_table_file;
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
    bool _chrtableSpecified{};
    int _ac{};
    char **_av{};
    std::vector<std::string> _chrs_to_parse;
    uint32_t maxThreads{};
};

#endif /* CMD_OPTION_PARSER_H_ */
