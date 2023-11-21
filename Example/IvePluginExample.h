#pragma once

#include <IvePluginAdapter.hpp>

namespace IvePluginExample
{
    class CopyMarker
    : public Vamp::Plugin
    , public Ive::PluginExtension
    {
    public:
        CopyMarker(float sampleRate);
        ~CopyMarker() override = default;

        bool initialise(size_t channels, size_t stepSize, size_t blockSize) override;

        std::string getMaker() const override;
        int getPluginVersion() const override;
        std::string getCopyright() const override;

        InputDomain getInputDomain() const override;
        std::string getIdentifier() const override;
        std::string getName() const override;
        std::string getDescription() const override;

        size_t getPreferredBlockSize() const override;
        OutputList getOutputDescriptors() const override;

        void reset() override;
        FeatureSet process(const float* const* inputBuffers, Vamp::RealTime timestamp) override;
        FeatureSet getRemainingFeatures() override;

        // Ive::PluginExtension
        InputList getInputDescriptors() const override;
        void setPreComputingFeatures(FeatureSet const& fs) override;

    private:
        FeatureSet mFeatureSet;
    };

    class RandomPoint
    : public Vamp::Plugin
    , public Ive::PluginExtension
    {
    public:
        RandomPoint(float sampleRate);
        ~RandomPoint() override = default;

        bool initialise(size_t channels, size_t stepSize, size_t blockSize) override;

        std::string getMaker() const override;
        int getPluginVersion() const override;
        std::string getCopyright() const override;

        InputDomain getInputDomain() const override;
        std::string getIdentifier() const override;
        std::string getName() const override;
        std::string getDescription() const override;

        size_t getPreferredBlockSize() const override;
        OutputList getOutputDescriptors() const override;

        // Ive::PluginExtension
        OutputExtraList getOutputExtraDescriptors(size_t outputDescriptorIndex) const override;

        void reset() override;
        FeatureSet process(const float* const* inputBuffers, Vamp::RealTime timestamp) override;
        FeatureSet getRemainingFeatures() override;
    };

    class RainbowMatrix
    : public Vamp::Plugin
    , public Ive::PluginExtension
    {
    public:
        RainbowMatrix(float sampleRate);
        ~RainbowMatrix() override = default;

        bool initialise(size_t channels, size_t stepSize, size_t blockSize) override;

        std::string getMaker() const override;
        int getPluginVersion() const override;
        std::string getCopyright() const override;

        InputDomain getInputDomain() const override;
        std::string getIdentifier() const override;
        std::string getName() const override;
        std::string getDescription() const override;

        size_t getPreferredBlockSize() const override;
        OutputList getOutputDescriptors() const override;

        void reset() override;
        FeatureSet process(const float* const* inputBuffers, Vamp::RealTime timestamp) override;
        FeatureSet getRemainingFeatures() override;

        // Ive::PluginExtension
        bool supportColorMap(int index) const override;
        std::vector<Color> getColorMap(int index, Vamp::Plugin::Feature const& feature) override;

    private:
        FeatureSet mFeatureSet;
        size_t mPosition = 0;
        static size_t constexpr gSize = 24;
    };
} // namespace IvePluginExample
