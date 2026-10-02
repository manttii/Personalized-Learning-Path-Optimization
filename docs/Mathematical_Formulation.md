# Mathematical Optimization Formulation
## Personalized Learning Path Optimization
**Academic Session 2026–27 | Graphic Era (Deemed to be University)**

---

## 1. Directed Acyclic Graph (DAG) Model

Let the curriculum knowledge base be represented as a Directed Acyclic Graph:
$$G = (V, E, W)$$
where:
- $V = \{v_1, v_2, \dots, v_n\}$ is the set of topic/module vertices.
- $E \subseteq V \times V$ is the set of directed prerequisite edges. An edge $(u, v) \in E$ indicates that topic $u$ is an immediate prerequisite for topic $v$.
- $W: V \to \mathbb{R}^+$ maps each vertex to its base estimated study hours $W(v)$.

### Acyclicity Invariant
To ensure logical progression without deadlocks or cyclic dependencies:
$$\forall \text{ path } \langle v_{i_1}, v_{i_2}, \dots, v_{i_k} \rangle \text{ in } G, \quad v_{i_1} \ne v_{i_k} \quad (k > 1)$$
This is formally validated at initialization using DFS 3-color cycle detection in $O(|V| + |E|)$ time.

---

## 2. Dynamic Effective Weight Formulation

For an individual student $s$ with evaluated proficiency state vector $\mathbf{P} = [P(v_1), P(v_2), \dots, P(v_n)]^T$ where $P(v_i) \in [0.0, 1.0]$, the **Dynamic Effective Weight** $W'(v_i)$ is formulated as:

$$W'(v_i) = W(v_i) \times \Big[1 + \alpha\big(1 - P(v_i)\big)\Big] + \text{PrereqPenalty}(v_i)$$

where:
- $\alpha \ge 0$ is the student learning velocity sensitivity factor ($\alpha = 0.35$).
- $\text{PrereqPenalty}(v_i)$ penalizes attempting advanced topics when foundational prerequisites have not achieved minimum mastery threshold $\tau = 0.60$:

$$\text{PrereqPenalty}(v_i) = \sum_{\substack{u \in \text{Pred}(v_i) \\ P(u) < \tau}} \beta \times (\tau - P(u))$$

with penalty multiplier $\beta = 1.5$.

### Boundary Cases:
1. **Full Mastery ($P(v_i) = 1.0$, all prerequisites satisfied):**
   $$W'(v_i) = W(v_i) \times 1.0$$
   *(Fast-tracked review factor reduces this to $0.15 \times W(v_i)$ during subsequent traversals).*
2. **Zero Prior Knowledge ($P(v_i) = 0.0$):**
   $$W'(v_i) = W(v_i) \times [1 + \alpha] = 1.35 \times W(v_i)$$
3. **Severe Prerequisite Deficit ($P(u) = 0.0$ for prerequisite $u$):**
   $$W'(v_i) = W(v_i) \times 1.35 + 1.5 \times (0.60 - 0.0) = 1.35 W(v_i) + 0.90$$

---

## 3. Dynamic Dijkstra Pathfinding with Min-Heap

The optimal trajectory $\mathcal{T}^*$ from root concepts to target goal node $v_{\text{goal}}$ minimizes the cumulative effective learning hours subject to prerequisite satisfaction:

$$\mathcal{T}^* = \arg\min_{\mathcal{T}} \sum_{v \in \mathcal{T}} W'(v)$$
$$\text{subject to } \forall v \in \mathcal{T}, \quad \text{Pred}(v) \subseteq \text{Prefix}(\mathcal{T}, v)$$

### Algorithmic Execution:
1. Extract required ancestor subgraph $V_{\text{req}} = \text{Ancestors}(v_{\text{goal}}) \cup \{v_{\text{goal}}\}$ using reverse BFS in $O(|V| + |E|)$.
2. Calculate in-degrees for all $u \in V_{\text{req}}$.
3. Initialize custom Binary Min-Heap $\mathcal{Q}$ with all candidate nodes having in-degree 0, keyed by their dynamic effective weight $W'(u)$.
4. While $\mathcal{Q} \ne \emptyset$:
   - Extract minimum element $u^* = \text{Extract-Min}(\mathcal{Q})$.
   - Append $u^*$ to trajectory.
   - For each dependent $(u^*, v) \in E$, decrement $\text{in-degree}(v)$. If $\text{in-degree}(v) = 0$, insert $v$ into $\mathcal{Q}$ with priority $W'(v)$.

### Time Complexity:
Using the custom templated Binary Min-Heap with decrease-key:
$$\mathcal{O}\big((|V| + |E|) \log |V|\big)$$
Space Complexity: $\mathcal{O}(|V| + |E|)$.
