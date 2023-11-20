#pragma once

#include <cassert>

class PluginExtension
{
public:
    PluginExtension() noexcept = default;
    virtual ~PluginExtension() = default;

    using InputDescriptor = Vamp::Plugin::OutputDescriptor;
    typedef std::vector<InputDescriptor> InputList;

    //! @brief Gets the input descriptors of the plugin
    virtual InputList getInputDescriptors() const { return {}; }

    //! @brief Sets the pre-computing features of the plugin
    //! @details Before any blocks have been processed, use pre-computing features to prepare the process.
    virtual void setPreComputingFeatures([[maybe_unused]] Vamp::Plugin::FeatureSet const& fs)
    {
        assert(false);
    }

    struct OutputExtraDescriptor
    {
        std::string identifier;
        std::string name;
        std::string description;
        std::string unit;
        bool hasKnownExtents{false};
        float minValue{0.0f};
        float maxValue{0.0f};
        bool isQuantized{false};
        float quantizeStep{0.0f};

        bool operator==(OutputExtraDescriptor const& rhs) const noexcept
        {
            return identifier == rhs.identifier &&
                   name == rhs.name &&
                   description == rhs.description &&
                   unit == rhs.unit &&
                   hasKnownExtents == rhs.hasKnownExtents &&
                   (!hasKnownExtents || std::abs(minValue - rhs.minValue) < std::numeric_limits<float>::epsilon()) &&
                   (!hasKnownExtents || std::abs(maxValue - rhs.maxValue) < std::numeric_limits<float>::epsilon()) &&
                   isQuantized == rhs.isQuantized &&
                   (!isQuantized || std::abs(quantizeStep - rhs.quantizeStep) < std::numeric_limits<float>::epsilon());
        }

        bool operator!=(OutputExtraDescriptor const& rhs) const noexcept
        {
            return !(*this == rhs);
        }
    };
    typedef std::vector<OutputExtraDescriptor> OutputExtraList;

    //! @brief Gets the input descriptors of the plugin
    virtual OutputExtraList getOutputExtraDescriptors([[maybe_unused]] size_t outputDescriptorIndex) const { return {}; }

    //! @brief Gets if the plugin supports color mapping
    //! @details The color mapping  might be only
    virtual bool supportColorMap([[maybe_unused]] int index) const { return false; }

    //! @brief A color
    //! @details The format is ((alpha << 24) | (red << 16) | (green << 8) | blue) but the alpha might be ignored
    using Color = std::uint32_t;

    //! @brief Gets color map for a feature
    virtual std::vector<Color> getColorMap([[maybe_unused]] int index, [[maybe_unused]] Vamp::Plugin::Feature const& feature)
    {
        assert(false);
        return {};
    }
};
