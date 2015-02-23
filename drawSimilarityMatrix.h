#pragma once
#ifndef DRAWSIMILARITYMATRIX_H
#define DRAWSIMILARITYMATRIX_H

#include <vtkSmartPointer.h>
#include <vtkDenseArray.h>

class DrawSimilarityMatrix
{
public:
	DrawSimilarityMatrix(void){}
	~DrawSimilarityMatrix(void){}

	void drawShadedMatrix(vtkSmartPointer<vtkDenseArray<double> > matrix);

private:
	int** findtheKNN(vtkSmartPointer<vtkDenseArray<double> > matrix);

};

#endif

