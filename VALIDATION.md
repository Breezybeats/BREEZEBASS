# BREZEBASS validation and limits

## Run locally — passed

- C++17 portable engine compiled with GCC 13.3; DSP-only CMake configure/build/CTest passed.
- Deterministic seed/table generation and section locks.
- FFT inverse roundtrip, STFT reconstruction and 512-sample alignment.
- Thirty bass families at 44.1, 48, 88.2 and 96 kHz.
- All 150 factory recipes rendered with finite, non-silent, sample-peak-bounded output; exactly five recipes per family verified.
- Two hundred additional random seeds, extreme eight-voice patch, note release, sustain, mono-note fallback, and invalid parameter repair.
- Gate and LFO phase progression at two host tempos and in Hz mode.
- AddressSanitizer and UndefinedBehaviorSanitizer passed. Leak detection disabled in that run; this is not a soak/leak certification.
- JUCE processor/editor/state-test/host-test syntax checked against the real JUCE 8.0.6 headers. This is not native linking.
- Preview WAV rendered from the actual new engine. Eight two-second excerpts: LOBOTIX-inspired, tearout gun, machine gun, color, spectral, Reese, color + tearout, hybrid. These are uncurated demonstrations, not artist reference recordings.

## Automated in the supplied Mac workflow — not run locally

- Native arm64 plugin/standalone compilation.
- All-parameter and rendered-audio state restore, history, and factory recall/export tests.
- Real VST3 scan/load, instrument/MIDI flags, prepared-host latency, output safety across buffer sizes/rates, editor creation and resizing.
- Ad-hoc signing and architecture check using the corrected lipo argument order.
- Real editor/splash PNG capture and 150 individual .brezebass preset exports.

The cloud build has to pass before a BREZEBASS Mac binary exists. This release's native appearance and animation have not been visually verified here. Generated CI screenshots help check its actual rendering; no concept image is substituted for them.

## Design limits

- The liquid room is a shaded 3D mesh animation with procedural deformation, glow and ripple drawings, not a fluid solver or raytraced renderer.
- The 500 ms splash appears when the editor is constructed. Background host loading without opening an editor has no visible splash.
- Effects use original algorithms, not Harmor/Vital/LOBOTIX implementations or presets. The source research did not include a full transcription or listening analysis of the referenced videos.
- FFT size is fixed at 512 with coarse bass-frequency resolution. Spectral mapping can be rough/inharmonic; it is not a transparent phase vocoder. Harmonic snapping uses the last active voice in poly mode and is approximate.
- Nonlinear DSP is not fully oversampled. The blown section averages two shaping evaluations, which is not bandlimited oversampling. Extreme folding/FM/crushing can alias.
- Clean-sub crossovers use gentle one-pole slopes, not brick-wall separation. Safety is a sample-peak saturator, not a true-peak/lookahead limiter.
- RMS calibration uses a short reference note and bounded gain; later control changes alter loudness. It is not LUFS matching or perceptual quality validation.
- Gate/LFO are synced to BPM and retrigger per note, not absolute song/PPQ phase. MIDI channels are combined; no MPE.
- Presets save parameters, wave samples, seeds, family and locks, not live delay/freeze tails or an already-held note position. BREZEBASS uses a new plugin identity/state schema; old NEUROJ presets are not loaded.
- No full modulation matrix, image/audio resynthesis, reference analyzer, dedicated reverb, A/B morph, or arbitrary FM-routing editor.
- Mac notarisation, real FL Studio scan/play/save-reopen/offline rendering, manual sound curation and sustained CPU tests remain to be checked on the user's Mac.
