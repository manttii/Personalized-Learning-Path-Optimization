-- ============================================================================
-- Seed Data for Personalized Learning Path Optimization
-- ============================================================================

INSERT OR IGNORE INTO courses (course_id, title, description) VALUES
('CS-201', 'Data Structures & Algorithms', 'Core computer science foundational and advanced problem solving');

INSERT OR IGNORE INTO topics (topic_id, course_id, title, category, base_hours, difficulty_rating, description) VALUES
('T01', 'CS-201', 'Time & Space Complexity', 'Foundations', 3.0, 1.5, 'Asymptotic notation, Big-O, recurrence relations'),
('T02', 'CS-201', 'Arrays & Memory Layout', 'Linear DS', 4.0, 2.0, 'Dynamic arrays, pointers, cache locality'),
('T03', 'CS-201', 'Linked Lists', 'Linear DS', 5.0, 2.5, 'Singly, Doubly linked lists, Floyd cycle detection'),
('T04', 'CS-201', 'Stacks & Queues', 'Linear DS', 4.0, 2.0, 'LIFO/FIFO, evaluation of expressions'),
('T05', 'CS-201', 'Recursion & Backtracking', 'Paradigms', 6.0, 3.5, 'Call stack, base case, recursion trees, N-Queens'),
('T06', 'CS-201', 'Binary Trees & Traversals', 'Hierarchical DS', 5.5, 3.0, 'Tree traversals: Inorder, Preorder, Postorder'),
('T07', 'CS-201', 'BST & Balanced Trees', 'Hierarchical DS', 6.5, 4.0, 'Search invariant, AVL rotations'),
('T08', 'CS-201', 'Binary Heaps & Priority Queues', 'Hierarchical DS', 4.5, 3.0, 'Min/Max Heap, heapify, priority queues'),
('T09', 'CS-201', 'Graph Representations', 'Graph Theory', 4.0, 2.5, 'Adjacency matrix, adjacency lists'),
('T10', 'CS-201', 'Graph Traversals (BFS & DFS)', 'Graph Theory', 6.0, 3.5, 'BFS queue, DFS recursive traversal'),
('T11', 'CS-201', 'Topological Sorting & DAGs', 'Graph Theory', 5.0, 3.5, 'Kahns algorithm, cycle detection'),
('T12', 'CS-201', 'Dijkstras Shortest Path', 'Graph Algorithms', 7.0, 4.5, 'Greedy path relaxation with min-heap'),
('T13', 'CS-201', 'Minimum Spanning Trees', 'Graph Algorithms', 6.5, 4.0, 'Prims & Kruskals with Disjoint Set Union'),
('T14', 'CS-201', 'Dynamic Programming Fundamentals', 'Paradigms', 8.0, 5.0, 'Memoization, tabulation, Knapsack'),
('T15', 'CS-201', 'Graph Dynamic Programming', 'Advanced', 7.5, 5.0, 'Floyd-Warshall, Bellman-Ford, DAG Longest Path');

-- Prerequisite Dependencies (Edges)
INSERT OR IGNORE INTO prerequisites (from_topic_id, to_topic_id, dependency_weight) VALUES
('T01', 'T02', 1.0),
('T02', 'T03', 1.0),
('T02', 'T04', 1.0),
('T01', 'T05', 1.2),
('T04', 'T05', 1.1),
('T03', 'T06', 1.0),
('T05', 'T06', 1.3),
('T06', 'T07', 1.2),
('T02', 'T08', 1.0),
('T06', 'T08', 1.1),
('T02', 'T09', 1.0),
('T03', 'T09', 1.0),
('T04', 'T10', 1.1),
('T05', 'T10', 1.3),
('T09', 'T10', 1.0),
('T10', 'T11', 1.2),
('T08', 'T12', 1.4),
('T10', 'T12', 1.3),
('T08', 'T13', 1.2),
('T10', 'T13', 1.2),
('T05', 'T14', 1.5),
('T06', 'T14', 1.2),
('T12', 'T15', 1.3),
('T14', 'T15', 1.4);

-- Default Student Profiles
INSERT OR IGNORE INTO students (student_id, name, email, target_goal_node) VALUES
('2510011893', 'Priyanshi Saini', 'priyanshi@geu.ac.in', 'T12'),
('251037038', 'Navdeep Singh Pundir', 'navdeep@geu.ac.in', 'T15'),
('2510998811', 'Ishita Doval', 'ishita@geu.ac.in', 'T14');
