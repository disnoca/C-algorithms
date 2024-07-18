#include <stdint.h>
#include <string.h>

#include "graph.h"
#include "../../wrapper_functions.h"


/* ---------------- Helper Functions ---------------- */

static size_t find_edge_pos(Graph* g, int id)
{
	if (id > 0 && (size_t)id < g->num_edges && g->edges[id]->id == id)
		return id;

	for (size_t i = 0; i < g->num_edges; i++)
		if (g->edges[i]->id == id)
			return i;
		
	exit_with_error("No such edge with id %llu\n", id);
	return SIZE_MAX;
}

static void add_vertex_from_to(GraphEdge* from, GraphEdge* to)
{
	if (from->num_vertexes == from->capacity) {
		from->capacity *= 2;
		from->vertexes = Realloc(from->vertexes, from->capacity * sizeof(GraphEdge*));
	}

	from->vertexes[from->num_vertexes] = to;
	from->num_vertexes++;
}

static void remove_vertex_from_to(GraphEdge* from, GraphEdge* to)
{
	for (size_t i = 0; i < from->num_vertexes - 1; i++)
		if (from->vertexes[i]->id == to->id) {
			memmove(from->vertexes + i, from->vertexes + i + 1, (from->num_vertexes - 1 - i) * sizeof(GraphEdge*));
			break;
		}

	from->num_vertexes--;
}


/* ---------------- Header Implementation ---------------- */

void graph_init(Graph* g, bool is_directed)
{
	g->capacity = GRAPH_DEFAULT_CAPACITY;
	g->edges = Malloc(g->capacity * sizeof(GraphEdge*));
	g->num_edges = 0;
	g->num_vertexes = 0;
	g->is_directed = is_directed;
}

void graph_add_edge(Graph* g, int id)
{
	if (g->num_edges == g->capacity) {
		g->capacity *= 2;
		g->edges = Realloc(g->edges, g->capacity * sizeof(GraphEdge*));
	}

	GraphEdge* edge = Malloc(sizeof(GraphEdge));
	edge->id = id;
	edge->capacity = EDGE_DEFAULT_CAPACITY;
	edge->vertexes = Malloc(edge->capacity * sizeof(GraphEdge*));
	edge->num_vertexes = 0;

	g->edges[g->num_edges++] = edge;
}

void graph_remove_edge(Graph* g, int id)
{
	size_t pos = find_edge_pos(g, id);
	GraphEdge* edge = g->edges[pos];

	if (!g->is_directed) {
		for (size_t i = 0; i < edge->num_vertexes; i++)
			remove_vertex_from_to(edge->vertexes[i], edge);
	} else {
		for (size_t i = 0; i < g->num_edges; i++) {
			if (i == pos) continue;
			
			GraphEdge* from_edge = g->edges[i];
			for (size_t j = 0; j < from_edge->num_vertexes; j++)
				if (from_edge->vertexes[j]->id == edge->id) {
					remove_vertex_from_to(from_edge, edge);
					g->num_vertexes--;
				}
		}
	}

	memmove(g->edges + pos, g->edges + pos + 1, (g->num_edges - 1 - pos) * sizeof(GraphEdge*));

	g->num_edges--;
	g->num_vertexes -= edge->num_vertexes;

	Free(edge->vertexes);
	Free(edge);
}

GraphEdge* graph_get_edge(Graph* g, int id)
{
	return g->edges[find_edge_pos(g, id)];
}

void graph_add_vertex(Graph* g, int from, int to)
{
	GraphEdge* from_edge = graph_get_edge(g, from);
	GraphEdge* to_edge = graph_get_edge(g, to);

	add_vertex_from_to(from_edge, to_edge);
	if (!g->is_directed)
		add_vertex_from_to(to_edge, from_edge);

	g->num_vertexes++;
}

void graph_remove_vertex(Graph* g, int from, int to)
{
	GraphEdge* from_edge = graph_get_edge(g, from);
	GraphEdge* to_edge = graph_get_edge(g, to);

	remove_vertex_from_to(from_edge, to_edge);
	if (!g->is_directed)
		remove_vertex_from_to(to_edge, from_edge);

	g->num_vertexes--;
}

void graph_clear_edges(Graph* g)
{
	for (size_t i = 0; i < g->num_edges; i++) {
		Free(g->edges[i]->vertexes);
		Free(g->edges[i]);
	}

	g->num_edges = 0;
	g->num_vertexes = 0;
}

void graph_clear_vertexes(Graph* g)
{
	for (size_t i = 0; i < g->num_edges; i++)
		g->edges[i]->num_vertexes = 0;
}

void graph_free(Graph* g)
{
	graph_clear_edges(g);
	Free(g->edges);
}
