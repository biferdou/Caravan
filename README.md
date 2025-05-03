# Caravan

![Caravan Plugin](plugin/caravan/resources/images/background.png)

Caravan is a specialized vocal processing VST3 plugin designed to enhance vocals with distinctive character and presence. This plugin features custom saturation, EQ optimization for trap/hip-hop vocals, stereo enhancement, and more - all wrapped in a desert-themed UI experience.

## Features

- **Dust Drive Saturation**: Add warmth and character with custom asymmetric saturation algorithm
- **Tune Mode**: Optimized EQ settings to improve AutoTune tracking
- **Stereo Width Enhancement**: Create spacious vocal mixes with natural-sounding width
- **Air/Presence Control**: Add clarity and brilliance to your vocals
- **De-Esser**: Smooth out harsh sibilance while maintaining vocal clarity
- **8 Desert-Themed Presets**: From subtle enhancement to dramatic effects

## Screenshots

*[Screenshots will be added in the future]*

## Installation

### Pre-built Binaries

Download the latest release for your platform from the [Releases page](../../releases).

#### Windows

- Run the installer (.exe) and follow the on-screen instructions
- Restart your DAW to scan for the new VST3 plugin

#### macOS

- Open the disk image (.dmg) and drag Caravan.vst3 to your VST3 folder (/Library/Audio/Plug-Ins/VST3/)
- Restart your DAW to scan for the new VST3 plugin

## Building from Source

### Prerequisites

- CMake 3.30 or higher
- C++23 compatible compiler
- JUCE 8.0.6 (automatically downloaded via CPM)
- Platform-specific dependencies:
  - **Windows**: Visual Studio 2022, NSIS (for creating installers)
  - **macOS**: Xcode, Ninja build system
  - **Linux**: Build essentials, X11 development packages, ALSA, and JACK

### Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/Caravan.git
   cd Caravan
   ```

2. Create a build directory:

   ```bash
   cmake -E make_directory build
   cd build
   ```

3. Configure the project:

   ```bash
   # Windows
   cmake .. -DCMAKE_BUILD_TYPE=Release

   # macOS/Linux
   cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
   ```

4. Build the project:

   ```bash
   cmake --build . --config Release
   ```

5. Create the installer package:

   ```bash
   cpack -C Release
   ```

## Development

The project is structured as follows:

- `libs/`: External libraries (JUCE will be downloaded here)
- `plugin/`: The VST3 plugin source code
  - `caravan/source/`: Main plugin source code
    - `core/`: Core audio processing
    - `effects/`: Audio effect implementations
    - `presets/`: Preset management
    - `ui/`: User interface components
  - `caravan/resources/`: Images, fonts, and other resources

## Presets

Caravan comes with 8 desert-themed presets:

- **Oasis**: Moderate saturation, strong presence, subtle width
- **Sandstorm**: Aggressive saturation, wide stereo image, less de-essing
- **Mirage**: Medium saturation, controlled width, balanced air
- **Dunes**: Heavy saturation, moderate width, emphasized air
- **Sahara**: Medium saturation, widest stereo, spacious air
- **Sunset**: Clean saturation, balanced width, strong air and presence
- **Pristine**: Minimal saturation, natural width, gentle presence enhancement
- **Nomad**: No saturation, minimal processing, subtle enhancement

## System Requirements

- Windows 10 or later (64-bit)
- macOS 11.0 (Big Sur) or later
- VST3-compatible DAW (Ableton Live, Logic Pro, FL Studio, etc.)

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details.

## Acknowledgments

- JUCE Framework - [https://juce.com/](https://juce.com/)
- Changa font - [https://fonts.google.com/specimen/Changa](https://fonts.google.com/specimen/Changa)

## Contact

KradKradLabs - <void@kradkrad.com>
