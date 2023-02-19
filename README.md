# Traveling salesman problem solver (Tabu Search method)

Implementation of the Tabu Search algorithm.

## Prerequisites

### UNIX

TO be able to run the build process and the compilation itself, you should have the following components installed :

* GNU Make
* CMake

### Windows

On Windows, you can use this project in MSVS, only if you have the CMake component installed.

## Compilation

### UNIX

To build the application on UNIX-like systems, run the following commands (assuming, you are in the project's directory):

```bash
mkdir build && cd build
cmake ..
make
```

### Windows

To build this project on Windows, you should have Microsoft Visual Studio with installed CMake component (it can be enabled in Visual Studio Installer). After the installation, the project will be automatically build upon opening.

## Usage

### Configuration

This program accepts config.ini files, which should have similar content to be understood (<> symbols highlight the places, which should be filled by the user, please, don't include them in your parameters):

```
[<name_of_the_testcase>]
filename=<path_to_the_tsp_file>
count=<amount_of_repeats>
max_tabu=<the_maximum_size_of_the_tabu_list>
max_iterations=<the_size_of_the_epoch_iterations>
time_limit=<time_limit_of_the_calculation_in_ms>
[output]
filename=<path_to_the_output_file>
```

The configuration file should be placed in the same folder as the executable file!

### Input files

The input files should have the extension *.tsp and be formatted in the similar way :

```
NAME: br17
DIMENSION: 17
EDGE_WEIGHT_SECTION
9999 3 5 48 48 8 8 5 5 3 3 0 3 5 8 8 5
3 9999 3 48 48 8 8 5 5 0 0 3 0 3 8 8 5
5 3 9999 72 72 48 48 24 24 3 3 5 3 0 48 48 24
48 48 74 9999 0 6 6 12 12 48 48 48 48 74 6 6 12
48 48 74 0 9999 6 6 12 12 48 48 48 48 74 6 6 12
8 8 50 6 6 9999 0 8 8 8 8 8 8 50 0 0 8
8 8 50 6 6 0 9999 8 8 8 8 8 8 50 0 0 8
5 5 26 12 12 8 8 9999 0 5 5 5 5 26 8 8 0
5 5 26 12 12 8 8 0 9999 5 5 5 5 26 8 8 0
3 0 3 48 48 8 8 5 5 9999 0 3 0 3 8 8 5
3 0 3 48 48 8 8 5 5 0 9999 3 0 3 8 8 5
0 3 5 48 48 8 8 5 5 3 3 9999 3 5 8 8 5
3 0 3 48 48 8 8 5 5 0 0 3 9999 3 8 8 5
5 3 0 72 72 48 48 24 24 3 3 5 3 9999 48 48 24
8 8 50 6 6 0 0 8 8 8 8 8 8 50 9999 0 8
8 8 50 6 6 0 0 8 8 8 8 8 8 50 0 9999 8
5 5 26 12 12 8 8 0 0 5 5 5 5 26 8 8 9999
```

Please, avoid double-spaces and other formatting symbols. Use only single spaces.

#### Output files

The output files will have the *.csv extension. The content of the file will have a similar look :

```
name_of_the_tescase_1
time_in_microseconds, used_memory_in_kbytes
...

name_of_the_tescase_2
time_in_microseconds, used_memory_in_kbytes
...
```
