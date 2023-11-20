#pragma once

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

typedef struct _IveColorList
{
    unsigned int colorCount;
    uint32_t* colors;
} IveColorList;

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

    // 0.0.3
    unsigned char (*supportColorMap)(VampPluginHandle handle, int index);
    IveColorList* (*getColorMap)(VampPluginHandle, int index, VampFeatureList const* features);
    void (*releaseColorMap)(IveColorList* map);
} IvePluginDescriptor;

IVE_EXTERN IvePluginDescriptor const* iveGetPluginDescriptor(unsigned int hostApiVersion, unsigned int index);
typedef IvePluginDescriptor const* (*IvePluginDescriptorFunction)(unsigned int, unsigned int);

IVE_EXTERN_CPP_END
