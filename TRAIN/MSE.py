
import numpy as np
import soundfile as sf
import os

desired_samplerate = 44100

#Original file 
Original_path = "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/OUTPUTS/TARGET_GUITAR_0.8.wav"
assert os.path.exists(Original_path), "Audio folder  not found. Looked for " + Original_path
Original, samplerate = sf.read(Original_path, dtype='float32')

if samplerate != desired_samplerate:       
    print("load_wav_file warning: sample rate wrong, resampling from ", samplerate, "to", desired_samplerate)
    Original = sf.resample(Original, target_samplerate=desired_samplerate)

if len(Original.shape) > 1 and Original.shape[1] > 1:
    Original = Original[:, 0]

Original = np.array(Original)

#Test files
test_folder = "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/OUTPUTS/TEST"
assert os.path.exists(test_folder), "Audio folder  not found. Looked for " + test_folder

test_list = []
for file in os.listdir(test_folder):
    if file.endswith(".wav"):
        test_list.append(os.path.join(test_folder, file))

test = []
for file_path in test_list:
    data, samplerate = sf.read(file_path, dtype='float32')

    if samplerate != desired_samplerate:       
        print("load_wav_file warning: sample rate wrong, resampling from ", samplerate, "to", desired_samplerate)
        data = sf.resample(data, target_samplerate=desired_samplerate)

    if len(data.shape) > 1 and data.shape[1] > 1:
        data = data[:, 0]
        
    test.append(data)
    

#MSE
counter = 1
for sample in test:
    sample = np.array(sample)
    mse = (np.square(Original - sample)).mean()
    print("The mse for the test file number", counter, "is equal to: ", mse)
    counter = counter + 1
    
    