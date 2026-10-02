-- ============================================================================
-- Personalized Learning Path Optimization - DBMS Relational Schema
-- Graphic Era (Deemed to be University) - 3rd Sem PBL (Academic Session 2026-27)
-- Team ID: DSCPP-III-2026-T284
-- ============================================================================

PRAGMA foreign_keys = ON;

-- Table: students
CREATE TABLE IF NOT EXISTS students (
    student_id VARCHAR(32) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100),
    target_goal_node VARCHAR(32) DEFAULT 'T15',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table: courses
CREATE TABLE IF NOT EXISTS courses (
    course_id VARCHAR(32) PRIMARY KEY,
    title VARCHAR(150) NOT NULL,
    description TEXT
);

-- Table: topics (DAG Nodes)
CREATE TABLE IF NOT EXISTS topics (
    topic_id VARCHAR(32) PRIMARY KEY,
    course_id VARCHAR(32) NOT NULL,
    title VARCHAR(150) NOT NULL,
    category VARCHAR(100) NOT NULL,
    base_hours REAL NOT NULL,
    difficulty_rating REAL NOT NULL,
    description TEXT,
    FOREIGN KEY (course_id) REFERENCES courses(course_id) ON DELETE CASCADE
);

-- Table: prerequisites (DAG Directed Edges: from_topic -> to_topic)
CREATE TABLE IF NOT EXISTS prerequisites (
    edge_id INTEGER PRIMARY KEY AUTOINCREMENT,
    from_topic_id VARCHAR(32) NOT NULL,
    to_topic_id VARCHAR(32) NOT NULL,
    dependency_weight REAL DEFAULT 1.0,
    FOREIGN KEY (from_topic_id) REFERENCES topics(topic_id) ON DELETE CASCADE,
    FOREIGN KEY (to_topic_id) REFERENCES topics(topic_id) ON DELETE CASCADE,
    UNIQUE(from_topic_id, to_topic_id)
);

-- Table: student_proficiencies (Learner Knowledge State Vector P(v))
CREATE TABLE IF NOT EXISTS student_proficiencies (
    student_id VARCHAR(32) NOT NULL,
    topic_id VARCHAR(32) NOT NULL,
    proficiency_score REAL NOT NULL CHECK (proficiency_score >= 0.0 AND proficiency_score <= 1.0),
    effective_weight REAL NOT NULL,
    is_mastered INTEGER DEFAULT 0 CHECK (is_mastered IN (0, 1)),
    last_evaluated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (student_id, topic_id),
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE,
    FOREIGN KEY (topic_id) REFERENCES topics(topic_id) ON DELETE CASCADE
);

-- Table: evaluation_logs (Historical assessment results for dynamic rerouting)
CREATE TABLE IF NOT EXISTS evaluation_logs (
    log_id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id VARCHAR(32) NOT NULL,
    topic_id VARCHAR(32) NOT NULL,
    score_obtained REAL NOT NULL,
    total_score REAL NOT NULL,
    passed INTEGER NOT NULL CHECK (passed IN (0, 1)),
    reroute_triggered INTEGER DEFAULT 0 CHECK (reroute_triggered IN (0, 1)),
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE,
    FOREIGN KEY (topic_id) REFERENCES topics(topic_id) ON DELETE CASCADE
);

-- Table: learning_trajectories (Computed paths from Dijkstra / Topological Sort)
CREATE TABLE IF NOT EXISTS learning_trajectories (
    trajectory_id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id VARCHAR(32) NOT NULL,
    goal_topic_id VARCHAR(32) NOT NULL,
    algorithm_used VARCHAR(50) NOT NULL,
    computed_path TEXT NOT NULL,
    total_estimated_hours REAL NOT NULL,
    is_active INTEGER DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
);

-- Indices for rapid query optimization
CREATE INDEX IF NOT EXISTS idx_prereq_from ON prerequisites(from_topic_id);
CREATE INDEX IF NOT EXISTS idx_prereq_to ON prerequisites(to_topic_id);
CREATE INDEX IF NOT EXISTS idx_student_prof ON student_proficiencies(student_id);
CREATE INDEX IF NOT EXISTS idx_eval_student ON evaluation_logs(student_id, topic_id);
