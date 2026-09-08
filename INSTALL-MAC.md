# Install BREZEBASS on Mac M1

Requires native Apple Silicon FL Studio and macOS 11+. This build is arm64, not Intel/Rosetta. Keep initial monitoring volume low.

1. Quit FL Studio.
2. In Finder press **Command + Shift + G**. Enter `~/Library/Audio/Plug-Ins/`.
3. Open `VST3`, creating that folder if missing.
4. Copy the entire **BREZEBASS.vst3** bundle into `VST3`.
5. Open FL Studio → **Options → Manage plugins**. Enable verification and rescan plugins with errors if needed, then **Find installed plugins**.
6. Add **BREZEBASS** to the Channel Rack as an instrument. Do not double-click the VST3 expecting a standalone app.

## macOS approval

The cloud workflow ad-hoc signs the plugin; it is not Apple-notarised. It may trigger the same security warning as NEUROJ. After a blocked load, use **System Settings → Privacy & Security → Open Anyway**, then reopen FL Studio and rescan.

If that does not work and you intentionally want to approve your own compiled bundle: open Terminal, type the following with a space after `quarantine`, drag the **actual BREZEBASS.vst3 in Finder** into Terminal, then press Return:

```sh
xattr -dr com.apple.quarantine 
```

Dragging inserts the actual path, avoiding the earlier “No such file” problem. This removes only the selected bundle's download quarantine, does not malware-scan it, and does not disable Gatekeeper globally. If permission or signature errors remain, send the exact message; do not add sudo or disable system security globally.

## First sounds

Select a family, hold a low MIDI note, press GENERATE. Alternatively use AUDITION, then switch it off before using MIDI clips. Use the 150-sound menu for five saved recipes per family.

COLOR and SPECTRAL have separate enable buttons and editing tabs. For gating, open GATE / LFO and raise Gate Depth; choose Gate Rate, Duty and Edge. Switch SYNC on to follow FL Studio BPM. LFO/gate phases retrigger per note; they do not align to absolute transport position. Blown Out has its own enable and tab.

SAVE writes a `.brezebass` preset including wave samples and settings. LOAD accepts this version's presets. Old NEUROJ presets are intentionally not interchangeable, and existing NEUROJ projects should keep using the old plugin.

The splash reads **BREEZEBASS**, while the plugin/filename is **BREZEBASS**, as requested.

References: [Image-Line plugin installation](https://www.image-line.com/fl-studio-learning/fl-studio-online-manual/html/basics_externalplugins.htm), [Apple security approvals](https://support.apple.com/en-us/102445).
