/*
 * bcp_algo.h
 * A wrapper for BCP.
 *
 * Xing H, Mo Y, Liao W, Zhang MQ (2012) Genome-Wide Localization of 
 * Protein-DNA Binding and Histone Modification by a Bayesian 
 * Change-Point Method with ChIP-seq Data. PLoS Comput Biol 8(7): e1002613. 
 * doi:10.1371/journal.pcbi.1002613
 *
 *  Created on: Sep 27, 2014
 *      Author: Xin Feng 
 */

#ifndef BCP_ALGO_H_
#define BCP_ALGO_H_

#include "region_detector.h"
#include "tnt_cmat.h"

class bcp_algo {
public:
    bcp_algo(uint32_t frag_size, uint32_t window_size, double pvalue_cutoff);

    void detectSummits(const Reads &treatment_reads, const Reads &control_reads, bool verbose);

    enriched_regions _resultRegions;
private:
    typedef std::vector<std::vector<int> > data_t;

    void insertPeak(const std::string &chr, called_peak &pk);

    void cmain(const Reads &treads, const Reads &creads, bool verbose);

    void loadPosData(data_t &data, const Reads &reads);

    void loadNegData(data_t &data, const Reads &reads);

    void buildChrMap(const Reads &reads);

    void printhelp();

    void Add(int aa, int bb);

    void trans2window(int r);

    void seg(TNT::Matrix<double>, double);

    int frag_count1(int g);

    int frag_count2(int g);

    double prob_pois(int g, double a);

    std::vector<int> flag;
    std::vector<double> q;
    std::vector<double> value;

private:
    double (*win_data)[4];
    double (*ss)[6];
    double (*rseg)[7];
    int *data_frag;
    int *input_frag;
    double st[100001];
    std::map<std::string, uint32_t> chr2Num;
    std::map<uint32_t, std::string> num2Chr;
    int frag_size;
    int win_size;
    double p_value;
    int num_win, num_data_frag, num_input_frag, num_seg, num_allseg;
    double len_bf, len_aft, sum_bf, sum_aft, average_aft, cutline;
    const size_t L1;
    const size_t L2;
    const size_t N;
    std::vector<int> Weight;

};

#endif /* BCP_ALGO_H_ */
