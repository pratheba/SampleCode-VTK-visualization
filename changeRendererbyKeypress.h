#pragma once
#ifndef CHANGERENDERERBYKEYPRESS_H
#define CHANGERENDERERBYKEYPRESS_H
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkSmartPointer.h>
#include <string>
#include "cellPick.h"

class changeRendererbyKeypress :public vtkInteractorStyleTrackballCamera
{
public:
	changeRendererbyKeypress(void);
	~changeRendererbyKeypress(void);
	static changeRendererbyKeypress* New();
    vtkTypeMacro(changeRendererbyKeypress, vtkInteractorStyleTrackballCamera);
	vtkSmartPointer<cellPick> pick;
	vtkSmartPointer<vtkInteractorStyleRubberBandZoom> zoom;
   
private:
	 virtual void OnKeyPress() ;
	 
};


#endif
