#include "SimArrayBuilder.hh"

void SimArrayBuilder::Init()
{
    pool_ = arrow::default_memory_pool();

    // Initialize builders
    detWorkerIdBuilder_ = std::make_shared<arrow::Int32Builder>(pool_); // worker(thread) id
    detEventIdBuilder_ = std::make_shared<arrow::UInt64Builder>(pool_); // event id
    detNameBuilder_ = std::make_shared<arrow::StringBuilder>(pool_);
    copyIdBuilder_ = std::make_shared<arrow::Int32Builder>(pool_);
    eDepBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particleWorkerIdBuilder_ = std::make_shared<arrow::Int32Builder>(pool_); // worker(thread) id
    particleEventIdBuilder_ = std::make_shared<arrow::UInt64Builder>(pool_); // event id
    particleNameBuilder_ = std::make_shared<arrow::StringBuilder>(pool_);
    particleEnergyBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particleDirThetaBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particleDirPhiBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particlePosXBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particlePosYBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);
    particlePosZBuilder_ = std::make_shared<arrow::DoubleBuilder>(pool_);

    // Create schema
    {
        arrow::FieldVector fieldVec;
        fieldVec.emplace_back(std::make_shared<arrow::Field>("workerId", arrow::int32()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("eventId", arrow::uint64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("detName", arrow::utf8()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("copyId", arrow::int32()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("eDep", arrow::float64()));
        detDataSchema_ = arrow::schema(fieldVec);
    }

    {
        arrow::FieldVector fieldVec;
        fieldVec.emplace_back(std::make_shared<arrow::Field>("workerId", arrow::int32()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("eventId", arrow::uint64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("energy", arrow::float64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("theta", arrow::float64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("phi", arrow::float64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("x", arrow::float64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("y", arrow::float64()));
        fieldVec.emplace_back(std::make_shared<arrow::Field>("z", arrow::float64()));
        particleDataSchema_ = arrow::schema(fieldVec);
    }
}

void SimArrayBuilder::FillEDep(const int workerId, const uint64_t &eventId, const std::string &detName, const double &eDep, const int copyId)
{
    PARQUET_THROW_NOT_OK(detWorkerIdBuilder_->Append(workerId));
    PARQUET_THROW_NOT_OK(detEventIdBuilder_->Append(eventId));
    PARQUET_THROW_NOT_OK(detNameBuilder_->Append(detName));
    PARQUET_THROW_NOT_OK(copyIdBuilder_->Append(copyId));
    PARQUET_THROW_NOT_OK(eDepBuilder_->Append(eDep));
}

void SimArrayBuilder::FillParticle(const int workerId, const uint64_t &eventId, const std::string &pName, const double &e, const double &theta, const double &phi, const double &posx, const double &posy, const double &posz)
{
    PARQUET_THROW_NOT_OK(particleWorkerIdBuilder_->Append(workerId));
    PARQUET_THROW_NOT_OK(particleEventIdBuilder_->Append(eventId));
    PARQUET_THROW_NOT_OK(particleNameBuilder_->Append(pName));
    PARQUET_THROW_NOT_OK(particleEnergyBuilder_->Append(e));
    PARQUET_THROW_NOT_OK(particleDirThetaBuilder_->Append(theta));
    PARQUET_THROW_NOT_OK(particleDirPhiBuilder_->Append(phi));
    PARQUET_THROW_NOT_OK(particlePosXBuilder_->Append(posx));
    PARQUET_THROW_NOT_OK(particlePosYBuilder_->Append(posy));
    PARQUET_THROW_NOT_OK(particlePosZBuilder_->Append(posz));
}

void SimArrayBuilder::FinalizeDetectorData(const std::string &filename)
{
    // Finalize arrays
    arrow::ArrayVector arrayVec;
    std::vector<std::shared_ptr<arrow::ArrayBuilder>> builders = {detWorkerIdBuilder_, detEventIdBuilder_, detNameBuilder_, copyIdBuilder_, eDepBuilder_};
    std::transform(builders.begin(), builders.end(), arrayVec.begin(), [](std::shared_ptr<arrow::ArrayBuilder> x)
                   { std::shared_ptr<arrow::Array> array; PARQUET_THROW_NOT_OK(x->Finish(&array)); return array; });

    // Write table to file
    std::string fName = filename;
    auto table = arrow::Table::Make(detDataSchema_, arrayVec);
    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    PARQUET_ASSIGN_OR_THROW(
        outfile,
        arrow::io::FileOutputStream::Open(filename));
    PARQUET_THROW_NOT_OK(parquet::arrow::WriteTable(*table, pool_, outfile));
}

void SimArrayBuilder::FinalizeParticleData(const std::string &filename)
{
    // Finalize arrays
    arrow::ArrayVector arrayVec;
    std::vector<std::shared_ptr<arrow::ArrayBuilder>> builders = {particleWorkerIdBuilder_, particleEventIdBuilder_, particleNameBuilder_, particleEnergyBuilder_, particleDirThetaBuilder_, particleDirPhiBuilder_, particlePosXBuilder_, particlePosYBuilder_, particlePosZBuilder_};
    std::transform(builders.begin(), builders.end(), arrayVec.begin(), [](std::shared_ptr<arrow::ArrayBuilder> x)
                   { std::shared_ptr<arrow::Array> array; PARQUET_THROW_NOT_OK(x->Finish(&array)); return array; });

    // Write table to file
    std::string fName = filename;
    auto table = arrow::Table::Make(particleDataSchema_, arrayVec);
    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    PARQUET_ASSIGN_OR_THROW(
        outfile,
        arrow::io::FileOutputStream::Open(filename));
    PARQUET_THROW_NOT_OK(parquet::arrow::WriteTable(*table, pool_, outfile));
}