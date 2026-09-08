#pragma once
#include "Parameters.h"
namespace nj
{
struct BassFX
{
    std::array<float, 6> allpass{};
    float phase = 0, ringPhase = 0, previous = 0, held = 0, envelope = 0, tone = 0;
    int holdCount = 0;
    void reset()
    {
        allpass.fill(0);
        phase = ringPhase = previous = held = envelope = tone = 0;
        holdCount = 0;
    }
    float tick(float input, const Values &p, float sr, float noteHz)
    {
        float x = input;
        phase += p[PhaserRate] / sr;
        phase -= std::floor(phase);
        if (p[PhaserMix] > .0001f)
        {
            float a = .2f + .65f * p[PhaserDepth] * std::sin(tau * phase), wet = x;
            for (auto &state : allpass)
            {
                float out = -a * wet + state;
                state = clamp(wet + a * out, -8, 8);
                wet = out;
            }
            x = x * (1 - p[PhaserMix] * .5f) + wet * p[PhaserMix] * .5f;
        }
        ringPhase += std::min(sr * .4f, noteHz * p[RingRatio]) / sr;
        ringPhase -= std::floor(ringPhase);
        x *= 1 - p[RingMix] + p[RingMix] * std::sin(tau * ringPhase);
        if (p[BlownOn] > .5f)
        {
            envelope += (std::abs(x) - envelope) * (1 - std::exp(-1 / (sr * .035f)));
            float drive = p[BlownDrive] / (1 + p[BlownSag] * envelope * 9);
            auto shape = [&](float v)
            {
                float b = p[BlownBias];
                float soft = std::tanh(v * drive + b) - std::tanh(b);
                float hard = clamp(v * drive + b, -1, 1) - clamp(b, -1, 1);
                return soft * (1 - p[BlownClip]) + hard * p[BlownClip];
            };
            float wet = .5f * (shape((previous + x) * .5f) + shape(x));
            previous = x;
            if (holdCount++ >= int(p[BlownRate]) - 1)
            {
                holdCount = 0;
                float levels = std::pow(2.f, p[BlownBits] - 1);
                held = std::round(wet * levels) / levels;
            }
            tone += (held - tone) * (1 - std::exp(-tau * std::min(p[BlownTone], sr * .4f) / sr));
            x = x * (1 - p[BlownMix]) + tone * p[BlownMix] * .65f;
        }
        return clamp(x, -8, 8);
    }
};
} // namespace nj
