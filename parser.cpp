#include "parser.h"
#include <vtkSmartPointer.h>
#include <vtkDelimitedTextReader.h>
#include <vtkIdTypeArray.h>
#include <vtkSortDataArray.h>
#include <vtkAbstractArray.h>
#include <vtkObject.h>
#include <vtkIdList.h>
#include <vtkVariantArray.h>
#include <map>

Parser::Parser(void)
{
	mainInterfaceObj = new mainInterface();
}


Parser::~Parser(void)
{
	delete mainInterfaceObj;
} 

/*
//
// Function to parse the Input file (CSV) by seperating field with delimited Character
//
*/
DenseArrPtr  Parser::parseInputFile(const std::string inputFileName) {
	
	vtkSmartPointer<vtkDelimitedTextReader> reader = vtkSmartPointer<vtkDelimitedTextReader>::New();
	reader->SetFileName(inputFileName.c_str());
	reader->SetFieldDelimiterCharacters(",");
	reader->Update();

	// Input File is read Into vtkTable with each line in Input 
	// filled as row information
	vtkSmartPointer<vtkTable> table = reader->GetOutput();	
	return(writeToMatrix(table));
}


/*
//
// Function to Read the values from the Table and obtain the feature and the class Matrix
//
*/
DenseArrPtr Parser::writeToMatrix(vtkSmartPointer<vtkTable> table) {

	int noOfCol = table->GetNumberOfColumns();
	int noOfRows = table->GetNumberOfRows();


	// get a Table without class Information
	vtkSmartPointer<vtkTable> tmp_Table = vtkSmartPointer<vtkTable>::New();

	for(int i =0; i< noOfCol ; i++)
	tmp_Table->AddColumn(table->GetColumn(i));
	
	tmp_Table->SetNumberOfRows(noOfRows);

	
	// Create a Table extracting only the features
	for(int i=0; i< noOfRows; i++) {
		tmp_Table->InsertNextRow(table->GetRow(i));
		std::cout << table->GetValue(i, noOfCol-1) << " => " << i << std::endl;
		mainInterfaceObj->ClassMap.insert(std::make_pair<vtkVariant, int>(table->GetValue(i, noOfCol-1),i));
	}
	tmp_Table->RemoveColumn(noOfCol-1);

	// Sort by Class
	SortFeatureMatrixbyClass(tmp_Table);

	// Create Feature Matrix
	DenseArrPtr featureMatrix = DenseArrPtr::New();
	featureMatrix->Resize(noOfRows, noOfCol-1);
 	
	
	for(vtkIdType i = 0; i < noOfRows; i++) 
		for(vtkIdType j = 0; j < noOfCol-1; j++) 
			featureMatrix->SetValue(i,j,(const double&) table->GetValue(i,j).ToDouble());

	return featureMatrix;
}


/*
//
// Function to Sort the the Feature Table by Class
//
*/
void Parser::SortFeatureMatrixbyClass(vtkSmartPointer<vtkTable> table) {

	vtkSmartPointer<vtkTable> tmp_Table = vtkSmartPointer<vtkTable>::New();

	for(int i =0; i< table->GetNumberOfColumns() ; i++)
	tmp_Table->AddColumn(table->GetColumn(i));

	std::multimap<vtkVariant, int>::const_iterator classMapIter;
	classMapIter = mainInterfaceObj->ClassMap.cbegin();

	int row = 0;

	for(classMapIter; classMapIter != mainInterfaceObj->ClassMap.cend(); classMapIter++) {
		std::cout << (*classMapIter).second << std::endl;
		tmp_Table->SetRow(row++, table->GetRow((*classMapIter).second));
	}
		

	table = tmp_Table;

	//table->DeepCopy
	
}


