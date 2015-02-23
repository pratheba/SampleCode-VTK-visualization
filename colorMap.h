#ifndef COLORMAP_H
#define COLORMAP_H
#include <vtkSmartPointer.h>
#include <vtkIntArray.h>
#include <vtkLookupTable.h>

struct CmapStruct{  
	vtkSmartPointer<vtkIntArray> cellData;
	vtkSmartPointer<vtkLookupTable> lut;
	int dim; 
	int k;
	int noOfClass;
};

class colorMap
{
public:
	colorMap(void);
	~colorMap(void);
	void createColorMap(CmapStruct* Cmap,int** indexPoints);
	
};


#endif
