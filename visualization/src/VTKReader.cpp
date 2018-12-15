#include "VTKReader.h"

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <omp.h>

//#define DEBUG
#define CHRONO
#define MULTI
namespace
{
//! Create a lookup table.
/*!
@param lut - An indexed lookup table.
*/
void MakeLUT(int const& colorScheme, vtkLookupTable* lut);


}

RBVisualizer::RBVisualizer(){
    int           colorScheme = 0;
    int           resolutionX = 0;
    int           resolutionY = 0;
    int           timeStep = 0;
    int           timeMax = 0;
}

RBVisualizer::RBVisualizer(int colorScheme, int resolutionX, int resolutionY,
   std::string filePath, std::string parameterCode, int timeStep, int timeMax){
    this->colorScheme = colorScheme;
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
    this->filePath    = filePath;
    if(this->filePath.back() != '/'){
      this->filePath.push_back('/');
    }
    this->parameterCode = parameterCode;
    this->timeStep = timeStep;
    this->timeMax = timeMax;

	// Initialize the vector with a fixed size
	this->_vectorSize = (timeMax - timeStep) / timeStep + 1;
	//this->rendererOutput.resize(_vectorSize);
}

RBVisualizer::~RBVisualizer(){}

int RBVisualizer::getColorScheme() const{
    return colorScheme;
}

int RBVisualizer::getResolutionX() const{
    return resolutionX;
}

int RBVisualizer::getResolutionY() const{
    return resolutionY;
}

int RBVisualizer::getTimeStep() const{
    return timeStep;
}

int RBVisualizer::getTimeMax() const{
    return timeMax;
}

double RBVisualizer::getParameterMax() const{
    return parameterMax;
}

double RBVisualizer::getParameterMin() const{
    return parameterMin;
}

void RBVisualizer::setColorScheme(int colors){
    colorScheme = colors;
}

void RBVisualizer::setResolutionX(int resoX){
    resolutionX = resoX;
}

void RBVisualizer::setResolutionY(int resoY){
    resolutionY = resoY;
}

void RBVisualizer::setParameterMin(double min){
    parameterMin = min;
}

void RBVisualizer::setParameterMax(double max){
    parameterMax = max;
}

void RBVisualizer::readParameterMinMax(){
    vtkSmartPointer<vtkUnstructuredGridReader> reader =
      vtkSmartPointer<vtkUnstructuredGridReader>::New();
    std::string fileName = filePath + "RBConvection_" +
      std::to_string(timeMax) + ".vtk";
    reader->SetFileName(fileName.c_str());
    reader->SetFieldDataName("attributes");
    reader->Update();
    vtkSmartPointer<vtkCellData> cellData =
      vtkSmartPointer<vtkCellData>::New();
    cellData = reader->GetOutput()->GetCellData();
    vtkSmartPointer<vtkFloatArray> arrayParameters =
      vtkSmartPointer<vtkFloatArray>::New();
    arrayParameters =
      vtkFloatArray::SafeDownCast(cellData->GetAbstractArray(parameterCode.c_str()));
    double parameterMinMax[2];
    arrayParameters->GetRange(parameterMinMax);
    //parameterMin = parameterMinMax[0];
    parameterMax = parameterMinMax[1];
}

template <typename T>
void RBVisualizer::vectorInitalizer(std::vector<vtkSmartPointer<T>>& pointerVector) {
	for (int initializerIndex = 0; initializerIndex < _vectorSize; initializerIndex ++){
		pointerVector.at(initializerIndex) = vtkSmartPointer<T>::New();
	}
}

RendererVector RBVisualizer::mainVisualizer(){
#ifdef CHRONO
	auto startTime = std::chrono::steady_clock::now();
	cout << "Hello!" << endl;
#endif
  
	///		Data Structures (Vector Version)	///
	PlaneSourceVector				parameterPlaneVec(_vectorSize);
	UnstructuredGridReaderVector	readerVec(_vectorSize);
	CellDataVector					cellDataVec(_vectorSize);
	FloatArrayVector				arrayParametersVec(_vectorSize);
	LookupTableVector				lutParameterVec(_vectorSize);
	NamedColorsVector				colorsVec(_vectorSize);
	PolyDataMapperVector			mapperVec(_vectorSize);
	ActorVector						dataActorVec(_vectorSize);
	RendererVector					rendererOutput(_vectorSize);

	vectorInitalizer<vtkPlaneSource>(parameterPlaneVec);
	vectorInitalizer<vtkUnstructuredGridReader>(readerVec);
	vectorInitalizer<vtkCellData>(cellDataVec);
	vectorInitalizer<vtkFloatArray>(arrayParametersVec);
	vectorInitalizer<vtkLookupTable>(lutParameterVec);
	vectorInitalizer<vtkNamedColors>(colorsVec);
	vectorInitalizer<vtkPolyDataMapper>(mapperVec);
	vectorInitalizer<vtkActor>(dataActorVec);
	vectorInitalizer<vtkRenderer>(rendererOutput);

 // vtkSmartPointer<vtkRenderWindow> renWin =
 //   vtkSmartPointer<vtkRenderWindow>::New();

//  vtkSmartPointer<vtkRenderWindowInteractor> iren =
 //   vtkSmartPointer<vtkRenderWindowInteractor>::New();

 #pragma omp parallel for ordered schedule(dynamic, 25) num_threads(4)
  for(int fileIndex = timeStep; fileIndex <= timeMax; fileIndex += timeStep){
	#ifdef CHRONO
	  auto tic = std::chrono::steady_clock::now();
	#endif
	  int currentVectorIndex = fileIndex / timeStep - 1;

	  ///   Data Structres   ///
	  //vtkSmartPointer<vtkPlaneSource> parameterPlane =
		 // vtkSmartPointer<vtkPlaneSource>::New();

	  //vtkSmartPointer<vtkUnstructuredGridReader> reader =
		 // vtkSmartPointer<vtkUnstructuredGridReader>::New();

	  //vtkSmartPointer<vtkCellData> cellData =
		 // vtkSmartPointer<vtkCellData>::New();

	  //// vtkSmartPointer<vtkIntArray> arrayID =
	  ////   vtkSmartPointer<vtkIntArray>::New();

	  //vtkSmartPointer<vtkFloatArray> arrayParameters =
		 // vtkSmartPointer<vtkFloatArray>::New();

	  //// vtkSmartPointer<vtkLookupTable> lutID =
	  ////   vtkSmartPointer<vtkLookupTable>::New();

	  //vtkSmartPointer<vtkLookupTable> lutParameter =
		 // vtkSmartPointer<vtkLookupTable>::New();

	  //vtkSmartPointer<vtkNamedColors> colors =
		 // vtkSmartPointer<vtkNamedColors>::New();

	  //vtkSmartPointer<vtkPolyDataMapper> mapper =
		 // vtkSmartPointer<vtkPolyDataMapper>::New();

	  //vtkSmartPointer<vtkActor> dataActor =
		 // vtkSmartPointer<vtkActor>::New();

	  //vtkSmartPointer<vtkRenderer> ren =
		 // vtkSmartPointer<vtkRenderer>::New();

      /////    Data manipulation    /////
      std::string fileName = filePath + "RBConvection_" +
        std::to_string(fileIndex) + ".vtk";
      std::cout << "Loading " << fileName.c_str() << std::endl;

      readerVec.at(currentVectorIndex)->SetFileName(fileName.c_str());
      readerVec.at(currentVectorIndex)->SetFieldDataName("attributes");
      readerVec.at(currentVectorIndex)->Update();
#ifdef MULTI
	  std::cout << "Current index: " << currentVectorIndex << std::endl;
	  std::cout << "Current reader: " << readerVec.at(currentVectorIndex) << std::endl;
	  std::cout << "Size of a reader: " << sizeof(*readerVec.at(currentVectorIndex)) << std::endl;
#endif
      // Achieve the Cell data
      cellDataVec.at(currentVectorIndex) = readerVec.at(currentVectorIndex)->GetOutput()->GetCellData();

      // Ge the array using SafeDownCast()
      arrayParametersVec.at(currentVectorIndex) =
        vtkFloatArray::SafeDownCast(cellDataVec.at(currentVectorIndex)->GetAbstractArray(parameterCode.c_str()));
      //arrayTemperature->SetName("Temperature");
      double arrayParameterRange[2];
      arrayParametersVec.at(currentVectorIndex)->GetRange(arrayParameterRange);

      // Set up lookupTables
      int tableSize = resolutionX*resolutionY + 1;
      lutParameterVec.at(currentVectorIndex)->SetNumberOfTableValues(tableSize);
      // Set up the color schemes
      MakeLUT(colorScheme, lutParameterVec.at(currentVectorIndex));
      lutParameterVec.at(currentVectorIndex)->SetTableRange(parameterMin, parameterMax);
      lutParameterVec.at(currentVectorIndex)->Build();

      parameterPlaneVec.at(currentVectorIndex)->SetXResolution(resolutionX);
      parameterPlaneVec.at(currentVectorIndex)->SetYResolution(resolutionY);

      parameterPlaneVec.at(currentVectorIndex)->Update();
      parameterPlaneVec.at(currentVectorIndex)->GetOutput()
		  ->GetCellData()->SetScalars(arrayParametersVec.at(currentVectorIndex));

    	  #ifdef DEBUG
        std::cout << "Hello!" << endl;
        // std::cout << "Range of arrayID: " << arrayIDRange[0]
        //   << " " << arrayIDRange[1] << endl;
        std::cout << "Range of current parameter: " << arrayParameterRange[0]
          << " " << arrayParameterRange[1] << endl;
        std::cout << "Range used in lut: " << parameterMin
          << " " << parameterMax << endl;
        // std::cout << "Values of the float array: " << endl;
        // for(int i = 0; i < resolution*resolution; i++){
        //   std::cout << arrayTemperature->GetVariantValue(i) << " ";
        //   if(i % resolution == (resolution-1)){
        //     std::cout << "\n";
        //   }
        // }
        // std::cout << "Values of the int array: " << endl;
        // for(int i = 0; i < resolution*resolution; i++){
        //   std::cout << arrayID->GetVariantValue(i) << " ";
        //   if(i % resolution == (resolution-1)){
        //     std::cout << "\n";
        //   }
        // }

        double tableColor[3];
        lutParameterVec.at(currentVectorIndex)->GetColor(arrayParameterRange[0], tableColor);
        std::cout << "The color of arrayParameterRange[0]: " <<
        tableColor[0] << " " << tableColor[1] << " " << tableColor[2] << std::endl;
        lutParameterVec.at(currentVectorIndex)->GetColor(arrayParameterRange[1], tableColor);
        std::cout << "The color of arrayParameterRange[1]: " <<
        tableColor[0] << " " << tableColor[1] << " " << tableColor[2] << std::endl;
      #endif  

      /////   Visualization Pipeline    /////

      mapperVec.at(currentVectorIndex)->SetInputConnection(parameterPlaneVec.at(currentVectorIndex)->GetOutputPort());
      mapperVec.at(currentVectorIndex)->SetScalarRange(parameterMin, parameterMax);
      mapperVec.at(currentVectorIndex)->SetLookupTable(lutParameterVec.at(currentVectorIndex));

      dataActorVec.at(currentVectorIndex)->SetMapper(mapperVec.at(currentVectorIndex));

	  rendererOutput.at(currentVectorIndex)->AddActor(dataActorVec.at(currentVectorIndex));
	  rendererOutput.at(currentVectorIndex)->SetBackground(colorsVec.at(currentVectorIndex)->GetColor3d("SlateGray").GetData());
#ifdef MULTI
	  std::cout << "Current ren: " << rendererOutput.at(currentVectorIndex) << endl;
#endif
	  #ifdef CHRONO
		auto toc = std::chrono::steady_clock::now();
		std::cout << "The runtime of executing chunk " << fileIndex << " is "
			<< std::chrono::duration <double, std::milli>(toc - tic).count()
			<< " ms" << std::endl;
	  #endif

	  //#pragma omp ordered
		
	  //rendererOutput.at(currentVectorIndex) = ren;
      //renWin->AddRenderer(ren);
      //renWin->SetSize(400, 400);

      //iren->SetRenderWindow(renWin);
      //renWin->Render();
      //std::this_thread::sleep_for(std::chrono::milliseconds(16));
      //iren->Start();
	  
  }
  #ifdef CHRONO
	auto endTime = std::chrono::steady_clock::now();
	std::cout << "Total runtime is: "
		<< std::chrono::duration <double, std::milli>(endTime - startTime).count()
		<< " ms" << std::endl;
  #endif	
  return rendererOutput;
}



// MakeLUT: Adapted from
// https://lorensen.github.io/VTKExamples/site/Cxx/Visualization/Hawaii/
namespace
{
void MakeLUT(int const& colorScheme, vtkLookupTable* lut)
{
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  // Select a color scheme.
  switch (colorScheme)
  {
    case 0:
    default:
    {
      // Make the lookup using a Brewer palette.
      vtkSmartPointer<vtkColorSeries> colorSeries =
        vtkSmartPointer<vtkColorSeries>::New();
      colorSeries->SetNumberOfColors(8);
      int colorSeriesEnum = colorSeries->BREWER_DIVERGING_BROWN_BLUE_GREEN_8;
      colorSeries->SetColorScheme(colorSeriesEnum);
      colorSeries->BuildLookupTable(lut, colorSeries->ORDINAL);
      lut->SetNanColor(1, 0, 0, 1);
      break;
    }
    case 1:
    {
      // A lookup table of 256 colours ranging from
      //  deep blue(water) to yellow - white(mountain top)
      //  is used to color map this figure.
      lut->SetHueRange(0.7, 0);
      lut->SetSaturationRange(1.0, 0);
      lut->SetValueRange(0.5, 1.0);
      break;
    }
    case 2:
      // Make the lookup table with a preset number of colours.
      vtkSmartPointer<vtkColorSeries> colorSeries =
        vtkSmartPointer<vtkColorSeries>::New();
      colorSeries->SetNumberOfColors(8);
      colorSeries->SetColorSchemeName("Hawaii");
      colorSeries->SetColor(0, colors->GetColor3ub("turquoise_blue"));
      colorSeries->SetColor(1, colors->GetColor3ub("sea_green_medium"));
      colorSeries->SetColor(2, colors->GetColor3ub("sap_green"));
      colorSeries->SetColor(3, colors->GetColor3ub("green_dark"));
      colorSeries->SetColor(4, colors->GetColor3ub("tan"));
      colorSeries->SetColor(5, colors->GetColor3ub("beige"));
      colorSeries->SetColor(6, colors->GetColor3ub("light_beige"));
      colorSeries->SetColor(7, colors->GetColor3ub("bisque"));
      colorSeries->BuildLookupTable(lut, colorSeries->ORDINAL);
      lut->SetNanColor(1, 0, 0, 1);
  };
}
}
