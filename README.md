# Caravan Audio Plugin

Caravan is a vocal processing audio plugin built using the JUCE framework. It features a unique "Dust Drive" saturation algorithm, vocal EQ with tune mode, stereo width enhancement, and preset management capabilities, all wrapped in a custom desert-themed UI.

## Project Architecture

The project is organized into several core modules:

### Core Components

- **CaravanProcessor**: The main audio processing class that handles parameter management, audio processing, and orchestrates the different DSP modules. It inherits from JUCE's AudioProcessor class and manages parameter connections through AudioProcessorValueTreeState.

- **AudioParameters**: Defines the plugin parameters and their ranges. Creates the parameter layout used by the processor and provides utility functions to map normalized parameter values (0-1) to appropriate ranges for DSP processing.

### Effects Chain

The audio processing chain consists of the following components in order:

1. **Input Gain**: Initial gain staging
2. **VocalEQ**: EQ processing with tune mode option
3. **DustDriveSaturator**: The main saturation effect
4. **StereoWidthProcessor**: Stereo image enhancement
5. **Output Gain**: Final gain staging

#### DustDriveSaturator

The signature effect of the plugin, providing asymmetric saturation tailored for vocal processing. It uses different saturation curves for positive and negative signal peaks, with multi-stage processing for higher drive settings.

#### VocalEQ

Handles frequency-specific processing with two modes:

- **Tune Mode**: Optimized for trap/hip-hop vocals with AutoTune, featuring higher low-cut, presence boost, and de-essing
- **Standard Mode**: More transparent EQ with gentler settings

#### StereoWidthProcessor

Enhances stereo imaging using mid-side processing with subtle saturation applied to the side signal for a more cohesive stereo image.

### Preset Management

The PresetManager handles factory presets with desert-themed names like "Oasis," "Sandstorm," and "Mirage." Each preset stores unique settings for dust drive, tune mode, width, air, and de-essing parameters.

### User Interface

The UI is built around a custom desert theme with a main "Dust Drive" knob as the central component. The editor includes:

- Main compass-styled Dust Drive control
- Tune Mode button
- Secondary controls for Width, Air, and De-Esser
- Preset carousel for switching between factory presets

The CaravanLookAndFeel class provides custom styling for UI elements with a desert color palette of sand, dune, sky, and sun colors. It includes custom rendering for rotary sliders, toggle buttons, combo boxes, and labels.

## Project Structure

```txt
Caravan/
├── CMakeLists.txt           # Root CMake configuration
├── cmake/
│   └── cpm.cmake            # CPM package manager integration
├── libs/
│   └── juce/                # JUCE library (downloaded by CPM)
└── plugin/
    ├── CMakeLists.txt       # Plugin-specific CMake configuration
    └── caravan/
        ├── source/
        │   ├── core/
        │   │   ├── AudioParameters.cpp/h
        │   │   └── CaravanProcessor.cpp/h
        │   ├── effects/
        │   │   ├── DustDriveSaturator.cpp/h
        │   │   ├── StereoWidthProcessor.cpp/h
        │   │   └── VocalEQ.cpp/h
        │   ├── presets/
        │   │   └── PresetManager.cpp/h
        │   └── ui/
        │       ├── components/
        │       │   ├── CaravanLookAndFeel.cpp/h
        │       │   └── CaravanFontManager.h
        │       ├── editor/
        │       │   └── CaravanEditor.cpp/h
        │       └── opengl/
        │           └── CaravanOpenGLContext.h
        └── resources/
            ├── images/
            │   ├── background.png
            │   └── compass.png
            └── fonts/
                └── changa.ttf
```

## Setup and Building

### Prerequisites

- CMake 3.30 or higher
- C++23 compatible compiler
- Git (for downloading JUCE via CPM)

### Building the Project

1. **Clone the repository**

   ```bash
   git clone https://github.com/yourusername/Caravan.git
   cd Caravan
   ```

2. **Create a build directory**

   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**

   ```bash
   cmake ..
   ```

4. **Build the project**

   ```bash
   cmake --build .
   ```

On Windows with Visual Studio, you may want to specify the build configuration:

   ```bash
   cmake --build . --config Release
   ```

### Plugin Output Location

After building, the VST3 plugin will be located in:

- **Windows**: `build/plugin/Caravan_artefacts/VST3/Caravan.vst3`
- **macOS**: `build/plugin/Caravan_artefacts/VST3/Caravan.vst3`
- **Linux**: `build/plugin/Caravan_artefacts/VST3/Caravan.vst3`

## Development Notes

### JUCE Integration

- The project uses CPM (CMake Package Manager) to download and integrate JUCE version 8.0.6
- OpenGL is enabled for potential UI enhancements (though currently minimally implemented)
- Warning levels are set high, with platform-specific compiler options:
  - MSVC: `/W4` and `_CRT_SECURE_NO_WARNINGS` definition
  - GCC/Clang: `-Wall -Wextra -Wpedantic`

### Plugin Configuration

The plugin is configured as a VST3 effect with:

- Manufacturer: KradKradLabs
- Manufacturer code: KKLS
- Plugin code: CRVN
- Product name: Caravan
- Version: 0.1.0

### Adding New Parameters

To add new parameters to the plugin:

1. Modify `AudioParameters.cpp` to add the parameter to the parameter layout.
2. Add corresponding getter methods if needed.
3. Add the parameter pointer in `CaravanProcessor.h`.
4. Initialize the pointer in the `CaravanProcessor` constructor.
5. Update the `updateProcessorSettings()` method to handle the new parameter.
6. Add UI controls in the `CaravanEditor` class.

### Creating New Presets

To add new presets:

1. Modify the `initializePresets()` method in `PresetManager.cpp`.
2. Add new entries to the `desertPresets` vector with appropriate names and parameter values.
3. Ensure the preset carousel in `CaravanEditor` is updated if needed to reflect the new total number of presets.

## Future Development Considerations

- Expand the OpenGL integration for more dynamic visualizations
- Add A/B comparison functionality
- Implement user preset saving/loading
- Add more processing algorithms (compressor, reverb, etc.)
- Expand available plugin formats (AU, AAX)

---

Caravan is developed by KradKradLabs and is currently in alpha (v1.0.1)
