# ALICE Analysis Task Example

---

## Description

The following code presents an example of data analysis macros used for ALICE Run 2 data. A future modification might include usage of the O<sup>2</sup> (Online-Offline) ALICE system. 
    
## Repository Content
    
This repository contains several files:
    
- This **README** file;
    
- A header (**AliAnalysisTaskMyTask.h**) file containing class methods' prototypes required for the analysis;
    
- A methods' implementation (**AliAnalysisTaskMyTask.cxx**) files corresponding to the header files;
    
- A class instance (**AddMyTask.C**) macro;
    
- A so called *steering* (**runAnalysis.C**) macro for defining JAliEn instructions for running on GRID.

The full code lisence can be found in *AliAnalysisTaskMyTask.cxx*.

## Prerequisites

In order to use the following code one must first install [AliPhysics](https://github.com/alisw/AliPhysics) via AliBuild. A complete guide for AliBuild download, installation and build can be found here: https://alice-doc.github.io/alice-analysis-tutorial/ (*please mind the platform-specific package requirements!*). After installing AliPhysics it can be verified by typing *alienv enter AliPhysics/latest*. If no errors appear, the installation has been successful.

## Usage

After entering the AliPhysics environment, the steering macros can be rub by typing *aliroot runAnalysis.C*.

The code must first be trouble-shot locally first. In order to run locally, the *localRun* option in the steering macro to *true*. An *AliAOD.root* file is required; it can be downloaded from any ALICE run folder (the easiest way to download it is via [AliMonitor](https://alimonitor.cern.ch/) from */alice/data/\**).

When all errors (and preferably warnings as well) are fixed, the code can be tested on GRID by setting the *gridTest* option in the steering macro to *true* (and the *localRun* option to *false*). For this repository's code 1 test file will be used. **Do not forget to use an up-to-date version of AliPhysics with JAliEn!**

> When this **README** file was written, the optimal AliPhysics version was *vAN-20201115_JALIEN-1*

After the code is tested on GRID, set the *gridTest* option to *false* and run the steering macros again. Several new files will be created: *.xml* collections for runs' data, *.sh* executables and merging *.C* macros. Your files will be copied to your GRID working directory. You can monitor the progress of your analysis with [AliMonitor](https://alimonitor.cern.ch/). 

When all GRID jobs are done, change the *SetRunMode* option to *terminate* at the end of the steering macro and run it. That will create merging jobs that will merge a lot of analysis results' files into a single *.root* file for each run. *All finished jobs will be killed automatically!* 

The last time the macro must be run with the *SetMergeViaJDL* option set to *false*. This will download a single *.root* file containing all analysis results on your PC/laptop. After that, the resulting file can be studied with additional ROOT macros.

## Gratitude

I am greatly thankful to Redmer Bertens, the author of the [ALICE Analysis Tutorial](https://alice-doc.github.io/alice-analysis-tutorial/), for detailed analysis instructions and for prompt responces to code modification proposals.

