# Sound-design research and implementation decisions

## Scope and source quality

The research used Image-Line's Harmor manual, Vital's own technical/product material, and the LOBOTIX channel's public tutorial listings. These support the synthesis directions below. Video listing text does **not** reveal every patch setting, so no claim is made that the tutorials were fully transcribed, their audio analyzed, or their presets reproduced. J-core is a broad musical context rather than a single standardized bass algorithm; the implementation choices here are original design inferences.

## Harmor: separating pitch structure from texture

Harmor's additive engine exposes harmonic-level manipulation. Its documented Prism control changes the relationship of partials to the fundamental, while Harmonizer creates shifted harmonic copies. Blur can spread partial energy over frequency and time. Low-harmonic protection preserves weight under aggressive processing. [Image-Line Harmor manual](https://www.image-line.com/fl-studio-learning/fl-studio-online-manual/html/plugins/Harmor.htm)

BREZEBASS applies these principles through a different engine: wavetable/FM synthesis followed by STFT transformation. Its Prism knob curves source-bin positions; Harmonic Clone layers a transposed spectral source; Time Smear integrates successive frames. This is not Harmor's additive architecture, image resynthesis or proprietary algorithm. The independent sub bypasses this processing.

Design implication: a stable root can coexist with highly displaced upper partials. For a tonal color bass, keep Prism moderate, use interval-based cloning and a stable resonant chord. For a razor/metallic texture, increase displacement and contrast while protecting the sub. Brightness alone is insufficient: harmonic placement and modulation determine the character.

## Vital: harmonics as a modulation target

Vital describes spectral warping as stretching, shifting, smearing and skewing a wavetable's harmonics. This supports changing timbre at the harmonic-structure level rather than relying only on a moving cutoff. [Vital press kit](https://vital.audio/presskit/), [Vital overview](https://vital.audio/)

BREZEBASS combines original four-frame wavetable banks, FM/PM and modulation with post-synthesis spectral processing. Its spectral effects are applied to the summed signal, not to Vital-style per-oscillator spectral wavetable transforms. The distinction matters: deep shift/stretch can produce rough phase artifacts, especially on polyphonic inputs. That roughness is useful for digital bass, but this is not transparent pitch shifting.

Design implication: vary wavetable position and FM in opposing rhythmic movements, then introduce spectral color in parallel. Keep wet amount and modulation depth independently editable. Bin grouping produces coarse digital textures; harmonic-source snapping biases the selected bins toward multiples of the tracked fundamental. At a 512-point FFT, low-frequency resolution is coarse, so snapping is approximate and works best above the protected sub.

## LOBOTIX reference direction

The channel publicly presents Vital color-bass, metallic riddim and glitch-generator material. Relevant listings include [ColourBass in VITAL (No Post FX)](https://www.youtube.com/watch?v=D0xpiFPc6v0), [Vital glitch bass generator](https://www.youtube.com/watch?v=twDIeOUIqC4), and [Vital sound-design session](https://www.youtube.com/watch?v=52wowgannCE). These establish a reference palette, not verified knob-by-knob recipes.

The **LOBOTIX-INSPIRED** family is explicitly original and unaffiliated. Its generator combines strong FM/PM, gate rhythm, moving vowel/filter content, phaser motion, restrained color resonance, moderate spectral layering and parallel blown-out saturation. Five variations change harmonic structure, LFO shape, voicing and destruction density. No downloaded commercial pack, wavetable or reference audio is embedded.

## Color lab

The color engine uses one to eight feedback comb resonators following the played MIDI note. Six chord structures provide fifth, minor, major, minor-seven, major-seven and suspended-second voicings. Transpose and octave spread change their placement; detune and slow motion add beating. Excitation drive changes how the incoming bass energizes the bank. Feedback damping and brightness control sustained upper-frequency energy. The wet signal is normalized by voice count to avoid an automatic level jump when adding voices.

For glassy liquid color, use moderate excitation, longer decay, low damping, small detune and some spectral smear. For a chorded gun, use faster amplitude/pitch transients and a shorter resonator decay. Large excitation with long decay can become dense; the output safety remains enabled by default.

## Blown-out and rhythmic character

The Blown Out section models a deliberately compressed, asymmetric, crushed midrange: drive and hardness shape peaks, bias creates even-harmonic asymmetry, bit-depth/rate reduction add digital grit, sag creates level-dependent drive reduction, and the tone filter controls resulting fizz. Wet blending keeps the original attack available. It does not claim to emulate a specific hardware circuit or branded compressor.

A smoothed rectangular gate has editable depth, duty and edge time. With sync enabled its rate is cycles per host beat; off means Hz. Both LFOs use the same unit switch, while keeping independent rates. Step rhythm and gun bursts remain separate sources of articulation. Fast gating and FM transients provide rhythmic detail without depending on copyrighted impact samples.

## Practical limits

There is no exact-artist imitation claim, perceptual preset-quality guarantee, full Harmor replacement, Vital dependency, or real-time fluid solver. The source implements the described controls. Actual listening, UI appearance, Apple security approval and FL Studio behavior still require the user's successful Mac build and testing.
