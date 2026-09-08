#pragma once
#include "Processor.h"
#include "RoomScene.h"
class NeuroEditor final : public juce::AudioProcessorEditor, private juce::Timer
{
  public:
    explicit NeuroEditor(NeuroProcessor &);
    ~NeuroEditor() override;
    void paint(juce::Graphics &) override;
    void paintOverChildren(juce::Graphics &) override;
    void resized() override;

  private:
    NeuroProcessor &processor;
    RoomScene room;
    juce::LookAndFeel_V4 look;
    juce::TooltipWindow tips{this, 500};
    juce::TextButton generate{"GENERATE"}, mutate{"MUTATE"}, undo{"UNDO"}, redo{"REDO"}, save{"SAVE"},
        load{"LOAD"}, newWT{"NEW WAVE"}, rhythm{"NEW RHYTHM"}, seedGo{"RECALL"};
    juce::ToggleButton color{"COLOR"}, spectral{"SPECTRAL"}, safe{"SAFE"}, clean{"CLEAN SUB"},
        audition{"AUDITION"}, sync{"SYNC GATE + LFO TO BPM"}, blown{"BLOWN OUT"}, freezeVisuals{"STILL ROOM"},
        seedLock{"LOCK SEED"}, wtLock{"LOCK WAVE"};
    juce::ComboBox family, factory;
    juce::TextEditor seed;
    juce::Label status, meters;
    juce::Viewport viewport;
    juce::Component content;
    std::array<juce::Slider, nj::Count> sliders;
    std::array<juce::Label, nj::Count> labels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, nj::Count> attachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttons;
    std::array<juce::TextButton, 6> tabs;
    std::array<juce::ToggleButton, 8> locks;
    std::vector<int> visibleParams;
    int selectedTab = 0;
    float energy = 0;
    double opened = juce::Time::getMillisecondCounterHiRes();
    std::unique_ptr<juce::FileChooser> chooser;
    void timerCallback() override;
    void syncUI();
    void result(bool, const juce::String &);
    void choose(bool);
    void selectTab(int);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuroEditor)
};
