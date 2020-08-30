/*
 * testwig.cpp
 *
 *  Created on: Jan 13, 2012
 *      Author: xfeng
 */

#include "testwig.h"
#include "wigbuilder.h"
#include "short_reads/readstools.h"

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

void test_wig::_print_wigfile_trackheader(ostream &pof,
                                          string &_name,
                                          vector<uint32_t> col) {
    pof << "track type=wiggle_0 name=\"" << _name << "\" " << "visibility=dense " << "color="
        << col[0] << "," << col[1] << "," << col[2] << " "
        << "altColor=" << col[0] << "," << col[1] << "," << col[2]
        << " " << "priority=" << _priority << "\n";
}

void test_wig::export_wiggle(Reads &reads,
                             ostream &pof,
                             string &pf) {

    vector<uint32_t>::iterator preadsstart, ppreadsstart, pppreadsstart,
            preadsend, ppreadsend, pppreadsend;

    vector<uint32_t>::iterator npreadsstart, nppreadsstart, npppreadsstart,
            npreadsend, nppreadsend, npppreadsend;
    vector<string> mergedchrs;
    reads_tools::get_merged_chrs_for_both_strands(reads,
                                                  mergedchrs);

    rt_assert_msg(pof.good(),
                  "file not good")

    ct.print_msg(pof);

    _print_wigfile_trackheader(pof,

                               pf,
                               _colorRGB);
    wigs _wigs, _wigss;
    wig_builder _wb;
    for (auto &chr: mergedchrs) {
        LOG_DEBUG1("In export_wiggle, start processing chr: " << chr);
        if (reads.pos_reads.hasReadsOnChr(chr)) {
            preadsstart = reads.pos_reads.begin_of(chr);
            preadsend = reads.pos_reads.end_of(chr);
        } else {
            preadsstart = preadsend;
        }
        if (reads.neg_reads.hasReadsOnChr(chr)) {
            npreadsstart = reads.neg_reads.begin_of(chr);
            npreadsend = reads.neg_reads.end_of(chr);
        } else {
            npreadsend = npreadsstart;
        }
        pof << "variableStep chrom=" << chr << " span=1\n";
        _wb._binned_wig_compiler(_binlength,
                                 _readlength,
                                 _readextlength,
                                 preadsstart,
                                 preadsend,
                                 npreadsstart,
                                 npreadsend,
                                 pof);

    }

}

void test_wig::export_wiggle(vector<uint32_t> &preads,
                             vector<uint32_t> &nreads,
                             string chr,
                             ostream &os) {
}

//void test_wig::export_wiggle(Reads & reads,
//                             const char *file,
//                             bool gzip) {
//
//    string sfile(file);
//
//    size_t ind = sfile.find_last_of(".wig");
//    if (ind != string::npos) {
//        //remove .wig
//        sfile = sfile.substr(0,
//                             ind - 3);
//    }
//
//    string pf(sfile + ".wig");
//    ofstream pof(pf.c_str());
//    ind = sfile.find_last_of("/\\");
//    pf = sfile.substr(ind + 1);
//
//    export_wiggle(reads,
//                  pof,
//                  pf);
//
//    pof.close();
//
//}

void test_wig::_compile_neg_strand(Reads &reads,
                                   string &chr,
                                   vector<uint32_t>::iterator npreadsstart,
                                   vector<uint32_t>::iterator npreadsend,
                                   ofstream &pof) {
    if (reads.neg_reads.hasReadsOnChr(chr)) {
        npreadsstart = reads.neg_reads.begin_of(chr);
        npreadsend = reads.neg_reads.end_of(chr);
    } else {
        npreadsend = npreadsstart;
    }
    pof << "variableStep chrom=" << chr << " span=1\n";
    _w._binned_wig_compiler(_binlength,
                            _readlength,
                            _readextlength,
                            npreadsstart,
                            npreadsend,
                            pof,
                            "-",
                            wig_builder::_get_ab_re);
}

void test_wig::_compile_pos_strand(Reads &reads,
                                   string &chr,
                                   vector<uint32_t>::iterator npreadsstart,
                                   vector<uint32_t>::iterator npreadsend,
                                   ofstream &pof) {
    if (reads.pos_reads.hasReadsOnChr(chr)) {
        npreadsstart = reads.pos_reads.begin_of(chr);
        npreadsend = reads.pos_reads.end_of(chr);
    } else {
        npreadsend = npreadsstart;
    }
    pof << "variableStep chrom=" << chr << " span=1\n";
    _w._binned_wig_compiler(_binlength,
                            _readlength,
                            _readextlength,
                            npreadsstart,
                            npreadsend,
                            pof,
                            "",
                            wig_builder::_get_ab);
}

void test_wig::split_export_wiggle(Reads &reads,
                                   ostream &os) {

}

void test_wig::export_wiggle_gzip(Reads &reads,
                                  const char *file) {
}

void test_wig::split_export_wiggle_gzip(Reads &reads,
                                        const char *file) {
}

