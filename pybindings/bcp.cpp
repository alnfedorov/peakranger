#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
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
            .def_property_readonly("control_files", &BCPOption::getControlFiles)
            .def("__eq__", [](const BCPOption &p1, const BCPOption &p2) {
                return p1.version() == p2.version() && p1.getHtmlRegionLength() == p2.getHtmlRegionLength() &&
                p1.getGeneAnnoFile() == p2.getGeneAnnoFile() && p1.getFormat() == p2.getFormat() &&
                p1.getVerboseRequested() == p2.getVerboseRequested() && p1.getCutOff() == p2.getCutOff() &&
                p1.getSlidingWinSize() == p2.getSlidingWinSize() && p1.getExtLength() == p2.getExtLength() &&
                p1.getTreatFiles() == p2.getTreatFiles() && p1.getControlFiles() == p2.getControlFiles() &&
                p1.getOutputFile() == p2.getOutputFile() && p1.getOutputDir() == p2.getOutputDir();
            })
            .def(py::pickle(
                    [](const BCPOption &p) { // __getstate__
                        return py::make_tuple(
                            p.version(), p.getHtmlRegionLength(), p.getGeneAnnoFile(), p.getFormat(),
                            p.getVerboseRequested(), p.getCutOff(), p.getSlidingWinSize(), p.getExtLength(),
                            p.getTreatFiles(), p.getControlFiles(), p.getOutputFile(), p.getOutputDir()
                        );
                    },
                    [](py::tuple t) { // __setstate__
                        if (t.size() != 12)
                            throw std::runtime_error("Invalid state!");

                        auto p = BCPOption(t[0].cast<std::string>());
                        p.setHtmlRegionLength(t[1].cast<uint32_t>());
                        p.setGeneAnnoFile(t[2].cast<std::string>());
                        p.setFormat(t[3].cast<std::string>());
                        p.setVerboseRequested(t[4].cast<bool>());
                        p.setCutOff(t[5].cast<double>());
                        p.setSlidingWinSize(t[6].cast<uint32_t>());
                        p.setExtLength(t[7].cast<uint32_t>());
                        p.setTreatFiles(t[8].cast<std::vector<std::string>>());
                        p.setControlFiles(t[9].cast<std::vector<std::string>>());
                        p.setOutputFile(t[10].cast<std::string>());
                        p.setOutputDir(t[11].cast<std::string>());

                        return p;
                    }
            ));

    py::class_<Reads>(m, "Reads")
            .def("__eq__", [](const BCPOption &p1, const BCPOption &p2) {
                return p1.version() == p2.version() && p1.getHtmlRegionLength() == p2.getHtmlRegionLength() &&
                       p1.getGeneAnnoFile() == p2.getGeneAnnoFile() && p1.getFormat() == p2.getFormat() &&
                       p1.getVerboseRequested() == p2.getVerboseRequested() && p1.getCutOff() == p2.getCutOff() &&
                       p1.getSlidingWinSize() == p2.getSlidingWinSize() && p1.getExtLength() == p2.getExtLength() &&
                       p1.getTreatFiles() == p2.getTreatFiles() && p1.getControlFiles() == p2.getControlFiles() &&
                       p1.getOutputFile() == p2.getOutputFile() && p1.getOutputDir() == p2.getOutputDir();
            })
            .def("finalize", &Reads::finalize)
            .def("size", &Reads::size)
            .def("poschrs", [](const Reads& r) { return r.pos_reads.chrs(); })
            .def("negchrs", [](const Reads& r) { return r.neg_reads.chrs(); })
            .def_property_readonly("readlen", &Reads::getReadlength)
            .def("__eq__", [](const Reads &r1, const Reads &r2) {
                // Finalize might be called here(it is not the best thing)
                if (r1.size() != r2.size() ||
                    r1.getReadlength() != r2.getReadlength() ||
                    r1.pos_reads.finalized() != r2.pos_reads.finalized() ||
                    r1.neg_reads.finalized() != r2.neg_reads.finalized() ||
                    r1.pos_reads.chrs() != r2.pos_reads.chrs() ||
                    r1.neg_reads.chrs() != r2.neg_reads.chrs())
                    return false;

                for (auto& chr: r1.pos_reads.chrs())
                    if (!std::equal(r1.pos_reads.begin_of(chr), r1.pos_reads.end_of(chr),
                                    r2.pos_reads.begin_of(chr), r2.pos_reads.end_of(chr)))
                        return false;

                for (auto& chr: r1.neg_reads.chrs())
                    if (!std::equal(r1.neg_reads.begin_of(chr), r1.neg_reads.end_of(chr),
                                    r2.neg_reads.begin_of(chr), r2.neg_reads.end_of(chr)))
                        return false;
                return true;
            })
            .def(py::pickle(
                    [](Reads &reads) { // __getstate__
                        static auto reads_to_buffer = [](StrandReads& sreads) {
                            auto result = py::list();
                            sreads.finalize();

                            for (auto& chr: sreads.chrs()) {
                                auto p = sreads.begin_of(chr);
                                auto size = sreads.end_of(chr) - p;
                                auto capsule = py::capsule(p, [](void *vec) { });
                                auto array = py::array(size, p, std::move(capsule));
                                result.append(py::make_tuple(chr, std::move(array)));
                            }
                            return result;
                        };

                        auto l = py::list();
                        l.append(reads.getReadlength());
                        l.append(reads_to_buffer(reads.pos_reads));
                        l.append(reads_to_buffer(reads.neg_reads));
                        return l;
                    },
                    [](py::list l) { // __setstate__
                        if (l.size() != 3)
                            throw std::runtime_error("Invalid state!");

                        static auto buffer_to_reads = [](const py::list& sreads) {
                            std::map<std::string, uint32_t*> reads;
                            std::map<std::string, size_t> sizes;
                            std::vector<py::array> arrays;

                            for (auto& e: sreads) {
                                auto t = e.cast<py::tuple>();
                                auto chr = t[0].cast<std::string>();
                                auto arr = t[1].cast<py::array>();
                                reads[chr] = (uint32_t*)arr.mutable_data();
                                sizes[chr] = arr.size();
                                arr.inc_ref();
                                arrays.push_back(std::move(arr));
                            }
                            return StrandReads(std::move(reads), std::move(sizes),
                                               [arrays]() { for(auto& arr: arrays) arr.dec_ref();});
                        };

                        auto reads = Reads();
                        reads.setReadlength(l[0].cast<uint32_t>());
                        reads.pos_reads = buffer_to_reads(l[1].cast<py::list>());
                        reads.neg_reads = buffer_to_reads(l[2].cast<py::list>());
                        return reads;
                    }
            ));
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
