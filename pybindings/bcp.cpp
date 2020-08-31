#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "option_parser/BCPOption.h"
#include "short_reads/reads.h"
#include "app/bcp.h"
#include "region_detector/calledpeak.h"

namespace py = pybind11;
using options::BCPOption;
using app::bcp::enriched_regions;

PYBIND11_MAKE_OPAQUE(enriched_regions);

PYBIND11_MODULE(pybcp, m) {
    m.doc() = "Python bindings for bcp algorithm from the peakranger software suite.";

    py::class_<BCPOption>(m, "BCPOption")
            .def(py::init<const std::string&>())
            .def_property("format", &BCPOption::getFormat, &BCPOption::setFormat)
            .def_property("verbose", &BCPOption::getVerboseRequested, &BCPOption::setVerboseRequested)
            .def_property("output_file", &BCPOption::getOutputFile, &BCPOption::setOutputFile)
            .def_property("output_dir", &BCPOption::getOutputDir, &BCPOption::setOutputDir)
            .def_property("cutoff", &BCPOption::getCutOff, &BCPOption::setCutOff)
//            .def_property("fdr_cutoff", &BCPOption::getFdrCutOff, &BCPOption::setFdrCutOff)
            .def_property("sliding_win_size", &BCPOption::getSlidingWinSize, &BCPOption::setSlidingWinSize)
            .def_property("ext_length", &BCPOption::getExtLength, &BCPOption::setExtLength)
            .def_property_readonly("treat_files", &BCPOption::getTreatFiles)
            .def_property_readonly("control_files", &BCPOption::getControlFiles);

    py::class_<Reads>(m, "Reads");
    py::class_<enriched_regions>(m, "EnrichedRegions");

    m.def("parse_options", [](int argc, const std::vector<std::string>& argv) {
        std::cout << argc << " " << argv.size() << std::endl;

        char** cstrs = new char*[argc];
        for (auto i = 0; i < argc; i++) {
            cstrs[i] = const_cast<char *>(argv[i].c_str());
            std::cout << argv[i] << std::endl;
        }

        static const std::string version = std::to_string(VERSION);
        auto result = app::bcp::parse_options(argc, cstrs, version);
        delete[] cstrs;

        return result;
    }, "Parse bcp options from the command line argc argv.");

    m.def("parse_data", &app::bcp::parse_data,
          "Parse reads according to the bcp config. Returns pair - (treatment, control) reads.");

    m.def("predict", &app::bcp::predict,
          "Predict enriched regions using bcp algorithm.");

    m.def("report_regions", &app::bcp::report_regions, "Report predicted regions to the output file.");
    m.def("report_details", &app::bcp::report_details, "Report region details to the output file.");

    m.def("run", [](int argc, const std::vector<std::string> argv) {
        char** cstrs = new char*[argc];
        for (auto i = 0; i < argc; i++) {
            cstrs[i] = const_cast<char *>(argv[i].c_str());
            std::cout << argv[i] << std::endl;
        }

        static const std::string version = std::to_string(VERSION);
        app::bcp::run(argc, cstrs, version);
        delete[] cstrs;
    }, "Run bcp end-to-end.");
}
