#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/vtk_context.hxx>

#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;

#include <map>
#include <set>
#include <string>
#include <vector>

#include <ExternalVTKWidget.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>

#include < vtkBMPWriter.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkPNGWriter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>

void createDepthMap1(vtkRenderWindow *renWin,
                     const std::string &outputFileName) {
        int width = renWin->GetSize()[0];
        int height = renWin->GetSize()[1];

        GLfloat *zBufferData =
            static_cast<GLfloat *>(renWin->GetZbufferData(0, 0, width, height));

        // float *zBufferData = new float[width * height];
        // glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT,
        //              zBufferData);

        float *depthArray = new float[width * height];
        for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                        int tx = (x - y + width) % width;
                        depthArray[y * width + tx] = zBufferData[y * width + x];
                }
                depthArray[y * width + width - 1] = 1.0;
        }

        vtkSmartPointer<vtkImageData> imageData =
            vtkSmartPointer<vtkImageData>::New();
        imageData->SetDimensions(width, height, 1);
        imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

        unsigned char *pixelData =
            static_cast<unsigned char *>(imageData->GetScalarPointer());
        for (int y = 0; y < height; ++y) {

                for (int x = 0; x < width; ++x) {
                        int index = y * width + x;
                        pixelData[index] = static_cast<unsigned char>(
                            depthArray[y * width + x] * 255);
                }
        }

        vtkSmartPointer<vtkBMPWriter> imageWriter =
            vtkSmartPointer<vtkBMPWriter>::New();
        imageWriter->SetFileName(outputFileName.c_str());
        imageWriter->SetInputData(imageData);
        imageWriter->Write();

        delete[] depthArray;
        delete[] zBufferData;
}

void createDepthMap(vtkRenderWindow *renWin,
                    const std::string &outputFileName) {
        // Create filters and writer
        vtkSmartPointer<vtkWindowToImageFilter> filter =
            vtkSmartPointer<vtkWindowToImageFilter>::New();
        vtkSmartPointer<vtkImageShiftScale> scale =
            vtkSmartPointer<vtkImageShiftScale>::New();
        vtkSmartPointer<vtkBMPWriter> imageWriter =
            vtkSmartPointer<vtkBMPWriter>::New();

        // Set up filters
        filter->SetInput(renWin);
        // filter->SetMagnification(1);
        filter->SetInputBufferTypeToZBuffer();

        scale->SetOutputScalarTypeToUnsignedChar();
        scale->SetInputConnection(filter->GetOutputPort());
        scale->SetShift(0);
        scale->SetScale(-255);

        // Set output file name and write
        imageWriter->SetFileName(outputFileName.c_str());
        imageWriter->SetInputConnection(scale->GetOutputPort());
        imageWriter->Write();
}
using namespace std;

Napi::FunctionReference *record_class_ = NULL;

void convertOpenGLProjectionMatrixToVTK(GLfloat glProjectionMatrix[16],
                                        vtkMatrix4x4 *vtkProjectionMatrix) {
        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                        vtkProjectionMatrix->SetElement(
                            i, j, glProjectionMatrix[j * 4 + i]);
                }
        }
}

#include <vtkBoundingBox.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>

class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera {
      public:
        static MouseInteractorStyle *New();
        vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);

        virtual void OnLeftButtonDown() override {
                std::cout << "Left button down" << std::endl;
                vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }

        virtual void OnMouseMove() override {
                std::cout << "Mouse move" << std::endl;
                vtkInteractorStyleTrackballCamera::OnMouseMove();
        }

        virtual void OnRightButtonDown() override {
                std::cout << "Right button down" << std::endl;
                vtkInteractorStyleTrackballCamera::OnRightButtonDown();
        }
};

vtkBoundingBox CalculateBoundingBoxFromRenderWindow(
    vtkSmartPointer<vtkRenderWindow> renderWindow) {
        vtkSmartPointer<vtkRenderer> renderer =
            renderWindow->GetRenderers()->GetFirstRenderer();
        vtkSmartPointer<vtkActorCollection> actorCollection =
            renderer->GetActors();
        vtkBoundingBox boundingBox;
        actorCollection->InitTraversal();
        vtkSmartPointer<vtkActor> actor = nullptr;
        while ((actor = actorCollection->GetNextActor()) != nullptr) {
                vtkSmartPointer<vtkDataSet> dataSet =
                    vtkDataSet::SafeDownCast(actor->GetMapper()->GetInput());
                if (dataSet) {
                        double bounds[6];
                        dataSet->GetBounds(bounds);
                        boundingBox.AddBounds(bounds);
                }
        }
        return boundingBox;
}

vtkStandardNewMacro(MouseInteractorStyle);

class TVTKContext : public IVtkContext, public Napi::ObjectWrap<TVTKContext> {
      public:
        static Napi::FunctionReference *Init(Napi::Env env);

      public:
        virtual bool Initialize(void *wID) {
                _initialize(wID);
                return true;
        }
        virtual bool Resize() {
                _resize();
                return true;
        }
        virtual bool Paint() {
                _paint();
                return true;
        }
        virtual bool GetZBuffer(float *&zbuf) {
                zbuf = pDepthBuffer;
                return true;
        }
        virtual bool GetRange(double &xmin, double &ymin, double &zmin,
                              double &xmax, double &ymax, double &zmax) {
                if (renderer.Get()) {
                        auto boundingBox =
                            CalculateBoundingBoxFromRenderWindow(renderWindow);
                        boundingBox.GetMinPoint(xmin, ymin, zmin);
                        boundingBox.GetMaxPoint(xmax, ymax, zmax);
                }
                return true;
        }

        virtual bool GetRenderer(void *&vrenderer) {
                vrenderer = (void *)renderer.Get();
                return true;
        }

        virtual bool ReceiveMouseEvent(TMouseEvent *event) {
                for (auto cb : _callbacks) {
                        cb(event, (void *)this);
                }
                return true;
        }
        virtual bool RegisterMouseListenCallback(mouse_listen_fn_t cb) {
                _callbacks.push_back(cb);
                return true;
        }

        virtual bool GetRenderWindow(void *&vrenderWindow) {
                vrenderWindow = (void *)renderWindow.Get();
                return true;
        }

        virtual bool SetProperty(const char *name, void *prop) {
                _props[name] = prop;
                return true;
        }
        virtual bool GetProperty(const char *name, void *&prop) {
                if (_props.find(name) != _props.end()) {
                        prop = _props[name];
                        return true;
                } else {
                        prop = NULL;
                        return false;
                }
        }

      private:
        vector<mouse_listen_fn_t> _callbacks;
        map<string, void *> _props;

      public:
        TVTKContext(const Napi::CallbackInfo &info)
            : Napi::ObjectWrap<TVTKContext>(info) {
                ppDepthBuffer = new (float *);

                pDepthBuffer = NULL;
                ppDepthBuffer = &pDepthBuffer;

                buffer_size = 0;
        }
        ~TVTKContext() { delete ppDepthBuffer; }

      private:
        vtkSmartPointer<vtkExternalOpenGLRenderWindow> renderWindow;
        vtkSmartPointer<vtkRenderer> renderer;
        vtkNew<ExternalVTKWidget> externalVTKWidget;

        float **ppDepthBuffer;
        float *pDepthBuffer;
        int buffer_size;

      private:
        void _initialize(void *wID) {

                renderWindow = externalVTKWidget->GetRenderWindow();
                renderWindow->SetParentId(wID);
                renderer = externalVTKWidget->AddRenderer();

                /*
                vtkCylinderSource *cylinderSource = vtkCylinderSource::New();
                cylinderSource->SetHeight(0.3);
                cylinderSource->SetRadius(0.1);
                cylinderSource->SetResolution(360);

                vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
                mapper->SetInputConnection(cylinderSource->GetOutputPort());

                vtkActor *actor = vtkActor::New();
                actor->SetMapper(mapper);

                renderer->AddActor(actor);*/
        }

        void _resize() {
                if (renderer.Get()) {
                        // cout << "resize" << endl;
                        vtkSmartPointer<vtkCamera> camera =
                            renderer->GetActiveCamera(); // GetCamera();

                        GLfloat projectionMatrix[16];
                        glMatrixMode(GL_PROJECTION);
                        glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

                        vtkSmartPointer<vtkMatrix4x4> vtkProjectionMatrix =
                            vtkSmartPointer<vtkMatrix4x4>::New();
                        convertOpenGLProjectionMatrixToVTK(projectionMatrix,
                                                           vtkProjectionMatrix);

                        camera->SetParallelProjection(true);
                        camera->SetExplicitProjectionTransformMatrix(
                            vtkProjectionMatrix);
                }
        }

        void _paint() {
                if (renderer.Get()) {
                        renderWindow->MakeCurrent();
                        externalVTKWidget->GetRenderWindow()->Render();

                        int width = renderWindow->GetSize()[0];
                        int height = renderWindow->GetSize()[1];

                        if (pDepthBuffer != NULL &&
                            buffer_size != width * height) {
                                delete[] pDepthBuffer;
                                pDepthBuffer = NULL;
                        }

                        if (pDepthBuffer == NULL) {
                                pDepthBuffer = new float[width * height];
                                buffer_size = width * height;
                        }

                        GLfloat *zBufferData =
                            renderWindow->GetZbufferData(0, 0, width, height);

                        for (int y = 0; y < height; ++y) {
                                for (int x = 0; x < width; ++x) {
                                        int tx = (x - y + width) % width;
                                        pDepthBuffer[y * width + tx] =
                                            zBufferData[y * width + x];
                                }

                                pDepthBuffer[y * width + width - 1] = 1.0;
                        }
                        delete[] zBufferData;
                        // createDepthMap1(renderWindow, "d:/vtk.png");
                }
        }

        Napi::Value Interface(const Napi::CallbackInfo &info) {
                uint64_t ullHandle = (uint64_t)(void *)(this);
                string strHandle = type_cast<string>(ullHandle);

                return Napi::Value::From(info.Env(), strHandle.c_str());
        }
};

Napi::FunctionReference *TVTKContext::Init(Napi::Env env) {
        Napi::Function func =
            DefineClass(env, "TVTKContext",
                        {InstanceMethod("Interface", &TVTKContext::Interface)});
        Napi::FunctionReference *constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);

        return constructor;
}

//////////////////////////////////////////////////////////////////////////////////////
/* DCiP plugin Javascript parameter and return value
   parsing/wrap guide

   1)Parse callback paramter by index from javascript by using
   the following line: var cb = JS_EXT_PARA(TComxCallback,
   [index]);

     Use the following codes to invokd callback function:
     cb([parameters list]);

     cb also can be saved as a global variable.

   2)Parse common paramter by index from javascript by using the
   following line: var val = JS_EXT_PARA([value_type], [index]);

   3)Parse buffer paramter by index from javascript by using the
   following line: TComxBuffer buf = JS_EXT_PARA(TComxBuffer,
   [index]);

     Use the following codes to operate buffer:

        void * arr_buf = buf.Data();
        size_t arr_byte_size = buf.ByteLength();

        int16_t *arr_data = (int16_t*)arr_buf;
        size_t arr_len = arr_byte_size / sizeof(int16_t);

        for (size_t i = 0; i < arr_len; i++)
        {
             arr_data[i] += 2;
        }

   4)Return a buffer by using the following line:
     JS_EXT_FUNC_ASSIGN_RET_EX(buf, 1);

   5)Return a common value by using the follwoing line
     JS_EXT_FUNC_ASSIGN_RET([value]);
*/

//////////////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following
// codes.

JS_EXT_DATA_DECLARE()

// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.

#define CREATECONTEXT_FUNC_USAGE                                               \
        "CreateContext Usage: comx.vtk.CreateContext();"
JS_EXT_FUNC_BEGIN(CreateContext, 0, CREATECONTEXT_FUNC_USAGE) {
        // Put your codes here
        if (record_class_ == NULL) {
                record_class_ = TVTKContext::Init(info.Env());
        }

        Napi::EscapableHandleScope scope(info.Env());
        Napi::Object obj = record_class_->New({});
        return scope.Escape(napi_value(obj)).ToObject();
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////
// please set your javascript extension namespace in the
// following codes.

#define JS_EXT_NS "comx.vtk"

//////////////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the
// following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(CreateContext)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 1)
JS_EXT_FUNC_REG(CreateContext)
JS_EXT_MAIN_END()
