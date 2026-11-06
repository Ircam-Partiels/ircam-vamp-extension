#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

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
    void release(VampParameterDescriptor* voiceDescriptor);
    VampParameterDescriptor* create(PluginExtension::VoiceDescriptor const& pluginVoiceDescriptor);
    Vamp::Plugin::FeatureSet convert(unsigned int numFeatures, VampFeatureList const* features);

    template <typename DerivedPlugin>
    IvePluginDescriptor const* getDescriptor()
    {
        static_assert(std::is_base_of_v<Vamp::Plugin, DerivedPlugin>, "DerivedPlugin must inherit from Vamp::Plugin");
        static_assert(std::is_base_of_v<PluginExtension, DerivedPlugin>, "DerivedPlugin must inherit from PluginExtension");

        static bool initialized = false;
        static std::string identifier;
        static IvePluginDescriptor descriptor;
        if(initialized)
        {
            return std::addressof(descriptor);
        }
        static auto const plugin = std::make_unique<DerivedPlugin>(48000.0f);
        if(plugin == nullptr)
        {
            return nullptr;
        }

        identifier = static_cast<Vamp::Plugin const*>(plugin.get())->getIdentifier();
        descriptor.identifier = identifier.c_str();
        descriptor.getInputCount = [](VampPluginHandle handle) -> unsigned int
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return 0u;
            }
            return static_cast<unsigned int>(plugin->getInputDescriptors().size());
        };

        descriptor.getInputDescriptor = [](VampPluginHandle handle, unsigned int index) -> VampInputDescriptor*
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

        descriptor.releaseInputDescriptor = [](VampInputDescriptor* inputDescriptor)
        {
            release(inputDescriptor);
        };

        descriptor.setPreComputingFeatures = [](VampPluginHandle handle, unsigned int numFeatures, VampFeatureList const* features)
        {
            auto* plugin = static_cast<PluginExtension*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return;
            }
            plugin->setPreComputingFeatures(convert(numFeatures, features));
        };

        descriptor.getOuputExtraCount = [](VampPluginHandle handle, unsigned int index) -> unsigned int
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return 0u;
            }
            return static_cast<unsigned int>(plugin->getOutputExtraDescriptors(static_cast<size_t>(index)).size());
        };

        descriptor.getOuputExtraDescriptor = [](VampPluginHandle handle, unsigned int index, unsigned int subindex) -> VampInputDescriptor*
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

        // Slots of the color map support removed in 0.0.5
        descriptor.reserved0 = []([[maybe_unused]] VampPluginHandle handle, [[maybe_unused]] int index) -> unsigned char
        {
            return 0u;
        };

        descriptor.reserved1 = []([[maybe_unused]] VampPluginHandle handle, [[maybe_unused]] int index, [[maybe_unused]] VampFeatureList const* features, [[maybe_unused]] float minThreshold, [[maybe_unused]] float maxThreshold) -> void*
        {
            return nullptr;
        };

        descriptor.reserved2 = []([[maybe_unused]] void* data)
        {
        };

        descriptor.getOutputVoiceCount = [](VampPluginHandle handle, unsigned int index) -> unsigned int
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return 1u;
            }
            return static_cast<unsigned int>(std::max(plugin->getOutputVoiceCount(static_cast<size_t>(index)), static_cast<size_t>(1)));
        };

        descriptor.getOutputVoiceDescriptor = [](VampPluginHandle handle, unsigned int index) -> VampParameterDescriptor*
        {
            auto const* plugin = static_cast<PluginExtension const*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr)
            {
                return nullptr;
            }
            auto const voiceDescriptor = plugin->getOutputVoiceDescriptor(static_cast<size_t>(index));
            if(!voiceDescriptor.has_value())
            {
                return nullptr;
            }
            return create(voiceDescriptor.value());
        };

        descriptor.releaseOutputVoiceDescriptor = [](VampParameterDescriptor* voiceDescriptor)
        {
            release(voiceDescriptor);
        };

        descriptor.setOutputVoiceCount = [](VampPluginHandle handle, char const* identifier, unsigned int count)
        {
            auto* plugin = static_cast<PluginExtension*>(reinterpret_cast<DerivedPlugin*>(handle));
            if(plugin == nullptr || identifier == nullptr)
            {
                return;
            }
            plugin->setOutputVoiceCount(std::string(identifier), static_cast<size_t>(count));
        };
        initialized = true;
        return std::addressof(descriptor);
    }
} // namespace PluginAdapter

IVE_FILE_END
