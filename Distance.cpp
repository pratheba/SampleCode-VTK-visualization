#include "Distance.h"
#include <vtkFieldData.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkArray.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkDataSet.h>
#include <vtkDoubleArray.h>

DenseArrPtr Distance::getEuclideanMatrix(DenseArrPtr matrix)
{
	return(calculateNormalizedMatrix(matrix));	 
}


// Calculate Normalized Matrix
DenseArrPtr Distance::calculateNormalizedMatrix(vtkSmartPointer<vtkDenseArray<double> > matrix)
{
	
 	DenseArrPtr squareMatrix = DenseArrPtr::New();
	DenseArrPtr transposeMatrix = DenseArrPtr::New();

	vtkArrayRange::CoordinateT m = matrix->GetExtents()[0].GetEnd();
	squareMatrix->Resize(m,m);

	vtkArrayRange::CoordinateT n = matrix->GetExtents()[1].GetEnd();
	transposeMatrix->Resize(n,m);

	transposeMatrix = calculateTransposeMatrix(matrix);

	//std::cout << "UnNormalized Matrix " << std::endl;
	double value = 0.0;
	for(vtkIdType i = 0 ; i < m; i++) {
		for(vtkIdType j = 0; j < m; j++) {
			value = 0.0;
			for(vtkIdType k = 0; k < n ; k++) {
				 value += (matrix->GetValue(i,k))*(transposeMatrix->GetValue(k,j));
			}
			squareMatrix->SetValue(i,j,value);
		}
	}
	
	double det = 0.0;
	det = Determinant(squareMatrix,m);
	
	

	for(vtkIdType i = 0 ; i < m; i++) 
		for(vtkIdType j = 0; j < m; j++) 
			squareMatrix->SetValue(i,j,(1/det)*(squareMatrix->GetValue(i,j)));

	return(calculateEuclideanMatrix(squareMatrix));
}

/*
//
// Function to calculate the  transpose Matrix
//
*/
DenseArrPtr Distance::calculateTransposeMatrix(DenseArrPtr matrix) {

	DenseArrPtr transMatrix = DenseArrPtr::New();

	vtkArrayRange::CoordinateT m = matrix->GetExtent(0).GetEnd();
	vtkArrayRange::CoordinateT n = matrix->GetExtent(1).GetEnd();

	transMatrix->Resize(n,m);

	
	for(vtkIdType i = 0 ; i < n; i++) 
		for(vtkIdType j= 0; j < m; j++) 
			transMatrix->SetValue(i,j,matrix->GetValue(j,i));


	return transMatrix;
}

/*
//
// Function to calculate the Euclidean matrix
//
*/
DenseArrPtr Distance::calculateEuclideanMatrix(DenseArrPtr matrix)
{
	DenseArrPtr euclideanMatrix = DenseArrPtr::New();
	vtkArrayRange::CoordinateT dim = matrix->GetExtents()[0].GetEnd();

	int y = matrix->GetExtents()[1].GetEnd();


	double *row_vector = NULL;
	double *col_vector = NULL;

	euclideanMatrix->Resize(dim,dim);

	int k = 0;

	
	for(vtkIdType i = 0; i < dim; i++) {
		for(vtkIdType k = 0; k < dim; k++) {
			
			row_vector = new double[dim];
			col_vector = new double[dim];
			for(vtkIdType j = 0; j < dim; j++)
			{	
				row_vector[j] = 0.0;
				col_vector[j] = 0.0;
				row_vector[j] = matrix->GetVariantValue(i,j).ToDouble();
				col_vector[j] = matrix->GetVariantValue(j,k).ToDouble();
			}
	
		double squaredDistance = vtkMath::Distance2BetweenPoints(row_vector, col_vector);
		double distance = sqrt(squaredDistance);
  
		euclideanMatrix->SetValue(i,k, distance);

		}
	}
	
	return euclideanMatrix;
}
  
/*
//
// Function to calculate the Determinant of a Matrix
//
*/
double Distance::Determinant(DenseArrPtr matrix,int order) {
  double det = 0.0;

  double* elements = NULL;

  vtkArrayRange::CoordinateT rows,cols;
  rows = matrix->GetExtents()[0].GetEnd();
  cols = matrix->GetExtents()[1].GetEnd();

  if(rows==cols) {

	  if(rows == 1) {
		  det = matrix->GetValue(0,0);
	  }
	  else if (rows == 2) {
		  det = matrix->GetValue(0,0) * matrix->GetValue(1,1) - matrix->GetValue(1,0) * matrix->GetValue(0,1);
	  }
	  else if (rows == 3) {

		  vtkSmartPointer<vtkMatrix3x3> mat3x3 =  vtkSmartPointer<vtkMatrix3x3>::New();

		  for(int i=0; i< 9;i++)
			  elements[i] = matrix->GetValueN(i);

		  det = mat3x3->Determinant(elements);
		  
	  }
	  else if( rows == 4) {
		  
		  vtkSmartPointer<vtkMatrix4x4> mat4x4 =  vtkSmartPointer<vtkMatrix4x4>::New(); 
		  
		  double elements[16] = {0};

		  for(int i=0;i < 16; i++)
			  elements[i] = matrix->GetValueN(i);

		  det = mat4x4->Determinant(elements);
		 
	  }
	  else {
		  for(vtkIdType i = 0; i < rows ; i++) {
			  double temp = Determinant(calculateMinor(matrix, rows, 0, i), rows - 1);
			  std::cout << "temp::" << temp << "\n";
			  det += static_cast<int>(pow(-1.0,(int)i)) * matrix->GetValue(i,0) * temp;
		  }
	  } }
  else {}

	return det;
}

/*
//
// Function to calculate the Minor of a Matrix 
//
*/
DenseArrPtr Distance::calculateMinor(DenseArrPtr matrix, int order, int i, int j)
{
	DenseArrPtr submatrix = DenseArrPtr::New();
	int mat_i, mat_j;				// Indexes for matrix
	int subMat_i = 0, subMat_j;		// Indexes for subm
	submatrix->Resize(order - 1, order -1);

	for(mat_i = 0; mat_i < order; mat_i++) {
		if(mat_i == i) continue;				//Skip ith row
			subMat_j = 0;

		for(mat_j = 0; mat_j < order; mat_j++) {
				if(mat_j ==j) continue;		//Skip jth column
				submatrix->SetValue(subMat_i,subMat_j++,matrix->GetValue(mat_i,mat_j));
		}
		subMat_i++; //Increment row index
	}
	return submatrix;
}

