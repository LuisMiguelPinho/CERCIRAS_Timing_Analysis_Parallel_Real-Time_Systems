# Timing Analysis - Methodology and Tools

This lecture explains a methodology for timing analysis, as well as a set of tools used for this purpose. A [Dockerfile](./Dockerfile) is provided to ease the installation of the necessary tools.

## Requirements
The following are the requirements to build a Docker image and run a Docker container for this class:

* Docker engine: [Install Docker Engine](https://docs.docker.com/engine/install/)
* Internet access
* About 2Gb of free memory 
## Suggested Tools for the Host Machine
These are the tools the lecturers suggest to use to ease the development during the lecture:

* Visual Studio Code with suggested C/C++ plugins: [Visual Studio Code - Mac, Linux, Windows](https://code.visualstudio.com/download)
* Docker plugin to easily work inside the docker container: [Docker extension](https://code.visualstudio.com/docs/containers/overview)
* Graphviz (dot) plugin for VS Code for task-dependency graph visualization [Graphviz language support](https://marketplace.visualstudio.com/items?itemName=joaompinto.vscode-graphviz)
* Web browser to visualize the tools (e.g. Chrome)

## Dockerfile contents
The docker file, upon build, will install an Ubuntu 22.04 LTS distribution with the necessary tools for the course, which will be explained during the course. More specifically, the container will have:

* PAPI: API for runtime performance counter extraction (an Extrae dependency)
* Extrae: monitor and generate traces of parallel applications
* BSC’s LLVM: adapted version of LLVM able to generate task-dependency graphs (TDGs) and
specify static task-to-thread mapping
* tdg-instrumentation-script: python script that converts extrae’s output into TDGs in json format
* Time-predictability: a framework for the analysis of TDGs and calculate timing-related metrics

## Building the Docker image

To build the docker image, two alternatives exist. 

The first option is to use one of the "build" scripts provided in the same directory level as the Dockerfile. Otherwise run the build command manually. 

The building process takes a long time (around 30 min), as one of the tasks is the complete build of LLVM. Therefore, it is ideally to start this process as soon as possible.

### Windows
Simply execute the command in the root folder:

```powershell
.\build.bat
```

Or, if you prefer to do it manually, run:
```powershell
docker build -t timing_analysis_course:latest .
```

### Ubuntu
Simply execute the command in the root folder:

```bash
./build
```

Or, if you prefer to do it manually, run:
```bash
docker build -t timing_analysis_course:latest .
```
## Running a Docker container

To run the container there are two options. The first option is to simply use one of the "run" scripts, while the second option is to run a docker command manually. The script will start a docker container and automatically open a terminal inside the container. This container will be kept alive while this terminal is openned.

After starting the container, we suggest the reader to  keep the terminal open and to "Attach" the container directly Vscode via the suggested plugin. This will open vscode inside the container and will allow the developer to program directly in the machine. More instructions, and how to attach to the running container, is provided in the slides and videos of the course.    

### Windows
Simply execute the command in the root folder:

```powershell
.\run.bat
```

Or, if you prefer to do it manually, run:
```powershell
docker run -w /home/cts/course --rm -ti timing_analysis_course:latest
```

### Ubuntu
Simply execute the command in the root folder:

```bash
./run
```

Or, if you prefer to do it manually, run:
```bash
docker run -w /home/cts/course --rm -ti timing_analysis_course:latest
```
