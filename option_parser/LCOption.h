/*
 * LCOption.h
 *
 *  Created on: Jul 10, 2012
 *      Author: xfeng
 */

#ifndef LCOPTION_H_
#define LCOPTION_H_
#include <boost/program_options.hpp>
#include "option_parser/OptionAux.h"
namespace options {

class LCOption {
public:
    LCOption(const std::string& version);
    virtual ~LCOption();
    void parse(int argc, char** argv);

    std::string printParsedOpts();
    std::string version;
    static int min_args;
    boost::program_options::variables_map mVM;
private:
    void hasEnoughArgs(int argc);
    void verifyOptions();
    po::opt all;
    po::p_opt popt;
};

} /* namespace options */
#endif /* LCOPTION_H_ */
