# DM‑2 Delay VST Design Document

## Overview

A JUCE‑based VST3/AU plugin emulating the Boss DM‑2 analog delay pedal. The design captures the perceptual characteristics of the MN3005 BBD chip and compander circuitry within human hearing limits, while allowing modern flexibility. Target is faithful "feel" without strict circuit‑level exactness.

---

## Signal Architecture

```
Input → Input Stage → Compressor → BBD Delay → Expander → Filter → Mix → Output
```

### 1. Input Stage
- High‑impedance buffer emulation
- Gentle saturation model (op‑amp clipping characteristics)

### 2. Compressor (Pre‑BBD)
- 2:1 compression ratio
- Fast attack (~1 ms), medium release (~50 ms)
- Boosts quiet signals, attenuates loud peaks
- Purpose: maximize BBD headroom, reduce perceived noise floor

### 3. BBD Delay (MN3005 Model)
- **4096 stages** (digital delay line)
- **Clock‑rate control**: maps delay time to sample‑rate decimation
  - Original range: ~20–205 ms (10–100 kHz clock equivalent)
  - Delay time = stages / (2 × clock frequency)
- **BBD artifacts**:
  - Bandwidth limiting: 3–5 kHz low‑pass (pre‑aliasing)
  - Clock bleed: subtle high‑frequency modulation
  - Sample‑and‑hold character: staircase waveform reconstruction
  - Gentle noise floor increase with longer delay times

### 4. Expander (Post‑BBD)
- Inverse of compressor (2:1 expansion)
- Restores dynamics, suppresses BBD noise by ~10–15 dB

### 5. Filter Stage
- Low‑pass filter: 3–5 kHz (removes clock artifacts)
- Optional tone control for high‑frequency rolloff adjustment

### 6. Mix Stage
- Dry/wet blend (0–100%)
- Level matching to preserve perceived loudness

---

## DSP Parameter Ranges

| Parameter      | Range          | Description                              |
|----------------|----------------|------------------------------------------|
| Delay Time     | 20–300 ms      | BBD clock‑rate equivalent (extendable)   |
| Feedback       | 0–95%          | Delayed signal fed back into input       |
| Mix            | 0–100%         | Dry/wet blend                            |
| Tone           | 0–100%         | High‑frequency rolloff (3–8 kHz)         |
| Modulation     | 0–10 Hz (opt)  | Subtle chorus/vibrato (BBD clock wobble) |

---

## JUCE Project Structure

### Core DSP Modules
- `DelayLine.h/cpp`: 4096‑stage fractional delay with interpolation
- `Compander.h/cpp`: Compression/expansion with envelope followers
- `BBDModel.h/cpp`: Bandwidth limiting, noise, clock bleed
- `Filter.h/cpp`: Biquad low‑pass and tone control
- `MixStage.h/cpp`: Dry/wet blending with gain compensation

### Plugin Files
- `PluginProcessor.h/cpp`: AudioProcessor, parameter management, DSP chain
- `PluginEditor.h/cpp`: UI layout, knobs, meters
- `Parameters.h`: Parameter IDs, ranges, value trees

---

## BBD Simulation Strategy

### Perceptual Accuracy Goals
1. **Bandwidth**: 3–5 kHz rolloff (human‑audible BBD character)
2. **Noise floor**: Gentle hiss increase with delay time (~−60 dB)
3. **Clock artifacts**: Subtle aliasing/modulation at high frequencies
4. **Saturation**: Soft clipping at input/output stages

### Technical Approach
- **Delay line**: Ring buffer with linear/cubic interpolation for fractional delays
- **Clock modeling**: Variable sample‑rate decimation + reconstruction filter
- **Noise**: Shaped noise generator (pink/white blend, scales with delay time)
- **Aliasing**: Controlled fold‑back via pre‑filter bandwidth limiting

### Acceptable Approximations
- No pin‑level VDD/VGG voltage simulation
- Simplified compander envelope (no exact NE570 THD curve)
- Idealized clock phase alignment (no CP1/CP2 timing jitter)

---

## Implementation Phases

### Phase 1: Core DSP ✅ COMPLETED
1. ✅ Implement `DelayLine` with feedback
2. ✅ Add `Compander` compression/expansion
3. ✅ Build `BBDModel` bandwidth and noise
4. ✅ Integrate `Filter` stage
5. ✅ Implement `MixStage` dry/wet blending
6. ✅ Wire DSP chain in `PluginProcessor`

**Implementation Notes:**
- All DSP modules follow design spec accurately
- Stereo processing (independent left/right channels)
- Sample-by-sample processing for accuracy
- Full DSP chain: Input → Compress → Delay → BBD → Expand → Filter → Mix

### Phase 2: JUCE Integration ✅ COMPLETED
1. ✅ Create JUCE plugin project (VST3/Standalone)
2. ✅ Wire DSP modules into `PluginProcessor`
3. ✅ Map parameters to APVTS (AudioProcessorValueTreeState)
4. ✅ Build UI with rotary knobs

**Implementation Notes:**
- Boss DM-2 authentic layout with external/internal sections
- **External controls** (main user-facing): ECHO, REPEAT, INTENSITY (larger knobs)
- **Internal controls** (trim pots): TONE, MODULATION (smaller, marked "Internal")
- Visual separation shows users which controls are for normal use vs fine-tuning
- Parameter attachments link all knobs to DSP chain

### Phase 3: Refinement ✅ COMPLETED
1. ✅ Add STANDARD/CUSTOM mode switching (King of Tone style cascaded delay)
2. Tune BBD artifacts (A/B with reference recordings)
3. Add modulation (optional chorus effect)
4. Optimize CPU performance
5. Design final UI (vintage pedal aesthetic)

**CUSTOM Mode Implementation:**
- STANDARD mode: Single BBD delay stage (classic DM-2 sound)
- CUSTOM mode: Cascaded dual delay stages (doubled delay time, more ambient character)
- Similar to King of Tone pedal architecture (two circuits in series)
- Feedback slightly reduced in second stage (70%) to prevent runaway
- Visual indicators show active mode with red LED
- Clickable mode buttons for easy switching

---

## Technical Constraints

- **Sample rate**: 44.1/48 kHz minimum (96 kHz optional)
- **Latency**: <10 ms (acceptable for guitar tracking)
- **CPU target**: <5% single core (real‑time safe)
- **Platform**: Windows/macOS, VST3/AU formats

---

## Compander Block Explanation

**Companders** solve the BBD noise problem:
- **Problem**: BBD chips add clock noise and have limited dynamic range
- **Solution**: Compress signal before BBD (boosts quiet parts), expand after (restores dynamics + suppresses noise)
- **Chip**: DM‑2 uses NE570/SA571 dual compander IC
- **DSP model**: Envelope follower + gain computer (attack/release, ratio control)

This is critical for authentic DM‑2 sound—without it, the delay would sound hissy and lo‑fi.

---

## How to Build

### Prerequisites

1. **CMake** (version 3.15 or higher)
   - Download from https://cmake.org/download/
   - Add to PATH during installation

2. **Visual Studio 2022** (or newer)
   - Install "Desktop development with C++" workload
   - Ensure C++ build tools are installed

3. **JUCE Framework**
   - Already included in `h:\Right_Round\JUCE`

### Build Steps

#### Option 1: Command Line (CMake + Visual Studio)

```powershell
# Navigate to plugin directory
cd h:\Right_Round\DM2Delay

# Generate Visual Studio solution
cmake -B build -G "Visual Studio 17 2022"

# Build the plugin (Release mode recommended)
cmake --build build --config Release

# Or open the solution in Visual Studio
start build\DM2Delay.sln
```

#### Option 2: Visual Studio Code (Recommended)

1. Open VS Code in `h:\Right_Round`
2. Use **Terminal > Run Task** menu
3. Select build task from tasks.json (see below)

### Build Outputs

After successful build, plugin binaries are located in:
- VST3: `build/DM2Delay_artefacts/Release/VST3/DM-2 Delay.vst3`
- Standalone: `build/DM2Delay_artefacts/Release/Standalone/DM-2 Delay.exe`
- AU (macOS only): `build/DM2Delay_artefacts/Release/AU/DM-2 Delay.component`

### Project Structure

```
DM2Delay/
├── CMakeLists.txt          # Build configuration
├── Source/
│   ├── PluginProcessor.h   # Main audio processor
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h      # UI editor
│   ├── PluginEditor.cpp
│   ├── Parameters.h        # Parameter definitions
│   └── DSP/                # DSP modules (Phase 1)
│       ├── DelayLine.h/cpp
│       ├── Compander.h/cpp
│       ├── BBDModel.h/cpp
│       ├── Filter.h/cpp
│       └── MixStage.h/cpp
└── build/                  # Generated build files
```

---

## Next Steps

1. ✅ Create JUCE plugin project structure
2. ✅ Set up CMakeLists.txt and parameters
3. ⏳ Implement core DSP modules (Phase 1)
4. Reference tune against DM‑2 recordings/samples