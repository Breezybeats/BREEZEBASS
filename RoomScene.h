#pragma once
#include "Parameters.h"
#include <juce_gui_basics/juce_gui_basics.h>
// Deforming 3D meshes projected and shaded on the UI thread.
class RoomScene
{
    struct V
    {
        float x, y, z;
    };
    struct Face
    {
        std::array<juce::Point<float>, 4> p;
        float depth, light;
    };

  public:
    void draw(juce::Graphics &g, juce::Rectangle<float> area, float seconds, float energy)
    {
        const float w = area.getWidth(), h = area.getHeight();
        g.fillAll(juce::Colour(0xff020305));
        juce::ColourGradient fog(juce::Colour(0xff12171c), w * .5f, h * .43f, juce::Colour(0xff010203),
                                 w * .5f, h, true);
        g.setGradientFill(fog);
        g.fillRect(area);
        g.setColour(juce::Colour(0xff1b242d));
        auto back = juce::Rectangle<float>(w * .18f, h * .19f, w * .64f, h * .6f);
        g.drawRect(back, 1);
        g.drawLine(0, 0, back.getX(), back.getY());
        g.drawLine(w, 0, back.getRight(), back.getY());
        g.drawLine(0, h, back.getX(), back.getBottom());
        g.drawLine(w, h, back.getRight(), back.getBottom());
        for (int i = 1; i < 7; ++i)
        {
            float t = i / 7.f;
            g.setColour(juce::Colour(0xff10191f));
            g.drawLine(w * t, h, w * .5f + (w * t - w * .5f) * .64f, back.getBottom());
        }
        const juce::Colour palette[] = {juce::Colour(0xff287eff), juce::Colour(0xffffd739),
                                        juce::Colour(0xffff3d54), juce::Colour(0xff3aff9f)};
        for (int corner = 0; corner < 4; ++corner)
        {
            float t = seconds * (.8f + corner * .11f) + corner * 1.7f, bounce = std::abs(std::sin(t * 1.7f));
            float cx = w * ((corner % 2) ? .91f : .09f),
                  cy = h * (corner < 2 ? .17f : .86f) - bounce * h * .027f,
                  radius = std::min(w * .075f, h * .115f) * (1 + energy * .2f);
            auto color = palette[corner];
            for (int ring = 7; ring > 0; --ring)
            {
                float size = radius * (1.3f + ring * .25f);
                g.setColour(color.withAlpha(.015f + energy * .012f));
                g.fillEllipse(cx - size, cy - size * .72f, size * 2, size * 1.44f);
            }
            g.setColour(color.withAlpha(.08f));
            g.fillEllipse(cx - radius * 1.25f, cy + radius * .88f, radius * 2.5f, radius * .3f);
            for (int ripple = 0; ripple < 3; ++ripple)
            {
                float age = std::fmod(seconds * .7f + ripple / 3.f, 1.f);
                g.setColour(color.withAlpha((1 - age) * .22f));
                g.drawEllipse(cx - radius * (.5f + age * 1.5f), cy + radius * .88f - radius * .1f * age,
                              radius * (1 + age * 3), radius * .3f * age, 1);
            }
            std::array<Face, 16 * 20> faces;
            int n = 0;
            auto vertex = [&](float u, float v)
            {
                float r = 1 + .11f * std::sin(3 * u + t * 2) + .07f * std::sin(v * 5 - t * 3) +
                          energy * .12f * std::sin(u * 7 + v * 3);
                V p{r * std::sin(v) * std::cos(u), r * std::cos(v), r * std::sin(v) * std::sin(u)};
                float c = std::cos(t * .4f), s = std::sin(t * .4f);
                return V{p.x * c + p.z * s, p.y * (.8f + .16f * bounce), -p.x * s + p.z * c};
            };
            auto project = [&](V v)
            {
                float f = 3 / (3 - v.z * .35f);
                return juce::Point<float>{cx + v.x * radius * f, cy + v.y * radius * f};
            };
            for (int lat = 0; lat < 16; ++lat)
                for (int lon = 0; lon < 20; ++lon)
                {
                    float u = nj::tau * lon / 20, v = nj::pi * lat / 16;
                    std::array<V, 4> q{vertex(u, v), vertex(u + nj::tau / 20, v),
                                       vertex(u + nj::tau / 20, v + nj::pi / 16), vertex(u, v + nj::pi / 16)};
                    auto &f = faces[n++];
                    f.depth = 0;
                    V normal{};
                    for (int k = 0; k < 4; ++k)
                    {
                        f.p[k] = project(q[k]);
                        f.depth += q[k].z * .25f;
                        normal.x += q[k].x * .25f;
                        normal.y += q[k].y * .25f;
                        normal.z += q[k].z * .25f;
                    }
                    f.light = nj::clamp(.2f - normal.x * .32f - normal.y * .52f + normal.z * .58f, .06f, 1.f);
                }
            std::sort(faces.begin(), faces.end(),
                      [](const Face &a, const Face &b) { return a.depth < b.depth; });
            for (const auto &f : faces)
            {
                juce::Path p;
                p.startNewSubPath(f.p[0]);
                for (int i = 1; i < 4; ++i)
                    p.lineTo(f.p[i]);
                p.closeSubPath();
                auto shade = color.withMultipliedBrightness(.12f + f.light * .88f);
                if (f.light > .8f)
                    shade = shade.interpolatedWith(juce::Colours::white, (f.light - .8f) * 3);
                g.setColour(shade);
                g.fillPath(p);
            }
            g.setColour(juce::Colours::white.withAlpha(.3f));
            g.fillEllipse(cx - radius * .3f, cy - radius * .44f, radius * .24f, radius * .13f);
        }
    }
};
