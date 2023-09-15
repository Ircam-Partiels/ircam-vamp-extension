#pragma once

#include <cstring>
#include <memory>
#include <type_traits>
#include <vamp-sdk/Plugin.h>

#include "ive.h"

// clang-format off
#define IVE_FILE_BEGIN namespace Ive {
#define IVE_FILE_END }
// clang-format on

IVE_FILE_BEGIN

using namespace _VampPlugin;
#include "IvePluginExtension.hpp"

namespace PluginAdapter
{
    void release(VampInputDescriptor* inputDescriptor);
    VampInputDescriptor* create(PluginExtension::InputDescriptor const& pluginInputDescriptor);
    VampOutputExtraDescriptor* create(PluginExtension::OutputExtraDescriptor const& pluginOutputExtraDescriptor);
    Vamp::Plugin::FeatureSet convert(unsigned int numFeatures, VampFeatureList const* features);

    template <typename DerivedPlugin>
    IvePluginDescriptor* getDescriptor()
    {
        static_assert(std::is_base_of_v<Vamp::Plugin, DerivedPlugin>, "DerivedPlugin must inherit from Vamp::Plugin");
        static_assert(std::is_base_of_v<PluginExtension, DerivedPlugin>, "DerivedPlugin must inherit from PluginExtension");

        auto descriptor = std::make_unique<IvePluginDescriptor>();
        if(descriptor == nullptr)
        {
            return nullptr;
        }

        static auto const plugin = std::make_unique<DerivedPlugin>(48000.0f);
        if(plugin == nullptr)
        {
            return nullptr;
        }

        descriptor->identifier = strdup(static_cast<Vamp::Plugin const*>(plugin.get())->getIdentifier().c_str());
        descriptor->getInputCount = [](VampPluginHandle handle) -> unsigned int
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return 0u;
            }
            return static_cast<unsigned int>(plugin->getInputDescriptors().size());
        };

        descriptor->getInputDescriptor = [](VampPluginHandle handle, unsigned int index) -> VampInputDescriptor*
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return nullptr;
            }
            auto const inputDescriptors = plugin->getInputDescriptors();
            if(static_cast<size_t>(index) >= inputDescriptors.size())
            {
                return nullptr;
            }
            return create(inputDescriptors.at(static_cast<size_t>(index)));
        };

        descriptor->releaseInputDescriptor = [](VampInputDescriptor* inputDescriptor)
        {
            release(inputDescriptor);
        };

        descriptor->setPreComputingFeatures = [](VampPluginHandle handle, unsigned int numFeatures, VampFeatureList const* features)
        {
            auto* plugin = static_cast<PluginExtension*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return;
            }
            plugin->setPreComputingFeatures(convert(numFeatures, features));
        };

        descriptor->getOuputExtraCount = [](VampPluginHandle handle, unsigned int index) -> unsigned int
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return 0u;
            }
            return static_cast<unsigned int>(plugin->getOutputExtraDescriptors(static_cast<size_t>(index)).size());
        };

        descriptor->getOuputExtraDescriptor = [](VampPluginHandle handle, unsigned int index, unsigned int subindex) -> VampInputDescriptor*
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return nullptr;
            }
            auto const outputExtraDescriptors = plugin->getOutputExtraDescriptors(index);
            if(static_cast<size_t>(subindex) >= outputExtraDescriptors.size())
            {
                return nullptr;
            }
            return create(outputExtraDescriptors.at(static_cast<size_t>(subindex)));
        };

        return descriptor.release();
    }
} // namespace PluginAdapter

IVE_FILE_END
