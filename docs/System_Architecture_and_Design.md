# System Architecture & Design Specification
## Personalized Learning Path Optimization
**Graphic Era (Deemed to be University) | Department of Computer Science & Engineering**

---

## 1. High-Level Architectural Diagram

```
+---------------------------------------------------------------------------------+
|                                PRESENTATION LAYER                               |
|  +-------------------------------------+   +---------------------------------+  |
|  | Interactive Terminal CLI Loop       |   | Vis.js / Chart.js Web Dashboard |  |
|  | (ANSI Colored Tables & ASCII DAG)   |   | (Dynamic Graph & Analytics)     |  |
|  +------------------+------------------+   +----------------+----------------+  |
+---------------------|---------------------------------------|-------------------+
                      |                                       |
                      v                                       v
+---------------------------------------------------------------------------------+
|                             CORE APPLICATION LAYER                              |
|  +----------------------+   +-----------------------+   +--------------------+  |
|  |  KnowledgeGraph      |   |  LearnerProfile       |   |  AssessmentEngine  |  |
|  |  (Adjacency Lists,   |   |  (Proficiency Vector  |   |  (Question Bank,   |  |
|  |   Topological Sort)  |   |   P(v), Mastery State)|   |   Diagnostic Test) |  |
|  +----------+-----------+   +-----------+-----------+   +---------+----------+  |
|             |                           |                         |             |
|             +-------------------> +-----+-----+ <-----------------+             |
|                                   | OptimizationEngine |                        |
|                                   | W'(v) Recalculation|                        |
|                                   +---------+----------+                        |
|                                             |                                   |
|                                   +---------+----------+                        |
|                                   |  PathFinder Subsystem |                     |
|                                   |  • DijkstraPathFinder  |                    |
|                                   |  • TopologicalFinder  |                     |
|                                   |  • AStarPathFinder    |                     |
|                                   +--------------------+                        |
+---------------------------------------------------------------------------------+
                      |                                       |
                      v                                       v
+---------------------------------------+   +-------------------------------------+
|         OS CONCURRENCY LAYER          |   |           DATABASE LAYER            |
|  +---------------------------------+  |   |  +-------------------------------+  |
|  | Multi-threaded Student Pool     |  |   |  | SQLite Relational Schema      |  |
|  | std::thread / Worker Pipeline   |  |   |  | (students, topics, prereqs,   |  |
|  | Mutex Synchronization           |  |   |  |  evaluations, trajectories)   |  |
|  | Throughput & Latency Engine     |  |   |  | ACID Transaction Logger       |  |
|  +---------------------------------+  |   |  +-------------------------------+  |
+---------------------------------------+   +-------------------------------------+
```

---

## 2. Relational Database Schema (ER Model)

- **`students`**: `(student_id PK, name, email, target_goal_node, created_at, updated_at)`
- **`courses`**: `(course_id PK, title, description)`
- **`topics`**: `(topic_id PK, course_id FK, title, category, base_hours, difficulty_rating, description)`
- **`prerequisites`**: `(edge_id PK, from_topic_id FK, to_topic_id FK, dependency_weight)`
- **`student_proficiencies`**: `(student_id PK/FK, topic_id PK/FK, proficiency_score, effective_weight, is_mastered, last_evaluated_at)`
- **`evaluation_logs`**: `(log_id PK, student_id FK, topic_id FK, score_obtained, total_score, passed, reroute_triggered, timestamp)`
- **`learning_trajectories`**: `(trajectory_id PK, student_id FK, goal_topic_id, algorithm_used, computed_path, total_estimated_hours, created_at)`

---

## 3. Object-Oriented Class Hierarchy

```
                      +-------------------+
                      |    IPathFinder    |  <<Abstract Base Class>>
                      +-------------------+
                      | +computePath()    |
                      | +getAlgorithmName()|
                      +---------+---------+
                                |
        +-----------------------+-----------------------+
        |                                               |
+-------v---------------+                       +-------v---------------+
|  DijkstraPathFinder   |                       | TopologicalPathFinder |
+-----------------------+                       +-----------------------+
| -MinHeapPriorityQueue |                       | -Kahn's in-degree q   |
+-----------------------+                       +-----------------------+
```

---

## 4. Concurrency & Synchronization Model

- **Worker Pipeline:** Simulated worker threads process batches of active learners.
- **Race Condition Prevention:** Shared learner record writes and database transaction logging are guarded with `std::mutex` and `std::lock_guard`.
- **Benchmark Metrics:** Wall clock execution time, throughput (ops/sec), and latency per student are measured using `std::chrono::high_resolution_clock`.
