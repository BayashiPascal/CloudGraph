// ============ MAIN.C ================

// ================= Include ==================

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "cloudgraph.h"

// ================= Main function ==================

int main(int argc, char **argv) {
  // Initialize the random generator
  time_t seed = time(NULL);
  srandom(seed);
  // Create the CloudGraph
  CloudGraph *cloud = CloudGraphCreate();
  // If we couldn't create the CloudGraph
  if (cloud == NULL) {
    // Display a message and stop
    fprintf(stderr, "Couldn't create the CloudGraph\n");
    return 1;
  }
  // Declare variables to memorize the arguments and set default values
  char flagPrint = 0;
  char *fileNameTGA = NULL;
  char *fileNameGraph = NULL;
  int nbNodeMin = 5;
  int nbNodeMax = 20;
  int nbFamilyMin = 1;
  int nbFamilyMax = 5;
  float density = 0.1;
  CloudGraphMode mode = CloudGraphModeLine;
  // Declare a variable for the graphical options when exporting to TGA
  CloudGraphOpt *opt = CloudGraphOptCreate();
  // If we couldn't create the CloudGraphOpt
  if (opt == NULL) {
    // Display a message
    fprintf(stderr, "Couldn't create the CloudGraphOpt\n");
    // Free memory
    CloudGraphFree(&cloud);
    // Stop here
    return 1;
  }
  // Decode arguments
  for (int iArg = 0; iArg < argc; ++iArg) {
    if (strcmp(argv[iArg] , "-tga") == 0 && iArg + 1 < argc) {
      fileNameTGA = argv[iArg + 1];
      ++iArg;
    } else if (strcmp(argv[iArg] , "-print") == 0) {
      flagPrint = 1;
    } else if (strcmp(argv[iArg] , "-curved") == 0 && iArg + 1 < argc) {
      CloudGraphOptSetCurvedLink(opt, true);
      float curvature = atof(argv[iArg + 1]);
      CloudGraphOptSetCurvature(opt, curvature);
      ++iArg;
    } else if (strcmp(argv[iArg] , "-circle") == 0) {
      mode = CloudGraphModeCircle;
    } else if (strcmp(argv[iArg] , "-line") == 0) {
      mode = CloudGraphModeLine;
    } else if (strcmp(argv[iArg] , "-nodeLabel") == 0) {
      CloudGraphOptSetNodeLabelMode(opt, CloudGraphOptNodeLabelAll);
    } else if (strcmp(argv[iArg] , "-familyLabel") == 0) {
      CloudGraphOptSetFamilyLabelMode(opt, CloudGraphOptFamilyLabelAll);
    } else if (strcmp(argv[iArg] , "-file") == 0 && iArg + 1 < argc) {
      fileNameGraph = argv[iArg + 1];
      ++iArg;
    } else if (strcmp(argv[iArg] , "-rnd") == 0 && iArg + 5 < argc) {
      nbNodeMin = atoi(argv[iArg + 1]);
      nbNodeMax = atoi(argv[iArg + 2]);
      nbFamilyMin = atoi(argv[iArg + 3]);
      nbFamilyMax = atoi(argv[iArg + 4]);
      density = atof(argv[iArg + 5]);
      iArg += 5;
    } else if (strcmp(argv[iArg] , "-help") == 0) {
      printf("arguments : [-tga <filename>] [-print]");
      printf(" [-file <filename>] [-free] [-circle] [-line]");
      printf(" [-rnd <nbNodeMin> <nbNodeMax> <nbFamilyMin>");
      printf(" <nbFamilyMax> <density>]");
      printf(" <-nodeLabel> <-familyLabel>");
      printf(" [-curved <curvature in [0.0,1.0]>]\n");
      printf("if -rnd and -file are both omitted, uses ");
      printf("'-rnd %d %d %d %d %f' by default\n", nbNodeMin, nbNodeMax, 
        nbFamilyMin, nbFamilyMax, density);
      // Stop here
      CloudGraphFree(&cloud);
      CloudGraphOptFree(&opt);
      return 0;
    }
  }
  // Set the mode
  CloudGraphOptSetMode(opt, mode);
  // If there is no input file
  if (fileNameGraph == NULL) {
    // Create a random graph
    bool ret = CloudGraphCreateRnd(&cloud, nbNodeMin, 
      nbNodeMax, nbFamilyMin, nbFamilyMax, density);
    // If we couldn't initialize the CloudGraph
    if (ret != true) {
      // Display a message
      fprintf(stderr, 
        "Error while creating the random graph\n");
      // Free the memory
      CloudGraphFree(&cloud);
      CloudGraphOptFree(&opt);
      // Stop here
      return 1;
    }
  // Else there is a input file
  } else {
    // Load the input file
    FILE *stream = fopen(fileNameGraph, "r");
    int ret = CloudGraphLoad(&cloud, stream);
    // If we couldn't load the CloudGraph
    if (ret != 0) {
      // Display a message
      fprintf(stderr, 
        "Error while loading the CloudGraph file (%d)\n", ret);
      // Free the memory
      CloudGraphFree(&cloud);
      CloudGraphOptFree(&opt);
      // Stop here
      return 1;
    }
    fclose(stream);
  }
  // Arrange the CloudGraph
  bool ret = CloudGraphArrange(cloud, opt);
  if (ret == false) {
    // Display a message
    fprintf(stderr, "Error while arranging the nodes\n");
    // Free the memory
    CloudGraphFree(&cloud);
    CloudGraphOptFree(&opt);
    // Stop here
    return 1;
  }
  // If there is a output TGA file
  if (fileNameTGA != NULL) {
    // Save the result in the TGA picture
    TGA *tga = CloudGraphToTGA(cloud, opt);
    if (tga == NULL) {
      // Display a message
      fprintf(stderr, "Error while exporting to TGA\n");
      // Free the memory
      CloudGraphFree(&cloud);
      CloudGraphOptFree(&opt);
      // Stop here
      return 1;
    }
    int ret = TGASave(tga, fileNameTGA);
    if (ret != 0) {
      // Display a message
      fprintf(stderr, "Error while saving TGA\n");
      // Free the memory
      CloudGraphFree(&cloud);
      CloudGraphOptFree(&opt);
      // Stop here
      return 1;
    }
    // Free the memory used by the TGA
    TGAFree(&tga);
  }
  // If the user requested printing of the CloudGraph
  if (flagPrint == 1) {
    // Print the cloud
    CloudGraphPrint(cloud, stdout);
  }
  // Free memory
  CloudGraphFree(&cloud);
  CloudGraphOptFree(&opt);
  // Return the success code
  return 0;
}

