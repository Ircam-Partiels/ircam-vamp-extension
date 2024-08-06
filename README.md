# Ircam Vamp Extension
**An extension to the Vamp analysis plug-in format.**

The Ircam Vamp Extension (IVE) provides an interface for 
- receiving features before analysis, 
- defining additional features on output,
- returning color maps based on the results. 

These features are implemented in the CopyMarker, RandomPoint and RainbowMatrix example plug-ins respectively.

## Compilation

The compilation system is based on [CMake](https://cmake.org/). The Vamp Plugin SDK is required and a patch must be applied to the sources. By default, the Vamp Plugin SDK is fetched from the [official git repository](https://github.com/vamp-plugins/vamp-plugin-sdk) and the patch is apply automatically. You can manually specify the path to the Vamp Plugin SDK with the `VAMP_SDK_PATH` cmake variable and manually apply the git patch *Patches/Add-IVE-support.patch*. Example plugins can be generated using the cmake variable `IVE_PLUGIN_EXAMPLE`. 

```
add_subdirectory(ircam-vamp-extension)
...
add_library(MyPlugin SHARED ...)
ive_prepare_plugin_target(MyPlugin) 
```

## Credits

Ircam Vamp Extension is designed and developed by Pierre Guillot at [IRCAM IMR department](https://www.ircam.fr/).  
Vamp and the Vamp SDK were designed and made at the Centre for Digital Music at Queen Mary, University of London.  
The Vamp SDK was written by Chris Cannam, copyright (c) 2005-2024 Chris Cannam and QMUL.

