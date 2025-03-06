# 🖥️ Introduction to Operating Systems - Linux  

This repository contains C code from four exercises related to **process management** and **interprocess communication (IPC)** in Linux. Each exercise demonstrates a key concept, building towards a deeper understanding of Linux process management.  

---

## 📚 Lesson Overview  

The goal of these exercises was to explore how Linux handles processes and how they communicate with each other. The key concepts covered include:  

- ⚙️ **Process creation** using `fork()`  
- 👨‍👩‍👦 **Parent and child process relationships**  
- ⏳ **Process synchronization** using `wait()`  
- 🔗 **Interprocess Communication (IPC) methods**, including:  
  - 📡 **Pipes** – Simple data exchange between processes  
  - 🚦 **Signals** – Process notifications and control  
  - 🌐 **Sockets** – Server-client communication  
  - 📮 **Message Queues** – Structured message passing  
  - 🗂️ **Shared Memory** – Fast data sharing between processes  

---

## 🔑 Key Concepts Learned  

### 1️⃣ Process Creation and Execution  
- Using `fork()` to create child processes  
- Executing new programs with `exec()`  
- Managing multiple processes in a single program  

### 2️⃣ Process Synchronization  
- Using `wait()` to ensure correct execution order  
- Avoiding **zombie and orphan** processes  
- Understanding **race conditions** and preventing them  

### 3️⃣ Interprocess Communication (IPC)  
- **Signals** – Handling events such as `SIGINT` and `SIGTERM`  
- **Pipes** – Unidirectional communication between related processes  
- **Sockets** – Establishing server-client communication  

### 4️⃣ Practical Applications  
- Implementing a **producer-consumer model**  
- Creating a **command-line utility** that spawns child processes  
- Using **signals** to manage process behavior dynamically  

---

## 📂 Repository Contents  

- `exercise1.c` – Basic process creation and execution  
- `exercise2.c` – Signal handling and process control  
- `exercise3.c` – Using pipes for communication  
- `exercise4.c` – Implementing sockets and server communication  

Feel free to explore the code, and reach out if you have any questions! 🚀  
