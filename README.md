# BREZEBASS 1.0 — liquid-room bass generator

Start with **START-HERE.md**. Upload the contents of this folder to a **new GitHub repository** and run **Build BREZEBASS Mac M1 VST3**. The workflow builds the actual Apple Silicon VST3 and standalone targets, then tests the VST3 before offering the download. You do not need Xcode on your Mac.

## What changed

- New plugin identity and filename: **BREZEBASS.vst3**, independent of NEUROJ.
- A black splash showing only **BREEZEBASS** for about half a second, with a quick scale/pop animation. Those two spellings intentionally follow the request: BREZEBASS for the plugin; BREEZEBASS for the splash.
- A perspective black room with four shaded, deforming, bouncing 3D meshes in blue, yellow, red and green. Glow, floor ripples and audio-reactive deformation. These are procedural visual effects, not a physical liquid simulation. STILL ROOM pauses motion.
- Six dedicated editing panels: COLOR LAB, SPECTRAL LAB, BLOWN OUT, FX RACK, GATE / LFO, SYNTH.
- **150 original factory recipes: exactly five per bass family**, including a LOBOTIX-inspired family. Auto selects a family; it does not have its own five presets. Recipes are engineering-tested, not a promise of 150 professionally hand-curated sounds.
- The Mac build also exports all 150 presets as `.brezebass` files and captures the real JUCE interface as PNG images.
- One **SYNC GATE + LFO TO BPM** button switches both gate and LFO between host-beat units and Hz. Rates remain independently editable. Raise Gate Depth above zero to hear gating.

## Sound controls

Eight macros remain available: aggression, motion, weight, character, metal, space, width and chaos. Four generation biases influence the seed-driven recipes. Generate, Mutate, New Wave, New Rhythm, seed recall, section locks, Undo/Redo, presets and audition are all functional.

**Color:** MIDI-relative transpose, chord, one to eight resonator voices, voicing motion, octave spread, detune, excitation drive, feedback damping, brightness, decay and mix. Clean sub bypasses the resonators.

**Spectral:** real 512-point FFT/STFT with shift, stretch, prism-style displacement, frequency blur, temporal smear, freeze, tilt, gate, contrast, bin grouping, harmonic-source snap and transposed harmonic-layer mixing. It uses original DSP inspired by documented synthesis principles, not Harmor or Vital code.

**Blown Out:** drive, wet level, asymmetry, bit depth, sample-rate reduction, tone, sag and clipping hardness. This is a dedicated section with an explicit enable switch. The independent clean sub is merged after it.

**FX:** animated six-stage phaser, note-relative ring modulation, drive/fold, filtered stereo delay, and a three-band interpolated vowel filter. The vowel controls are in SYNTH beside filter settings. Existing pitch-drop/burst controls support tearout and machine-gun patterns.

## Development / validation

The portable DSP is compiled and tested locally. JUCE C++ syntax is checked against pinned 8.0.6 headers. This release's Mac compilation, real host loading, screenshots and FL Studio behaviour still need the supplied cloud build/user checks. Read VALIDATION.md for exact status; do not treat a source folder as a precompiled plugin.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build build --config Release --parallel 3
ctest --test-dir build -C Release --output-on-failure
```

For DSP-only tests: configure with `-DBREZEBASS_CORE_ONLY=ON` (no JUCE required).

No sound-design samples or third-party presets are bundled. The normal workflow fetches pinned JUCE source automatically. See THIRD-PARTY-NOTICES.md.
