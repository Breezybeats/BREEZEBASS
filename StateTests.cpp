#include "Editor.h"
#include "Processor.h"
#include <iostream>
#include <stdexcept>
static void check(bool b, const char *m)
{
    if (!b)
        throw std::runtime_error(m);
}
int main()
{
    try
    {
        juce::ScopedJuceInitialiser_GUI gui;
        auto a = std::make_unique<NeuroProcessor>(), b = std::make_unique<NeuroProcessor>();
        a->setFamily(10);
        a->recallSeed(32761);
        a->setValue(nj::ColorOn, 0);
        a->setValue(nj::SpectralOn, 1);
        a->setValue(nj::Shift, 7);
        a->setValue(nj::Step3, .124f);
        a->lockGroup(nj::Mod, true);
        a->lockWave(true);
        juce::MemoryBlock data;
        a->getStateInformation(data);
        b->setStateInformation(data.getData(), int(data.getSize()));
        for (int i = 0; i < nj::Count; ++i)
            check(std::abs(a->value(i) - b->value(i)) < 1e-4f, "State parameter mismatch");
        auto pa = a->patchInfo(), pb = b->patchInfo();
        check(pa.seed == pb.seed && pa.waveSeed == pb.waveSeed && pa.locks == pb.locks &&
                  pa.lockWave == pb.lockWave,
              "Metadata mismatch");
        a->prepareToPlay(48000, 128);
        b->prepareToPlay(48000, 128);
        juce::AudioBuffer<float> ba(2, 128), bb(2, 128);
        juce::MidiBuffer ma, mb;
        for (int n = 0; n < 100; ++n)
        {
            ma.clear();
            mb.clear();
            if (n == 0)
            {
                ma.addEvent(juce::MidiMessage::noteOn(1, 36, .8f), 0);
                mb = ma;
            }
            if (n == 80)
            {
                ma.addEvent(juce::MidiMessage::noteOff(1, 36), 17);
                mb = ma;
            }
            a->processBlock(ba, ma);
            b->processBlock(bb, mb);
            for (int c = 0; c < 2; ++c)
                for (int i = 0; i < 128; ++i)
                    check(std::abs(ba.getSample(c, i) - bb.getSample(c, i)) < 1e-5f, "Audio recall mismatch");
        }
        check(a->getLatencySamples() == 512, "Latency mismatch");
        a->setFamily(6);
        a->recallSeed(111);
        auto first = a->patchInfo();
        a->recallSeed(222);
        check(a->undo(), "Undo failed");
        check(a->patchInfo().seed == first.seed, "Undo seed mismatch");
        check(a->redo(), "Redo failed");
        check(a->patchInfo().seed == 222, "Redo seed mismatch");
        auto bank = juce::File::getCurrentWorkingDirectory().getChildFile("Factory-Presets");
        check(bank.createDirectory().wasOk(), "Factory directory failed");
        for (int i = 0; i < nj::presetCount; ++i)
        {
            check(a->factory(i), "Factory recall failed");
            auto name = juce::String(i + 1).paddedLeft('0', 3) + "_" +
                        juce::String(nj::families[1 + i / 5]).replaceCharacter(' ', '_') + "_" +
                        juce::String(1 + i % 5) + ".brezebass";
            check(a->savePreset(bank.getChildFile(name)), "Factory preset export failed");
        }
        auto editor = std::unique_ptr<juce::AudioProcessorEditor>(a->createEditor());
        auto capture = [&](const char *name)
        {
            auto image = editor->createComponentSnapshot(editor->getLocalBounds());
            auto f = juce::File::getCurrentWorkingDirectory().getChildFile(name);
            auto stream = f.createOutputStream();
            check(stream != nullptr, "Preview file could not be created");
            juce::PNGImageFormat png;
            check(png.writeImageToStream(image, *stream), "Preview encoding failed");
        };
        capture("BREEZEBASS-SPLASH.png");
        juce::Thread::sleep(550);
        capture("BREZEBASS-UI.png");
        editor.reset();
        auto state = a->patchInfo();
        char junk[8] = {};
        a->setStateInformation(junk, 8);
        check(a->patchInfo().seed == state.seed, "Invalid state modified patch");
        std::cout << "PASS: full state and rendered audio recall, toggles, metadata, latency, undo/redo, 150 "
                     "factory entries, invalid state rejection\n";
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
