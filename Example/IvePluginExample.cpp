#include "IvePluginExample.h"
#include <IvePluginAdapter.hpp>
#include <vamp-sdk/PluginAdapter.h>

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

IvePluginExample::RainbowMatrix::RainbowMatrix(float sampleRate)
: Vamp::Plugin(sampleRate)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::string IvePluginExample::RainbowMatrix::getMaker() const
{
    return "IVE";
}

int IvePluginExample::RainbowMatrix::getPluginVersion() const
{
    return 1;
}

std::string IvePluginExample::RainbowMatrix::getCopyright() const
{
    return "IVE Plugin.";
}

bool IvePluginExample::RainbowMatrix::initialise(size_t, size_t, size_t)
{
    return true;
}

Vamp::Plugin::InputDomain IvePluginExample::RainbowMatrix::getInputDomain() const
{
    return TimeDomain;
}

std::string IvePluginExample::RainbowMatrix::getIdentifier() const
{
    return "iverainbowmatrix";
}

std::string IvePluginExample::RainbowMatrix::getName() const
{
    return "Raindow Matrix";
}

std::string IvePluginExample::RainbowMatrix::getDescription() const
{
    return "Generates a raindow.";
}

size_t IvePluginExample::RainbowMatrix::getPreferredBlockSize() const
{
    return 256;
}

Vamp::Plugin::OutputList IvePluginExample::RainbowMatrix::getOutputDescriptors() const
{
    OutputList list;
    {
        OutputDescriptor d;
        d.identifier = "hue";
        d.name = "Hue";
        d.description = "A hue of cell";
        d.unit = "";
        d.hasFixedBinCount = true;
        d.binCount = gSize;
        d.hasKnownExtents = true;
        d.minValue = 0.0;
        d.maxValue = 1.0;
        d.isQuantized = false;
        d.sampleType = OutputDescriptor::SampleType::OneSamplePerStep;
        d.hasDuration = false;
        list.push_back(std::move(d));
    }
    return list;
}

void IvePluginExample::RainbowMatrix::reset()
{
}

Vamp::Plugin::FeatureSet IvePluginExample::RainbowMatrix::process(const float* const*, Vamp::RealTime timeStamp)
{
    Vamp::Plugin::FeatureSet set;
    set[0].push_back({});
    auto& values = set[0][0].values;
    values.resize(gSize);
    static auto constexpr maxSize = gSize * gSize;
    for(auto& value : values)
    {
        value = static_cast<float>(mPosition++) / static_cast<float>(maxSize);
        if(mPosition >= maxSize)
        {
            mPosition = 0;
        }
    }
    return set;
}

Vamp::Plugin::FeatureSet IvePluginExample::RainbowMatrix::getRemainingFeatures()
{
    return {};
}

bool IvePluginExample::RainbowMatrix::supportColorMap(int index) const
{
    return index == 0;
}

std::vector<IvePluginExample::RainbowMatrix::Color> IvePluginExample::RainbowMatrix::getColorMap(int index, Vamp::Plugin::Feature const& feature)
{
    if(index != 0)
    {
        return {};
    }

    auto const hToColor = [](float h) -> std::uint32_t
    {
        auto const hueToRgb = [](float t)
        {
            t = t < 0.0f ? t + 1.0f : (t > 1.0f ? t - 1.0f : t);
            if(t < 1.0f / 6.0f)
            {
                return 6.0f * t;
            }
            if(t < 1.0f / 2.0f)
            {
                return 1.0f;
            }
            if(t < 2.0f / 3.0f)
            {
                return (2.0f / 3.0f - t) * 6.0f;
            }
            return 0.0f;
        };

        static constexpr std::uint32_t alpha = 255;
        static constexpr float oneThird = 1.0f / 3.0f;
        auto const red = static_cast<std::uint32_t>(std::round(hueToRgb(h + oneThird) * 255.0f));
        auto const green = static_cast<std::uint32_t>(std::round(hueToRgb(h) * 255.0f));
        auto const blue = static_cast<std::uint32_t>(std::round(hueToRgb(h - oneThird) * 255.0f));
        return ((alpha << 24) | (red << 16) | (green << 8) | blue);
    };

    std::vector<Color> colors;
    colors.reserve(feature.values.size());
    for(auto const& value : feature.values)
    {
        colors.push_back(hToColor(value));
    }
    return colors;
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
                static Vamp::PluginAdapter<IvePluginExample::RainbowMatrix> adaptater;
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
                return Ive::PluginAdapter::getDescriptor<IvePluginExample::RainbowMatrix>();
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
