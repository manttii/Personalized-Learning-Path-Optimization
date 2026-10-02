# Personalized Learning Path Optimization
### Dynamic DAG Knowledge Mapping & Dijkstra Adaptive Trajectory Engine
**Department of Computer Science & Engineering | Graphic Era (Deemed to be University), Dehradun**  
**Course:** Project-Based Learning (PBL) | **Academic Session:** 2026–27 | **Semester:** 3rd (2nd Year B.Tech CSE)  
**Team ID:** `DSCPP-III-2026-T284` | **Domain:** AI & Adaptive Educational Systems  
**Mentor:** Ram ji Chauhan

---

[![C++14/17](https://img.shields.io/badge/C%2B%2B-14%2F17-blue.svg)](https://isocpp.org/)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Graphic Era University](https://img.shields.io/badge/Graphic%20Era-PBL%20Phase--I%20%26%20II-red.svg)](https://www.geu.ac.in)

---

## 👥 Team Members & Roles

| S.No. | Student Name | Student ID | Project Role | Core Contribution |
| :--- | :--- | :--- | :--- | :--- |
| 1 | **Priyanshi Saini** | `2510011893` | **Team Lead / Algorithms** | Mathematical optimization model, DAG engine, Min-Heap Dijkstra, OOP architecture. |
| 2 | **Ishita Doval** | — | **DB & Backend** | Relational SQLite schema, foreign key constraints, transaction logging, web synchronization. |
| 3 | **Navdeep Singh Pundir** | `251037038` | **Testing & Documentation** | Concurrency simulator, benchmark suite, evaluation report, test datasets. |

---

## 📌 Problem Statement & Motivation

Traditional static educational curricula adopt a rigid "one-size-fits-all" model, forcing students through identical concept sequences regardless of prior knowledge, learning velocity, or individual weak points. This rigid progression leads to learning gaps, cognitive overload, and high drop-out rates in self-paced environments.

### Core Challenges Solved:
1. **Static Prerequisite Failure:** Static curricula cannot adapt when a learner struggles with a foundational sub-concept (e.g., struggling with recursion while attempting graph traversals or dynamic programming).
2. **Wasted Study Time:** Advanced learners waste significant time reviewing already mastered material.
3. **Lack of Dynamic Graph Trajectory Calculation:** Absence of real-time graph pathfinding to dynamically compute personalized shortest learning routes based on live evaluation scores.

---

## 🎯 Key Objectives

1. **Dynamic Knowledge Representation:** Map course modules as a **Directed Acyclic Graph (DAG)** where nodes represent topics and edges represent prerequisite dependencies with weighted difficulty levels.
2. **Adaptive Trajectory Calculation:** Implement dynamic path optimization algorithms (**Dijkstra's Algorithm with Min-Heap Priority Queue** & **Topological Sorting**) to generate real-time customized learning routes.
3. **Multi-Subject Integration:** Seamlessly unite 4 core CSE disciplines: **Data Structures (C++)**, **Object-Oriented Programming (OOPs)**, **Operating Systems (OS Concurrency)**, and **Database Management Systems (DBMS)**.
4. **Performance Metrics Evaluation:** Quantify efficacy by measuring path length optimization, retention score improvement, and **20% to 35% time-to-mastery reduction**.
5. **Interactive System & Dashboard:** Provide an interactive terminal loop application alongside a rich visual web dashboard with live DAG rendering.

---

## 🔬 Mathematical Optimization Formulation

For each topic $v_i \in V$ in the curriculum knowledge DAG:
- $W(v_i)$: Base difficulty / estimated study hours of topic $v_i$.
- $P(v_i) \in [0.0, 1.0]$: Learner's live evaluated proficiency / mastery score on topic $v_i$.
- $\alpha$: Tuning sensitivity parameter (default $\alpha = 0.35$).

### Dynamic Effective Weight Formula:
$$W'(v_i) = W(v_i) \times \Big[1 + \alpha\big(1 - P(v_i)\big)\Big] + \text{PrereqPenalty}(v_i)$$

Where the prerequisite bottleneck penalty is:
$$\text{PrereqPenalty}(v_i) = \sum_{u \in \text{Pred}(v_i), P(u) < 0.60} 1.5 \times \big(0.60 - P(u)\big)$$

- When a student demonstrates high mastery ($P(v) \ge 0.70$), effective review time drops dramatically ($0.15 \times W(v)$).
- When a student struggles ($P(v) < 0.60$), the engine inflates effective weights, isolates the foundational bottleneck, and **dynamically re-routes** the learner through prerequisite remediation modules.

---

## 🧩 Integration of Core CSE Subjects

```
+---------------------------------------------------------------------------------------+
|                             COHESIVE PBL SYSTEM ENGINE                                |
+---------------------------------------------------------------------------------------+
|                                                                                       |
|  [ DATA STRUCTURES in C++ ]        [ OBJECT-ORIENTED PROGRAMMING (OOPs) ]             |
|  • Directed Acyclic Graphs (DAG)   • Abstract Base Classes (IPathFinder)              |
|  • Custom Binary Min-Heap PQ       • Encapsulation (LearnerProfile, KnowledgeGraph)   |
|  • Kahn's Topological Sorting      • STL Containers, Dynamic Memory & Templates       |
|  • Dijkstra's Shortest Path        • Clean Modular Class Architecture                 |
|                                                                                       |
|  [ OPERATING SYSTEMS ]             [ DATABASE MANAGEMENT SYSTEMS (DBMS) ]             |
|  • Multithreading (std::thread)    • Relational Schema (SQLite / SQL)                 |
|  • Mutex Locks & Critical Sections • Foreign Keys, Unique & Check Constraints         |
|  • Concurrent Learner Simulation   • ACID Transaction Logging (transactions.sql)      |
|  • Throughput & Latency Metrics    • JSON Export for Web Dashboard Synchronization    |
|                                                                                       |
+---------------------------------------------------------------------------------------+
```

---

## 📊 Empirical Benchmarks (Static vs Dynamic)

| Goal Topic ID | Goal Topic Title | Static Rigid Curriculum | Dynamic Adaptive Engine (Ours) | Time Saved | % Study Time Reduced |
| :--- | :--- | :---: | :---: | :---: | :---: |
| **T04** | Stacks & Queues | 16.0 h | **1.7 h** | 14.3 h | **89.3%** |
| **T06** | Binary Trees & Traversals | 27.5 h | **18.9 h** | 8.6 h | **31.2%** |
| **T10** | Graph Traversals (BFS & DFS) | 48.5 h | **25.9 h** | 22.6 h | **46.6%** |
| **T12** | Dijkstra's Shortest Path | 60.5 h | **45.4 h** | 15.1 h | **24.9%** |
| **T13** | Minimum Spanning Trees | 67.0 h | **44.8 h** | 22.2 h | **33.2%** |
| **T14** | Dynamic Programming | 75.0 h | **31.5 h** | 43.5 h | **58.0%** |
| **T15** | Graph Dynamic Programming | 82.5 h | **70.0 h** | 12.5 h | **15.2%** |
| **OVERALL** | **Full Curriculum Average** | **53.8 h** | **34.9 h** | **18.9 h** | **35.1% Reduction** |

---

## 📂 Project Directory Structure

```
PBL 2/
├── .gitignore                      # Git ignore rules for build binaries and DBs
├── README.md                       # Comprehensive project documentation
├── Makefile                        # POSIX / MinGW GNU Makefile
├── build.bat                       # One-click Windows compilation script
├── run.bat                         # One-click Windows application runner
├── CMakeLists.txt                  # CMake build configuration
├── include/                        # C++ Header Files
│   ├── Graph.hpp                   # DAG Knowledge Graph, Adjacency & Predecessor Lists
│   ├── PathFinder.hpp              # Abstract IPathFinder, Dijkstra, Topological, A*
│   ├── PriorityQueue.hpp           # Templated Binary Min-Heap Priority Queue
│   ├── LearnerProfile.hpp          # Student state, mastery vector P(v), assessment logs
│   ├── OptimizationEngine.hpp      # Dynamic weight calculation & real-time rerouting
│   ├── AssessmentEngine.hpp        # Question banks, diagnostic baseline, MCQ grading
│   ├── DatabaseManager.hpp         # Relational SQLite/SQL transaction manager
│   ├── OSConcurrencySimulator.hpp  # Multithreaded concurrent learner simulation
│   ├── AnalyticsEngine.hpp         # Static vs dynamic comparative benchmark metrics
│   └── TerminalUI.hpp              # Rich ANSI/ASCII terminal interface & DAG charts
├── src/                            # C++ Source Implementations
│   ├── Graph.cpp
│   ├── PathFinder.cpp
│   ├── LearnerProfile.cpp
│   ├── OptimizationEngine.cpp
│   ├── AssessmentEngine.cpp
│   ├── DatabaseManager.cpp
│   ├── OSConcurrencySimulator.cpp
│   ├── AnalyticsEngine.cpp
│   ├── TerminalUI.cpp
│   └── main.cpp                    # Interactive System Loop Application
├── web/                            # Interactive Web Visualizer Dashboard
│   ├── index.html                  # Responsive visualizer with Vis.js & Chart.js
│   ├── style.css                   # Dark UI theme stylesheet
│   ├── app.js                      # Interactive DAG physics rendering & live simulation
│   └── server.py                   # Local Python web server
├── database/                       # Relational DBMS Schema & Data
│   ├── schema.sql                  # DDL SQL Schema with foreign keys & indexes
│   ├── seed_data.sql               # Seed dataset for 15 topics & prerequisite edges
│   └── transactions.sql            # Live ACID transaction audit log
├── data/                           # JSON Datasets
│   ├── course_curricula.json       # 15 Core Computer Science modules
│   └── quiz_bank.json              # Diagnostic question bank with explanations
├── tests/                          # Test Suites & Verification
│   ├── test_dijkstra.cpp           # Min-Heap and Dijkstra pathfinder unit test
│   ├── test_topological.cpp        # Kahn's algorithm & cycle detection test
│   ├── test_optimization.cpp       # Effective weights & rerouting test
│   └── benchmark_comparison.cpp    # Empirical static vs adaptive benchmark
└── docs/                           # Detailed Academic Documentation
    ├── PBL_Phase_I_II_Report.md    # Formal Academic Evaluation Report
    ├── Mathematical_Formulation.md # Mathematical proofs and algorithmic derivations
    └── System_Architecture_and_Design.md # UML, ER, and Concurrency Architecture
```

---

## 🚀 Quick Start Guide

### Prerequisites
- **Compiler:** `g++` with C++14/C++17 support (MinGW on Windows, GCC on Linux/macOS)
- **Optional:** Python 3.x (to run the web dashboard server)

### 1. Build the Entire Project
On Windows:
```cmd
.\build.bat
```
Or with `make`:
```bash
make
```

### 2. Run the Interactive Loop Application
```cmd
.\run.bat
```
Or execute directly:
```cmd
.\bin\pbl_optimizer.exe
```

### 3. Run Automated End-to-End Demo
```cmd
.\bin\pbl_optimizer.exe --demo
```

### 4. Run Test Suites & Verification
```cmd
.\bin\test_dijkstra.exe
.\bin\test_topological.exe
.\bin\test_optimization.exe
.\bin\benchmark_comparison.exe
```

### 5. Launch the Interactive Web Dashboard
Run the Python visualizer server:
```cmd
python web/server.py
```
Or simply double-click `web/index.html` in your browser!

---

## 📜 References
1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
2. Graphic Era (Deemed to be University) Computer Science Department — *Project-Based Learning (PBL) Academic Guidelines 2026-27*.
3. Desmarais, M. C., & Baker, R. S. (2012). A review of recent advances in learner data modeling. *User Modeling and User-Adapted Interaction*, 22(1-2), 9-38.

---
*Developed with pride by Team DSCPP-III-2026-T284 for PBL Evaluation.*
