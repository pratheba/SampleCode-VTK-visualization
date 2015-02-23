#include "drawSimilarityMatrix.h"
#include "cellPick.h"
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
 
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPlaneSource.h>
#include <vtkIntArray.h>

#include <map>
#include <set>


const int k= 2;
const int noOfClass = 3;
const int MAX = 100;

vtkStandardNewMacro(cellPick);
 

void DrawSimilarityMatrix::drawShadedMatrix(vtkSmartPointer<vtkDenseArray<double> > matrix) {
	
	int** indexPoints = findtheKNN(matrix);

	int dim = matrix->GetExtents()[0].GetEnd();

	vtkSmartPointer<vtkPlaneSource> aPlane = vtkSmartPointer<vtkPlaneSource>::New();
	aPlane->SetXResolution(dim);
	aPlane->SetYResolution(dim);

	// create cell data

	vtkSmartPointer<vtkIntArray> cellData = vtkSmartPointer<vtkIntArray>::New();
	for(int i=0; i< dim; i++) 
		for(int j=0; j< dim ; j++) 
			cellData->InsertNextValue(MAX);

	for(int i=0; i< dim; i++) {
		for(int j =0; j< k; j++) {
			cellData->SetValue((int)indexPoints[i][j]+ dim*i,j +(i/(dim/noOfClass))*k);
		}
	}

	// create Lookup Table mapping cell data to colors

	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	int tableSize = std::max(dim*dim+1, 10);
	lut->SetNumberOfTableValues(k*noOfClass + 1);
	lut->Build();

	for(int i=0; i < k ; i++)
		lut->SetTableValue(i,1,0,0, ((float)1/(float)k)*(i+1));
	for(int i=0;i<k;i++)
		lut->SetTableValue(i+(k*1), 0,1,0,((float)1/(float)k)*(i+1));
	for(int i=0;i<k;i++)
		lut->SetTableValue(i + (k*2), 0,0,1,((float)1/(float)k)*(i+1));

	lut->SetTableValue((k*3),1,1,1,1);

	aPlane->Update();
	aPlane->GetOutput()->GetCellData()->SetScalars(cellData);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(aPlane->GetOutputPort());
  mapper->SetScalarRange(0, (k*noOfClass)+1);
  mapper->SetLookupTable(lut);
 
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  // Setup render window, renderer, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->AddActor(actor);
  renderer->SetBackground(0,0,0);
  renderWindow->Render();
  renderWindowInteractor->Start();

}


int** DrawSimilarityMatrix::findtheKNN(vtkSmartPointer<vtkDenseArray<double> > matrix) {

	int dim = matrix->GetExtents()[0].GetEnd();
	std::map<double, const int> KNNMap;
	std::map<double, const int>::const_iterator iter;

	//std::set<const int> *KNNSet = new std::set<const int>[dim]();

	int** indexPoints = new int*();
	int index =0;
	
	for(int i = 0; i < dim; i++) {
		index = 0;
		KNNMap.clear();
		for(int j = 0; j < dim ; j++) {
			//std::cout << matrix->GetValue(i,j) << "\t";
			KNNMap.insert(std::pair<double,const int>(matrix->GetValue(i,j),j));
		}

		std::cout<<"\n";
		//std::cout << "i=" << i << std::endl;
		//std::cout << "here " << std::endl;

		iter = KNNMap.begin();
		indexPoints[i] = new int[dim];
		
		//KNNSet[i] = new std::set<const int>();

		std::cout << "Matrix value " << std::endl;

		for(iter; iter!= KNNMap.end(); iter++) {
			std::cout << (*iter).second << "\t";
			indexPoints[i][index++] = (*iter).second;
		}
		std::cout << "\n";
	}

	std::cout << "\n";
	

	return indexPoints;
}