import spacy
import json
import re
import pandas as pd

def extract_information(description):
    nlp = spacy.load("zh_core_web_sm")
    doc = nlp(description)

    # Extract name
    name_match = re.search(r"\*\*(.*?)\*\*", description)
    name = name_match.group(1) if name_match else ""

    # Extract workplace and department
    workplace = ""
    department = ""
    for ent in doc.ents:
        if ent.label_ == "ORG":
            if "大学" in ent.text:
                workplace = ent.text
            else:
                department = ent.text

    return {
        "name": name,
        "workplace": workplace,
        "department": department
    }

def process_data(input_file, output_file):
    # Load data
    df = pd.read_csv(input_file)

    # Extract information from description field
    extracted_data = []
    for _, row in df.iterrows():
        description = row.get("description", "")
        if description:
            info = extract_information(description)
            extracted_data.append({
                "subject": info["name"],
                "relation": "工作于",
                "object": info["workplace"],
                "department": info["department"]
            })

    # Save extracted data to JSON
    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(extracted_data, f, ensure_ascii=False, indent=4)

if __name__ == "__main__":
    input_file = "data/preprocess_data.csv"
    output_file = "data/entities_relations.json"
    process_data(input_file, output_file)