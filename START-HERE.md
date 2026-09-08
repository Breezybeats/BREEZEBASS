# Your new GitHub build — easiest route

Use a **new repository**, e.g. `brezebass`, so your existing NEUROJ build stays intact.

1. Unzip this package and open the **BREZEBASS-GitHub** folder.
2. Create the repository on GitHub. Add a README when creating it.
3. Choose **Add file → Upload files**. Upload **every file INSIDE BREZEBASS-GitHub**, not the ZIP and not the enclosing folder. All C++ source/test files intentionally live at the top level this time. Commit changes.
4. In Finder press **Command + Shift + .** to show `.github`. Upload this folder too if it wasn't included.
5. If the workflow is missing, choose **Add file → Create new file**. In the filename box paste the following plain path, without quotation marks or backticks:

```text
.github/workflows/build-mac.yml
```

Open **WORKFLOW-COPY.txt** from this package, copy its entire contents into GitHub's editor, and commit.

6. Open **Actions → Build BREZEBASS Mac M1 VST3**. Click the newest run. If necessary choose **Run workflow**.
7. After the green check, open that run's **Summary**, scroll to **Artifacts**, and download **BREZEBASS-Mac-M1-VST3**.
8. Unzip that download, then unzip the inner **BREZEBASS-Mac-M1.zip**. Follow INSTALL-MAC.md to install **BREZEBASS.vst3**.

The build timeout is 40 minutes. It checks uploaded files up front, uses the corrected `lipo` argument order, and checks host latency **after** audio preparation. Do not remove those validation gates to get a green build. Send the first failed-step message if anything fails.

Public standard GitHub runners are free; a private repository uses your included Actions allowance and account billing settings. Choose visibility based on whether you want the source public. You do not have to publish it publicly.

[Official GitHub runner documentation](https://docs.github.com/en/actions/reference/runners/github-hosted-runners)

## Files you receive from the successful cloud build

- BREZEBASS.vst3 — the actual Apple Silicon plugin bundle.
- Factory-Presets — 150 `.brezebass` preset files, five per named family.
- BREZEBASS-UI.png and BREEZEBASS-SPLASH.png — snapshots from the actual JUCE editor.
- Audio-preview.wav — synthesized audio demonstration.
- Installation guide, source archives and dependency notices.

The standalone target is also compiled, but the packaged download focuses on the requested VST3.
