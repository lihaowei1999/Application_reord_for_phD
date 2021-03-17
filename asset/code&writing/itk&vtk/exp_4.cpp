#include "vtkImageReader2.h"
#include "vtkOutlineFilter.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageMapper3D.h"
#include "vtkNrrdReader.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCallbackCommand.h>
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkStripper.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkDICOMImageReader.h"


#include "itkConnectedThresholdImageFilter.h"
#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkImageFileWriter.h"
#include "vtkImageCast.h"
#include "vtkLookupTable.h"
#include "vtkImageMathematics.h"
#include "vtkImageBlend.h"
#include "vtkImageSliceMapper.h"

#include "stdio.h"

int main(int argc, char*argv[]){
    if (argc < 6){
        cout<<"folder names to be set up"<<endl;
        return 0
    }
    
    vtkDICOMImageReader* dicom_data_reader=vtkDICOMImageReader::New();
    vtkDICOMImageReader* spine_reader=vtkDICOMImageReader::New();
    vtkDICOMImageReader* hip_right_reader=vtkDICOMImageReader::New();
    vtkDICOMImageReader* hip_left_reader=vtkDICOMImageReader::New();
    vtkDICOMImageReader* femur_right_reader=vtkDICOMImageReader::New();
    vtkDICOMImageReader* femur_left_reader=vtkDICOMImageReader::New();
    
    dicom_data_reader->SetDirectoryName(argv[1]);
    spine_reader->SetDirectoryName(argv[2]);
    hip_right_reader->SetDirectoryName(argv[3]);
    hip_left_reader->SetDirectoryName(argv[4]);
    femur_right_reader->SetDirectoryName(argv[5]);
    femur_left_reader->SetDirectoryName(argv[6]);
    
    dicom_data_reader->Update();
    spine_reader->Update();
    hip_right_reader->Update();
    hip_left_reader->Update();
    femur_right_reader->Update();
    femur_left_reader->Update();
    
    // extraction components definition
    vtkContourFilter* skin_extract=vtkContourFilter::New();
    vtkContourFilter* spine_extract=vtkContourFilter::New();
    vtkContourFilter* hip_right_extract=vtkContourFilter::New()；
    vtkContourFilter* hip_left_extract=vtkContourFilter::New();
    vtkContourFilter* femur_right_extract=vtkContourFilter::New();
    vtkContourFilter* femur_left_extract=vtkContourFilter::New();
    
    vtkPolyDataNormals* skin_normal=vtkPolyDataNormals::New();
    vtkPolyDataNormals* spine_normal=vtkPolyDataNormals::New();
    vtkPolyDataNormals* hip_right_normal=vtkPolyDataNormals::New();
    vtkPolyDataNormals* hip_left_normal=vtkPolyDataNormals::New();
    vtkPolyDataNormals* femur_right_normal=vtkPolyDataNormals::New();
    vtkPolyDataNormals* femur_left_normal=vtkPolyDataNormals::New();
    
    vtkStripper* spine_stripper=vtkStripper::New();
    vtkStripper* hip_right_stripper=vtkStripper::New();
    vtkStripper* hip_left_stripper=vtkStripper::New();
    vtkStripper* femur_right_stripper=vtkStripper::New();
    vtkStripper* femur_left_stripper=vtkStripper::New();
    
    vtkPolyDataMapper* skin_mapper=vtkPolyDataMapper::New();
    vtkPolyDataMapper* spine_mapper=vtkPolyDataMapper::New();
    vtkPolyDataMapper* hip_right_mapper=vtkPolyDataMapper::New();
    vtkPolyDataMapper* hip_left_mapper=vtkPolyDataMapper::New();
    vtkPolyDataMapper* femur_right_mapper=vtkPolyDataMapper::New();
    vtkPolyDataMapper* femur_left_mapper=vtkPolyDataMapper::New();
    
    vtkActor* skin_actor=vtkActor::New();
    vtkActor* spine_actor=vtkActor::New();
    vtkActor* hip_left_actor=vtkActor::New();
    vtkActor* hip_right_actor=vtkActor::New();
    vtkActor* femur_left_actor=vtkActor::New();
    vtkActor* femur_right_actor=vtkActor::New();
    
    vtkOutlineFilter* struct_outline=vtkOutlineFilter::New();
    vtkPolyDataMapper* struct_outline_mapper=vtkPolyDataMapper::New();
    vtkActor* struct_outline_actor=vtkActor::New();
    
    vtkcamera* L_camera=vtkcamera::New();
    
    vtkRenderer* L_render=vtkRenderer::New();
    vtkRenderer* ren1=vtkRenderer::New();
    vtkRenderWindow* render_window=vtkRenderWindow::New();
    vtkRenderWindowInteractor* window_interact=vtkRenderWindowInteractor::New();
    
    vtkScalarToColors* skin_to_color=vtkScalarToColors::New();
    vtkLookupTable* spine_lk_table=vtkLookupTable::New();
    vtkLookupTable* hip_right_lk_table=vtkLookupTable::New();
    vtkLookupTable* hip_left_lk_table=vtkLookupTable::New();
    vtkLookupTable* femur_right_lk_table=vtkLookupTable::New();
    vtkLookupTable* femur_left_lk_table=vtkLookupTable::New();
    
    vtkImageMapToColors* skin_color_mapper=vtkImageMapToColors::New();
    vtkImageMapToColors* spine_color_mapper=vtkImageMapToColors::New();
    vtkImageMapToColors* hip_right_color_mapper=vtkImageMapToColors::New();
    vtkImageMapToColors* hip_left_color_mapper=vtkImageMapToColors::New();
    vtkImageMapToColors* femur_right_color_mapper=vtkImageMapToColors::New();
    vtkImageMapToColors* femur_left_color_mapper=vtkImageMapToColors::New();
    
    vtkImageBlend* Blender=vtkImageBlend::New();
    vtkImageSliceMapper* slicer_mapper=vtkImageSliceMapper::New();
    vtkSmartPointer<vtkImageSlice> img_slicer=vtkSmartPointer<vtkImageSlice>::New();
    
    
    //renderer setup
    render_window->AddRender(ren1);
    render_window->AddRender(L_render);
    window_interact->SetRenderWindow(render_window);
    
    // setup for skin
    skin_extract->SetInputConnection(dicom_data_reader->GetOutputPort());
    skin_extract->SetValue(0,255);
    skin_normal->SetInputConnection(skin_extract->GetOutputPort());
    skin_normal->SetFeatureAngle(90.0);
    skin_mapper->SetInputConnection(skin_normal->GetOutputPort());
    skin_mapper->ScalarVisibilityOff();
    skin_actor->SetMapper(skin_mapper);
    skin_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for spine
    spine_extract->SetInputConnection(spine_reader->GetOutputPort());
    spine_extract->SetValue(0,255);
    spine_normal->SetInputConnection(spine_extract->GetOutputPort());
    spine_normal->SetFeatureAngle(90.0);
    spine_stripper->SetInputConnection(spine_normal->GetOutputPort());
    spine_mapper->SetInputConnection(spine_stripper->GetOutputPort());
    spine_mapper->ScalarVisibilityOff();
    spine_actor->SetMapper(spine_mapper);
    spine_actor->GetProperty()->SetDiffuseColor(0.3,0.4,0.6);
    spine_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for hip left
    hip_left_extract->SetInputConnection(hip_left_reader->GetOutputPort());
    hip_left_extract->SetValue((0,255));
    hip_left_normal->SetInputConnection(hip_left_extract->GetOutputPort());
    hip_left_normal->SetFeatureAngle(90.0);
    hip_left_stripper->SetInputConnection(hip_left_normal->GetOutputPort());
    hip_left_mapper->SetInputConnection(hip_left_stripper->GetOutputPort());
    hip_left_mapper->ScalarVisibilityOff();
    hip_left_actor->SetMapper(hip_left_mapper);
    hip_left_actor->GetProperty()->SetDiffuseColor(0.6,0.2,0.4);
    hip_left_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for hip right
    hip_right_extract->SetInputConnection(hip_right_reader->GetOutputPort());
    hip_right_extract->SetValue((0,255));
    hip_right_normal->SetInputConnection(hip_right_extract->GetOutputPort());
    hip_right_normal->SetFeatureAngle(90.0);
    hip_right_stripper->SetInputConnection(hip_right_normal->GetOutputPort());
    hip_right_mapper->SetInputConnection(hip_right_stripper->GetOutputPort());
    hip_right_mapper->ScalarVisibilityOff();
    hip_right_actor->SetMapper(hip_right_mapper);
    hip_right_actor->GetProperty()->SetDiffuseColor(0.6,0.2,0.4);
    hip_right_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for femur left
    femur_left_extract->SetInputConnection(femur_left_reader->GetOutputPort());
    femur_left_extract->SetValue((0,255));
    femur_left_normal->SetInputConnection(femur_left_extract->GetOutputPort());
    femur_left_normal->SetFeatureAngle(90.0);
    femur_left_stripper->SetInputConnection(femur_left_normal->GetOutputPort());
    femur_left_mapper->SetInputConnection(femur_left_stripper->GetOutputPort());
    femur_left_mapper->ScalarVisibilityOff();
    femur_left_actor->SetMapper(femur_left_mapper);
    femur_left_actor->GetProperty()->SetDiffuseColor(0.6,0.2,0.4);
    femur_left_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for femur right
    femur_right_extract->SetInputConnection(femur_right_reader->GetOutputPort());
    femur_right_extract->SetValue((0,255));
    femur_right_normal->SetInputConnection(femur_right_extract->GetOutputPort());
    femur_right_normal->SetFeatureAngle(90.0);
    femur_right_stripper->SetInputConnection(femur_right_normal->GetOutputPort());
    femur_right_mapper->SetInputConnection(femur_right_stripper->GetOutputPort());
    femur_right_mapper->ScalarVisibilityOff();
    femur_right_actor->SetMapper(femur_right_mapper);
    femur_right_actor->GetProperty()->SetDiffuseColor(0.6,0.2,0.4);
    femur_right_actor->GetProperty()->SetOpacity(0.6);
    
    //set up for struct
    struct_outline->SetInputConnection(dicom_data_reader->GetOutputPort());
    struct_outline_mapper->SetInputConnection(struct_outline->GetOutputPort());
    struct_outline_actor->SetMapper(struct_outline_mapper);
    struct_outline_actor->GetProperty()->SetColor(0,0,0);
    
    L_render->AddActor(skin_actor);
    L_render->AddActor(spine_actor);
    L_render->AddActor(hip_left_actor);
    L_render->AddActor(hip_right_actor);
    L_render->AddActor(femur_left_actor);
    L_render->AddActor(femur_right_actor);
    L_render->AddActor(struct_outline_actor);
    
    L_camera->SetPosition(0,1,0);
    L_camera->SetViewUp(0,0,-1);
    L_camera->SetFocalPoint(0,0,0);
    L_camera->ComputeViewPlaneNormal();
    //L_camera->Dolly(2)
    
    L_render->SetActiveCamera(L_camera);
    L_render->ResetCamera();
    L_render->SetBackground(0.0,0.0,0.0);
    
    
    
    // skin color map
    skin_to_color->SetRange(-800,2000);
    skin_color_mapper->SetInputData(dicom_data_reader->GetOutput());
    skin_color_mapper->SetLookupTable(skin_to_color);
    skin_color_mapper->SetOutputFormatToRGB();
    skin_color_mapper->Update();
    
    // spine color map
    spine_lk_table->SetTableValue(0,0.0,0.0,0.0,0.0);
    spine_lk_table->SetTableValue(255,0.3,0.3,0.3,1.0);
    spine_lk_table->Build();
    spine_color_mapper->SetInputData(spine_reader->GetOutput());
    spine_color_mapper->SetLookupTable(spine_lk_table);
    spine_color_mapper->Update();
    
    hip_left_lk_table->SetTableValue(0,0.0,0.0,0.0,0.0);
    hip_left_lk_table->SetTableValue(255,0.3,0.3,0.3,1.0);
    hip_left_lk_table->Build();
    hip_left_color_mapper->SetInputData(hip_left_reader->GetOutput());
    hip_left_color_mapper->SetLookupTable(hip_left_lk_table);
    hip_left_color_mapper->Update();
    
    hip_right_lk_table->SetTableValue(0,0.0,0.0,0.0,0.0);
    hip_right_lk_table->SetTableValue(255,0.3,0.3,0.3,1.0);
    hip_right_lk_table->Build();
    hip_right_color_mapper->SetInputData(hip_right_reader->GetOutput());
    hip_right_color_mapper->SetLookupTable(hip_right_lk_table);
    hip_right_color_mapper->Update();
    
    femur_left_lk_table->SetTableValue(0,0.0,0.0,0.0,0.0);
    femur_left_lk_table->SetTableValue(255,0.3,0.3,0.3,1.0);
    femur_left_lk_table->Build();
    femur_left_color_mapper->SetInputData(femur_left_reader->GetOutput());
    femur_left_color_mapper->SetLookupTable(femur_left_lk_table);
    femur_left_color_mapper->Update();
    
    femur_right_lk_table->SetTableValue(0,0.0,0.0,0.0,0.0);
    femur_right_lk_table->SetTableValue(255,0.3,0.3,0.3,1.0);
    femur_right_lk_table->Build();
    femur_right_color_mapper->SetInputData(femur_right_reader->GetOutput());
    femur_right_color_mapper->SetLookupTable(femur_right_lk_table);
    femur_right_color_mapper->Update();
    
    
    //
    Blender->AddInputData(skin_color_mapper->GetOutput());
    Blender->AddInputData(spine_color_mapper->GetOutput());
    Blender->AddInputData(hip_right_color_mapper->GetOutput());
    Blender->AddInputData(hip_left_color_mapper->GetOutput());
    Blender->AddInputData(femur_right_color_mapper->GetOutput());
    Blender->AddInputData(femur_left_color_mapper->GetOutput());
    Blender->SetOpacity(1,0.1);
    Blender->Update();
    
    slicer_mapper->SetInputData(Blender->GetOutput());
    slicer_mapper->SetSliceNumber(atof(argv[7]));
    slicer_mapper->Update();
    img_slicer->SetMapper(slicer_mapper);
    ren1->AddViewProp(img_slicer);
    ren1->SetBackground(0.0,0.0,0.0);
    L_render->ResetCameraClippingRange();
    L_render->SetViewport(0,0,0,1);
    render_window->SetSize(1400,700);
    
    ren1->SetViewport(0,0,0,1);
    ren1->ResetCamera();
    window_interact->Initialize();
    window_interact->Start();
    windoe_interact->Render();
    return 0;
    
    
    
    
    
}
