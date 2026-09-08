#include "Editor.h"
using namespace nj;
NeuroEditor::NeuroEditor(NeuroProcessor &p) : AudioProcessorEditor(&p), processor(p)
{
    look.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff73caff));
    look.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff263039));
    look.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    look.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff10151c));
    look.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1b252d));
    look.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff5fffa9));
    setLookAndFeel(&look);
    setResizable(true, true);
    setResizeLimits(1050, 790, 1650, 1250);
    setSize(1180, 880);
    for (auto *c : std::initializer_list<juce::Component *>{
             &generate, &mutate,   &undo,    &redo,  &save,     &load,   &newWT,   &rhythm,        &seedGo,
             &color,    &spectral, &safe,    &clean, &audition, &sync,   &blown,   &freezeVisuals, &seedLock,
             &wtLock,   &family,   &factory, &seed,  &status,   &meters, &viewport})
        addAndMakeVisible(c);
    for (int i = 0; i < familyCount; ++i)
        family.addItem(families[i], i + 1);
    factory.addItem("150 FACTORY SOUNDS / SELECT", 1);
    const char *variations[] = {"01 CHROME", "02 SHARDS", "03 LIQUID", "04 IMPACT", "05 FRACTURE"};
    for (int i = 0; i < presetCount; ++i)
        factory.addItem(juce::String(families[1 + i / 5]) + " / " + variations[i % 5], i + 2);
    factory.setSelectedId(1);
    factory.onChange = [this]
    {
        if (factory.getSelectedId() > 1)
            result(processor.factory(factory.getSelectedId() - 2), "Factory sound loaded");
    };
    family.onChange = [this] { processor.setFamily(family.getSelectedId() - 1); };
    generate.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff23577f));
    generate.onClick = [this] { result(processor.makePatch(), "New bass generated"); };
    mutate.onClick = [this] { result(processor.makePatch(true), "Variation generated"); };
    undo.onClick = [this] { result(processor.undo(), "Undo"); };
    redo.onClick = [this] { result(processor.redo(), "Redo"); };
    newWT.onClick = [this] { result(processor.newWavetable(), "Original wave regenerated"); };
    rhythm.onClick = [this] { result(processor.randomRhythm(), "Rhythm regenerated"); };
    save.onClick = [this] { choose(true); };
    load.onClick = [this] { choose(false); };
    seed.setInputRestrictions(10, "0123456789");
    seedGo.onClick = [this]
    { result(processor.recallSeed(uint32_t(seed.getText().getLargeIntValue())), "Seed recalled"); };
    seed.onReturnKey = seedGo.onClick;
    audition.onClick = [this] { processor.audition(audition.getToggleState()); };
    seedLock.onClick = [this] { processor.lockSeed(seedLock.getToggleState()); };
    wtLock.onClick = [this] { processor.lockWave(wtLock.getToggleState()); };
    auto attach = [&](int i, juce::ToggleButton &b)
    {
        buttons.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processor.params, specs[i].id, b));
    };
    attach(ColorOn, color);
    attach(SpectralOn, spectral);
    attach(SafeOutput, safe);
    attach(CleanSub, clean);
    attach(Sync, sync);
    attach(BlownOn, blown);
    for (int i = 0; i < Count; ++i)
    {
        if ((i >= ColorOn && i <= SafeOutput) || i == Sync || i == BlownOn)
            continue;
        auto &s = sliders[i];
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 19);
        s.setTooltip(specs[i].name);
        labels[i].setText(specs[i].name, juce::dontSendNotification);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setFont(juce::Font(juce::FontOptions(11)));
        if (i < 12)
        {
            addAndMakeVisible(s);
            addAndMakeVisible(labels[i]);
        }
        else
        {
            content.addAndMakeVisible(s);
            content.addAndMakeVisible(labels[i]);
        }
        attachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.params, specs[i].id, s);
    }
    const char *tabNames[] = {"COLOR LAB", "SPECTRAL LAB", "BLOWN OUT", "FX RACK", "GATE / LFO", "SYNTH"};
    for (int i = 0; i < 6; ++i)
    {
        tabs[i].setButtonText(tabNames[i]);
        addAndMakeVisible(tabs[i]);
        tabs[i].onClick = [this, i] { selectTab(i); };
    }
    const char *lockNames[] = {"OSC", "FM", "FILTER", "GATE / LFO", "COLOR", "SPECTRAL", "FX", "SUB"};
    for (int i = 0; i < 8; ++i)
    {
        locks[i].setButtonText(juce::String("LOCK ") + lockNames[i]);
        content.addAndMakeVisible(locks[i]);
        locks[i].onClick = [this, i] { processor.lockGroup(i, locks[i].getToggleState()); };
    }
    viewport.setViewedComponent(&content, false);
    viewport.setScrollBarsShown(true, false);
    sync.setTooltip(
        "On: gate/LFO cycles per host beat. Off: cycles per second. Rates remain independently editable.");
    blown.setTooltip("Editable saturation / crush / sag. Clean sub bypasses this chain.");
    freezeVisuals.setTooltip("Pause the room animation.");
    status.setText("BREZEBASS / ORIGINAL BASS ENGINE", juce::dontSendNotification);
    status.setFont(juce::Font(juce::FontOptions(11)));
    syncUI();
    selectTab(0);
    opened=juce::Time::getMillisecondCounterHiRes();
    startTimerHz(30);
}
NeuroEditor::~NeuroEditor()
{
    processor.audition(false);
    stopTimer();
    setLookAndFeel(nullptr);
}
void NeuroEditor::selectTab(int tab)
{
    selectedTab = tab;
    visibleParams.clear();
    for (int i = WtA; i < Count; ++i)
    {
        bool show = false;
        auto group = specs[i].group;
        if (tab == 0)
            show = group == Colour;
        if (tab == 1)
            show = group == Spectral;
        if (tab == 2)
            show = i >= BlownDrive && i <= BlownClip;
        if (tab == 3)
            show = group == FX && i < BlownOn;
        if (tab == 4)
            show = group == Mod && i != Sync;
        if (tab == 5)
            show = group == Osc || group == FM || group == Filter || group == Sub || group == Global;
        if (!attachments[i])
            show = false;
        sliders[i].setVisible(show);
        labels[i].setVisible(show);
        if (show)
            visibleParams.push_back(i);
    }
    for (int i = 0; i < 6; ++i)
        tabs[i].setColour(juce::TextButton::buttonColourId, juce::Colour(i == tab ? 0xff365063 : 0xff11191f));
    resized();
}
void NeuroEditor::syncUI()
{
    auto p = processor.patchInfo();
    family.setSelectedId(p.selection + 1, juce::dontSendNotification);
    if (!seed.hasKeyboardFocus(false))
        seed.setText(juce::String(p.seed), false);
    seedLock.setToggleState(p.lockSeed, juce::dontSendNotification);
    wtLock.setToggleState(p.lockWave, juce::dontSendNotification);
    for (int i = 0; i < 8; ++i)
        locks[i].setToggleState(p.locks[i], juce::dontSendNotification);
}
void NeuroEditor::result(bool ok, const juce::String &text)
{
    status.setText(ok ? text : "No change: check section locks or history.", juce::dontSendNotification);
    if (ok)
        syncUI();
}
void NeuroEditor::choose(bool saving)
{
    auto f = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                 .getChildFile("BREZEBASS-" + juce::String(processor.patchInfo().seed) + ".brezebass");
    chooser = std::make_unique<juce::FileChooser>(saving ? "Save bass" : "Load bass", f, "*.brezebass");
    juce::Component::SafePointer<NeuroEditor> self(this);
    int flags =
        juce::FileBrowserComponent::canSelectFiles |
        (saving ? (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::warnAboutOverwriting)
                : juce::FileBrowserComponent::openMode);
    chooser->launchAsync(flags,
                         [self, saving](const juce::FileChooser &c)
                         {
                             if (!self || c.getResult() == juce::File{})
                                 return;
                             self->result(saving ? self->processor.savePreset(c.getResult())
                                                 : self->processor.loadPreset(c.getResult()),
                                          saving ? "Preset saved" : "Preset loaded");
                         });
}
void NeuroEditor::resized()
{
    int w = getWidth(), h = getHeight(), left = 160, right = w - 160, span = right - left;
    factory.setBounds(200, 20, w - 520, 28);
    save.setBounds(w - 280, 20, 65, 28);
    load.setBounds(w - 210, 20, 65, 28);
    undo.setBounds(w - 140, 20, 55, 28);
    redo.setBounds(w - 80, 20, 55, 28);
    family.setBounds(left, 82, 240, 30);
    seed.setBounds(right - 255, 82, 145, 28);
    seedGo.setBounds(right - 103, 82, 103, 28);
    seedLock.setBounds(right - 255, 113, 130, 25);
    generate.setBounds(w / 2 - 170, 145, 340, 60);
    mutate.setBounds(w / 2 - 70, 216, 140, 30);
    audition.setBounds(left, 143, 140, 30);
    color.setBounds(left, 183, 115, 30);
    spectral.setBounds(left, 216, 120, 30);
    blown.setBounds(right - 140, 147, 140, 30);
    safe.setBounds(right - 140, 182, 80, 27);
    clean.setBounds(right - 140, 216, 140, 27);
    for (int i = 0; i < 8; ++i)
    {
        int x = left + i * span / 8;
        labels[i].setBounds(x, 267, span / 8, 20);
        sliders[i].setBounds(x, 288, span / 8, 92);
    }
    for (int i = 8; i < 12; ++i)
    {
        int x = left + (i - 8) * span / 4;
        labels[i].setBounds(x, 395, span / 4 - 5, 19);
        sliders[i].setSliderStyle(juce::Slider::LinearHorizontal);
        sliders[i].setBounds(x, 415, span / 4 - 9, 44);
    }
    sync.setBounds(left, 476, 260, 28);
    newWT.setBounds(left + 265, 476, 95, 28);
    wtLock.setBounds(left + 365, 476, 105, 28);
    rhythm.setBounds(right - 140, 476, 140, 28);
    for (int i = 0; i < 6; ++i)
        tabs[i].setBounds(left + i * span / 6, 524, span / 6 - 3, 31);
    viewport.setBounds(left, 568, span, std::max(100, h - 623));
    const int cw = span - 18;
    for (int i = 0; i < 8; ++i)
        locks[i].setBounds((i % 4) * cw / 4, (i / 4) * 26, cw / 4, 24);
    for (size_t n = 0; n < visibleParams.size(); ++n)
    {
        int i = visibleParams[n], x = int(n % 5) * cw / 5, y = 67 + int(n / 5) * 114;
        labels[i].setBounds(x, y, cw / 5 - 4, 30);
        sliders[i].setBounds(x, y + 30, cw / 5 - 6, 79);
    }
    content.setSize(cw, 74 + int((visibleParams.size() + 4) / 5) * 114);
    status.setBounds(25, h - 30, w - 490, 24);
    meters.setBounds(w - 455, h - 30, 295, 24);
    freezeVisuals.setBounds(w - 154, h - 31, 145, 25);
}
void NeuroEditor::paint(juce::Graphics &g)
{
    float t = freezeVisuals.getToggleState()
                  ? 0
                  : float((juce::Time::getMillisecondCounterHiRes() - opened) / 1000);
    room.draw(g, getLocalBounds().toFloat(), t, energy);
    auto panel = juce::Rectangle<float>(145, 64, float(getWidth() - 290), float(getHeight() - 110));
    g.setColour(juce::Colour(0xe5080d12));
    g.fillRoundedRectangle(panel, 18);
    g.setColour(juce::Colour(0xff29343f));
    g.drawRoundedRectangle(panel, 18, 1);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(23).withStyle("Bold")));
    g.drawText("BREZEBASS", 24, 18, 168, 32, juce::Justification::centredLeft);
}
void NeuroEditor::paintOverChildren(juce::Graphics &g)
{
    float age = float(juce::Time::getMillisecondCounterHiRes() - opened);
    if (age >= 500)
        return;
    g.fillAll(juce::Colours::black);
    float t = age / 500, scale = .7f + .3f * (1 - std::exp(-t * 14) * std::cos(t * 17));
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(62 * scale).withStyle("Bold")));
    g.drawText("BREEZEBASS", getLocalBounds(), juce::Justification::centred);
}
void NeuroEditor::timerCallback()
{
    energy += (processor.meter.load() - energy) * .2f;
    syncUI();
    meters.setText("CPU " + juce::String(processor.cpu.load(), 1) + "%  |  " +
                       juce::String(processor.hostBpm.load(), 1) + " BPM",
                   juce::dontSendNotification);
    repaint();
}
