# CloudGraph
CloudGraph is a C library of functions generating a 2D graphical representation of a graph based on the relations between its nodes. Two types of relation are considered: edges of the graph, and categories of nodes in the graph.

It also provides a front end which reads the graph definition from a text file or generate a random one, produces a TGA picture representing the network, and/or prints the nodes' 2D coordinates.

The representation of the graph has 2 modes: circular and linear. The representation of the links has 2 modes: straight line and curved line. Categories are represented by different color, and links between two categories have shading colors. Nodes and categories are also identified by labels which can be displayed.
