# Process Scheduler

## Description
- This is a program that simulates the operation of the process scheduler in a multiprocessor system and calculates some related statistics in order to help improce the overall scheduling system.
- This program was written in C++ as our second programming project for Cairo University.

## Simulation
### I. Processes states
- **Early**: A process that didn't pass its deadline time.
- **Late**: A process that missed its deadline time.
- **RR migration**: A process that migrated from a FCFS processor to a RR processor.
- **SJF migration**: A process that migrated from a RR processor to a SJF processor.
- **Parent**: A process that is a parent of a forked process.
- **Child**: A process that was forked in a FCFS processor.  
![States](https://github.com/Mo2Hefny/Process_Scheduler/assets/111001850/1d90e83e-5a2f-4ac0-ae1d-528fd9158019)  

### II. Input file  
![Input](https://github.com/Mo2Hefny/Process_Scheduler/assets/111001850/b4ca8f92-6afd-46f7-8163-477c9a7923ad)  

### III. Simulation
#####  Assumptions
- A process can encounter more than one state transition in the same timestep.
- Process Migration and Work Stealing is not applicable for forked processes.  
##### Scheduling algorithms
- **FCFS**: First come first serve scheduling algorithm states that the process that requests the CPU first is allocated the CPU first.
- **SJF**: Shortest job first scheduling algorithm that selects the waiting process with the smallest execution time to execute next.
- **RR**: In Round Robin algorithm the CPU will divide the time for each process given the same time until the entire process in the queue is complete.
- **EDF**: Earliest Deadline First is an optimal dynamic priority scheduling algorithm. It assigns priorities to the task according to the absolute deadline.  
##### At each timestep
- Checks for transitions from the NEW list to the appropriate RDY list.
- Handle work stealing and process migrations.
- Move processes requesting I/O from RUN state to BLK list.
- Move processes that finish IO_D from BLK list to the appropriate RDY list.
- Move processes that finish execution from RUN state to TRM list.
- Handle process forking.
- Handle process killing and orphan processes.
- Collect statistics for the output file.  
![Simulation](https://github.com/Mo2Hefny/Process_Scheduler/assets/111001850/7136eb19-0f1a-494d-a8e7-d7aa5365e260)  

### IV. Output file
Contains the simulation statistics and it can be broken down to two parts:
###### - **Processes statistics** :
1. Average waiting time, response time, and turnaround duration.
2. FCFS to RR migration and RR to SJF migration.
3. Early, stolen, forked, and killed processes.  
###### - **Processors statistics** :
1. Processors load.
2. Processors utilization.  
![Output](https://github.com/Mo2Hefny/Process_Scheduler/assets/111001850/e8c02b25-eb73-401d-98b7-0b63e036cc7d)  

## Authors

- [@Moamen Hefny](https://github.com/Mo2Hefny)
- [@nourabouzeid](https://github.com/nourabouzeid)
- [@MHW](https://github.com/Monwilliam)


## Installation

Clone this repository, Open and run the .sln file.

## Environment

This program was built and tested on Visual Studio 2022.
