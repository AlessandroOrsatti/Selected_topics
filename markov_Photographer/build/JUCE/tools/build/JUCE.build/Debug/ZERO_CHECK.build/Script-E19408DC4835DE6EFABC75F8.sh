#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools
  make -f /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools
  make -f /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools
  make -f /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools
  make -f /Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi

