#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
namespace nj
{
constexpr float pi = 3.14159265358979323846f, tau = 2 * pi;
inline float clamp(float x, float a = 0, float b = 1)
{
    return std::clamp(std::isfinite(x) ? x : a, a, b);
}
enum Group
{
    Osc,
    FM,
    Filter,
    Mod,
    Colour,
    Spectral,
    FX,
    Sub,
    Global,
    GroupCount
};
struct Spec
{
    const char *id;
    const char *name;
    float lo, hi, def;
    Group group;
    bool integer = false;
};
// Append new parameters only. BREZEBASS uses state schema version 3.
enum Param
{
    Aggression,
    Motion,
    Weight,
    Character,
    Metal,
    Space,
    Width,
    Chaos,
    Brightness,
    Destruction,
    Complexity,
    Atonality,
    ColorOn,
    SpectralOn,
    CleanSub,
    SafeOutput,
    WtA,
    WtB,
    Blend,
    Detune,
    FMAmount,
    Ratio,
    PMAmount,
    Noise,
    SubLevel,
    Crossover,
    Cutoff,
    Resonance,
    FilterMode,
    Drive,
    Fold,
    Attack,
    Decay,
    Sustain,
    Release,
    LFOSpeed,
    LFOShape,
    LFO2Speed,
    ModFM,
    ModFilter,
    ModWT,
    RhythmOn,
    BurstRate,
    Shots,
    PitchDrop,
    PitchDecay,
    ColorAmount,
    Chord,
    ColorDecay,
    ColorMotion,
    SpectralMix,
    Shift,
    Stretch,
    Blur,
    Freeze,
    Tilt,
    Gate,
    DelayTime,
    DelayFeedback,
    DelayMix,
    Output,
    Glide,
    Mono,
    Sync,
    PitchBendRange,
    Mutation,
    Step0,
    Step1,
    Step2,
    Step3,
    Step4,
    Step5,
    Step6,
    Step7,
    Step8,
    Step9,
    Step10,
    Step11,
    Step12,
    Step13,
    Step14,
    Step15,
    Normalise,
    GateDepth,
    GateSpeed,
    GateDuty,
    GateSmooth,
    ColorRoot,
    ColorDensity,
    ColorSpread,
    ColorBrightness,
    ColorDetune,
    ColorExcite,
    ColorDamping,
    Prism,
    SpectralSmear,
    SpectralContrast,
    SpectralQuantise,
    HarmonicSnap,
    HarmonicMix,
    HarmonicInterval,
    PhaserMix,
    PhaserRate,
    PhaserDepth,
    VowelMix,
    Vowel,
    RingMix,
    RingRatio,
    BlownOn,
    BlownDrive,
    BlownMix,
    BlownBias,
    BlownBits,
    BlownRate,
    BlownTone,
    BlownSag,
    BlownClip,
    Count
};
inline const std::array<Spec, Count> specs{
    {{"aggression", "AGGRESSION", 0, 1, .55f, Global},
     {"motion", "MOTION", 0, 1, .65f, Global},
     {"weight", "WEIGHT", 0, 1, .65f, Global},
     {"character", "CHARACTER", 0, 1, .5f, Global},
     {"metal", "METAL", 0, 1, .3f, Global},
     {"space", "SPACE", 0, 1, .1f, Global},
     {"width", "WIDTH", 0, 1, .45f, Global},
     {"chaos", "CHAOS", 0, 1, .2f, Global},
     {"brightness", "Dark - Bright", 0, 1, .55f, Global},
     {"destruction", "Clean - Destroyed", 0, 1, .65f, Global},
     {"complexity", "Simple - Complex", 0, 1, .65f, Global},
     {"atonality", "Tonal - Atonal", 0, 1, .25f, Global},
     {"color", "COLOR", 0, 1, 0, Colour, true},
     {"spectral", "SPECTRAL", 0, 1, 0, Spectral, true},
     {"cleanSub", "CLEAN SUB", 0, 1, 1, Sub, true},
     {"safeOutput", "SAFE OUTPUT", 0, 1, 1, Global, true},
     {"wtA", "A Wavetable Position", 0, 1, .3f, Osc},
     {"wtB", "B Wavetable Position", 0, 1, .6f, Osc},
     {"blend", "A / B Mix", 0, 1, .25f, Osc},
     {"detune", "B Detune cents", 0, 40, 7, Osc},
     {"fm", "B to A FM", 0, 5, 1.5f, FM},
     {"ratio", "C FM Ratio", .5f, 16, 2, FM},
     {"pm", "C to B Phase Mod", 0, 3, .45f, FM},
     {"noise", "Noise Transient", 0, 1, .12f, Osc},
     {"sub", "Sub Level", 0, 1, .65f, Sub},
     {"crossover", "Clean Sub Crossover Hz", 60, 180, 110, Sub},
     {"cutoff", "Filter Cutoff Hz", 100, 18000, 3400, Filter},
     {"resonance", "Filter Resonance", .5f, 5, 1, Filter},
     {"filterMode", "Filter Type: LP BP Notch", 0, 2, 0, Filter, true},
     {"drive", "Distortion Drive", 1, 14, 4, FX},
     {"fold", "Wavefold Mix", 0, 1, .2f, FX},
     {"attack", "Attack seconds", .001f, .5f, .003f, Mod},
     {"decay", "Decay seconds", .015f, 2, .2f, Mod},
     {"sustain", "Sustain", 0, 1, .8f, Mod},
     {"release", "Release seconds", .01f, 1, .12f, Mod},
     {"lfoSpeed", "LFO1 cycles per beat / Hz", .125f, 8, 2, Mod},
     {"lfoShape", "LFO1: Sine Tri Saw Square Steps", 0, 4, 1, Mod, true},
     {"lfo2Speed", "LFO2 cycles per beat / Hz", .125f, 8, .5f, Mod},
     {"modFM", "LFO to FM", 0, 1, .55f, Mod},
     {"modFilter", "LFO to Filter octaves", 0, 5, 2, Mod},
     {"modWT", "LFO to Wavetable", 0, 1, .35f, Mod},
     {"rhythm", "Step Rhythm", 0, 1, 0, Mod, true},
     {"burstRate", "Gun Shots per Beat (0 = off)", 0, 12, 0, Mod},
     {"shots", "Gun Shot Count (0 = repeat)", 0, 16, 0, Mod, true},
     {"pitchDrop", "Pitch Transient semitones", 0, 48, 3, FM},
     {"pitchDecay", "Pitch Transient seconds", .005f, .3f, .055f, FM},
     {"colorAmount", "Color Resonator Mix", 0, 1, .65f, Colour},
     {"chord", "Chord: Fifth Minor Major Min7 Maj7 Sus2", 0, 5, 1, Colour, true},
     {"colorDecay", "Color Resonance seconds", .025f, .6f, .15f, Colour},
     {"colorMotion", "Color Voicing Motion", 0, 1, .3f, Colour},
     {"spectralMix", "Spectral Wet", 0, 1, .7f, Spectral},
     {"spectralShift", "Spectral Shift bins", -24, 24, 3, Spectral},
     {"spectralStretch", "Spectral Stretch", .5f, 2, 1.15f, Spectral},
     {"spectralBlur", "Spectral Blur", 0, 1, .2f, Spectral},
     {"spectralFreeze", "Spectral Freeze", 0, 1, 0, Spectral, true},
     {"spectralTilt", "Spectral Tilt", -1, 1, 0, Spectral},
     {"spectralGate", "Spectral Gate", 0, .2f, .005f, Spectral},
     {"delayTime", "Delay beats", .125f, 2, .5f, FX},
     {"delayFeedback", "Delay Feedback", 0, .7f, .25f, FX},
     {"delayMix", "Delay Mix", 0, .6f, .15f, FX},
     {"output", "Output dB", -30, 0, -7, Global},
     {"glide", "Glide seconds", 0, 1, 0, Global},
     {"mono", "Mono (off = 8 voices)", 0, 1, 1, Global, true},
     {"sync", "Tempo Sync", 0, 1, 1, Mod, true},
     {"bendRange", "Pitch Bend semitones", 1, 24, 2, Global},
     {"mutation", "Mutation amount", .05f, .25f, .12f, Global},
     {"step01", "Step 01", 0, 1, 1, Mod},
     {"step02", "Step 02", 0, 1, .8f, Mod},
     {"step03", "Step 03", 0, 1, .9f, Mod},
     {"step04", "Step 04", 0, 1, .4f, Mod},
     {"step05", "Step 05", 0, 1, 1, Mod},
     {"step06", "Step 06", 0, 1, .7f, Mod},
     {"step07", "Step 07", 0, 1, .9f, Mod},
     {"step08", "Step 08", 0, 1, .3f, Mod},
     {"step09", "Step 09", 0, 1, 1, Mod},
     {"step10", "Step 10", 0, 1, .4f, Mod},
     {"step11", "Step 11", 0, 1, .8f, Mod},
     {"step12", "Step 12", 0, 1, .9f, Mod},
     {"step13", "Step 13", 0, 1, 1, Mod},
     {"step14", "Step 14", 0, 1, .8f, Mod},
     {"step15", "Step 15", 0, 1, .6f, Mod},
     {"step16", "Step 16", 0, 1, .4f, Mod},
     {"normalise", "Patch Loudness Calibration", 0, 1, 1, Global, true},
     {"gateDepth", "Gate Depth", 0, 1, 0, Mod},
     {"gateSpeed", "Gate Rate: cycles/beat or Hz", .125f, 16, 4, Mod},
     {"gateDuty", "Gate Duty", .05f, .95f, .5f, Mod},
     {"gateSmooth", "Gate Edge ms", 1, 50, 5, Mod},
     {"colorRoot", "Color Transpose semitones", -12, 12, 0, Colour, true},
     {"colorDensity", "Color Voices", 1, 8, 4, Colour, true},
     {"colorSpread", "Color Octave Spread", 0, 2, .4f, Colour},
     {"colorBrightness", "Color Brightness Hz", 500, 18000, 7000, Colour},
     {"colorDetune", "Color Detune cents", 0, 30, 3, Colour},
     {"colorExcite", "Color Excitation Drive", .2f, 5, 1, Colour},
     {"colorDamping", "Color Feedback Damping", 0, 1, .3f, Colour},
     {"prism", "Spectral Prism", -1, 1, 0, Spectral},
     {"spectralSmear", "Spectral Time Smear", 0, .93f, 0, Spectral},
     {"spectralContrast", "Spectral Contrast", .4f, 2, 1, Spectral},
     {"binQuantise", "Bin Group Size", 1, 12, 1, Spectral, true},
     {"harmonicSnap", "Harmonic Source Snap", 0, 1, 0, Spectral},
     {"harmonicMix", "Harmonic Clone Mix", 0, 1, 0, Spectral},
     {"harmonicInterval", "Clone semitones", 0, 24, 7, Spectral},
     {"phaserMix", "Phaser Mix", 0, 1, 0, FX},
     {"phaserRate", "Phaser Rate Hz", .05f, 8, .4f, FX},
     {"phaserDepth", "Phaser Depth", 0, 1, .6f, FX},
     {"vowelMix", "Vowel Mix", 0, 1, 0, Filter},
     {"vowel", "Vowel A E I O U", 0, 4, 0, Filter},
     {"ringMix", "Ring Mod Mix", 0, 1, 0, FX},
     {"ringRatio", "Ring Mod Ratio", .25f, 12, 2, FX},
     {"blownOn", "BLOWN OUT", 0, 1, 0, FX, true},
     {"blownDrive", "Blown Drive", 1, 32, 6, FX},
     {"blownMix", "Blown Wet", 0, 1, .5f, FX},
     {"blownBias", "Blown Asymmetry", -.8f, .8f, 0, FX},
     {"blownBits", "Blown Bit Depth", 4, 16, 12, FX, true},
     {"blownRate", "Blown Rate Reduction", 1, 32, 1, FX, true},
     {"blownTone", "Blown Tone Hz", 300, 18000, 7000, FX},
     {"blownSag", "Blown Sag", 0, 1, .2f, FX},
     {"blownClip", "Blown Hardness", 0, 1, .4f, FX}}};
using Values = std::array<float, Count>;
inline Values defaults()
{
    Values v{};
    for (int i = 0; i < Count; ++i)
        v[i] = specs[i].def;
    return v;
}
inline void sanitise(Values &v)
{
    for (int i = 0; i < Count; ++i)
    {
        v[i] = clamp(v[i], specs[i].lo, specs[i].hi);
        if (specs[i].integer)
            v[i] = std::round(v[i]);
    }
}
struct RNG
{
    uint32_t s;
    explicit RNG(uint32_t seed) : s(seed ? seed : 0x12345678u) {}
    uint32_t next()
    {
        s ^= s << 13;
        s ^= s >> 17;
        s ^= s << 5;
        return s;
    }
    float unit()
    {
        return (next() >> 8) * (1.f / 16777216.f);
    }
    float range(float a, float b)
    {
        return a + (b - a) * unit();
    }
    int pick(int n)
    {
        return int(next() % uint32_t(n));
    }
};
inline constexpr const char *families[] = {
    "AUTO",           "NEURO",         "RAZOR",           "TEAROUT GUN",
    "MACHINE GUN",    "GROWL",         "REESE",           "WOBBLE",
    "SPECTRAL",       "COLOR",         "COLOR + TEAROUT", "SPECTRAL + TEAROUT",
    "PULSE",          "LASER",         "FORMANT",         "METALLIC",
    "INDUSTRIAL",     "GLITCH",        "SUSTAIN",         "TALKING BASS",
    "FM MONSTER",     "RESONATOR",     "LIQUID DIGITAL",  "RHYTHM-GAME",
    "JAPANESE NEURO", "FUTURE RIDDIM", "MELODIC RIDDIM",  "NEURO DNB",
    "WEIRD",          "HYBRID",        "LOBOTIX-INSPIRED"};
inline constexpr int familyCount = 31;
inline constexpr int presetsPerFamily = 5, presetCount = (familyCount - 1) * presetsPerFamily;
} // namespace nj
