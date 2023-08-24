# RTNPlugIn - A parametric distortion neural effect
## Introduction 
In this project for the "Selected Topics in Music and Acoustic Engineering" course, we presents RTNPlugIn, a parametric neural effect for distortion. The Plug In is implemented in C++ using the JUCE framework for reliability and speed. For what concerns the management of the neural networks, we decided to go with RTNeural, a lightweight neural network inferencing engine. 
## Training
For the training procedure, we used Google Colab for generating the models and Google Drive for storing the .json files. 
The training pipeline is the following: 
- clone this repository
- upload the "TRAIN" folder on Google Drive
- in the "parametric_data.txt" file you can find the data folder for the training procedure
- open the trainParametric.ipynb in Colab
- insert the paths for your "TRAIN" folder and your "parametric_data" folder
- train 
## GUI 
For what concerns the Graphical User Interface, it is composed by: 
- an On/Off button that enables or disables the effect
- a knob that controls the amount of effect to apply (in our case, the distortion)
## Requirements and Usage
### Requirements
- clone this repository
- download RTNeural here: https://github.com/jatinchowdhury18/RTNeural
- download JUCE here: https://juce.com/download/
- put the RTNeural folder inside the repository folder
- open the CmakeLists.txt file, and insert your JUCE path at line 7
- in the src folder, open the PLugInProcessor.cpp, and insert the model "modelParametricDIST16.json" (you can find it into the "Models" folder) path at line 26
- from terminal, go to the plug in folder and run: cmake --build build --config Release
- from the build folder created in thge previous point, go to the Release folder and find the .vst3 file
### Usage
The default model is the one using 16 hidden layers. In case you want to try the other models (8 or 24 layers) you have to edit the code: 
- in the src folder, open the PLugInProcessor.cpp, and insert the new model path at line 26
- in the src folder, open the PLugInProcessor.cpp, replace 16 with either 8 or 24 at line 202
- in the src folder, open the PLugInProcessor.h, replace 16 with either 8 or 24 at lines 57 and 63 
