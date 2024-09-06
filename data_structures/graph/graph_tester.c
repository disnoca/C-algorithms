#include <stdio.h>
#include <assert.h>

#include "graph.h"
#include "../../wrapper_functions.h"


__attribute__((unused)) static void print_graph_contents(Graph* graph)
{
	for (size_t i = 0; i < graph->num_edges; i++) {
		GraphEdge* edge = graph->edges[i];
		printf("Edge %d: ", edge->id);
		for (size_t j = 0; j < edge->num_vertexes; j++)
			printf("%d ", edge->vertexes[j]->id);
		printf("\n");
	}
}


static void test_add_get_edge()
{
	Graph graph;
	graph_init(&graph, false);

	for (int i = 0; i < 100; i++)
		graph_add_edge(&graph, i);

	for (int i = 0; i < 100; i++)
		assert(graph_get_edge(&graph, i)->id == i);

	assert(graph.num_edges == 100);

	graph_free(&graph);
}

static void test_remove_edge()
{
	Graph graph;
	graph_init(&graph, false);
	for (int i = 0; i < 100; i++)
		graph_add_edge(&graph, i);

	for (int i = 0; i < 100; i += 2)
		graph_remove_edge(&graph, i);

	for (int i = 1; i < 100; i += 2)
		assert(graph_get_edge(&graph, i)->id == i);

	assert(graph.num_edges == 50);

	graph_free(&graph);
}

static void test_add_vertex()
{
	Graph graph, digraph;
	graph_init(&graph, false);
	graph_init(&digraph, true);

	for (int i = 0; i < 101; i++) {
		graph_add_edge(&graph, i);
		graph_add_edge(&digraph, i);
	}

	for (int i = 0; i < 100; i++) {
		graph_add_vertex(&graph, i, i + 1);
		graph_add_vertex(&digraph, i, i + 1);
	}

	assert(graph.num_vertexes == 100);
	assert(digraph.num_vertexes == 100);

	for (int i = 1; i < 100; i++) {
		GraphEdge* edge = graph_get_edge(&graph, i);
		GraphEdge* diedge = graph_get_edge(&digraph, i);

		assert(edge->num_vertexes == 2);
		assert(edge->vertexes[0]->id == i - 1);
		assert(edge->vertexes[1]->id == i + 1);

		assert(diedge->num_vertexes == 1);
		assert(diedge->vertexes[0]->id == i + 1);
	}

	graph_free(&graph);
	graph_free(&digraph);
}

static void test_remove_vertex()
{
	Graph graph, digraph;
	graph_init(&graph, false);
	graph_init(&digraph, true);

	for (int i = 0; i < 101; i++) {
		graph_add_edge(&graph, i);
		graph_add_edge(&digraph, i);
	}

	for (int i = 0; i < 100; i++) {
		graph_add_vertex(&graph, i, i + 1);
		graph_add_vertex(&digraph, i, i + 1);
	}

	for (int i = 0; i < 99; i += 2) {
		graph_remove_vertex(&graph, i, i + 1);
		graph_remove_vertex(&digraph, i, i + 1);
	}

	assert(graph.num_vertexes == 50);
	assert(digraph.num_vertexes == 50);

	for (int i = 1; i < 100; i += 2) {
		GraphEdge* edge1 = graph_get_edge(&graph, i);
		GraphEdge* edge2 = graph_get_edge(&graph, i + 1);
		GraphEdge* diedge1 = graph_get_edge(&digraph, i);
		GraphEdge* diedge2 = graph_get_edge(&digraph, i + 1);

		assert(edge1->num_vertexes == 1);
		assert(edge2->num_vertexes == 1);
		assert(edge1->vertexes[0]->id == i + 1);
		assert(edge2->vertexes[0]->id == i);

		assert(diedge1->num_vertexes == 1);
		assert(diedge2->num_vertexes == 0);
		assert(diedge1->vertexes[0]->id == i + 1);
	}

	graph_free(&graph);
	graph_free(&digraph);
}

static void test_remove_edge_with_vertexes()
{
	Graph graph, digraph;
	graph_init(&graph, false);
	graph_init(&digraph, true);

	for (int i = 0; i < 101; i++) {
		graph_add_edge(&graph, i);
		graph_add_edge(&digraph, i);
	}

	for (int i = 0; i < 100; i++) {
		graph_add_vertex(&graph, i, i + 1);
		graph_add_vertex(&digraph, i, i + 1);
	}

	for (int i = 1; i < 101; i += 2) {
		graph_remove_edge(&graph, i);
		graph_remove_edge(&digraph, i);
	}

	assert(graph.num_vertexes == 0);
	assert(digraph.num_vertexes == 0);

	for (int i = 1; i < 101; i += 2) {
		graph_add_edge(&graph, i);
		graph_add_edge(&digraph, i);
	}

	for (int i = 0; i < 100; i++) {
		graph_add_vertex(&graph, i, i + 1);
		graph_add_vertex(&digraph, i, i + 1);
	}

	for (int i = 0; i < 101; i += 3) {
		graph_remove_edge(&graph, i);
		graph_remove_edge(&digraph, i);
	}

	assert(graph.num_vertexes == 33);
	assert(digraph.num_vertexes == 33);

	for (int i = 1; i < 100; i += 3) {
		GraphEdge* edge1 = graph_get_edge(&graph, i);
		GraphEdge* edge2 = graph_get_edge(&graph, i + 1);
		GraphEdge* diedge1 = graph_get_edge(&digraph, i);
		GraphEdge* diedge2 = graph_get_edge(&digraph, i + 1);

		assert(edge1->num_vertexes == 1);
		assert(edge2->num_vertexes == 1);
		assert(edge1->vertexes[0]->id == i + 1);
		assert(edge2->vertexes[0]->id == i);

		assert(diedge1->num_vertexes == 1);
		assert(diedge2->num_vertexes == 0);
		assert(diedge1->vertexes[0]->id == i + 1);
	}

	graph_free(&graph);
	graph_free(&digraph);
}


int main()
{
    test_add_get_edge();
    test_remove_edge();
	test_add_vertex();
	test_remove_vertex();
	test_remove_edge_with_vertexes();

	assert(malloc_calls == free_calls);

    printf("All tests passed.\n");
    return 0;
}
