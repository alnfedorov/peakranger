/*
 * cmd_option_parser.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: xin
 */

#include "cmd_option_parser.h"

using namespace std;
const string cmd_option_parser::format_bowtie = "bowtie";
const string cmd_option_parser::format_bed = "bed";
const string cmd_option_parser::format_sam = "sam";
const string cmd_option_parser::format_bam = "bam";
const string cmd_option_parser::format_eland = "eland";

double cmd_option_parser::getCutOff() const {
    return _p_cut_off;
}

double cmd_option_parser::getDelta() const {
    return _delta;
}

uint32_t cmd_option_parser::getExt_length() const {
    return _ext_length;
}

string cmd_option_parser::getFormat() const {
    return _format;
}

string cmd_option_parser::getMode() const {
    return _mode;
}

uint32_t cmd_option_parser::getNo_of_thread() const {
    return _no_of_thread;
}

string cmd_option_parser::getOutput_dir() const {
    return _output_dir;
}

string cmd_option_parser::getOutput_file() const {
    return _output_file;
}

bool cmd_option_parser::getPad() const {
    return _pad;
}

const vector<string> &cmd_option_parser::getTreatFiles() const {
    return _treat_files;
}

const vector<string> &cmd_option_parser::getControlFiles() const {
    return _control_files;
}

uint32_t cmd_option_parser::getBandwidth() const {
    return _bandwidth;
}


void cmd_option_parser::setExt_length(uint32_t _ext_length) {
    this->_ext_length = _ext_length;
}

void cmd_option_parser::setFormat(string _format) {
    this->_format = _format;
}

void cmd_option_parser::setOutput_dir(string _output_dir) {
    this->_output_dir = _output_dir;
}

void cmd_option_parser::setOutput_file(string _output_file) {
    this->_output_file = _output_file;
}

void cmd_option_parser::setPad(bool _pad) {
    this->_pad = _pad;
}

bool cmd_option_parser::getChrtableSpecified() const {
    return _chrtableSpecified;
}

void cmd_option_parser::setChrtableSpecified(bool _chrtableSpecified) {
    this->_chrtableSpecified = _chrtableSpecified;
}

bool cmd_option_parser::getVerboseRequested() const {
    return _verboseRequested;
}

void cmd_option_parser::setVerboseRequested(bool _verboseRequested) {
    this->_verboseRequested = _verboseRequested;
}

vector<string> cmd_option_parser::getChrs_to_parse() const {
    return _chrs_to_parse;
}

void cmd_option_parser::setConfigFile(string _config_file) {
    this->_config_file = _config_file;
}

double cmd_option_parser::getFdrCutOff() const {
    return _fdr_cut_off;
}

string cmd_option_parser::getChr_table() const {
    return _chr_table_file;
}

void cmd_option_parser::setChr_table(string _chr_table) {
}

uint32_t cmd_option_parser::getBinlength() const {
    return _binlength;
}

uint32_t cmd_option_parser::getHtmlRegionLength() const {
    return _html_region_length;
}

