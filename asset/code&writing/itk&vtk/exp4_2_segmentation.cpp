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

#include "stdio.h"

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
// exp4_01_3
class vtkWidgetKeypressMyCallback : public vtkCommand
{
public:
    static vtkWidgetKeypressMyCallback* New()
    {
        return new vtkWidgetKeypressMyCallback;
    }

    void Execute(vtkObject* caller, unsigned long vtkNotUsed(event), void* callData)
    {
        vtkRenderWindowInteractor* self = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
        // 检测是否按下Ctrl键
        if (self->GetControlKey()) {
            double value;
            WidgetZ->GetCurrentCursorPosition(pos);        // 获取Widget的拾点位置
            value = WidgetZ->GetCurrentImageValue();    // 获取Widget的拾点灰度
            cout << "Selecting Seed: " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << " -- " << "Pixel Value: " << value << '\n';    // 命令行输出

            // 在窗口右上角显示当前拾点结果
            std::string fmt("Current Selected Seed: ( %d, %d, %d ): %d");
            char targetString[100];
            int realLen = snprintf(targetString, sizeof(targetString), fmt.c_str(), int(pos[0]), int(pos[1]), int(pos[2]), int(value));
            Annotation->SetText(3, targetString);
        }
    }
    vtkWidgetKeypressMyCallback() :WidgetZ(), Annotation(), pos() {}
    vtkImagePlaneWidget* WidgetZ;
    vtkCornerAnnotation* Annotation;
    double* pos;
};


int main(int argc, char* argv[]){
    
    
    if(argc<2){
        cout<<"error"<<endl;
        return EXIT_FAILURE
    }
    const char* Out_Folder=argv[2];
    itksys::SystemTools::MakeDirectory(Out_Folder);
    
    // 3_01_3
    typedef signed short    PixelType;
    const unsigned int      Dimension = 3;
    typedef itk::Image< PixelType, Dimension >      ImageType;
    typedef itk::GDCMImageIO                        ImageIOType;
    typedef itk::ImageSeriesReader< ImageType >     ReaderType;
    typedef itk::GDCMSeriesFileNames                NamesGeneratorType;
    typedef itk::Image< float, Dimension > floatImageType;
    typedef itk::CastImageFilter<ImageType, floatImageType> CastFilter_img_float;
    typedef itk::CastImageFilter<floatImageType, ImageType> CastFilter_float_img;
    typedef itk::CurvatureFlowImageFilter< floatImageType, floatImageType > CurvatureFlowImageFilterType;
    typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedFilterType;
    
    
    const unsigned int    outDimension = 2;
    typedef itk::Image<PixelType, outDimension > OutputImageType;
    typedef itk::ImageSeriesWriter<ImageType,OutputImageType>  serieswritertype;
    
    
    ImageIOType::Pointer GDCM_IO=ImageIOType::New();
    NamesGeneratorType::Pointer namesgenerator=NamesGeneratorType::new();
    namesgenerator->SetInputDirectory(argv[1])
    const ReaderType::FileNamesContainer& filenames = namesgenerator->GetInputFileNames();
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetImageIO(GDCM_IO);
    reader->SetFileNames(filenames);
    
    
    
    
    
    try{
        reader->Update();
    }
    catch{
        std::cerr << "Exception thrown while writing the image" << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }
    
    //itk-vtk联合demo2
    typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
    gdcmConnectorType::Pointer connector=gdcmConnectorType::New();
    connector->SetInput(reader->GetOutput());
    connector->Update();
    
    vtkOutlineFilter* data_outline_dcm=vtkOutlineFilter::New();
    vtkPolyDataMapper* data_mapper_dcm=vtkPolyDataMapper::New();
    vtkActor* data_dcm_actor=vtkActor::New();
    vtkCellPicker* dcm_picker=vtkCellPicker::New();
    vtkRenderWindow renwin=vtkRenderWindow::New();
    vtkRenderer* ren=vtkRenderer::New();
    vtkRenderWindowInteractor* iren=vtkRenderWindowInteractor::New();
    vtkImagePlaneWidget* planeWidgetX=vtkImagePlaneWidget::New();
    vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::New();
    
    serieswritertype::Pointe serieswriter=serieswritertype::new();
    
    ImageType::IndexType seed;
    
    data_outline_dcm->SetInputData(connector->GetOutput());
    data_mapper_dcm->SetInoutConnection(data_outline_dcm->GetOutputPort());
    data_dcm_actor->SetMapper(data_mapper_dcm);
    dcm_picker->SetTolerace(0.01);
    
    renwin->AddRenderer(ren);
    iren->SetRendeWindow(renwin)
    
    //exp 4_01_3
    //set up for planewidget
    planeWidgetZ->SetInteractor(iren);
    planeWidgetZ->SetKeyPressActivationValue('x');
    planeWidgetZ->SetPicker(picker);
    planeWidgetZ->RestrictPlaneToVolumeOn();
    planeWidgetZ->GetPlaneProperty()->SetColor(0.0, 0.0, 1.0);
    planeWidgetZ->TextureInterpolateOff();
    planeWidgetZ->SetResliceInterpolateToLinear();
    planeWidgetZ->SetInputData((vtkDataSet*)connector->GetOutput());
    planeWidgetZ->SetPlaneOrientationToZAxes();
    planeWidgetZ->SetSliceIndex(20);
    planeWidgetZ->GetTexturePlaneProperty()->LightingOff();
    planeWidgetZ->GetTexturePlaneProperty()->SetOpacity(1);
    planeWidgetZ->SetCurrentRenderer(ren);
    planeWidgetZ->DisplayTextOn();
    planeWidgetZ->On();
    planeWidgetZ->InteractionOn();
    cornerAnnotation->SetLinearFontScaleFactor(4);
    cornerAnnotation->SetNonlinearFontScaleFactor(1);
    cornerAnnotation->SetMaximumFontSize(25);
    cornerAnnotation->SetText(1, "Select Seed: Ctrl + Left Button\nFinish Selection: Press 'Q'");
    cornerAnnotation->GetTextProperty()->SetColor(1, 1, 1);
    cornerAnnotation->GetTextProperty()->SetLineSpacing(1.5);
    ren->AddViewProp(cornerAnnotation);

    double data_pos[3];
    vtkWidgetKeypressMyCallback* cbk = vtkWidgetKeypressMyCallback::New();
    cbk->WidgetZ = planeWidgetZ;
    cbk->Annotation = cornerAnnotation;
    cbk->pos = data_pos;
    iren->AddObserver(vtkCommand::KeyPressEvent, cbk);
    ren->AddActor(DicomActor);
    ren->SetBackground(0.3, 0.3, 0.6);
    renWin->SetSize(800, 800);
    iren->Initialize();
    iren->Start();
    renwin->Render();
    //Exp4_02_4
    CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
    smoothing->SetNumberOfIterations(5);
    smoothing->SetTimeStep(0.125);
    
    ConnectedFilterType::Pointer isolatedConnected = ConnectedFilterType::New();

    const PixelType lowerThreshold = atof(argv[3]);
    const PixelType upperThreshold = atof(argv[4]);
    //InternalImageType::IndexType  indexSeed2;
    //indexSeed2[0] = atoi(argv[6]);
    //indexSeed2[1] = atoi(argv[7]);
    isolatedConnected->SetLower(lowerThreshold);
    isolatedConnected->SetUpper(upperThreshold);
    //isolatedConnected->SetSeed1(indexSeed1);
    //isolatedConnected->SetSeed2(indexSeed2);
    isolatedConnected->SetReplaceValue(255);

    seed[0]=data_pos[0];
    seed[1]=data_pos[1];
    seed[2]=data_pos[2];
    isolatedConnected->SetSeed(seed);
    isolatedConnected->SetInput(reader->GetOutput());
    isolatedConnected->Update();
    namesgenerator->SetOutputDirectory(Out_Folder);
    serieswriter->SetInput(isolatedConnected->GetOutput());
    serieswriter->SetImageIO(GDCM_IO);
    serieswriter->SetFileNames(namesgenerator->GetOutputFileNames());
    serieswriter->SetMetaDataDictionaryArray(reader->GetMetaDataDictionaryArray());
    
    try{
        serieswriter->Update();
    }
    catch (itk::ExceptionObject& excp){
        std::cerr << "Exception thrown while writing the series " << std::endl;
        return EXIT_FAILURE;
    }
    
    cout<<"end"<<endl;
    
    renWin->Finalize();
    iren->TerminateApp();
    planeWidgetZ->Delete();
    cbk->Delete();
    ren->Delete();
    renWin->Delete();
    iren->Delete();
    
    system("pause");
}
