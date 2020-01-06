JUCE UI component for a MUltiple Stimulus Hidden Anchor (MUSHRA) style perceptual evaluation [1] in C++.

Randomises the order in which audio scenes (i.e. source materials) are presented, and the association between slider number and stimulus for each scene.

![alt text][https://github.com/chrisyeoward/mushra-component/raw/master/src/assets/interface.png "User Interface"]


Consists of:
- MushraComponent: UI component that controls which stimulus is playing, and records the results at the end of session
- MushraProcessor: Maintains the scene order, which scene is currently loaded, and which stimulus is playing ,

# Installation

1. Clone repo.
2. Open Projucer VST project.
3. Drag cloned folder projucer file explorer
4. Add cloned folder path to Exporters > Xcode > Debug/Release > Header Search Paths
5. Include processor in AudioProcessor source: `#include <MushraProcessor.h>`
6. Include ui component in AudioProcessorEditor source: `#include <MushraComponent.h>`

# Usage
## MushraComponent

Treat the MushraComponent like any other JUCE UI component.

The constructor takes 3 parameters:
1. The AudioProcessor
2. number of sliders to display, equivalent to the number of stimuli under test.
3. the directory in which to save the results


<!-- show/hide reference button -->

## MushraProcessor

The AudioProcessor inherits from this class.

Takes two constructor parameters:
1. Number of stimuli under test.
2. Number of different audio scenes/source materials that the stimuli are derived from

The AudioProcessor decides what audio to play based on the properties:

```
int selectedStimulus
int currentScene
```

Which are controlled by changes to the MushraComponent.


# References
[1] Method for the subjective assessment of intermediate quality level of audio systems (ITU-R BS.1534-3)

# Licence
This software uses the JUCE C++ framework, which is under GPL license. More information: https://github.com/WeAreROLI/JUCE
