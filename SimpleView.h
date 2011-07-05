#ifndef SimpleView_H
#define SimpleView_H

#include "ui_SimpleView.h"

#include "vtkSmartPointer.h"
#include <QMainWindow>

class vtkRenderer;
class vtkBorderWidget;
class BorderCallbackClass;
class vtkImageData;
class vtkImageActor;
class vtkActor;
class vtkPTXReader;

class SimpleView : public QMainWindow
{
  Q_OBJECT
public:

  // Constructor/Destructor
  SimpleView(int argc, char** argv);
  ~SimpleView() {};

  void MainFunction();

  void SetupImage();
  void SetupLidar();

  char** argv;
  int argc;

  void BorderCallback(vtkObject* caller,
                    long unsigned int eventId,
                    void* callData );

public slots:
  void btnOpenPTXFile_clicked();

protected:

  vtkSmartPointer<vtkPTXReader> PTXReader;
  vtkSmartPointer<vtkImageData> PTX;

  vtkSmartPointer<vtkBorderWidget> BorderWidget;

  vtkSmartPointer<vtkRenderer> RightRenderer;
  vtkSmartPointer<vtkRenderer> LeftRenderer;

  // Designer form
  Ui_SimpleView ui;

  vtkSmartPointer<vtkImageActor> ImageActor;
  vtkSmartPointer<vtkActor> PointsActor;
  vtkSmartPointer<vtkActor> SelectedPointsActor;
};

#endif // SimpleView_H
