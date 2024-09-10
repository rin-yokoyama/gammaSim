#ifndef __SIM_ARRAY_BUILDER_HH__
#define __SIM_ARRAY_BUILDER_HH__
#include <algorithm>
#include <memory>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

class SimArrayBuilder
{
public:
    SimArrayBuilder() {};
    virtual ~SimArrayBuilder() {};
    void Init();
    void FillEDep(const int workerId, const uint64_t &eventId, const std::string &detName, const double &eDep, const int copyId);
    void FillParticle(const int workerId, const uint64_t &eventId, const std::string &pName, const double &e, const double &theta, const double &phi, const double &posx, const double &posy, const double &posz);
    void FinalizeDetectorData(const std::string &filename);
    void FinalizeParticleData(const std::string &filename);

protected:
    arrow::MemoryPool *pool_;
    // Schema
    std::shared_ptr<arrow::Schema> detDataSchema_;
    std::shared_ptr<arrow::Schema> particleDataSchema_;

    // Arrow Builders
    // Detector data
    std::shared_ptr<arrow::Int32Builder> detWorkerIdBuilder_; // worker(thread) id
    std::shared_ptr<arrow::UInt64Builder> detEventIdBuilder_; // event id
    std::shared_ptr<arrow::StringBuilder> detNameBuilder_;    // Detector name
    std::shared_ptr<arrow::Int32Builder> copyIdBuilder_;      // Copy Id
    std::shared_ptr<arrow::DoubleBuilder> eDepBuilder_;       // Energy deposit
    // Initial particle data
    std::shared_ptr<arrow::Int32Builder> particleWorkerIdBuilder_;  // worker(thread) id
    std::shared_ptr<arrow::UInt64Builder> particleEventIdBuilder_;  // event id
    std::shared_ptr<arrow::StringBuilder> particleNameBuilder_;     // Particle name
    std::shared_ptr<arrow::DoubleBuilder> particleEnergyBuilder_;   // Particle energy
    std::shared_ptr<arrow::DoubleBuilder> particleDirThetaBuilder_; // Momentum direction theta
    std::shared_ptr<arrow::DoubleBuilder> particleDirPhiBuilder_;   // Momentum direction phi
    std::shared_ptr<arrow::DoubleBuilder> particlePosXBuilder_;     // Emission position X
    std::shared_ptr<arrow::DoubleBuilder> particlePosYBuilder_;     // Emission position Y
    std::shared_ptr<arrow::DoubleBuilder> particlePosZBuilder_;     // Emission position Z
};

#endif