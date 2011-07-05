#include "ui_SimpleView.h"
#include "SimpleView.h"

#include <QFileDialog>

#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkCommand.h>
#include <vtkMath.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkProperty2D.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkProperty.h>

#include "vtkPTXReader.h"

void SimpleView::BorderCallback(vtkObject *caller, unsigned long vtkNotUsed(eventId),
                                void* vtkNotUsed(callData))
{
  vtkBorderWidget *borderWidget =
      reinterpret_cast<vtkBorderWidget*>(caller);

  // Get the world coordinates of the two corners of the box
  vtkCoordinate* lowerLeftCoordinate = static_cast<vtkBorderRepresentation*>(
      borderWidget->GetRepresentation())->GetPositionCoordinate();
  double* lowerLeft = lowerLeftCoordinate->GetComputedWorldValue(this->LeftRenderer);
  //std::cout << "Lower left coordinate: " << lowerLeft[0] << ", " << lowerLeft[1] << ", " << lowerLeft[2] << std::endl;

  vtkCoordinate* upperRightCoordinate = static_cast<vtkBorderRepresentation*>(
      borderWidget->GetRepresentation())->GetPosition2Coordinate();
  double* upperRight = upperRightCoordinate ->GetComputedWorldValue(this->LeftRenderer);
  //std::cout << "Upper right coordinate: " << upperRight[0] << ", " << upperRight[1] << ", " << upperRight[2] << std::endl;

  double* bounds = this->ImageActor->GetBounds();
  double xmin = bounds[0];
  double xmax = bounds[1];
  double ymin = bounds[2];
  double ymax = bounds[3];

  // First, assume the border widget is entirely inside the image
  double thetaMin = lowerLeft[0];
  double thetaMax = upperRight[0];
  double phiMin = lowerLeft[1];
  double phiMax = upperRight[1];

  if( (thetaMax < xmin) || (thetaMin > xmax) ||
      (phiMax < ymin) || (phiMin > ymax ) )
    {
    // The entire border widget is outside the image, return.
    return;
    }

  if(thetaMin < xmin)
    {
    thetaMin = xmin;
    }

  if(thetaMax > xmax)
    {
    thetaMax = xmax;
    }

  if(phiMin < ymin)
    {
    phiMin = ymin;
    }

  if(phiMax > ymax)
    {
    phiMax = ymax;
    }

  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  this->PTXReader->GetValidOutputPointsInRange(polydata, static_cast<int>(phiMin), static_cast<int>(phiMax),
                                               static_cast<int>(thetaMin), static_cast<int>(thetaMax));
  polydata->GetPointData()->SetActiveScalars(NULL);

  vtkSmartPointer<vtkPolyDataMapper> pointsMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  pointsMapper->SetInputConnection(polydata->GetProducerPort());

  this->SelectedPointsActor->SetMapper(pointsMapper);
  this->SelectedPointsActor->GetProperty()->SetColor(1,0,0);
  this->SelectedPointsActor->GetProperty()->SetPointSize(4);

  this->RightRenderer->AddActor(this->SelectedPointsActor);
  this->RightRenderer->Render();
  this->ui.qvtkWidgetRight->GetRenderWindow()->Render();
}

// Constructor
SimpleView::SimpleView(int argCount, char** arguments)
{
  this->ui.setupUi(this);

  this->LeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  this->RightRenderer = vtkSmartPointer<vtkRenderer>::New();

  this->ui.qvtkWidgetLeft->GetRenderWindow()->AddRenderer(this->LeftRenderer);
  this->ui.qvtkWidgetRight->GetRenderWindow()->AddRenderer(this->RightRenderer);

  this->argv = arguments;
  this->argc = argCount;

  this->ImageActor = vtkSmartPointer<vtkImageActor>::New();
  this->PointsActor = vtkSmartPointer<vtkActor>::New();
  this->SelectedPointsActor = vtkSmartPointer<vtkActor>::New();

  this->PTXReader = vtkSmartPointer<vtkPTXReader>::New();
  this->PTX = vtkSmartPointer<vtkImageData>::New();

  // Set up action signals and slots
  connect( this->ui.btnOpenPTXFile, SIGNAL(clicked()), this, SLOT(btnOpenPTXFile_clicked()));
};

void SimpleView::btnOpenPTXFile_clicked()
{
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this,
     //tr("Open File"), "/home/doriad", tr("Image Files (*.vtp)"));
     tr("Open File"), "/media/portable/Projects/src/LidarImageHighlight", tr("PTX Files (*.ptx)"));

  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    return;
    }

  // Open the file
  this->PTXReader->SetFileName(fileName.toStdString().c_str());
  this->PTXReader->Update();

  this->PTX->ShallowCopy(this->PTXReader->GetOutput());
  this->PTX->SetOrigin(0,0,0);
  /*
  this->PTX->Set
  int dims[3];
  this->PTX->GetDimensions(dims);


  this->ImageActor->AddPosition(dims[0]/2, dims[1]/2, 0);
  */

  this->ImageActor->SetInput(this->PTX);

  // Add Actor to renderer
  this->LeftRenderer->AddActor(this->ImageActor);
  this->LeftRenderer->ResetCamera();

  vtkSmartPointer<vtkInteractorStyleImage> interactorStyle =
      vtkSmartPointer<vtkInteractorStyleImage>::New();
  this->ui.qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->SetInteractorStyle(interactorStyle);

  this->BorderWidget = vtkSmartPointer<vtkBorderWidget>::New();
  this->BorderWidget->SetInteractor(this->ui.qvtkWidgetLeft->GetRenderWindow()->GetInteractor());
  static_cast<vtkBorderRepresentation*>(this->BorderWidget->GetRepresentation())->GetBorderProperty()->SetColor(1,0,0); //red

  //borderWidget->SelectableOff();

  //this->BorderWidget->AddObserver(vtkCommand::InteractionEvent,this->BorderCallback);
  //this->BorderWidget->AddObserver(vtkCommand::EndInteractionEvent,this->BorderCallback);
  this->BorderWidget->AddObserver(vtkCommand::EndInteractionEvent, this, &SimpleView::BorderCallback);
  this->BorderWidget->On();

  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  //this->PTXReader->GetValidOutputPoints(polydata);
  this->PTXReader->GenerateGridMesh(polydata);

  vtkSmartPointer<vtkPolyDataMapper> pointsMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  pointsMapper->SetInputConnection(polydata->GetProducerPort());

  this->PointsActor->SetMapper(pointsMapper);

  this->RightRenderer->AddActor(this->PointsActor);
  this->RightRenderer->ResetCamera();

}