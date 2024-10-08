#include "IvePluginAdapter.hpp"
#include <cstring>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

IVE_FILE_BEGIN

void PluginAdapter::release(VampInputDescriptor* inputDescriptor)
{
    auto descriptor = std::unique_ptr<VampInputDescriptor>(inputDescriptor);
    if(descriptor == nullptr)
    {
        return;
    }
    auto const free_string = [](char const* str)
    {
        if(str != nullptr)
        {
            std::free(static_cast<void*>(const_cast<char*>(str)));
        }
    };
    free_string(descriptor->identifier);
    free_string(descriptor->name);
    free_string(descriptor->description);
    free_string(descriptor->unit);
    if(descriptor->binNames != nullptr)
    {
        for(size_t i = 0; i < static_cast<size_t>(descriptor->binCount); ++i)
        {
            free_string(descriptor->binNames[i]);
        }
        std::free(static_cast<void*>(descriptor->binNames));
    }
}

VampInputDescriptor* PluginAdapter::create(PluginExtension::InputDescriptor const& pluginInputDescriptor)
{
    auto descriptor = std::make_unique<VampInputDescriptor>();
    if(descriptor == nullptr)
    {
        return nullptr;
    }
    descriptor->identifier = strdup(pluginInputDescriptor.identifier.c_str());
    descriptor->name = strdup(pluginInputDescriptor.name.c_str());
    descriptor->description = strdup(pluginInputDescriptor.description.c_str());
    descriptor->unit = strdup(pluginInputDescriptor.unit.c_str());
    descriptor->hasFixedBinCount = pluginInputDescriptor.hasFixedBinCount;
    descriptor->binCount = static_cast<unsigned int>(pluginInputDescriptor.binCount);
    if(!pluginInputDescriptor.hasFixedBinCount || pluginInputDescriptor.binNames.empty())
    {
        descriptor->binNames = NULL;
    }
    else
    {
        descriptor->binNames = static_cast<char const**>(std::malloc(pluginInputDescriptor.binCount * sizeof(char const*)));
        if(descriptor->binNames != nullptr)
        {
            for(size_t i = 0; i < pluginInputDescriptor.binCount; ++i)
            {
                if(i < pluginInputDescriptor.binNames.size())
                {
                    descriptor->binNames[i] = strdup(pluginInputDescriptor.binNames.at(i).c_str());
                }
                else
                {
                    descriptor->binNames[i] = NULL;
                }
            }
        }
    }

    descriptor->hasKnownExtents = static_cast<int>(pluginInputDescriptor.hasKnownExtents);
    descriptor->minValue = pluginInputDescriptor.minValue;
    descriptor->maxValue = pluginInputDescriptor.maxValue;
    descriptor->isQuantized = pluginInputDescriptor.isQuantized;
    descriptor->quantizeStep = pluginInputDescriptor.quantizeStep;
    switch(pluginInputDescriptor.sampleType)
    {
        case PluginExtension::InputDescriptor::OneSamplePerStep:
            descriptor->sampleType = vampOneSamplePerStep;
            break;
        case PluginExtension::InputDescriptor::FixedSampleRate:
            descriptor->sampleType = vampFixedSampleRate;
            break;
        case PluginExtension::InputDescriptor::VariableSampleRate:
            descriptor->sampleType = vampVariableSampleRate;
            break;
    }
    descriptor->sampleRate = pluginInputDescriptor.sampleRate;
    descriptor->hasDuration = static_cast<int>(pluginInputDescriptor.hasDuration);
    return descriptor.release();
}

VampOutputExtraDescriptor* PluginAdapter::create(PluginExtension::OutputExtraDescriptor const& pluginOutputExtraDescriptor)
{
    auto descriptor = std::make_unique<VampOutputExtraDescriptor>();
    if(descriptor == nullptr)
    {
        return nullptr;
    }
    descriptor->identifier = strdup(pluginOutputExtraDescriptor.identifier.c_str());
    descriptor->name = strdup(pluginOutputExtraDescriptor.name.c_str());
    descriptor->description = strdup(pluginOutputExtraDescriptor.description.c_str());
    descriptor->unit = strdup(pluginOutputExtraDescriptor.unit.c_str());
    descriptor->hasFixedBinCount = true;
    descriptor->binCount = 1;
    descriptor->binNames = NULL;
    descriptor->hasKnownExtents = static_cast<int>(pluginOutputExtraDescriptor.hasKnownExtents);
    descriptor->minValue = pluginOutputExtraDescriptor.minValue;
    descriptor->maxValue = pluginOutputExtraDescriptor.maxValue;
    descriptor->isQuantized = pluginOutputExtraDescriptor.isQuantized;
    descriptor->quantizeStep = pluginOutputExtraDescriptor.quantizeStep;
    descriptor->sampleType = vampFixedSampleRate;
    descriptor->sampleRate = 0.0;
    descriptor->hasDuration = false;
    return descriptor.release();
}

Vamp::Plugin::FeatureSet PluginAdapter::convert(unsigned int numFeatures, VampFeatureList const* features)
{
    if(features == nullptr)
    {
        return {};
    }
    Vamp::Plugin::FeatureSet fs;
    Vamp::Plugin::Plugin::Feature feature;
    for(auto descriptorIndex = 0u; descriptorIndex < numFeatures; ++descriptorIndex)
    {
        auto const featureCount = features[descriptorIndex].featureCount;
        for(auto featureIndex = 0u; featureIndex < featureCount; ++featureIndex)
        {
            auto const& sourcev1 = features[descriptorIndex].features[featureIndex];
            auto const& sourcev2 = features[descriptorIndex].features[featureCount + featureIndex];
            feature.hasTimestamp = sourcev1.v1.hasTimestamp;
            if(feature.hasTimestamp)
            {
                feature.timestamp = _VampPlugin::Vamp::RealTime(sourcev1.v1.sec, sourcev1.v1.nsec);
            }
            else
            {
                feature.timestamp = _VampPlugin::Vamp::RealTime(0, 0);
            }
            feature.hasDuration = sourcev2.v2.hasDuration;
            if(feature.hasDuration)
            {
                feature.duration = _VampPlugin::Vamp::RealTime(sourcev2.v2.durationSec, sourcev2.v2.durationNsec);
            }
            else
            {
                feature.duration = _VampPlugin::Vamp::RealTime(0, 0);
            }
            feature.values.clear();
            feature.values.reserve(sourcev1.v1.valueCount);
            for(auto k = 0u; k < sourcev1.v1.valueCount; ++k)
            {
                feature.values.push_back(sourcev1.v1.values[k]);
            }

            if(sourcev1.v1.label != nullptr)
            {
                feature.label = sourcev1.v1.label;
            }
            else
            {
                feature.label.clear();
            }

            fs[descriptorIndex].push_back(feature);
        }
    }
    return fs;
}

void PluginAdapter::release(IveColorList* colorList)
{
    auto list = std::unique_ptr<IveColorList>(colorList);
    if(list == nullptr)
    {
        return;
    }
    if(list->colorCount != 0 && list->colors != nullptr)
    {
        std::free(list->colors);
        list->colors = nullptr;
    }
}

IveColorList* PluginAdapter::create(std::vector<std::uint32_t> const& colorMap)
{
    auto list = std::make_unique<IveColorList>();
    if(list == nullptr)
    {
        return nullptr;
    }
    auto const size = colorMap.size();
    list->colors = static_cast<uint32_t*>(malloc(size * sizeof(uint32_t)));
    std::copy(colorMap.cbegin(), colorMap.cend(), list->colors);
    list->colorCount = list->colors != nullptr ? static_cast<unsigned int>(size) : 0;
    return list.release();
}

IVE_FILE_END

#ifdef _MSC_VER
#pragma warning(pop)
#endif
