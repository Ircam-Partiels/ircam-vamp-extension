#pragma once

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

    using VoiceDescriptor = Vamp::PluginBase::ParameterDescriptor;

    //! @brief Gets the descriptor of the number of voices of an output
    //! @details The voices of an output are the consecutive outputs, starting at the output index, that share the
    //! same nature (e.g. the pitches of a polyphonic analysis or the partials of a sinusoidal analysis). The features
    //! of each voice are returned in the feature list of the corresponding output in the feature set. The number of
    //! voices is controlled by the host independently of the parameters (and the programs) and the output descriptors
    //! depend on it. The identifier of the first voice must not depend on the number of voices. The descriptor defines
    //! the name, the description, the range and the default value of the number of voices (the value names are not
    //! used).
    //! @param outputDescriptorIndex The index of the first output of the voices
    //! @return The descriptor of the number of voices or an empty optional if the output has no voices
    virtual std::optional<VoiceDescriptor> getOutputVoiceDescriptor([[maybe_unused]] size_t outputDescriptorIndex) const { return {}; }

    //! @brief Gets the number of voices of an output
    //! @param outputDescriptorIndex The index of the first output of the voices
    //! @return The number of voices (1 for a conventional output)
    virtual size_t getOutputVoiceCount([[maybe_unused]] size_t outputDescriptorIndex) const { return 1; }

    //! @brief Sets the number of voices of an output
    //! @details The host sets the number of voices before retrieving the output descriptors from the instance of the
    //! plugin (because the Vamp plugin adapter caches them until a parameter or a program changes) and before the
    //! initialisation of the plugin. The outputs that follow the voices are moved according to the number of voices.
    //! @param identifier The identifier of the first output of the voices
    //! @param count The number of voices (in the range of the descriptor)
    virtual void setOutputVoiceCount([[maybe_unused]] std::string const& identifier, [[maybe_unused]] size_t count) {}

    //! @brief Gets if a parameter is an alias parameter
    //! @details An alias parameter defines and represents other parameters, the real parameters (e.g. a window size
    //! in samples that is an alias of a window duration in milliseconds). Its value returned by getParameter() only
    //! depends on the values of the real parameters (and on the sample rate), so the host neither saves nor compares
    //! it: the host restores the state of the plugin by setting the real parameters, and it retrieves the values of
    //! the real parameters with getParameter() after setting an alias parameter with setParameter(). An alias
    //! parameter must not be the alias of another alias parameter.
    //! @param identifier The identifier of the parameter
    //! @return True if the parameter is an alias parameter
    virtual bool isAliasParameter([[maybe_unused]] std::string const& identifier) const { return false; }

    //! @brief Sets the window applied by the host to each block of audio before the FFT
    //! @details For a plugin that expects frequency-domain input, the window contains one coefficient per sample of
    //! the block, so the plugin can know the exact scaling of the spectrum (e.g. the sum of the coefficients is the gain
    //! of the window for a sinusoid). The host sets the window after the initialisation of the plugin and before the
    //! first call to process(), and sets it again if the window changes. If the host doesn't set the window, the plugin
    //! must assume a default window.
    //! @param window The coefficients of the window
    virtual void setFrequencyDomainWindow([[maybe_unused]] std::vector<float> const& window) {}
};
