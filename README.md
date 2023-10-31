# Timing Analysis of Parallel Real-Time Systems <br> Short Course - CERCIRAS Training School 2023


This repository provides the contents for the short course on Timing Analysis of Parallel Real-Time Systems, given at the [CERCIRAS Training School 2023](https://www.cerciras.org/cerciras-training-school-2023/).

## Outline

Real-time systems are those systems where the computing system needs to provide results within specific time intervals (deadlines), emanating from application requirements. The development of real-time applications is a challenging task, as it requires guaranteeing functional correctness as well as application-dependent non-functional requirements. This is even more challenging when considering high-performance real-time applications executing on parallel processors. 

When considering parallel-based applications, it is important to analyse and understand the impact that the mapping and scheduling of computation have on the real-time response of the application. In fact, different strategies for the mapping of the computation to the parallel threads may produce significantly different interference, leading to different timing behaviour. Tuning the application and system parameters proves to be one of the most fitting solutions. Nevertheless, the design space can be very cumbersome for a developer to verify manually all possible configurations. 

This course will provide a methodology, developed at the School of Engineering of the Polytechnic Institute of Porto (ISEP) in the scope of the AMPERE European project, to profile, analyse and configure the timing behaviour of high-performance cyber-physical applications and target platforms. The methodology leverages on the possibility to generate a task dependency graph from the parallel computation, using this information to evaluate, through measurements, different mapping configurations, selecting the one that minimizes applications' response time.

The provided toolset, includes a design space exploration process, developed by ISEP, using as external tools a custom tailored LLVM compiler and a profiling tool using performance counters, both from the Barcelona Supercomputing Center.


## Introduction to real-time parallel models

The methodology for the design of real-time systems has been challenged due to the increasing processing requirements of modern cyber-physical systems, and the corresponding use of fine-grained parallelism. This topic provides a general overview of real-time parallel computing, including the use of parallel programming models in real-time systems, and the vertical stack of mapping and scheduling parallel computation.

Lecture: [Presentation](./Resources/Brief%20Review%20of%20Real-Time%20and%20Parallelism.pdf) | [Video](./Videos/Brief%20overview%20of%20real-time%20systems.mp4)


## Using the OpenMP Tasking Model for Parallel Real-Time Systems

This lecture/laboratory focuses on the tasking model of OpenMP, and on how this model can be used to develop parallel programming amenable to real-time analysis.  

Lecture: [Presentation](./Resources/Using%20OpenMP%20Tasking%20Model%20for%20Real-time%20Parallel%20Systems.pdf) | [Video - Part 1](./Videos/Using%20OpenMP%20Tasking%20Model%20for%20Real-time%20Parallel%20Systems%20-%20Part%201.mp4) [Video - Part 2](./Videos/Using%20OpenMP%20Tasking%20Model%20for%20Real-time%20Parallel%20Systems%20-%20Part%202.mp4)

Laboratory: [Guide](./Resources/Laboratory%20Guide%20-%20OpenMP%20Tasking%20Model.pdf) | [Source Code](./OpenMP_Exercises/Incomplete/) | [Solutions](./OpenMP_Exercises/Solved/)


## Methodology and Toolset for Timing Analysis

In the third lecture/laboratory we explain the methodology for timing analysis, by means of a toolset aiming for Task-based OpenMP parallel applications. 

We divide this lecture in three parts: introduction and installation, compilation phase, and timing analysis. The following sections explain how the resources are used in each part.

**Introduction and Installation**

In this part we explain the methodology for timing analysis and how to build the dockerfile and run a docker container.

The following are the resources necessary for this part of the lecture:

+ Presentation: [3. Methodology and Toolset - 1 intro.pdf](./Resources/3.%20Methodology%20and%20Toolset%20-%201%20intro.pdf)
+ Video: [lecture 3 - 1 intro.mp4](./Videos/lecture%203%20-%201%20intro.mp4)
+ Programming: [Dockerfile directory](./Timing_Analysis_Tools/)

The tools are built in a docker environment. Although not necessary, it is also possible to install the tools manually: [Guide](./Resources/Manual%20Install%20Guide.pdf).

**Compilation**

In this phase we explain how to use the tools to instrument the target application and obtain the results in JSON format, organizing the results in a Task-Dependency Graph.

The following are the resources necessary for this part of the lecture:

+ Presentation: [3. Methodology and Toolset - 2 compiling and profiling.pdf](./Resources/3.%20Methodology%20and%20Toolset%20-%202%20compiling%20and%20profiling.pdf)
+ Video: [lecture 3 - 2 compilation.mp4](./Videos/lecture%203%20-%202%20compilation.mp4)
+ Programming: VSCode attached to the running container 


**Timing analysis** 

In this phase we explain how to use the tools to analyse the results obtained in the previous phase and how to use the analysis to provide a static task-to-thread mapping, optimized for timing and for time predictability.

The following are the resources necessary for this part of the lecture:

+ Presentation: [3. Methodology and Toolset - 3 analysis and static mapping.pdf](./Resources/3.%20Methodology%20and%20Toolset%20-%203%20analysis%20and%20static%20mapping.pdf)
+ Video: [lecture 3 - 3 analysis_and_static_mapping-part1.mp4](./Videos/lecture%203%20-%203%20analysis_and_static_mapping-part1.mp4) and [lecture 3 - 3 analysis_and_static_mapping-part2.mp4](./Videos/lecture%203%20-%203%20analysis_and_static_mapping-part2.mp4)
+ Programming: VSCode attached to the running container 


## References

Projects:
- P-SOCRATES Project: [p-socrates.github.io](https://p-socrates.github.io)
- AMPERE Project: [ampere-euproject.eu](https://ampere-euproject.eu/)

Book (Open Access): 
[High-Performance and Time-Predictable Embedded Computing](https://www.riverpublishers.com/research_details.php?book_id=507) - [PDF](https://www.riverpublishers.com/download.php?file=RP_E9788793609624.pdf)

Paper:
[Framework for the Analysis and Configuration of Real-Time OpenMP Applications](https://ieeexplore.ieee.org/abstract/document/10218276)

Some contents of the course are provided in the [Critical Computing Systems Engineering Master](https://www.isep.ipp.pt/mescc) at ISEP.

## License

Copyright: ISEP, Polytechnic Institute of Porto

Authors: Luis Miguel Pinho, Tiago Carvalho


Shield: [![CC BY 4.0][cc-by-shield]][cc-by]

This work is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg