# Caravan

![Caravan](plugin/caravan/resources/images/background.png)

Caravan is a vocal processing audio plugin designed for modern productions, combining gentle saturation, vocal EQ, stereo enhancement, and presence control in an intuitive desert-themed interface.

## Features

- **Dust Drive**: A custom saturation algorithm designed specifically for vocal processing, providing warm harmonic distortion that preserves clarity
- **Tune Mode**: Optimized EQ curve for vocals that will be processed with Auto-Tune or pitch correction
- **Width Control**: Mid-side stereo width enhancement with intelligent frequency-dependent processing
- **Air**: Presence and high-frequency enhancement for vocal clarity and brightness
- **De-Esser**: Smooth sibilance reduction for natural-sounding vocals
- **Preset System**: 8 carefully crafted presets designed for different vocal styles

## Requirements

- Windows 10/11 or macOS 10.13+
- VST3-compatible DAW
- C++23-compatible compiler for building from source

## Building from Source

Caravan uses CMake as its build system:

```bash
# Clone the repository
git clone https://github.com/KradKradLabs/Caravan.git
cd Caravan

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

### Dependencies

- JUCE 8.0.6 (automatically fetched via CPM)
- OpenGL (for UI rendering)

## Parameter Guide

### Dust Drive

The main processing control that adds saturation and harmonic character to vocals. Inspired by classic analog equipment with modern sensibilities.

- Low settings (0-0.3): Subtle warmth and gentle saturation
- Medium settings (0.3-0.7): Balanced harmonics and color
- High settings (0.7-1.0): Aggressive saturation with multi-stage processing

### Tune Mode

When enabled, applies an optimized EQ curve that helps vocals sit better with Auto-Tune or other pitch correction:

- Removes low-frequency content that can cause pitch correction artifacts
- Enhances mid-range clarity for better pitch tracking
- Smooths sibilance for more natural-sounding pitch correction

### Width

Controls the stereo width enhancement using advanced mid-side processing techniques:

- At 0.0: No width enhancement
- At 0.5: Balanced stereo enhancement
- At 1.0: Maximum stereo width

### Air

Adds presence and high-frequency enhancement for vocal clarity:

- At 0.0: No air enhancement
- At 0.5: Moderate air and presence
- At 1.0: Maximum brightness and air

### De-Esser

Controls the amount of sibilance reduction:

- At 0.0: No de-essing
- At 0.5: Moderate sibilance reduction
- At 1.0: Maximum de-essing

## Presets

Caravan includes 8 carefully crafted presets:

1. **Oasis**: Characterized by moderate saturation, strong presence, and subtle width
2. **Sandstorm**: More aggressive saturation, wide stereo image, less de-essing
3. **Mirage**: Medium saturation, controlled width, balanced air
4. **Dunes**: Heavy saturation, moderate width, emphasized air
5. **Sahara**: Medium saturation, widest stereo, spacious air
6. **Sunset**: Cleanest saturation, balanced width, strong air and presence
7. **Pristine**: Minimal saturation, natural width, gentle presence enhancement
8. **Nomad**: No saturation, minimal processing, just subtle enhancement

## UI Overview

The Caravan interface features a desert-themed design with:

- A large central "Dust Drive" knob that controls the primary saturation effect
- Secondary controls for Width, Air, and De-Esser
- A toggle switch for Tune Mode
- A preset carousel to quickly access different vocal processing flavors

## License

© 2025 KradKradLabs. All rights reserved.

## Credits

- Developed by KradKradLabs
- Uses the JUCE framework for audio processing and UI
- Special thanks to the modern vocal production community for inspiration and testing

## Future Roadmap

- AAX and AU plugin format support
- Additional vocal processing modules
- MIDI control mapping
- A/B comparison feature
- Expanded preset library
