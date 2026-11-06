#include "IvePluginExample.h"
#include <IvePluginAdapter.hpp>
#include <vamp-sdk/PluginAdapter.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <random>

IvePluginExample::CopyMarker::CopyMarker(float sampleRate)
: Vamp::Plugin(sampleRate)
{
}

std::string IvePluginExample::CopyMarker::getMaker() const
{
    return "IVE";
}

int IvePluginExample::CopyMarker::getPluginVersion() const
{
    return 1;
}

std::string IvePluginExample::CopyMarker::getCopyright() const
{
    return "IVE Plugin.";
}

bool IvePluginExample::CopyMarker::initialise(size_t, size_t, size_t)
{
    return true;
}

Vamp::Plugin::InputDomain IvePluginExample::CopyMarker::getInputDomain() const
{
    return TimeDomain;
}

std::string IvePluginExample::CopyMarker::getIdentifier() const
{
    return "ivecopymarker";
}

std::string IvePluginExample::CopyMarker::getName() const
{
    return "Copy Marker";
}

std::string IvePluginExample::CopyMarker::getDescription() const
{
    return "Copy a marker analysis.";
}

size_t IvePluginExample::CopyMarker::getPreferredBlockSize() const
{
    return 256;
}

Vamp::Plugin::OutputList IvePluginExample::CopyMarker::getOutputDescriptors() const
{
    OutputList list;
    {
        OutputDescriptor d;
        d.identifier = "markers";
        d.name = "Marker";
        d.description = "Marker";
        d.unit = "m";
        d.hasFixedBinCount = true;
        d.binCount = 0;
        d.hasKnownExtents = false;
        d.isQuantized = false;
        d.sampleType = OutputDescriptor::SampleType::VariableSampleRate;
        d.hasDuration = false;
        list.push_back(std::move(d));
    }
    return list;
}

Ive::PluginExtension::InputList IvePluginExample::CopyMarker::getInputDescriptors() const
{
    return getOutputDescriptors();
}

void IvePluginExample::CopyMarker::reset()
{
    mFeatureSet.clear();
}

Vamp::Plugin::FeatureSet IvePluginExample::CopyMarker::process(const float* const*, Vamp::RealTime timeStamp)
{
    return {};
}

Vamp::Plugin::FeatureSet IvePluginExample::CopyMarker::getRemainingFeatures()
{
    return mFeatureSet;
}

void IvePluginExample::CopyMarker::setPreComputingFeatures(FeatureSet const& fs)
{
    mFeatureSet = fs;
}

IvePluginExample::RandomPoint::RandomPoint(float sampleRate)
: Vamp::Plugin(sampleRate)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::string IvePluginExample::RandomPoint::getMaker() const
{
    return "IVE";
}

int IvePluginExample::RandomPoint::getPluginVersion() const
{
    return 1;
}

std::string IvePluginExample::RandomPoint::getCopyright() const
{
    return "IVE Plugin.";
}

bool IvePluginExample::RandomPoint::initialise(size_t, size_t, size_t)
{
    return true;
}

Vamp::Plugin::InputDomain IvePluginExample::RandomPoint::getInputDomain() const
{
    return TimeDomain;
}

std::string IvePluginExample::RandomPoint::getIdentifier() const
{
    return "iverandompoint";
}

std::string IvePluginExample::RandomPoint::getName() const
{
    return "Random Points";
}

std::string IvePluginExample::RandomPoint::getDescription() const
{
    return "Generates random points with extra output.";
}

size_t IvePluginExample::RandomPoint::getPreferredBlockSize() const
{
    return 256;
}

Vamp::Plugin::OutputList IvePluginExample::RandomPoint::getOutputDescriptors() const
{
    OutputList list;
    {
        OutputDescriptor d;
        d.identifier = "points";
        d.name = "Point";
        d.description = "Point";
        d.unit = "p";
        d.hasFixedBinCount = true;
        d.binCount = 1;
        d.hasKnownExtents = true;
        d.minValue = 0.0f;
        d.maxValue = 1.0f;
        d.isQuantized = false;
        d.sampleType = OutputDescriptor::SampleType::OneSamplePerStep;
        d.hasDuration = false;
        list.push_back(std::move(d));
    }
    return list;
}

IvePluginExample::RandomPoint::OutputExtraList IvePluginExample::RandomPoint::getOutputExtraDescriptors(size_t outputDescriptorIndex) const
{
    OutputExtraList list;
    if(outputDescriptorIndex == 0)
    {
        OutputExtraDescriptor d;
        d.identifier = "threshold";
        d.name = "Threshold";
        d.description = "A threshold value that can filter the results";
        d.unit = "%";
        d.hasKnownExtents = true;
        d.minValue = 0.0f;
        d.maxValue = 100.0f;
        d.isQuantized = true;
        d.quantizeStep = 1.0f;
        list.push_back(std::move(d));
    }
    return list;
}

void IvePluginExample::RandomPoint::reset()
{
}

Vamp::Plugin::FeatureSet IvePluginExample::RandomPoint::process(const float* const*, Vamp::RealTime timeStamp)
{
    Vamp::Plugin::FeatureSet set;
    set[0].push_back({});
    auto& values = set[0][0].values;
    values.push_back(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
    values.push_back(std::round(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 100.0f));
    return set;
}

Vamp::Plugin::FeatureSet IvePluginExample::RandomPoint::getRemainingFeatures()
{
    return {};
}

IvePluginExample::MultiVoices::MultiVoices(float sampleRate)
: Vamp::Plugin(sampleRate)
{
}

std::string IvePluginExample::MultiVoices::getMaker() const
{
    return "IVE";
}

int IvePluginExample::MultiVoices::getPluginVersion() const
{
    return 1;
}

std::string IvePluginExample::MultiVoices::getCopyright() const
{
    return "IVE Plugin.";
}

bool IvePluginExample::MultiVoices::initialise(size_t, size_t, size_t)
{
    reset();
    return true;
}

Vamp::Plugin::InputDomain IvePluginExample::MultiVoices::getInputDomain() const
{
    return TimeDomain;
}

std::string IvePluginExample::MultiVoices::getIdentifier() const
{
    return "ivemultivoices";
}

std::string IvePluginExample::MultiVoices::getName() const
{
    return "Multi Voices";
}

std::string IvePluginExample::MultiVoices::getDescription() const
{
    return "Generates deterministic points on several voices that start and stop at different times.";
}

size_t IvePluginExample::MultiVoices::getPreferredBlockSize() const
{
    return 256;
}

Vamp::Plugin::OutputList IvePluginExample::MultiVoices::getOutputDescriptors() const
{
    OutputList list;
    for(size_t voice = 0; voice < mNumVoices; ++voice)
    {
        OutputDescriptor d;
        d.identifier = "voice" + std::to_string(voice + 1);
        d.name = "Voice " + std::to_string(voice + 1);
        d.description = "The points of the voice " + std::to_string(voice + 1);
        d.unit = "";
        d.hasFixedBinCount = true;
        d.binCount = 1;
        d.hasKnownExtents = true;
        d.minValue = 0.0f;
        d.maxValue = static_cast<float>(gMaxVoices + 1);
        d.isQuantized = false;
        d.sampleType = OutputDescriptor::SampleType::VariableSampleRate;
        d.sampleRate = 0.0f;
        d.hasDuration = false;
        list.push_back(std::move(d));
    }
    // A conventional output after the voices (its index depends on the number of voices)
    {
        OutputDescriptor d;
        d.identifier = "active";
        d.name = "Active Voices";
        d.description = "The number of active voices";
        d.unit = "";
        d.hasFixedBinCount = true;
        d.binCount = 1;
        d.hasKnownExtents = true;
        d.minValue = 0.0f;
        d.maxValue = static_cast<float>(gMaxVoices);
        d.isQuantized = true;
        d.quantizeStep = 1.0f;
        d.sampleType = OutputDescriptor::SampleType::OneSamplePerStep;
        d.hasDuration = false;
        list.push_back(std::move(d));
    }
    return list;
}

std::optional<IvePluginExample::MultiVoices::VoiceDescriptor> IvePluginExample::MultiVoices::getOutputVoiceDescriptor(size_t outputDescriptorIndex) const
{
    if(outputDescriptorIndex != 0)
    {
        return {};
    }
    VoiceDescriptor d;
    d.identifier = "voices";
    d.name = "Voices";
    d.description = "The number of voices";
    d.unit = "";
    d.minValue = 1.0f;
    d.maxValue = static_cast<float>(gMaxVoices);
    d.defaultValue = 3.0f;
    d.isQuantized = true;
    d.quantizeStep = 1.0f;
    return d;
}

size_t IvePluginExample::MultiVoices::getOutputVoiceCount(size_t outputDescriptorIndex) const
{
    return outputDescriptorIndex == 0 ? mNumVoices : 1;
}

void IvePluginExample::MultiVoices::setOutputVoiceCount(std::string const& identifier, size_t count)
{
    if(identifier == "voice1")
    {
        mNumVoices = std::clamp(count, static_cast<size_t>(1), gMaxVoices);
    }
}

void IvePluginExample::MultiVoices::reset()
{
    mBlockIndex = 0;
}

Vamp::Plugin::FeatureSet IvePluginExample::MultiVoices::process(const float* const*, Vamp::RealTime timeStamp)
{
    // Each voice is active two thirds of the time with a different phase, so the voices start and stop at different
    // times, and the value of a voice oscillates around its number
    static auto constexpr pi = 3.14159265358979323846;
    Vamp::Plugin::FeatureSet set;
    auto numActiveVoices = 0;
    for(size_t voice = 0; voice < mNumVoices; ++voice)
    {
        if((mBlockIndex / 16 + voice) % 3 != 0)
        {
            ++numActiveVoices;
            Feature feature;
            feature.hasTimestamp = true;
            feature.timestamp = timeStamp;
            auto const phase = static_cast<double>(mBlockIndex) / 32.0 * 2.0 * pi;
            feature.values.push_back(static_cast<float>(static_cast<double>(voice + 1) + 0.25 * std::sin(phase)));
            set[static_cast<int>(voice)].push_back(std::move(feature));
        }
    }
    Feature active;
    active.values.push_back(static_cast<float>(numActiveVoices));
    set[static_cast<int>(mNumVoices)].push_back(std::move(active));
    ++mBlockIndex;
    return set;
}

Vamp::Plugin::FeatureSet IvePluginExample::MultiVoices::getRemainingFeatures()
{
    return {};
}

#ifdef __cplusplus
extern "C"
{
#endif
    VampPluginDescriptor const* vampGetPluginDescriptor(unsigned int version, unsigned int index)
    {
        if(version < 1)
        {
            return nullptr;
        }
        switch(index)
        {
            case 0:
            {
                static Vamp::PluginAdapter<IvePluginExample::CopyMarker> adaptater;
                return adaptater.getDescriptor();
            }
            case 1:
            {
                static Vamp::PluginAdapter<IvePluginExample::RandomPoint> adaptater;
                return adaptater.getDescriptor();
            }
            case 2:
            {
                static Vamp::PluginAdapter<IvePluginExample::MultiVoices> adaptater;
                return adaptater.getDescriptor();
            }
            default:
            {
                return nullptr;
            }
        }
    }

    IVE_EXTERN IvePluginDescriptor const* iveGetPluginDescriptor(unsigned int version, unsigned int index)
    {
        if(version < 1)
        {
            return nullptr;
        }
        switch(index)
        {
            case 0:
            {
                return Ive::PluginAdapter::getDescriptor<IvePluginExample::CopyMarker>();
            }
            case 1:
            {
                return Ive::PluginAdapter::getDescriptor<IvePluginExample::RandomPoint>();
            }
            case 2:
            {
                return Ive::PluginAdapter::getDescriptor<IvePluginExample::MultiVoices>();
            }
            default:
            {
                return nullptr;
            }
        }
    }
#ifdef __cplusplus
}
#endif
