# S0012E Engine

A lightweight engine for assignments in S0012E.
This engine is a work in progress, and things might not work as intended. Bugs and/or instabilities are to be expected.

## Example project setup

Download Pod Racer assets & place all files from the unzipped folder into: {project_root}/bin/assets/pod_racer
(If the folder pod_racer does not exist, create it)

Download found here under creative commons liscense:
https://kenney.nl/assets/space-kit


Download FreeSans font & place the .ttf file into: {project_root}/bin/assets/fonts
(If the folder 'fonts' does not exist, create it)

Download found here under GNU GPL:
https://github.com/opensourcedesign/fonts/blob/master/gnu-freefont_freesans/FreeSans.ttf

We still use some of the placeholder Space assets, so for now, still download the assets from the LTU assignment & place them in a space_deprecated folder inside the asset folder


## Once set-up as above, run the executable from the terminal while in {project_root}/bin/


## Build/Deployment target - Raspberry Pi 4

Our RPI 4 is running:
* cmake version 3.18.4
* g++ compiler version 10.2.1


## Requirements

* CMake
* Compiler (tested on MSVC, GCC)

## Changing the chunk data
* Download [Flatbuffers version 23.3.3](https://github.com/google/flatbuffers/releases/tag/v23.3.3) and extract the executable somewhere you can access it.
    * Alternatively compile it from this project.
* `cd` into `bin/assets/chunk_data`
* Run the `flatc` executable like this for every JSON file you've modified in `bin/assets/chunk_data`:
    * `[FLATC_PATH] --binary [REPO_ROOT]/projects/EJPodRacer/code/chunkdata.fbs [REPO_ROOT]/bin/assets/chunk_data/[CHUNK_NAME].json`
