import json
import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_agraph import graphviz_layout

# Load the JSON data
with open('minimax.json') as f:
    data = json.load(f)

# Create a directed graph
G = nx.DiGraph()

# Recursive function to add nodes and edges to the graph
def add_nodes_edges(node, parent=None):
    G.add_node(node['id'], label=f"ID: {node['id']}\nValue: {node['value']}")
    if parent is not None:
        G.add_edge(parent, node['id'])
    for child in node.get('children', []):
        add_nodes_edges(child, node['id'])

# Add nodes and edges starting from the root
add_nodes_edges(data)

# Draw the graph
pos = graphviz_layout(G, prog='dot')
labels = nx.get_node_attributes(G, 'label')
plt.figure(figsize=(12, 8))
nx.draw(G, pos, with_labels=True, labels=labels, node_size=3000, node_color='skyblue', font_size=10, font_color='black', font_weight='bold')
plt.title('Minimax Tree')

# Save the plot as an image file (e.g., PNG)
plt.savefig('minimax_tree.png')

# # Optionally, display the plot
# plt.show()
