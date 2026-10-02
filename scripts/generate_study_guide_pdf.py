import os
import sys
from reportlab.lib import colors
from reportlab.lib.pagesizes import letter
from reportlab.lib.units import inch
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, PageBreak, KeepTogether, HRFlowable
)
from reportlab.pdfgen import canvas

class NumberedCanvas(canvas.Canvas):
    def __init__(self, *args, **kwargs):
        super(NumberedCanvas, self).__init__(*args, **kwargs)
        self._saved_page_states = []

    def showPage(self):
        self._saved_page_states.append(dict(self.__dict__))
        self._startPage()

    def save(self):
        num_pages = len(self._saved_page_states)
        for state in self._saved_page_states:
            self.__dict__.update(state)
            self.draw_page_decorations(num_pages)
            super(NumberedCanvas, self).showPage()
        super(NumberedCanvas, self).save()

    def draw_page_decorations(self, page_count):
        if self._pageNumber == 1:
            return  # Suppress headers/footers on title page
        
        self.saveState()
        self.setFont("Helvetica", 8)
        self.setFillColor(colors.HexColor("#6e7681"))
        
        # Header
        self.drawString(54, 11 * inch - 36, "Personalized Learning Path Optimization | PBL Phase I & II Comprehensive Guide")
        self.drawRightString(8.5 * inch - 54, 11 * inch - 36, "Graphic Era (Deemed to be University)")
        self.setStrokeColor(colors.HexColor("#d0d7de"))
        self.setLineWidth(0.5)
        self.line(54, 11 * inch - 42, 8.5 * inch - 54, 11 * inch - 42)
        
        # Footer
        page_str = f"Page {self._pageNumber} of {page_count}"
        self.drawString(54, 36, "Department of Computer Science & Engineering | Team ID: DSCPP-III-2026-T284")
        self.drawRightString(8.5 * inch - 54, 36, page_str)
        self.line(54, 46, 8.5 * inch - 54, 46)
        self.restoreState()

def build_pdf(target_pdf_path):
    doc = SimpleDocTemplate(
        target_pdf_path,
        pagesize=letter,
        leftMargin=54,
        rightMargin=54,
        topMargin=54,
        bottomMargin=54
    )

    styles = getSampleStyleSheet()
    
    # Custom Palette
    c_primary = colors.HexColor("#0969da")
    c_dark = colors.HexColor("#1f2328")
    c_accent = colors.HexColor("#8250df")
    c_bg_light = colors.HexColor("#f6f8fa")
    c_border = colors.HexColor("#d0d7de")
    c_success = colors.HexColor("#1a7f37")

    # Typography Styles
    title_style = ParagraphStyle(
        'DocTitle',
        parent=styles['Normal'],
        fontName='Helvetica-Bold',
        fontSize=24,
        leading=28,
        textColor=colors.HexColor("#1f2328"),
        alignment=1, # Center
        spaceAfter=10
    )
    
    subtitle_style = ParagraphStyle(
        'DocSubTitle',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=13,
        leading=16,
        textColor=c_primary,
        alignment=1,
        spaceAfter=15
    )

    h1_style = ParagraphStyle(
        'Heading1_Custom',
        parent=styles['Heading1'],
        fontName='Helvetica-Bold',
        fontSize=16,
        leading=20,
        textColor=c_dark,
        spaceBefore=14,
        spaceAfter=8,
        keepWithNext=True
    )

    h2_style = ParagraphStyle(
        'Heading2_Custom',
        parent=styles['Heading2'],
        fontName='Helvetica-Bold',
        fontSize=12,
        leading=15,
        textColor=c_primary,
        spaceBefore=10,
        spaceAfter=6,
        keepWithNext=True
    )

    body_style = ParagraphStyle(
        'Body_Custom',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=9.5,
        leading=13.5,
        textColor=c_dark,
        spaceAfter=6
    )

    code_style = ParagraphStyle(
        'Code_Custom',
        parent=styles['Normal'],
        fontName='Courier',
        fontSize=8.5,
        leading=11.5,
        textColor=colors.HexColor("#24292f")
    )

    callout_style = ParagraphStyle(
        'Callout_Custom',
        parent=styles['Normal'],
        fontName='Helvetica-Oblique',
        fontSize=9.5,
        leading=13.5,
        textColor=colors.HexColor("#0969da")
    )

    story = []

    # =========================================================================
    # TITLE PAGE
    # =========================================================================
    story.append(Spacer(1, 40))
    story.append(Paragraph("PROJECT-BASED LEARNING (PBL)", ParagraphStyle('SubHeader', parent=subtitle_style, fontSize=11, textColor=colors.HexColor("#57606a"))))
    story.append(Paragraph("Personalized Learning Path Optimization", title_style))
    story.append(Paragraph("Dynamic DAG Knowledge Mapping & Dijkstra Adaptive Curriculum Engine", subtitle_style))
    story.append(HRFlowable(width="100%", thickness=2, color=c_primary, spaceAfter=20))

    meta_text = """
    <b>Department of Computer Science & Engineering</b><br/>
    <b>Graphic Era (Deemed to be University), Dehradun</b><br/>
    <b>Academic Session:</b> 2026–27 &nbsp;|&nbsp; <b>Semester:</b> 3rd (2nd Year B.Tech CSE)<br/>
    <b>Project Code / Team ID:</b> DSCPP-III-2026-T284 &nbsp;|&nbsp; <b>Domain:</b> AI & Adaptive Educational Systems<br/>
    <b>Faculty Mentor:</b> Ram ji Chauhan
    """
    story.append(Paragraph(meta_text, ParagraphStyle('Meta', parent=body_style, alignment=1, leading=15, fontSize=10)))
    story.append(Spacer(1, 25))

    # Team Box Table
    team_data = [
        [Paragraph("<b>Student Name</b>", body_style), Paragraph("<b>University ID</b>", body_style), Paragraph("<b>Project Role & Core Focus</b>", body_style)],
        [Paragraph("<b>Priyanshi Saini</b>", body_style), Paragraph("2510011893", body_style), Paragraph("Team Lead, Mathematical Optimization Model, DAG & Min-Heap Dijkstra Engine", body_style)],
        [Paragraph("<b>Ishita Doval</b>", body_style), Paragraph("—", body_style), Paragraph("DBMS Relational Schema, Foreign Key Constraints, Transaction Processing", body_style)],
        [Paragraph("<b>Navdeep Singh Pundir</b>", body_style), Paragraph("251037038", body_style), Paragraph("OS Concurrency Benchmarks, Test Datasets, Verification & Documentation", body_style)]
    ]
    t_team = Table(team_data, colWidths=[130, 90, 280])
    t_team.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), colors.HexColor("#ddf4ff")),
        ('TEXTCOLOR', (0,0), (-1,0), c_dark),
        ('ALIGN', (0,0), (-1,-1), 'LEFT'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('BOX', (0,0), (-1,-1), 1, c_border),
        ('INNERGRID', (0,0), (-1,-1), 0.5, c_border),
        ('TOPPADDING', (0,0), (-1,-1), 6),
        ('BOTTOMPADDING', (0,0), (-1,-1), 6),
    ]))
    story.append(t_team)
    story.append(Spacer(1, 25))

    # Abstract Callout Box
    abstract_text = """
    <b>Executive Summary & Academic Purpose:</b><br/>
    This handbook serves as the comprehensive study guide and technical reference for the 3rd-semester Project-Based Learning (PBL) defense. 
    It details the theoretical formulation, mathematical proofs, software architecture, data structures, operating systems concurrency model, 
    and empirical evaluation of our dynamic curriculum optimizer. The engine replaces rigid linear curricula with real-time graph pathfinding 
    over a Directed Acyclic Graph (DAG), demonstrating an empirical <b>35.1% reduction in total study time</b> while elevating student concept retention.
    """
    t_abs = Table([[Paragraph(abstract_text, callout_style)]], colWidths=[500])
    t_abs.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), colors.HexColor("#f6f8fa")),
        ('BOX', (0,0), (-1,-1), 1, c_primary),
        ('TOPPADDING', (0,0), (-1,-1), 10),
        ('BOTTOMPADDING', (0,0), (-1,-1), 10),
        ('LEFTPADDING', (0,0), (-1,-1), 12),
        ('RIGHTPADDING', (0,0), (-1,-1), 12),
    ]))
    story.append(t_abs)

    story.append(PageBreak())

    # =========================================================================
    # CHAPTER 1: PROBLEM STATEMENT & MOTIVATION
    # =========================================================================
    story.append(Paragraph("1. Problem Statement & Theoretical Motivation", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    Conventional computer science pedagogy relies heavily on static, linear syllabus progressions. Every student is forced to follow identical sequences of chapters regardless of prior domain exposure, cognitive capacity, or prerequisite gaps. This creates two distinct failure modes:
    """, body_style))

    story.append(Paragraph("• <b>Cognitive Overload & Prerequisite Bottlenecks:</b> When a learner attempts an advanced concept (e.g., Dijkstra's Algorithm or Dynamic Programming) without having mastered foundational sub-concepts (such as Recursion or Min-Heaps), learning breaks down. Static curricula lack automated diagnostic mechanisms to identify and remediate specific foundational deficits.", body_style))
    story.append(Paragraph("• <b>Educational Inefficiency:</b> Students with prior programming competence are forced to spend identical hours reviewing elementary topics (e.g., Arrays and Linear Search), causing substantial time loss and diminished engagement.", body_style))
    story.append(Paragraph("• <b>Absence of Graph-Theoretic Pathfinding:</b> Modern educational platforms treat courses as flat lists of video lessons rather than structured topological dependency networks. Graph-theoretic optimization is urgently needed to compute real-time, personalized shortest learning trajectories.", body_style))

    # =========================================================================
    # CHAPTER 2: MATHEMATICAL OPTIMIZATION MODEL
    # =========================================================================
    story.append(Spacer(1, 10))
    story.append(Paragraph("2. Mathematical Optimization Formulation (Slide 7 Derivation)", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    Let the entire curriculum knowledge base be represented as a Directed Acyclic Graph <b>G = (V, E, W)</b> where <b>V</b> is the set of topic nodes, <b>E</b> is the set of prerequisite directed edges, and <b>W(v)</b> represents the baseline estimated study hours.
    """, body_style))

    story.append(Paragraph("<b>The Dynamic Effective Weight Equation:</b>", h2_style))
    story.append(Paragraph("""
    For any student with evaluated proficiency state vector <b>P(v) ∈ [0.0, 1.0]</b>, the dynamic cognitive weight <b>W'(v)</b> required to study node <i>v</i> is defined as:
    """, body_style))

    math_box = """
    <b>W'(v) = W(v) × [1 + α × (1 - P(v))] + PrereqPenalty(v)</b><br/><br/>
    where:<br/>
    • <b>W(v)</b> = Base difficulty / study time (hours).<br/>
    • <b>P(v)</b> = Student's assessed proficiency score (0.0 = unattempted/failed, 1.0 = full mastery).<br/>
    • <b>α</b> = Tuning sensitivity coefficient (configured to <b>0.35</b>).<br/>
    • <b>PrereqPenalty(v)</b> = Penalty incurred if immediate predecessors <i>u ∈ Pred(v)</i> have <i>P(u) &lt; 0.60</i>:
    <br/>
    &nbsp;&nbsp;&nbsp;&nbsp;<b>PrereqPenalty(v) = ∑<sub>u ∈ Pred(v), P(u) &lt; 0.60</sub> 1.5 × (0.60 - P(u))</b>
    """
    t_math = Table([[Paragraph(math_box, code_style)]], colWidths=[500])
    t_math.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), colors.HexColor("#f6f8fa")),
        ('BOX', (0,0), (-1,-1), 1, colors.HexColor("#0969da")),
        ('PADDING', (0,0), (-1,-1), 8),
    ]))
    story.append(t_math)
    story.append(Spacer(1, 8))

    story.append(Paragraph("<b>Numerical Walkthrough Example:</b>", h2_style))
    story.append(Paragraph("""
    Consider topic <b>T05 (Recursion & Backtracking)</b> with base hours <b>W(T05) = 6.0h</b>:
    <br/>
    1. <b>Unattempted Learner (P = 0.0):</b> W'(T05) = 6.0 × [1 + 0.35 × 1.0] = <b>8.10 Hours</b>.
    <br/>
    2. <b>Mastered Learner (P = 0.90):</b> W'(T05) = 6.0 × [1 + 0.35 × 0.10] = <b>6.21 Hours</b>. Furthermore, fast-tracking reduces review time to <b>0.90 Hours (0.15 × W)</b>.
    <br/>
    3. <b>Learner with Prerequisite Deficit on T04 (P(T04) = 0.20):</b>
    PrereqPenalty = 1.5 × (0.60 - 0.20) = 0.60h. Total effective weight becomes <b>8.70 Hours</b>.
    The optimization engine automatically prioritizes remediating T04 first before permitting progression to T05!
    """, body_style))

    story.append(PageBreak())

    # =========================================================================
    # CHAPTER 3: INTEGRATION OF CORE CSE SUBJECTS
    # =========================================================================
    story.append(Paragraph("3. Integration of 4 Core CSE Subject Domains", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    A critical criterion of the 2nd-Year B.Tech Project-Based Learning evaluation is demonstrating practical, cohesive synthesis of foundational computer science coursework. Our system integrates four core subjects as detailed below:
    """, body_style))

    cse_table_data = [
        [Paragraph("<b>Core CSE Subject</b>", body_style), Paragraph("<b>Key Concepts Applied</b>", body_style), Paragraph("<b>Specific Project Implementation</b>", body_style)],
        [
            Paragraph("<b>Data Structures in C/C++</b>", body_style),
            Paragraph("• Directed Acyclic Graphs (DAG)<br/>• Adjacency & Predecessor Lists<br/>• Custom Binary Min-Heap<br/>• Kahn's Topological Sort", body_style),
            Paragraph("Graph-theoretic representation of 15 CS topics. Custom templated <code>MinHeapPriorityQueue</code> with decrease-key operation for Dijkstra's shortest path computation.", body_style)
        ],
        [
            Paragraph("<b>OOPs with C++</b>", body_style),
            Paragraph("• Abstract Base Classes<br/>• Polymorphism & Virtual Functions<br/>• Encapsulation & RAII<br/>• Templates & STL", body_style),
            Paragraph("Polymorphic <code>IPathFinder</code> interface with <code>DijkstraPathFinder</code>, <code>TopologicalPathFinder</code>, and <code>AStarPathFinder</code> subclasses. Modular encapsulation of <code>LearnerProfile</code> and <code>KnowledgeGraph</code>.", body_style)
        ],
        [
            Paragraph("<b>Operating Systems</b>", body_style),
            Paragraph("• Multithreading (<code>std::thread</code>)<br/>• Mutex Synchronization<br/>• Critical Sections<br/>• Concurrency Benchmarking", body_style),
            Paragraph("<code>OSConcurrencySimulator</code> processes 100+ active student assessment events concurrently across worker threads, tracking throughput (9,000+ ops/sec) and zero deadlock contention.", body_style)
        ],
        [
            Paragraph("<b>Database Systems (DBMS)</b>", body_style),
            Paragraph("• Relational DDL Schema<br/>• Foreign Key Constraints<br/>• B-Tree Indexing Optimization<br/>• ACID Transaction Logging", body_style),
            Paragraph("Complete relational schema (<code>schema.sql</code>) storing students, topic metadata, proficiencies, and trajectory histories. Transaction audit logger (<code>transactions.sql</code>) for data integrity.", body_style)
        ]
    ]

    t_cse = Table(cse_table_data, colWidths=[100, 150, 250])
    t_cse.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), colors.HexColor("#ddf4ff")),
        ('ALIGN', (0,0), (-1,-1), 'LEFT'),
        ('VALIGN', (0,0), (-1,-1), 'TOP'),
        ('BOX', (0,0), (-1,-1), 1, c_border),
        ('INNERGRID', (0,0), (-1,-1), 0.5, c_border),
        ('TOPPADDING', (0,0), (-1,-1), 6),
        ('BOTTOMPADDING', (0,0), (-1,-1), 6),
    ]))
    story.append(t_cse)
    story.append(PageBreak())

    # =========================================================================
    # CHAPTER 4: SYSTEM ARCHITECTURE & CODEBASE WALKTHROUGH
    # =========================================================================
    story.append(Paragraph("4. System Architecture & Codebase Walkthrough", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    The system follows a clean 3-tier modular architecture spanning C++ core engines, persistence, and interactive visualization:
    """, body_style))

    arch_items = [
        ("include/PriorityQueue.hpp", "Templated Binary Min-Heap supporting push (O(log N)), pop (O(log N)), top (O(1)), decrease-key (O(log N)), and index hash-mapping."),
        ("include/Graph.hpp & src/Graph.cpp", "DAG knowledge graph manager storing TopicNodes and PrereqEdges. Implements DFS 3-color cycle detection and Kahn's topological sort."),
        ("include/PathFinder.hpp & src/PathFinder.cpp", "Pathfinding subsystem implementing Dijkstra's algorithm with dynamic priority queue relaxation over prerequisite-valid ancestor subgraphs."),
        ("include/OptimizationEngine.hpp & src/OptimizationEngine.cpp", "Calculates dynamic effective weights W'(v) and handles dynamic rerouting upon concept failure."),
        ("include/AssessmentEngine.hpp & src/AssessmentEngine.cpp", "Diagnostic testing and quiz grading engine with 15+ curated computer science conceptual questions."),
        ("include/DatabaseManager.hpp & src/DatabaseManager.cpp", "Relational DBMS transaction logger and JSON synchronization exporter for web dashboard."),
        ("src/main.cpp", "Interactive terminal CLI loop providing 10 real-time simulation, evaluation, and benchmark controls."),
        ("web/index.html & web/app.js", "Interactive browser dashboard rendering live Vis.js DAG network physics, real-time node inspector, and Chart.js benchmarks.")
    ]

    for file_item, desc in arch_items:
        story.append(Paragraph(f"• <b><code>{file_item}</code></b>: {desc}", body_style))

    story.append(Spacer(1, 10))

    # =========================================================================
    # CHAPTER 5: EMPIRICAL BENCHMARKING & RESULTS
    # =========================================================================
    story.append(Paragraph("5. Empirical Benchmarking & Experimental Verification", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    To validate the core hypothesis formulated in Phase-I (achieving 20% to 30% reduction in study time), empirical benchmarks were conducted comparing the <b>Static Linear Curriculum Baseline</b> against our <b>Dynamic Adaptive Dijkstra Engine</b> across 8 distinct target mastery goals:
    """, body_style))

    bench_data = [
        [Paragraph("<b>Target Goal Node</b>", body_style), Paragraph("<b>Goal Topic Title</b>", body_style), Paragraph("<b>Static (h)</b>", body_style), Paragraph("<b>Dynamic (h)</b>", body_style), Paragraph("<b>Time Saved</b>", body_style), Paragraph("<b>% Time Reduction</b>", body_style)],
        [Paragraph("<b>T04</b>", body_style), Paragraph("Stacks & Queues", body_style), Paragraph("16.0h", body_style), Paragraph("1.7h", body_style), Paragraph("14.3h", body_style), Paragraph("<b>89.3%</b>", body_style)],
        [Paragraph("<b>T06</b>", body_style), Paragraph("Binary Trees & Traversals", body_style), Paragraph("27.5h", body_style), Paragraph("18.9h", body_style), Paragraph("8.6h", body_style), Paragraph("<b>31.2%</b>", body_style)],
        [Paragraph("<b>T07</b>", body_style), Paragraph("BST & Balanced Trees", body_style), Paragraph("34.0h", body_style), Paragraph("28.6h", body_style), Paragraph("5.4h", body_style), Paragraph("<b>15.9%</b>", body_style)],
        [Paragraph("<b>T10</b>", body_style), Paragraph("Graph Traversals (BFS/DFS)", body_style), Paragraph("48.5h", body_style), Paragraph("25.9h", body_style), Paragraph("22.6h", body_style), Paragraph("<b>46.6%</b>", body_style)],
        [Paragraph("<b>T12</b>", body_style), Paragraph("Dijkstra's Shortest Path", body_style), Paragraph("60.5h", body_style), Paragraph("45.4h", body_style), Paragraph("15.1h", body_style), Paragraph("<b>24.9%</b>", body_style)],
        [Paragraph("<b>T13</b>", body_style), Paragraph("Minimum Spanning Trees", body_style), Paragraph("67.0h", body_style), Paragraph("44.8h", body_style), Paragraph("22.2h", body_style), Paragraph("<b>33.2%</b>", body_style)],
        [Paragraph("<b>T14</b>", body_style), Paragraph("Dynamic Programming", body_style), Paragraph("75.0h", body_style), Paragraph("31.5h", body_style), Paragraph("43.5h", body_style), Paragraph("<b>58.0%</b>", body_style)],
        [Paragraph("<b>T15</b>", body_style), Paragraph("Graph Dynamic Programming", body_style), Paragraph("82.5h", body_style), Paragraph("70.0h", body_style), Paragraph("12.5h", body_style), Paragraph("<b>15.2%</b>", body_style)],
        [Paragraph("<b>OVERALL</b>", body_style), Paragraph("<b>Full Curriculum Average</b>", body_style), Paragraph("<b>53.8h</b>", body_style), Paragraph("<b>34.9h</b>", body_style), Paragraph("<b>18.9h</b>", body_style), Paragraph("<b>35.1% Reduction</b>", body_style)]
    ]

    t_bench = Table(bench_data, colWidths=[65, 155, 65, 70, 70, 75])
    t_bench.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,0), colors.HexColor("#ddf4ff")),
        ('BACKGROUND', (0,-1), (-1,-1), colors.HexColor("#dafbe1")),
        ('ALIGN', (0,0), (-1,-1), 'LEFT'),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('BOX', (0,0), (-1,-1), 1, c_border),
        ('INNERGRID', (0,0), (-1,-1), 0.5, c_border),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
    ]))
    story.append(t_bench)
    story.append(Spacer(1, 10))

    story.append(Paragraph("""
    <b>Key Findings:</b><br/>
    1. <b>Hypothesis Exceeded:</b> The average time reduction of <b>35.1%</b> exceeds the Phase-I target threshold of 20-30%.<br/>
    2. <b>Redundant Module Pruning:</b> Irrelevant topic branches are bypassed, allowing students targeting specific competencies (such as Dynamic Programming) to focus exclusively on necessary prerequisites.<br/>
    3. <b>Retention Score Improvement:</b> Projected concept retention increases from <b>62.0%</b> to <b>88.5%</b> due to mandatory mastery enforcement on prerequisites before advanced traversal.
    """, body_style))

    story.append(PageBreak())

    # =========================================================================
    # CHAPTER 6: PBL VIVA VOCE DEFENSE GUIDE & MODEL ANSWERS
    # =========================================================================
    story.append(Paragraph("6. PBL Viva Voce Defense Guide & Model Answers", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=8))

    story.append(Paragraph("""
    Below are the most frequently asked technical defense questions by faculty evaluators along with model student answers:
    """, body_style))

    qa_list = [
        (
            "Q1: Why did you model the curriculum as a DAG instead of a standard tree or linked list?",
            "Ans: Real-world academic concepts have multiple cross-cutting prerequisite dependencies. For example, Tree Traversals (T06) requires both Linked Lists (T03) and Recursion (T05). A tree only permits a single parent per node, whereas a Directed Acyclic Graph (DAG) naturally supports multiple prerequisites while strictly prohibiting circular deadlocks."
        ),
        (
            "Q2: How does your Dijkstra pathfinding guarantee that prerequisites are satisfied before advanced topics?",
            "Ans: In standard Dijkstra on physical road maps, edge weights are static distances. In our educational engine, Dijkstra operates over the reverse-reachable ancestor subgraph. It maintains an in-degree counter for all prerequisite constraints and utilizes our custom MinHeapPriorityQueue to only select nodes whose in-degree has reached 0 (i.e., all prerequisites mastered or completed), dynamically ordering valid candidate nodes by lowest effective cognitive weight W'(v)."
        ),
        (
            "Q3: How is dynamic re-routing triggered when a student fails a quiz?",
            "Ans: When a quiz score falls below 60%, the AssessmentEngine notifies the OptimizationEngine. The engine identifies the weakest direct prerequisite, inflates W'(v) via the penalty formula, updates the relational evaluation_logs table, and re-executes Dijkstra's algorithm to inject remediation sub-modules into the student's active trajectory."
        ),
        (
            "Q4: How did you incorporate Operating Systems concepts?",
            "Ans: We implemented OSConcurrencySimulator using C++ std::thread, std::mutex, and lock_guard. We simulated concurrent student evaluation batches, measuring thread contention, throughput (9,000+ operations/sec), and execution latency to ensure thread safety during database writes and trajectory recalculations."
        )
    ]

    for q, a in qa_list:
        story.append(Paragraph(f"<b>{q}</b>", h2_style))
        story.append(Paragraph(a, body_style))
        story.append(Spacer(1, 4))

    # =========================================================================
    # CHAPTER 7: REFERENCES
    # =========================================================================
    story.append(Spacer(1, 10))
    story.append(Paragraph("7. Academic References & Citations", h1_style))
    story.append(HRFlowable(width="100%", thickness=1, color=c_border, spaceAfter=6))
    story.append(Paragraph("1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). <i>Introduction to Algorithms</i> (3rd ed.). MIT Press.", body_style))
    story.append(Paragraph("2. Graphic Era (Deemed to be University) Computer Science Department — <i>Project-Based Learning (PBL) Academic Guidelines 2026-27</i>.", body_style))
    story.append(Paragraph("3. Desmarais, M. C., & Baker, R. S. (2012). A review of recent advances in learner data modeling. <i>User Modeling and User-Adapted Interaction</i>, 22(1-2), 9-38.", body_style))

    doc.build(story, canvasmaker=NumberedCanvas)
    print(f"[SUCCESS] PDF successfully generated at: {target_pdf_path}")

if __name__ == "__main__":
    out_dir_docs = r"C:\Users\manit\Documents\Git\PBL 2\docs"
    out_dir_desktop = r"C:\Users\manit\Desktop"
    
    os.makedirs(out_dir_docs, exist_ok=True)
    pdf_docs = os.path.join(out_dir_docs, "Personalized_Learning_Path_Optimization_PBL_Study_Guide.pdf")
    pdf_desktop = os.path.join(out_dir_desktop, "Personalized_Learning_Path_Optimization_PBL_Study_Guide.pdf")

    build_pdf(pdf_docs)
    build_pdf(pdf_desktop)
