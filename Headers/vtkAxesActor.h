/*=========================================================================

   Program:   Visualization Toolkit
   Module:    vtkAxesActor.h

   Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
   All rights reserved.
   See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

      This software is distributed WITHOUT ANY WARRANTY; without even
      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
      PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/
#ifndef vtkAxesActor_h
#define vtkAxesActor_h

#include "vtkProp3D.h"
#include "vtkRenderingAnnotationModule.h" // For export macro

class vtkActor;
class vtkCaptionActor2D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;
class vtkSphereSource;

class VTKRENDERINGANNOTATION_EXPORT vtkAxesActor : public vtkProp3D
{
public:
    static vtkAxesActor* New();
    vtkTypeMacro(vtkAxesActor, vtkProp3D);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    void GetActors(vtkPropCollection*) override;


    int RenderOpaqueGeometry(vtkViewport* viewport) override;
    int RenderTranslucentPolygonalGeometry(vtkViewport* viewport) override;
    int RenderOverlay(vtkViewport* viewport) override;

    vtkTypeBool HasTranslucentPolygonalGeometry() override;

    void ShallowCopy(vtkProp* prop) override;

    void ReleaseGraphicsResources(vtkWindow*) override;


    void GetBounds(double bounds[6]);
    double* GetBounds() VTK_SIZEHINT(6) override;

    vtkMTimeType GetMTime() override;

    vtkMTimeType GetRedrawMTime() override;


    void SetTotalLength(double v[3]) { this->SetTotalLength(v[0], v[1], v[2]); }
    void SetTotalLength(double x, double y, double z);
    vtkGetVectorMacro(TotalLength, double, 3);


    void SetNormalizedShaftLength(double v[3]) { this->SetNormalizedShaftLength(v[0], v[1], v[2]); }
    void SetNormalizedShaftLength(double x, double y, double z);
    vtkGetVectorMacro(NormalizedShaftLength, double, 3);


    void SetNormalizedTipLength(double v[3]) { this->SetNormalizedTipLength(v[0], v[1], v[2]); }
    void SetNormalizedTipLength(double x, double y, double z);
    vtkGetVectorMacro(NormalizedTipLength, double, 3);


    void SetNormalizedLabelPosition(double v[3])
    {
        this->SetNormalizedLabelPosition(v[0], v[1], v[2]);
    }
    void SetNormalizedLabelPosition(double x, double y, double z);
    vtkGetVectorMacro(NormalizedLabelPosition, double, 3);


    vtkSetClampMacro(ConeResolution, int, 3, 128);
    vtkGetMacro(ConeResolution, int);
    vtkSetClampMacro(SphereResolution, int, 3, 128);
    vtkGetMacro(SphereResolution, int);
    vtkSetClampMacro(CylinderResolution, int, 3, 128);
    vtkGetMacro(CylinderResolution, int);


    vtkSetClampMacro(ConeRadius, double, 0, VTK_FLOAT_MAX);
    vtkGetMacro(ConeRadius, double);
    vtkSetClampMacro(SphereRadius, double, 0, VTK_FLOAT_MAX);
    vtkGetMacro(SphereRadius, double);
    vtkSetClampMacro(CylinderRadius, double, 0, VTK_FLOAT_MAX);
    vtkGetMacro(CylinderRadius, double);


    void SetShaftType(int type);
    void SetShaftTypeToCylinder() { this->SetShaftType(vtkAxesActor::CYLINDER_SHAFT); }
    void SetShaftTypeToLine() { this->SetShaftType(vtkAxesActor::LINE_SHAFT); }
    void SetShaftTypeToUserDefined() { this->SetShaftType(vtkAxesActor::USER_DEFINED_SHAFT); }
    vtkGetMacro(ShaftType, int);


    void SetTipType(int type);
    void SetTipTypeToCone() { this->SetTipType(vtkAxesActor::CONE_TIP); }
    void SetTipTypeToSphere() { this->SetTipType(vtkAxesActor::SPHERE_TIP); }
    void SetTipTypeToUserDefined() { this->SetTipType(vtkAxesActor::USER_DEFINED_TIP); }
    vtkGetMacro(TipType, int);


    void SetUserDefinedTip(vtkPolyData*);
    vtkGetObjectMacro(UserDefinedTip, vtkPolyData);


    void SetUserDefinedShaft(vtkPolyData*);
    vtkGetObjectMacro(UserDefinedShaft, vtkPolyData);


    vtkProperty* GetXAxisTipProperty();
    vtkProperty* GetYAxisTipProperty();
    vtkProperty* GetZAxisTipProperty();


    vtkProperty* GetXAxisShaftProperty();
    vtkProperty* GetYAxisShaftProperty();
    vtkProperty* GetZAxisShaftProperty();

    vtkCaptionActor2D* GetXAxisCaptionActor2D() { return this->XAxisLabel; }
    vtkCaptionActor2D* GetYAxisCaptionActor2D() { return this->YAxisLabel; }
    vtkCaptionActor2D* GetZAxisCaptionActor2D() { return this->ZAxisLabel; }


    vtkSetStringMacro(XAxisLabelText);
    vtkGetStringMacro(XAxisLabelText);
    vtkSetStringMacro(YAxisLabelText);
    vtkGetStringMacro(YAxisLabelText);
    vtkSetStringMacro(ZAxisLabelText);
    vtkGetStringMacro(ZAxisLabelText);


    vtkSetMacro(AxisLabels, vtkTypeBool);
    vtkGetMacro(AxisLabels, vtkTypeBool);
    vtkBooleanMacro(AxisLabels, vtkTypeBool);

    enum
    {
        CYLINDER_SHAFT,
        LINE_SHAFT,
        USER_DEFINED_SHAFT
    };

    enum
    {
        CONE_TIP,
        SPHERE_TIP,
        USER_DEFINED_TIP
    };

protected:
    vtkAxesActor();
    ~vtkAxesActor() override;

    vtkCylinderSource* CylinderSource;
    vtkLineSource* LineSource;
    vtkConeSource* ConeSource;
    vtkSphereSource* SphereSource;

    vtkActor* XAxisShaft;
    vtkActor* YAxisShaft;
    vtkActor* ZAxisShaft;

    vtkActor* XAxisTip;
    vtkActor* YAxisTip;
    vtkActor* ZAxisTip;

    void UpdateProps();

    double TotalLength[3];
    double NormalizedShaftLength[3];
    double NormalizedTipLength[3];
    double NormalizedLabelPosition[3];

    int ShaftType;
    int TipType;

    vtkPolyData* UserDefinedTip;
    vtkPolyData* UserDefinedShaft;

    char* XAxisLabelText;
    char* YAxisLabelText;
    char* ZAxisLabelText;

    vtkCaptionActor2D* XAxisLabel;
    vtkCaptionActor2D* YAxisLabel;
    vtkCaptionActor2D* ZAxisLabel;

    vtkTypeBool AxisLabels;

    int ConeResolution;
    int SphereResolution;
    int CylinderResolution;

    double ConeRadius;
    double SphereRadius;
    double CylinderRadius;

private:
    vtkAxesActor(const vtkAxesActor&) = delete;
    void operator=(const vtkAxesActor&) = delete;
};

#endif
