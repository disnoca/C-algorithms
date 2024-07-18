#pragma once

#include <stddef.h>
#include <stdbool.h>


#define EDGE_DEFAULT_CAPACITY 	2
#define GRAPH_DEFAULT_CAPACITY 	10

typedef struct GraphEdge {
	int id;
	struct GraphEdge** vertexes;
	size_t num_vertexes;
	size_t capacity;
} GraphEdge;

typedef struct {
	GraphEdge** edges;
	size_t num_edges;
	size_t num_vertexes;
	size_t capacity;
	bool is_directed;
} Graph;


void graph_init(Graph* graph, bool is_directed);
void graph_free(Graph* graph);

void graph_add_edge(Graph* graph, int id);
void graph_remove_edge(Graph* graph, int id);
GraphEdge* graph_get_edge(Graph* graph, int id);
void graph_clear_edges(Graph* graph);

void graph_add_vertex(Graph* graph, int from, int to);
void graph_remove_vertex(Graph* graph, int from, int to);
void graph_clear_vertexes(Graph* graph);
