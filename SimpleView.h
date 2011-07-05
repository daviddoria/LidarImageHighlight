/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

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
