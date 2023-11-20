#include "IvePluginWrapper.hpp"
#include <cassert>
#include <src/vamp-hostsdk/Files.h>

IVE_FILE_BEGIN

void PluginWrapper::FeatureContainer::initialize(Vamp::Plugin::FeatureSet const& fs)
{
    auto const size = fs.empty() ? static_cast<size_t>(0) : fs.crbegin()->first + static_cast<size_t>(1);
    mFeatureList.resize(size);
    mFeatures.resize(size);
    mData.resize(size);
    for(size_t descriptorIndex = 0; descriptorIndex < size; ++descriptorIndex)
    {
        auto& descFeatures = mFeatures[descriptorIndex];
        auto& descDatas = mData[descriptorIndex];
        auto const it = fs.find(static_cast<int>(descriptorIndex));
        if(it != fs.cend())
        {
            auto const featureCount = it->second.size();
            descFeatures.resize(featureCount * static_cast<size_t>(2));
            descDatas.resize(featureCount);
            for(size_t featureIndex = 0; featureIndex < featureCount; ++featureIndex)
            {
                auto const& source = it->second.at(featureIndex);
                auto& descFeaturev1 = descFeatures[featureIndex];
                auto& descFeaturev2 = descFeatures[featureCount + featureIndex];
                auto& descData = descDatas[featureIndex];

                descFeaturev1.v1.hasTimestamp = source.hasTimestamp ? 1 : 0;
                descFeaturev1.v1.sec = source.hasTimestamp ? source.timestamp.sec : 0;
                descFeaturev1.v1.nsec = source.hasTimestamp ? source.timestamp.nsec : 0;

                descFeaturev2.v2.hasDuration = source.hasDuration ? 1 : 0;
                descFeaturev2.v2.durationSec = source.hasDuration ? source.duration.sec : 0;
                descFeaturev2.v2.durationNsec = source.hasDuration ? source.duration.nsec : 0;

                std::get<0>(descData) = source.label;
                descFeaturev1.v1.label = std::get<0>(descData).data();

                std::get<1>(descData) = source.values;
                descFeaturev1.v1.valueCount = static_cast<unsigned int>(std::get<1>(descData).size());
                descFeaturev1.v1.values = std::get<1>(descData).data();
            }
        }
        else
        {
            descFeatures.clear();
            descDatas.clear();
        }

        mFeatureList[descriptorIndex].featureCount = descFeatures.empty() ? 0u : static_cast<unsigned int>(descFeatures.size()) / 2u;
        mFeatureList[descriptorIndex].features = descFeatures.empty() ? nullptr : descFeatures.data();
    }
}

unsigned int PluginWrapper::FeatureContainer::getSize() const
{
    return static_cast<unsigned int>(mFeatureList.size());
}

VampFeatureList const* PluginWrapper::FeatureContainer::getData() const
{
    return static_cast<VampFeatureList const*>(mFeatureList.data());
}

PluginWrapper::PluginWrapper(Vamp::Plugin* plugin, std::string const& key)
: Vamp::HostExt::PluginWrapper(plugin)
{
    using namespace Vamp;
    using namespace Vamp::HostExt;
    auto const getHostAdapter = [&]() -> PluginHostAdapter*
    {
        auto* wrapper = static_cast<Vamp::HostExt::PluginWrapper*>(this);
        PluginHostAdapter* adapter = nullptr;
        while(wrapper != nullptr && adapter == nullptr)
        {
            auto* ha = dynamic_cast<PluginHostAdapter*>(wrapper->getPlugin());
            if(ha != nullptr)
            {
                adapter = ha;
            }
            else
            {
                wrapper = dynamic_cast<Vamp::HostExt::PluginWrapper*>(wrapper->getPlugin());
            }
        }
        return adapter;
    };
    auto* adapter = getHostAdapter();
    assert(adapter != nullptr);
    auto* loader = PluginLoader::getInstance();
    assert(loader != nullptr);
    if(adapter && loader)
    {
        mPluginHandle = adapter->getPluginHandle();
        auto const identifier = plugin->getIdentifier();
        assert(!key.empty());
        void* handle = Files::loadLibrary(loader->getLibraryPathForPlugin(key));
        if(handle != nullptr)
        {
            auto fn = reinterpret_cast<IvePluginDescriptorFunction>(Files::lookupInLibrary(handle, "iveGetPluginDescriptor"));
            if(fn != nullptr)
            {
                auto index = 0u;
                IvePluginDescriptor const* descriptor = nullptr;
                while((descriptor = fn(VAMP_API_VERSION, index)))
                {
                    if(descriptor->identifier != nullptr && std::string(descriptor->identifier) == identifier)
                    {
                        mDescriptor = descriptor;
                        break;
                    }
                    ++index;
                }
                if(mDescriptor != nullptr)
                {
                    mLibraryHandle = handle;
                }
            }
            else
            {
                Files::unloadLibrary(handle);
            }
        }
    }
}

PluginWrapper::~PluginWrapper()
{
    if(mLibraryHandle != nullptr)
    {
        Files::unloadLibrary(mLibraryHandle);
    }
    if(mDescriptor != nullptr)
    {
        if(mDescriptor->identifier != nullptr)
        {
            std::free((void*)(mDescriptor->identifier));
        }
        std::free((void*)mDescriptor);
    }
}

PluginWrapper::InputList PluginWrapper::getInputDescriptors() const
{
    if(!isVersionSupported(0, 0, 1))
    {
        return {};
    }

    InputList list;
    auto const count = mDescriptor->getInputCount(mPluginHandle);
    for(auto i = 0u; i < count; ++i)
    {
        auto* inputDescriptor = mDescriptor->getInputDescriptor(mPluginHandle, i);
        if(inputDescriptor != nullptr)
        {
            InputDescriptor d;
            d.identifier = inputDescriptor->identifier;
            d.name = inputDescriptor->name;
            d.description = inputDescriptor->description;
            d.unit = inputDescriptor->unit;
            d.hasFixedBinCount = inputDescriptor->hasFixedBinCount;
            d.binCount = inputDescriptor->binCount;
            if(d.hasFixedBinCount && inputDescriptor->binNames != nullptr)
            {
                for(auto j = 0u; j < inputDescriptor->binCount; ++j)
                {
                    if(inputDescriptor->binNames[j] != nullptr)
                    {
                        d.binNames.push_back(inputDescriptor->binNames[j]);
                    }
                    else
                    {
                        d.binNames.push_back({});
                    }
                }
            }
            d.hasKnownExtents = inputDescriptor->hasKnownExtents;
            d.minValue = inputDescriptor->minValue;
            d.maxValue = inputDescriptor->maxValue;
            d.isQuantized = inputDescriptor->isQuantized;
            d.quantizeStep = inputDescriptor->quantizeStep;

            switch(inputDescriptor->sampleType)
            {
                case vampOneSamplePerStep:
                    d.sampleType = OutputDescriptor::OneSamplePerStep;
                    break;
                case vampFixedSampleRate:
                    d.sampleType = OutputDescriptor::FixedSampleRate;
                    break;
                case vampVariableSampleRate:
                    d.sampleType = OutputDescriptor::VariableSampleRate;
                    break;
            }

            d.sampleRate = inputDescriptor->sampleRate;
            d.hasDuration = inputDescriptor->hasDuration;
            list.push_back(std::move(d));
            mDescriptor->releaseInputDescriptor(inputDescriptor);
        }
    }

    return list;
}

void PluginWrapper::setPreComputingFeatures(Vamp::Plugin::FeatureSet const& fs)
{
    if(!isVersionSupported(0, 0, 1))
    {
        return;
    }
    mComputingFeatures.initialize(fs);
    mDescriptor->setPreComputingFeatures(mPluginHandle, mComputingFeatures.getSize(), mComputingFeatures.getData());
}

PluginWrapper::OutputExtraList PluginWrapper::getOutputExtraDescriptors(size_t outputDescriptorIndex) const
{
    if(!isVersionSupported(0, 0, 2))
    {
        return {};
    }
    OutputExtraList list;
    auto const count = mDescriptor->getOuputExtraCount(mPluginHandle, static_cast<unsigned int>(outputDescriptorIndex));
    for(auto i = 0u; i < count; ++i)
    {
        auto* outputExtraDescriptor = mDescriptor->getOuputExtraDescriptor(mPluginHandle, static_cast<unsigned int>(outputDescriptorIndex), i);
        if(outputExtraDescriptor != nullptr)
        {
            OutputExtraDescriptor d;
            d.identifier = outputExtraDescriptor->identifier;
            d.name = outputExtraDescriptor->name;
            d.description = outputExtraDescriptor->description;
            d.unit = outputExtraDescriptor->unit;
            d.hasKnownExtents = outputExtraDescriptor->hasKnownExtents;
            d.minValue = outputExtraDescriptor->minValue;
            d.maxValue = outputExtraDescriptor->maxValue;
            d.isQuantized = outputExtraDescriptor->isQuantized;
            d.quantizeStep = outputExtraDescriptor->quantizeStep;
            list.push_back(std::move(d));
            mDescriptor->releaseInputDescriptor(outputExtraDescriptor);
        }
    }

    return list;
}

bool PluginWrapper::supportColorMap(int index) const
{
    if(!isVersionSupported(0, 0, 3))
    {
        return false;
    }
    return mDescriptor->supportColorMap(mPluginHandle, index);
}

std::vector<PluginWrapper::Color> PluginWrapper::getColorMap(int index, Vamp::Plugin::Feature const& feature)
{
    if(!isVersionSupported(0, 0, 3))
    {
        return {};
    }
    Vamp::Plugin::FeatureSet fs;
    fs[index] = {feature};
    mColorFeatures.initialize(fs);
    if(mColorFeatures.getSize() == 0)
    {
        return {};
    }
    auto* colorList = mDescriptor->getColorMap(mPluginHandle, index, mColorFeatures.getData());
    if(colorList == nullptr || colorList->colors == nullptr || colorList->colorCount == 0)
    {
        return {};
    }
    std::vector<PluginWrapper::Color> colorMap;
    colorMap.resize(static_cast<size_t>(colorList->colorCount));
    std::copy(colorList->colors, colorList->colors + colorList->colorCount, colorMap.begin());
    mDescriptor->releaseColorMap(colorList);
    return colorMap;
}

bool PluginWrapper::isVersionSupported(int major, int minor, int patch) const
{
    if(mDescriptor == nullptr || mPluginHandle == nullptr)
    {
        return false;
    }
    if(mDescriptor->api_version_major < major)
    {
        return false;
    }
    if(mDescriptor->api_version_major > major)
    {
        return true;
    }
    if(mDescriptor->api_version_minor < minor)
    {
        return false;
    }
    if(mDescriptor->api_version_minor > minor)
    {
        return true;
    }
    return mDescriptor->api_version_patch >= patch;
}

IVE_FILE_END
