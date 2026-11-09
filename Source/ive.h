#pragma once

// The version of the API advertised by the plugins in their descriptors (the host only uses the functions supported by
// this version). It must be incremented when functions are added to the descriptor, and it is the version of the
// project used by CMake.
#define IVE_VERSION_MAJOR 0
#define IVE_VERSION_MINOR 0
#define IVE_VERSION_PATCH 7

// clang-format off
#ifdef __cplusplus
#define IVE_EXTERN_CPP_BEGIN extern "C" {
#define IVE_EXTERN_CPP_END }
#else
#define IVE_EXTERN_CPP_BEGIN
#define IVE_EXTERN_CPP_END
#endif
// clang-format on

#ifdef _WIN32
#define IVE_EXTERN_STRUCT extern
#ifdef IVE_LIB_EXPORT
#define IVE_EXTERN __declspec(dllexport) extern
#elif IVE_LIB_IMPORT
#define IVE_EXTERN __declspec(dllimport) extern
#else
#define IVE_EXTERN
#endif
#else // _WIN32
#define IVE_EXTERN_STRUCT
#ifdef IVE_LIB_EXPORT
#define IVE_EXTERN extern
#elif IVE_LIB_IMPORT
#define IVE_EXTERN extern
#else
#define IVE_EXTERN
#endif
#endif

IVE_EXTERN_CPP_BEGIN

#include <stdint.h>
#include <vamp/vamp.h>

typedef VampOutputDescriptor VampInputDescriptor;
typedef VampOutputDescriptor VampOutputExtraDescriptor;

typedef struct _IvePluginDescriptor
{
    int const api_version_major = IVE_VERSION_MAJOR;
    int const api_version_minor = IVE_VERSION_MINOR;
    int const api_version_patch = IVE_VERSION_PATCH;
    char const* identifier;

    // 0.0.1
    unsigned int (*getInputCount)(VampPluginHandle handle);
    VampInputDescriptor* (*getInputDescriptor)(VampPluginHandle handle, unsigned int index);
    void (*releaseInputDescriptor)(VampInputDescriptor* inputDescriptor);

    void (*setPreComputingFeatures)(VampPluginHandle handle, unsigned int numFeatures, VampFeatureList const* features);

    // 0.0.2
    unsigned int (*getOuputExtraCount)(VampPluginHandle handle, unsigned int index);
    VampOutputExtraDescriptor* (*getOuputExtraDescriptor)(VampPluginHandle handle, unsigned int index, unsigned int subindex);

    // 0.0.3 & 0.0.4 (color map support, removed in 0.0.5)
    // These slots preserve the descriptor layout for the hosts that still use them.
    // Plugins must set them to functions that disable the feature and hosts must not use them.
    unsigned char (*reserved0)(VampPluginHandle handle, int index);
    void* (*reserved1)(VampPluginHandle handle, int index, VampFeatureList const* features, float minThreshold, float maxThreshold);
    void (*reserved2)(void* data);

    // 0.0.6
    // The voices of an output are consecutive outputs, starting at the output index, that share the same nature (e.g.
    // the pitches of a polyphonic analysis). The features of the voices are the features lists of the outputs in the
    // feature set. The number of voices is controlled by the host (like a parameter but independently of the parameters,
    // the programs and the presets) and the output descriptors depend on it. The identifier of the first voice must not
    // depend on the number of voices.
    // Gets the current number of voices of the output (1 for a conventional output).
    unsigned int (*getOutputVoiceCount)(VampPluginHandle handle, unsigned int index);
    // Gets the descriptor of the number of voices of the output (name, range, default value...) or NULL if the output
    // has no voices. The value names of the descriptor are not used.
    VampParameterDescriptor* (*getOutputVoiceDescriptor)(VampPluginHandle handle, unsigned int index);
    void (*releaseOutputVoiceDescriptor)(VampParameterDescriptor* descriptor);
    // Sets the number of voices of the output whose first voice has the identifier. The number of voices must be set
    // before retrieving the output descriptors from the instance of the plugin (because the Vamp plugin adapter caches
    // them until a parameter or a program changes) and before the initialisation of the plugin.
    void (*setOutputVoiceCount)(VampPluginHandle handle, char const* identifier, unsigned int count);

    // 0.0.7
    // An alias parameter is a parameter that defines and represents other parameters, the real parameters (e.g. a
    // window size in samples that is an alias of a window duration in milliseconds). Its value only depends on the
    // values of the real parameters (and on the sample rate), so the host neither saves nor compares it: it sets the
    // real parameters to restore the state of the plugin, and it retrieves the values of the real parameters after
    // setting an alias parameter. An alias parameter must not be the alias of another alias parameter. Returns 1 if
    // the parameter is an alias parameter, otherwise 0.
    int (*isAliasParameter)(VampPluginHandle handle, char const* identifier);
} IvePluginDescriptor;

IVE_EXTERN IvePluginDescriptor const* iveGetPluginDescriptor(unsigned int hostApiVersion, unsigned int index);
typedef IvePluginDescriptor const* (*IvePluginDescriptorFunction)(unsigned int, unsigned int);

IVE_EXTERN_CPP_END
