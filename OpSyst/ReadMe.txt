                               INTRODUCTION OPERATING SYSTEMS  -  LINUX

This repository contains C code from four exercises related to process management and interprocess communication (IPC)
in Linux.

--------------------------------------------------
PROJECT OVERVIEW
--------------------------------------------------

The goal of these exercises was to understand how Linux handles processes and how they communicate with each other.
The key concepts covered include:

- Process creation using fork()
- Parent and child process relationships
- Process synchronization using wait()
- Interprocess communication (IPC) methods:
  - Pipes (for simple data exchange between processes)
  - Signals (for process notifications and control)
  - Message Queues (for structured message passing)
  - Shared Memory (for fast data sharing between processes)
  

--------------------------------------------------
KEY CONCEPTS LEARNED
--------------------------------------------------

1. Process Creation and Execution
   - Using fork() to create child processes.
   - Executing new programs with exec().
   - Handling multiple processes in a single program.

2. Process Synchronization
   - Using wait() to ensure correct execution order.
   - Avoiding zombie and orphan processes.
   - Understanding race conditions and how to prevent them.

3. Interprocess Communication (IPC)
   - Pipes: Simple unidirectional communication between related processes.
   - Message Queues: Sending structured messages between unrelated processes.
   - Shared Memory: Fast data exchange by mapping memory between processes.
   - Signals: Notifying processes of events such as SIGINT and SIGTERM.

4. Practical Applications
   - Implementing a simple producer-consumer model.
   - Creating a basic command-line utility that spawns child processes.
   - Using signals to control process behavior dynamically.

--------------------------------------------------
CONTENTS
--------------------------------------------------

- exercise1.c – Basic process creation and execution.
- exercise2.c – Using pipes for communication.
- exercise3.c – Implementing message queues and shared memory.
- exercise4.c – Signal handling and process control.

Each exercise demonstrates a key concept, building towards a deeper understanding of Linux process management.

Feel free to explore the code, and do not hesitate to reach out if you have any questions!
