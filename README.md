# DM-2 Delay VST3 Plugin

A vintage-style cascaded BBD (Bucket Brigade Device) delay emulation plugin inspired by the Boss DM-2 and Boss CE-1 pedals, featuring dual-stage delay processing with independent control.

## Features

- **MN3005 BBD Chip Emulation**: Accurate simulation of the classic bucket brigade delay algorithm with 4096-stage delay line
- **Cascaded Dual-Stage Architecture**: Two independent delay stages that can be connected in series (King of Tone style)
- **Two Operation Modes**:
  - **Standard Mode**: Single stage delay with classic characteristics
  - **Custom Mode**: Dual-stage cascaded delay with independent parameters for each stage
- **Vintage Pedal UI**: Authentic retro visualization with two side-by-side pedal design in dual-stage mode
- **VST3 & Standalone Formats**: Use as a plugin or standalone application
- **Real-time Parameter Control**: Dynamic pedal visualization responding to parameter changes

## System Requirements

- **Windows 10/11** (64-bit)
- **Visual Studio 2022** Build Tools (or later) with C++17 support
- **CMake 4.2.2+**
- **JUCE Framework 7.x**

## Building from Source

### Prerequisites

1. Install **CMake** (version 4.2.2 or later)
   - Download from: https://cmake.org/download/

2. Install **Visual Studio 2022 Build Tools**
   - Include C++ development tools
   - Windows SDK 10.0.26100.0 (or compatible)

3. Download **JUCE Framework**
   - Clone or download from: https://github.com/juce-framework/JUCE
   - Place in your project directory or update CMakeLists.txt path

### Build Steps

```bash
# Clone the repository
git clone https://github.com/JackieAlacrity2373/Right_Round.git
cd Right_Round

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build Debug version
cmake --build . --config Debug

# Build Release version (optional)
cmake --build . --config Release
```

### Output Locations

- **VST3 Plugin**: `DM2Delay/build/DM2Delay_artefacts/Debug/VST3/DM-2 Delay.vst3`
- **Standalone App**: `DM2Delay/build/DM2Delay_artefacts/Debug/Standalone/DM-2 Delay.exe`

## Installation

### VST3 Plugin

Copy the `.vst3` folder to your DAW's VST3 plugin directory:

- **Windows**: `C:\Program Files\Common Files\VST3\`
- **Reaper**: Check your VST3 path in preferences
- **FL Studio**: `C:\Program Files\Image-Line\FL Studio 21\Plugins\VST3\`

Rescan plugins in your DAW.

### Standalone

Run the `.exe` directly or create a shortcut on your desktop.

## Usage

### Parameters

#### Stage 1 (Always Available)

- **Delay Time**: 10ms - 600ms (delay duration)
- **Feedback**: 0% - 100% (amount of delayed signal fed back)
- **Mix**: 0% - 100% (dry/wet balance)
- **Tone**: 0% - 100% (high-frequency absorption)
- **Modulation**: 0% - 100% (BBD clock variation)

#### Mode Button

- **Standard**: Single-stage delay only
- **Custom**: Enables Stage 2 with independent controls

#### Stage 2 (Custom Mode Only)

When Custom mode is active, Stage 2 controls become available:

- **Stage 2 Delay Time**: Independent delay for cascaded processing
- **Stage 2 Feedback**: Separate feedback control
- **Stage 2 Mix**: Output level of second stage
- **Stage 2 Tone**: Independent tone shaping
- **Stage 2 Modulation**: Separate modulation control

### Cascaded Processing

In Custom mode, the signal flow is:

```
Input → Stage 1 → Stage 2 → Output
```

The output of Stage 1 feeds into Stage 2, creating deep, lush delay textures when both stages are active.

## Project Structure

```
Right_Round/
├── DM2Delay/
│   ├── Source/
│   │   ├── DSP/
│   │   │   ├── BBDModel.h/cpp          # MN3005 emulation
│   │   │   ├── Compander.h/cpp         # Companding circuit
│   │   │   ├── DelayLine.h/cpp         # 4096-stage delay line
│   │   │   ├── Filter.h/cpp            # Low-pass filter
│   │   │   └── MixStage.h/cpp          # Mix/output stage
│   │   ├── Parameters.h                # All plugin parameters
│   │   ├── PluginProcessor.h/cpp       # Audio engine
│   │   └── PluginEditor.h/cpp          # UI & visualization
│   └── CMakeLists.txt                  # Build configuration
├── .gitignore                          # Excludes build/ and large files
└── README.md                           # This file
```

## Technical Details

### BBD Emulation

The plugin emulates the MN3005 bucket brigade device using:

- Companding (compression/expansion) for dynamic range preservation
- Clock-rate modulation for characteristic BBD artifacts
- Multi-stage delay line with proper signal flow
- Frequency-dependent loss modeling

### DSP Chain

1. **Input** → Companding → Delay Line 1
2. **Delay Line 1** → Mix/Tone → (Optional) Delay Line 2
3. **Delay Line 2** → Output

## Development

### Adding Features

1. Add new parameters to `Parameters.h`
2. Implement DSP in appropriate `DSP/*.cpp` file
3. Update `PluginProcessor.cpp` to use new parameters
4. Add UI controls to `PluginEditor.cpp`
5. Rebuild and test

### Building Release Version

```bash
cd build
cmake --build . --config Release
```

The Release build will be optimized for performance.

## Troubleshooting

### Plugin Not Appearing in DAW

- Ensure VST3 folder is in the correct location
- Rescan plugins (most DAWs have this in settings)
- Check Windows Event Viewer for load errors
- Verify 64-bit architecture matches DAW

### Build Errors

- Ensure CMake version is 4.2.2+
- Verify Visual Studio 2022 Build Tools is installed
- Check JUCE path in CMakeLists.txt
- Delete `build/` folder and reconfigure

### Audio Issues

- Check I/O level meters in plugin
- Adjust feedback carefully (can lead to instability)
- Use Mix parameter to blend with dry signal
- Verify DAW sample rate (44.1kHz, 48kHz recommended)

## License

This project uses the JUCE Framework, which is available under dual licensing. See JUCE documentation for details.

## Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Commit changes with clear messages
4. Submit a pull request

## Credits

- Inspired by Boss DM-2 and Boss CE-1 delay pedals
- Built with JUCE Framework
- MN3005 BBD emulation based on classic analog circuit design

## Support

For issues or questions:

- Check existing GitHub issues
- Review the Technical Details section above
- Inspect build output for error messages
- Enable verbose logging during development

---

**Happy Delaying!** 🎸
