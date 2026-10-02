let network = null;
let graphData = null;
let benchmarkChart = null;

const defaultData = {
  "student": {
    "id": "2510011893",
    "name": "Priyanshi Saini",
    "email": "priyanshi@geu.ac.in",
    "goal_node": "T12",
    "avg_proficiency": 0.72,
    "mastered_count": 4,
    "total_estimated_hours": 24.5
  },
  "nodes": [
    {"id": "T01", "title": "Time & Space Complexity", "category": "Foundations", "base_hours": 3.0, "difficulty": 1.5, "proficiency": 0.90, "effective_weight": 0.45, "is_mastered": true, "in_path": true, "path_order": 1, "description": "Asymptotic notation, Big-O, recurrence relations"},
    {"id": "T02", "title": "Arrays & Dynamic Memory", "category": "Linear DS", "base_hours": 4.0, "difficulty": 2.0, "proficiency": 0.85, "effective_weight": 0.60, "is_mastered": true, "in_path": true, "path_order": 2, "description": "Dynamic vectors, pointer arithmetic, contiguous cache"},
    {"id": "T03", "title": "Linked Lists", "category": "Linear DS", "base_hours": 5.0, "difficulty": 2.5, "proficiency": 0.80, "effective_weight": 0.75, "is_mastered": true, "in_path": true, "path_order": 3, "description": "Singly/Doubly linked lists, Floyd cycle detection"},
    {"id": "T04", "title": "Stacks & Queues", "category": "Linear DS", "base_hours": 4.0, "difficulty": 2.0, "proficiency": 0.80, "effective_weight": 0.60, "is_mastered": true, "in_path": true, "path_order": 4, "description": "LIFO/FIFO, evaluation of expressions"},
    {"id": "T05", "title": "Recursion & Backtracking", "category": "Paradigms", "base_hours": 6.0, "difficulty": 3.5, "proficiency": 0.40, "effective_weight": 8.70, "is_mastered": false, "in_path": true, "path_order": 5, "description": "Call stack frame, base conditions, N-Queens"},
    {"id": "T06", "title": "Binary Trees & Traversals", "category": "Hierarchical DS", "base_hours": 5.5, "difficulty": 3.0, "proficiency": 0.50, "effective_weight": 7.50, "is_mastered": false, "in_path": true, "path_order": 6, "description": "Tree traversals: Inorder, Preorder, Postorder"},
    {"id": "T07", "title": "BST & Balanced AVL Trees", "category": "Hierarchical DS", "base_hours": 6.5, "difficulty": 4.0, "proficiency": 0.00, "effective_weight": 10.40, "is_mastered": false, "in_path": false, "path_order": -1, "description": "Search invariant, AVL rotations"},
    {"id": "T08", "title": "Binary Heaps & Priority Queues", "category": "Hierarchical DS", "base_hours": 4.5, "difficulty": 3.0, "proficiency": 0.75, "effective_weight": 5.40, "is_mastered": false, "in_path": true, "path_order": 7, "description": "Min/Max heap, heapify, priority queues"},
    {"id": "T09", "title": "Graph Representations", "category": "Graph Theory", "base_hours": 4.0, "difficulty": 2.5, "proficiency": 0.70, "effective_weight": 4.00, "is_mastered": false, "in_path": true, "path_order": 8, "description": "Adjacency matrix vs Adjacency list"},
    {"id": "T10", "title": "Graph Traversals (BFS & DFS)", "category": "Graph Theory", "base_hours": 6.0, "difficulty": 3.5, "proficiency": 0.50, "effective_weight": 8.40, "is_mastered": false, "in_path": true, "path_order": 9, "description": "Queue BFS, recursive/stack DFS"},
    {"id": "T11", "title": "Topological Sort & DAGs", "category": "Graph Theory", "base_hours": 5.0, "difficulty": 3.5, "proficiency": 0.00, "effective_weight": 7.00, "is_mastered": false, "in_path": false, "path_order": -1, "description": "Kahns algorithm, dependency scheduling"},
    {"id": "T12", "title": "Dijkstras Shortest Path", "category": "Graph Algorithms", "base_hours": 7.0, "difficulty": 4.5, "proficiency": 0.00, "effective_weight": 12.60, "is_mastered": false, "in_path": true, "path_order": 10, "description": "Greedy relaxation with min-heap priority queue"},
    {"id": "T13", "title": "Minimum Spanning Trees", "category": "Graph Algorithms", "base_hours": 6.5, "difficulty": 4.0, "proficiency": 0.00, "effective_weight": 10.40, "is_mastered": false, "in_path": false, "path_order": -1, "description": "Prims & Kruskals with Disjoint Set Union"},
    {"id": "T14", "title": "Dynamic Programming", "category": "Advanced Paradigms", "base_hours": 8.0, "difficulty": 5.0, "proficiency": 0.00, "effective_weight": 16.00, "is_mastered": false, "in_path": false, "path_order": -1, "description": "Memoization, tabulation, Knapsack"},
    {"id": "T15", "title": "Graph Dynamic Programming", "category": "Advanced Paradigms", "base_hours": 7.5, "difficulty": 5.0, "proficiency": 0.00, "effective_weight": 15.00, "is_mastered": false, "in_path": false, "path_order": -1, "description": "Floyd-Warshall, Bellman-Ford, DAG DP"}
  ],
  "edges": [
    {"from": "T01", "to": "T02", "weight": 1.0},
    {"from": "T02", "to": "T03", "weight": 1.0},
    {"from": "T02", "to": "T04", "weight": 1.0},
    {"from": "T01", "to": "T05", "weight": 1.2},
    {"from": "T04", "to": "T05", "weight": 1.1},
    {"from": "T03", "to": "T06", "weight": 1.0},
    {"from": "T05", "to": "T06", "weight": 1.3},
    {"from": "T06", "to": "T07", "weight": 1.2},
    {"from": "T02", "to": "T08", "weight": 1.0},
    {"from": "T06", "to": "T08", "weight": 1.1},
    {"from": "T02", "to": "T09", "weight": 1.0},
    {"from": "T03", "to": "T09", "weight": 1.0},
    {"from": "T04", "to": "T10", "weight": 1.1},
    {"from": "T05", "to": "T10", "weight": 1.3},
    {"from": "T09", "to": "T10", "weight": 1.0},
    {"from": "T10", "to": "T11", "weight": 1.2},
    {"from": "T08", "to": "T12", "weight": 1.4},
    {"from": "T10", "to": "T12", "weight": 1.3},
    {"from": "T08", "to": "T13", "weight": 1.2},
    {"from": "T10", "to": "T13", "weight": 1.2},
    {"from": "T05", "to": "T14", "weight": 1.5},
    {"from": "T06", "to": "T14", "weight": 1.2},
    {"from": "T12", "to": "T15", "weight": 1.3},
    {"from": "T14", "to": "T15", "weight": 1.4}
  ],
  "current_path": ["T01", "T02", "T03", "T04", "T05", "T06", "T08", "T09", "T10", "T12"],
  "evaluation_history": [
    {"topic_id": "T01", "score": 0.90, "passed": true, "timestamp": "2026-10-02 21:00", "weight_before": 3.0, "weight_after": 0.45},
    {"topic_id": "T02", "score": 0.85, "passed": true, "timestamp": "2026-10-02 21:15", "weight_before": 4.0, "weight_after": 0.60},
    {"topic_id": "T05", "score": 0.40, "passed": false, "timestamp": "2026-10-02 21:30", "weight_before": 6.0, "weight_after": 8.70}
  ]
};

async function loadData() {
    try {
        const res = await fetch('data.json');
        if (res.ok) {
            graphData = await res.json();
        } else {
            graphData = defaultData;
        }
    } catch (e) {
        console.warn('Using default bundled data:', e);
        graphData = defaultData;
    }
    initDashboard();
}

function initDashboard() {
    renderStats();
    renderNetwork();
    renderTopicSelect();
    renderLogsTable();
    renderBenchmarkChart();
    attachEventHandlers();
}

function renderStats() {
    const s = graphData.student;
    document.getElementById('stat-student-name').textContent = s.name;
    document.getElementById('stat-student-id').textContent = `ID: ${s.id}`;
    
    const goalNode = graphData.nodes.find(n => n.id === s.goal_node) || { title: s.goal_node };
    document.getElementById('stat-goal-node').textContent = `${s.goal_node}: ${goalNode.title}`;
    document.getElementById('stat-path-hours').textContent = `${s.total_estimated_hours.toFixed(1)} h`;
    document.getElementById('stat-avg-prof').textContent = `${(s.avg_proficiency * 100).toFixed(1)}%`;
    document.getElementById('stat-mastered-count').textContent = `${s.mastered_count} / ${graphData.nodes.length} Mastered`;
}

function renderNetwork() {
    const container = document.getElementById('network-container');
    const nodesArray = [];
    const edgesArray = [];

    const pathSet = new Set(graphData.current_path || []);

    graphData.nodes.forEach(node => {
        let bgColor = '#8b949e';
        let borderColor = '#30363d';
        let fontColor = '#ffffff';
        let borderWidth = 2;

        if (node.is_mastered) {
            bgColor = '#238636';
            borderColor = '#2ea043';
        } else if (node.proficiency < 0.60 && node.proficiency > 0.0) {
            bgColor = '#da3633';
            borderColor = '#f85149';
        } else if (pathSet.has(node.id)) {
            bgColor = '#1f6feb';
            borderColor = '#58a6ff';
            borderWidth = 3;
        }

        let label = `${node.id}\n${node.title.length > 18 ? node.title.substr(0, 16) + '..' : node.title}\n(W': ${node.effective_weight.toFixed(1)}h)`;
        if (node.in_path && node.path_order > 0) {
            label = `#${node.path_order} ` + label;
        }

        nodesArray.push({
            id: node.id,
            label: label,
            color: {
                background: bgColor,
                border: borderColor,
                highlight: { background: '#bc8cff', border: '#d2a8ff' }
            },
            font: { color: fontColor, size: 12, face: 'Segoe UI' },
            shape: 'box',
            margin: 10,
            borderWidth: borderWidth
        });
    });

    graphData.edges.forEach(edge => {
        const isPathEdge = pathSet.has(edge.from) && pathSet.has(edge.to);
        edgesArray.push({
            from: edge.from,
            to: edge.to,
            arrows: 'to',
            color: {
                color: isPathEdge ? '#58a6ff' : '#30363d',
                highlight: '#bc8cff'
            },
            width: isPathEdge ? 3 : 1,
            smooth: { type: 'cubicBezier', roundness: 0.4 }
        });
    });

    const data = {
        nodes: new vis.DataSet(nodesArray),
        edges: new vis.DataSet(edgesArray)
    };

    const options = {
        layout: {
            hierarchical: {
                direction: 'LR',
                sortMethod: 'directed',
                levelSeparation: 160,
                nodeSpacing: 100
            }
        },
        physics: {
            hierarchicalRepulsion: { nodeDistance: 120 }
        },
        interaction: { hover: true }
    };

    network = new vis.Network(container, data, options);

    network.on('click', params => {
        if (params.nodes.length > 0) {
            const nodeId = params.nodes[0];
            inspectNode(nodeId);
        }
    });
}

function inspectNode(nodeId) {
    const node = graphData.nodes.find(n => n.id === nodeId);
    if (!node) return;

    const prereqs = graphData.edges.filter(e => e.to === nodeId).map(e => e.from);
    const dependents = graphData.edges.filter(e => e.from === nodeId).map(e => e.to);

    const content = document.getElementById('inspector-content');
    content.innerHTML = `
        <h4 style="color: #58a6ff; margin-bottom: 8px;">${node.id}: ${node.title}</h4>
        <p style="font-size: 0.88rem; color: #c9d1d9; margin-bottom: 10px;">${node.description}</p>
        <div style="font-size: 0.85rem; display: grid; grid-template-columns: 1fr 1fr; gap: 6px; margin-bottom: 10px;">
            <div><strong>Category:</strong> ${node.category}</div>
            <div><strong>Difficulty:</strong> ${node.difficulty} / 5.0</div>
            <div><strong>Base Time:</strong> ${node.base_hours} Hours</div>
            <div><strong>Proficiency P(v):</strong> ${(node.proficiency * 100).toFixed(1)}%</div>
            <div><strong>Effective W'(v):</strong> <span style="color: #58a6ff; font-weight: bold;">${node.effective_weight.toFixed(2)}h</span></div>
            <div><strong>Mastery:</strong> ${node.is_mastered ? '<span style="color:#3fb950">Mastered</span>' : 'Pending'}</div>
        </div>
        <div style="font-size: 0.82rem; color: #8b949e;">
            <div><strong>Prerequisites:</strong> ${prereqs.length > 0 ? prereqs.join(', ') : 'None (Root)'}</div>
            <div><strong>Unlocks:</strong> ${dependents.length > 0 ? dependents.join(', ') : 'Goal / Terminal'}</div>
        </div>
    `;
}

function renderTopicSelect() {
    const sel = document.getElementById('sim-topic-select');
    sel.innerHTML = '';
    graphData.nodes.forEach(n => {
        const opt = document.createElement('option');
        opt.value = n.id;
        opt.textContent = `${n.id}: ${n.title} (P: ${(n.proficiency * 100).toFixed(0)}%)`;
        sel.appendChild(opt);
    });
}

function renderLogsTable() {
    const tbody = document.getElementById('logs-table-body');
    tbody.innerHTML = '';

    const hist = graphData.evaluation_history || [];
    hist.forEach(h => {
        const node = graphData.nodes.find(n => n.id === h.topic_id) || { title: 'Unknown' };
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>${h.timestamp}</td>
            <td><strong>${h.topic_id}</strong></td>
            <td>${node.title}</td>
            <td>${(h.score * 100).toFixed(1)}%</td>
            <td><span class="tag ${h.passed ? 'tag-pass' : 'tag-fail'}">${h.passed ? 'PASS' : 'FAIL'}</span></td>
            <td>${h.weight_before.toFixed(1)}h</td>
            <td>${h.weight_after.toFixed(1)}h</td>
            <td><span class="tag ${h.passed ? 'tag-pass' : 'tag-reroute'}">${h.passed ? 'Advanced' : 'Rerouted to Remediation'}</span></td>
        `;
        tbody.appendChild(tr);
    });
}

function renderBenchmarkChart() {
    const ctx = document.getElementById('benchmarkChart').getContext('2d');
    if (benchmarkChart) benchmarkChart.destroy();

    benchmarkChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: ['Static Linear (Baseline)', 'Adaptive Dijkstra (Ours)'],
            datasets: [{
                label: 'Total Estimated Study Hours',
                data: [38.5, 24.5],
                backgroundColor: ['rgba(218, 54, 51, 0.7)', 'rgba(35, 134, 54, 0.8)'],
                borderColor: ['#da3633', '#238636'],
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: { display: false }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: { display: true, text: 'Hours', color: '#8b949e' },
                    grid: { color: '#30363d' },
                    ticks: { color: '#8b949e' }
                },
                x: {
                    grid: { display: false },
                    ticks: { color: '#c9d1d9' }
                }
            }
        }
    });
}

function attachEventHandlers() {
    document.getElementById('btn-simulate-pass').addEventListener('click', () => {
        const topicId = document.getElementById('sim-topic-select').value;
        const node = graphData.nodes.find(n => n.id === topicId);
        if (node) {
            node.proficiency = 0.90;
            node.is_mastered = true;
            node.effective_weight = node.baseHours * 0.15;
            
            const alert = document.getElementById('reroute-alert');
            alert.className = 'alert alert-success';
            alert.innerHTML = `<strong>Concept Mastered!</strong> ${node.id} (${node.title}) verified at 90%. Weight reduced to ${node.effective_weight.toFixed(1)}h.`;
            alert.classList.remove('d-none');

            renderStats();
            renderNetwork();
            renderTopicSelect();
        }
    });

    document.getElementById('btn-simulate-fail').addEventListener('click', () => {
        const topicId = document.getElementById('sim-topic-select').value;
        const node = graphData.nodes.find(n => n.id === topicId);
        if (node) {
            node.proficiency = 0.30;
            node.is_mastered = false;
            node.effective_weight = node.baseHours * 1.8;

            const alert = document.getElementById('reroute-alert');
            alert.className = 'alert alert-danger';
            alert.innerHTML = `<strong>Concept Failure Detected!</strong> ${node.id} scored 30%. Engine dynamically inflated weight to ${node.effective_weight.toFixed(1)}h & rerouted trajectory.`;
            alert.classList.remove('d-none');

            renderStats();
            renderNetwork();
            renderTopicSelect();
        }
    });

    document.getElementById('goal-select').addEventListener('change', (e) => {
        graphData.student.goal_node = e.target.value;
        renderStats();
        renderNetwork();
    });
}

window.addEventListener('DOMContentLoaded', loadData);
