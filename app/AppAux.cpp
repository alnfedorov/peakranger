/*
 * AppAux.cpp
 *
 *  Created on: Jun 27, 2012
 *      Author: xfeng
 */

#include <stdexcept>
#include <fstream>
#include "common/boost_header.h"
#include "app/AppAux.h"

using namespace std;
using namespace utils;
using namespace boost;
typedef map<string, vector<called_peak> > enriched_regions;
typedef vector<called_peak>::iterator ritrr;
typedef enriched_regions::iterator pritrr;

namespace app {
    namespace aux {
        void filterByFDR(const map<string, vector<called_peak> > &toFilter,
                         map<string, vector<called_peak> > &passFDR, double fdr) {

            map<string, vector<called_peak> >::const_iterator it;
            it = toFilter.begin();
            for (; it != toFilter.end(); it++) {
                        for(const auto& pk: it->second) {
                                if (pk.q <= fdr) {
                                    passFDR[it->first].push_back(pk);
                                }
                            }
            }

        }
    }
} /* namespace app */
