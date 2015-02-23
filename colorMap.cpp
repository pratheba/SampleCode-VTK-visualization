#include "colorMap.h"
#include <time.h>
const int MAX = 100;

colorMap::colorMap(void)
{}


colorMap::~colorMap(void)
{}

void colorMap::createColorMap(CmapStruct* Cmap, int** indexPoints){

	int dim = Cmap->dim;
	int k = Cmap->k;
	int noOfClass = Cmap->noOfClass;

	for(int i=0; i< dim; i++) 
		for(int j=0; j< dim ; j++) 
			Cmap->cellData->InsertNextValue(MAX);

	for(int i=0; i< dim; i++) 
		for(int j =0; j< k; j++) 
			Cmap->cellData->SetValue((int)indexPoints[i][j]+ dim*i,j +(i/(dim/noOfClass))*k);
		
	

	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	int tableSize = std::max(dim*dim+1, 10);
	
	Cmap->lut->SetNumberOfTableValues(k*noOfClass + 1);
	Cmap->lut->Build();

	
	srand ( time(NULL) );

	for(int i =0; i < noOfClass; i++) {
		float r1 = (float)(rand() % 255 + 1)/(float)255 ;
		float r2 = (float)(rand() % 255 + 1)/(float)255 ;
		float r3 = (float)(rand() % 255 + 1)/(float)255 ;
		
		for(int j =0; j < k ; j++)
			lut->SetTableValue(j+(k*j),r1,r2,r3, ((float)1/(float)k)*(j+1));
	}
	

	lut->SetTableValue((k*noOfClass),1,1,1,1);

}
