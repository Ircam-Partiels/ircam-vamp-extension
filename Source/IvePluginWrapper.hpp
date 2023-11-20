#pragma once

#include <memory>
#include <type_traits>
#include <vamp-hostsdk/Plugin.h>
#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginLoader.h>
#include <vamp-hostsdk/PluginWrapper.h>

#include "ive.h"

// clang-format off
#define IVE_FILE_BEGIN namespace Ive {
#define IVE_FILE_END }
// clang-format on

IVE_FILE_BEGIN

#include "IvePluginExtension.hpp"

class PluginWrapper
: public Vamp::HostExt::PluginWrapper
, public PluginExtension
{
public:
    PluginWrapper(Vamp::Plugin* plugin, std::string const& key);
    ~PluginWrapper() override;

    // PluginExtension
    InputList getInputDescriptors() const override;
    void setPreComputingFeatures(Vamp::Plugin::FeatureSet const& fs) override;

    OutputExtraList getOutputExtraDescriptors(size_t outputDescriptorIndex) const override;

    bool supportColorMap(int index) const override;
    std::vector<Color> getColorMap(int index, Vamp::Plugin::Feature const& feature) override;

private:
    bool isVersionSupported(int major, int minor, int patch) const;

    class FeatureContainer
    {
    public:
        FeatureContainer() = default;
        ~FeatureContainer() = default;

        void initialize(Vamp::Plugin::FeatureSet const& fs);
        unsigned int getSize() const;
        VampFeatureList const* getData() const;

    private:
        std::vector<VampFeatureList> mFeatureList;
        std::vector<std::vector<VampFeatureUnion>> mFeatures;
        std::vector<std::vector<std::tuple<std::string, std::vector<float>>>> mData;
    };

    VampPluginHandle mPluginHandle{nullptr};
    void* mLibraryHandle{nullptr};
    IvePluginDescriptor const* mDescriptor{nullptr};
    FeatureContainer mComputingFeatures;
    FeatureContainer mColorFeatures;
};

IVE_FILE_END
