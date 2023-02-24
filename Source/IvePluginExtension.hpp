#pragma once

#include <cassert>

class PluginExtension
{
public:
    using InputDescriptor = Vamp::Plugin::OutputDescriptor;
    typedef std::vector<InputDescriptor> InputList;

    PluginExtension() noexcept = default;
    virtual ~PluginExtension() = default;

    //! @brief Gets the input descriptors of the plugin
    virtual InputList getInputDescriptors() const { return {}; }

    //! @brief Sets the pre-computing features of the plugin
    //! @details Before any blocks have been processed, use pre-computing features to prepare the process.
    virtual void setPreComputingFeatures([[maybe_unused]] Vamp::Plugin::FeatureSet const& fs)
    {
        assert(false);
    }
};
