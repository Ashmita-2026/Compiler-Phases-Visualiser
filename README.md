# Modular C Compiler Visualization Tool


## TEAM MEMBERS:
## 1) Misty Grover
## 2) Ashmita Barthwal
## 3) Vanshika Nag

A modular, phase-wise compiler visualization tool that allows users to interactively see how a C program is processed through each stage of compilation.

---

## Overview

This project implements a simplified C compiler with a modular architecture, separating the user interface (UI) from the core compiler logic. The tool is designed to provide a smooth, interactive experience, helping users visualize the transformation of their C code through various compiler phases:

- Lexical Analysis  
- Syntax Analysis  
- Semantic Analysis  
- Intermediate Code Generation

Each phase is implemented as an independent backend module, outputting structured and readable results to aid learning and debugging.

---

## Features

- **Modular Compiler Backend**  
  Implemented in C/C++, each compiler phase works independently, producing detailed outputs.

- **User-Friendly Interface**  
  Two UI options available:  
  - **Desktop GUI:** Built with Python's Tkinter for quick and lightweight interaction  
  - **Web UI:** Built with HTML, CSS, and JavaScript for flexible deployment and modern styling

- **File Upload & Execution**  
  Users can upload `.c` source files, run them through the entire compiler pipeline, and view detailed phase-wise output.

- **Seamless Integration**  
  The frontend interfaces communicate with the backend compiler executable using Python’s `subprocess` module, enabling smooth output handling and error reporting.

---

## Technologies Used

- **Frontend:**  
  - Python Tkinter (Desktop GUI)  
  - HTML, CSS, JavaScript (Web-based UI)

- **Backend:**  
  - C/C++ compiler modules compiled as standalone executable (`main2.exe`)


---


