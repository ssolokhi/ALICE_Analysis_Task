# ALICE Analysis Task Example

---

## Description

The following code presents an example of data analysis macros used for ALICE Run 3 (2022 to this day) data. 

THe analysis procedure is coded using C++ by creating a structure **AnalysisExample** that has four main parts:

- definition of configurables and filters;

- function **init**, where histograms are defined;

- function **processCollisions**, where general information on collisions is obtained;

- function **processVerteces**, where one studies properties of neutral decay verteces.

    
## Repository Content
    
This repository contains several files:
    
- This **README** file;

- **AnalysisExample.cxx**, containing the analysis procedure's description (namely, the structure's methods);

- **config.json**, containing the configuration settings of the analysis;

- a bash script **run.sh**, used to initialize the analysis.

The full code lisence can be found in *AnalysisExample.cxx*.


## Prerequisites

In order to use the following code one must first install [O2 and O2Physics](https://aliceo2group.github.io/analysis-framework/docs/gettingstarted/installing.html) via [AliBuild](https://alice-doc.github.io/alice-analysis-tutorial/building/custom.html). After installing O2Physics it can be verified by typing
```
alienv enter VO_ALICE@O2Physics::latest
```
If no errors appear, the installation has been successful.

## Usage

> [!TIP]
> In order to avoid the necessity to add a cmake file to the O2Physics repository, one can simply place the analysis code into some unused *.cxx* file already present there, e.g. replace the contents of **~/O2Physics/Tutorials/PWGMM/myExampleTask.cxx** with the new code. The build can then be completed withoud modifying the cmake files. ROOT, JSON, and bash files do not require any extra actions either way

### Code Modification

Any modification of the code requires a rebuild of the existing installation of O2Physics. To do that, move to the directory where O2Physics is installed and run the following command:
```
aliBuild build O2Physics --debug
```
This will (the *debug* option allows to get a full backtrace, shall an error occur). If the build is successful, one can run the analysis.

### Running Code Locally


Move to the folder where your code is located. Running locally requires having an input file of type **AO2D.root**. Enter the O2Physics shell by typing 
```
alienv enter VO_ALICE@O2Physics::latest
```
Now run the bash script triggering the analysis (this may require changing the ownership of the file via *chmod 777*):
```
./run.sh
```
This will initialize the analysis by invoking relevant tables and using the settings from the configuration file.

After the execution is over, the folder will contain a **AnalysisResults.root** file containing all output histograms and tables.

## Further Reading
Please refer to the [O<sup>2</sup> Analysis Framework Documentation](https://aliceo2group.github.io/analysis-framework/docs/), for detailed analysis and troubleshooting instructions.