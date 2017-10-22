// ============ CLOUDGRAPH.H ================

#ifndef CLOUDGRAPH_H
#define CLOUDGRAPH_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "gset.h"
#include "pbmath.h"
#include "bcurve.h"
#include "tgapaint.h"

// ================= Define ==================

#define CG_NBMAXFAMILY 100

// ================= Data structures ===================

// Node of the cloud
typedef struct CloudGraphNode {
  // ID of the node
  int _id;
  // ID of the family of this node
  int _family;
  // Label of this node
  char *_label;
  // Position
  VecFloat *_pos;
  // Bounding box of the node
  Shapoid *_boundingBox;
  // Bounding box of the label
  Shapoid *_boundingBoxLbl;
  // Vector indicating the right direction from this node
  VecFloat *_right;
  // Angle with the abciss 
  float _theta;
} CloudGraphNode;

// Family of the node
typedef struct CloudGraphFamily {
  // ID of the family
  int _id;
  // Color of the family
  unsigned char _rgba[4];
  // Label of this family
  char *_label;
  // Bounding box of the label
  Shapoid *_boundingBox;
  // Position of the label
  VecFloat *_pos;
  // Vector indicating the right direction of the label
  VecFloat *_right;
} CloudGraphFamily;

// Link of the CloudGraph
typedef struct CloudGraphLink {
  // ID of the nodes
  int _nodes[2];
  // BCurve to trace this link
  BCurve *_curve;
  // Bounding box of the link
  Shapoid *_boundingBox;
  // ID of families (for color selection);
  int _families[2];
} CloudGraphLink;

// CloudGraph
typedef struct CloudGraph {
  // SpringSys representing the CloudGraph
  GSet *_nodes;
  // List of families
  GSet *_families;
  // List of links
  GSet *_links;
  // Font to write the labels
  TGAFont *_font;
  // Bounding bos of the cloud
  Shapoid *_boundingBox;
} CloudGraph;

// Modes of CloudGraph representation
typedef enum CloudGraphMode {
  // Default, Nodes are placed along a line
  CloudGraphModeLine,
  // Nodes are placed on the circumference of a circle
  CloudGraphModeCircle
} CloudGraphMode;

// Modes of node's label representation
typedef enum CloudGraphOptNodeLabel {
  // Default, no label
  CloudGraphOptNodeLabelNone, 
  // Label of all nodes
  CloudGraphOptNodeLabelAll
} CloudGraphOptNodeLabel;

// Modes of family's label representation
typedef enum CloudGraphOptFamilyLabel {
  // Default, no label
  CloudGraphOptFamilyLabelNone, 
  // Label at the center of the family
  CloudGraphOptFamilyLabelAll
} CloudGraphOptFamilyLabel;

// Graphical options while exporting to TGA
typedef struct CloudGraphOpt {
  // Mode of CloudGraph representation
  CloudGraphMode _mode;
  // Flag to memorize if the links should be represented as curves
  // In linear mode they are always curved 
  bool _curvedLink;
  // Curvature in ]0.0, inf[
  float _curvature;
  // Mode for nodes' label
  CloudGraphOptNodeLabel _nodeLabelMode;
  // Mode for families' label
  CloudGraphOptFamilyLabel _familyLabelMode;
  // Font size for nodes
  float _fontSizeNode;
  // Font size for families
  float _fontSizeFamily;
} CloudGraphOpt;

// ================ Functions declaration ====================

// Create a new CloudGraph
// Return NULL if we couldn't create the CloudGraph
CloudGraph* CloudGraphCreate(void);

// Free the memory used by a CloudGraph
// Do nothing if arguments are invalid
void CloudGraphFree(CloudGraph **cloud);

// Free the memory used by a CloudGraphNode
// Do nothing if arguments are invalid
void CloudGraphNodeFree(CloudGraphNode** node);

// Free the memory used by a CloudGraphFamily
// Do nothing if arguments are invalid
void CloudGraphFamilyFree(CloudGraphFamily** family);

// Free the memory used by a CloudGraphLink
// Do nothing if arguments are invalid
void CloudGraphLinkFree(CloudGraphLink** link);

// Set the represention mode to 'mode'
// Do nothing if arguments are invalid
void CloudGraphOptSetMode(CloudGraphOpt *opt, CloudGraphMode mode);

// Create a random CloudGraph having between 'nbNodeMin' and 'nbNodeMax'
// nodes, and between 'nbFamilyMin' and 'nbFamilyMax' families, and 
// 'density' (in [0,1]) probability of connection between each pair of 
// nodes
// If 'cloud' is not NULL it is first freed
// The random generator must be initialized before calling this function
// Return true on success, false else (invalid arguments or malloc failed)
bool CloudGraphCreateRnd(CloudGraph **cloud, int nbNodeMin, 
  int nbNodeMax, int nbFamilyMin, int nbFamilyMax, float density);

// Create a CloudGraphFamily with default values:
// _id = 0
// _rgba = {0, 0, 0, 255}
// _label = NULL;
// Return NULL if couldn't create the family
CloudGraphFamily* CloudGraphCreateFamily(void);

// Add a copy of the family 'f' to the CloudGraph
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool CloudGraphAddFamily(CloudGraph *cloud, CloudGraphFamily *f);

// Create a CloudGraphNode with default values:
// _id = 0
// _family = 0
// _label = NULL
// Return NULL if couldn't create the family
CloudGraphNode* CloudGraphCreateNode(void);

// Add a copy of the node 'n' to the CloudGraph
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool CloudGraphAddNode(CloudGraph *cloud, CloudGraphNode *n);

// Create a CloudGraphLink with default values:
// _nodes[0] = _nodes[1] = -1
// Return NULL if couldn't create the link
CloudGraphLink* CloudGraphCreateLink(void);

// Add a copy of the link 'l' to the CloudGraph
// Return false if the arguments are invalid or memory allocation failed
// else return true
bool CloudGraphAddLink(CloudGraph *cloud, CloudGraphLink *l);

// Load the CloudGraph from 'stream'
// If 'cloud' is not NULL it is first freed
// Return 0 on success
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int CloudGraphLoad(CloudGraph **cloud, FILE *stream);

// Arrange the position of the nodes of the graph
// Return true if it could arrange nodes
// Return false if arguments are invalid or it couldn't arrange nodes
bool CloudGraphArrange(CloudGraph *cloud, CloudGraphOpt *opt);

// Get a TGA picture representing the CloudGraph using the graphical
// options 'opt'
// Return NULL if we couldn't create the TGA
TGA* CloudGraphToTGA(CloudGraph *cloud, CloudGraphOpt *opt);

// Print the CloudGraph on 'stream'
// Do nothing if arguments are invalid
void CloudGraphPrint(CloudGraph *cloud, FILE* stream);

// Print the CloudGraphFamily on 'stream'
// Do nothing if arguments are invalid
void CloudGraphFamilyPrint(void *f, FILE *stream);

// Print the CloudGraphNode 'n' on 'stream'
// Do nothing if arguments are invalid
void CloudGraphNodePrint(void *n, FILE *stream);

// Print the CloudGraphLink on 'stream'
// Do nothing if arguments are invalid
void CloudGraphLinkPrint(void *l, FILE *stream);

// Create a new CloudGraphOpt
// Default _mode = CloudGraphModeFree
// Default _curvedLink = false
// Default _curvature = 1.0
// Default _nodeLabelMode = CloudGraphOptNodeLabelNone
// Default _familyLabelMode = CloudGraphOptFamilyLabelNone
// Default _fontSizeNode = 15
// Default _fontSizeFamily = 18
// Return NULL if we couldn't create the CloudGraphOpt
CloudGraphOpt* CloudGraphOptCreate(void);

// Free the memory used by the CloudGraphOpt
// Do nothing if arguments are invalid
void CloudGraphOptFree(CloudGraphOpt **opt);

// Set the flag defining if the links are curved to 'curved'
// Do nothing if arguments are invalid
void CloudGraphOptSetCurvedLink(CloudGraphOpt *opt, bool curved);

// Set the curvature to 'v' (in [0.0,1.0])
// Do nothing if arguments are invalid
void CloudGraphOptSetCurvature(CloudGraphOpt *opt, float v);

// Set the mode of display for nodes' label to 'mode'
// Do nothing if arguments are invalid
void CloudGraphOptSetNodeLabelMode(CloudGraphOpt *opt, 
  CloudGraphOptNodeLabel mode);

// Set the mode of display for families' label to 'mode'
// Do nothing if arguments are invalid
void CloudGraphOptSetFamilyLabelMode(CloudGraphOpt *opt, 
  CloudGraphOptFamilyLabel mode);

// Set the font size for nodes' label to 'size'
// Do nothing if arguments are invalid
void CloudGraphOptSetFontSizeNode(CloudGraphOpt *opt, float size);

// Set the font size for families' label to 'size'
// Do nothing if arguments are invalid
void CloudGraphOptSetFontSizeFamily(CloudGraphOpt *opt, float size);

// Return the length of the longest displayed node label
// Return 0.0 of arguments are invalid or there is no displayed label
float CloudGraphGetMaxLengthLblNode(CloudGraph *cloud, CloudGraphOpt *opt);

// Return the length of the longest displayed family label
// Return 0.0 of arguments are invalid or there is no displayed label
float CloudGraphGetMaxLengthLblFamily(CloudGraph *cloud, CloudGraphOpt *opt);

// Return the node 'id' or NULL if arguments are invalid
CloudGraphNode* CloudGraphGetNode(CloudGraph *cloud, int id);

// Return the family 'id' or NULL if arguments are invalid
CloudGraphFamily* CloudGraphGetFamily(CloudGraph *cloud, int id);

#endif
