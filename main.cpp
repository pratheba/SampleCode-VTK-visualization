#include "project_config.h"

#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkDelimitedTextReader.h>
#include "parser.h"
#include "Distance.h"
#include "drawSimilarityMatrix.h"
#include "mainInterface.h"
 
int main(int, char *[])
{
	std::string inputFileName = "sampletest.csv";

	Parser* parser_ = new Parser();
	Distance* distance_ = new Distance();
	mainInterface* maininterface_ = new mainInterface();
	DrawSimilarityMatrix* simMatrix_ = new DrawSimilarityMatrix();

	DenseArrPtr InputMatrix  = DenseArrPtr::New();
	maininterface_->euclideanMatrix  = DenseArrPtr::New();

	// Parse the Input File for Feature Matrix and Class matrix
	InputMatrix = parser_->parseInputFile(inputFileName);

	// Find the euclidean matrix for the feature set
	maininterface_->euclideanMatrix = distance_->getEuclideanMatrix(InputMatrix);

	// Draw the Shaded Similarity Matrix
	simMatrix_->drawShadedMatrix(maininterface_->euclideanMatrix);

	

}