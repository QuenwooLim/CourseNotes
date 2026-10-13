import csv
import json
from neo4j import GraphDatabase

def create_neo4j_data(input_file, output_file):
    nodes = []
    edges = []
    node_ids = {}
    node_counter = 1

    with open(input_file, "r", encoding="utf-8") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            # Add Person node
            person_name = row["derived_teacher_name"]
            if person_name not in node_ids:
                node_ids[person_name] = str(node_counter)
                nodes.append({"id": str(node_counter), "label": "Person", "properties": {"name": person_name}})
                node_counter += 1

            # Add Organization node
            organization_name = row["college_name"]
            if organization_name not in node_ids:
                node_ids[organization_name] = str(node_counter)
                nodes.append({"id": str(node_counter), "label": "Organization", "properties": {"name": organization_name}})
                node_counter += 1

            # Add University node
            university_name = row["school_name"]
            if university_name not in node_ids:
                node_ids[university_name] = str(node_counter)
                nodes.append({"id": str(node_counter), "label": "University", "properties": {"name": university_name}})
                node_counter += 1

            # Add edges
            edges.append({"source": node_ids[person_name], "target": node_ids[organization_name], "label": "WORKS_AT"})
            edges.append({"source": node_ids[organization_name], "target": node_ids[university_name], "label": "BELONGS_TO"})

    # Save to JSON file
    with open(output_file, "w", encoding="utf-8") as jsonfile:
        json.dump({"nodes": nodes, "edges": edges}, jsonfile, ensure_ascii=False, indent=4)

if __name__ == "__main__":
    input_file = "data/preprocess_data.csv"
    output_file = "data/neo4j_data.json"
    create_neo4j_data(input_file, output_file)