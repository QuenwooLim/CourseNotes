from py2neo import Graph, Node, Relationship, NodeMatcher
import json

def import_to_neo4j(uri, user, password, json_file):
    # Connect to the Neo4j database
    graph = Graph(uri, auth=(user, password))
    print("Connected to Neo4j Database")

    # Load JSON data
    with open(json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    # Create nodes
    for node in data['nodes']:
        n = Node(node['label'], id=node['id'], name=node['properties']['name'])
        graph.merge(n, node['label'], 'id')

    # Create relationships
    for edge in data['edges']:
        matcher = NodeMatcher(graph)
        source_node = matcher.match(id=edge['source']).first()
        target_node = matcher.match(id=edge['target']).first()
        if source_node and target_node:
            rel = Relationship(source_node, edge['label'], target_node)
            graph.merge(rel)

if __name__ == "__main__":
    # Update these with your Neo4j connection details
    uri = "http://localhost:7474"
    user = "neo4j"
    password = "neo4jneo4j"

    # Path to the JSON file
    json_file = "data/neo4j_data.json"

    # Import data to Neo4j
    import_to_neo4j(uri, user, password, json_file)