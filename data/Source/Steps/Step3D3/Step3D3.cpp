#include <KsAPI.h>
#include <KompasLibraryActions.h>
#include "Resource.h"

#include <format>

static ksapi::IApplication * kompasApp = nullptr;

void * GetModule();
void LoadResources(ksapi::IApplication & app);

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса.
*/
//---
std::wstring LoadResourceString(int32_t strId)
{
  if (ksapi::ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(libname))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
      }
    }
  }

  return std::wstring();
}

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void ConstrAxisConeface(ksapi::IPartPtr & part);
void ConstrAxis2Point(ksapi::IPartPtr & part);
void ConstrAxisEdge(ksapi::IPartPtr & part);
void ConstrPlane3Point(ksapi::IPartPtr & part);
void ConstrElem(ksapi::IPartPtr & part);

//------------------------------------------------------------------------------
/**
  Сообщение пользователю.
*/
//---
void MessageT(const std::wstring & message, const std::wstring & title = LoadResourceString(IDS_MSG_TITTLE),
              ksMessageTypeEnum dlgType = ksMessageWarning, ksMessageButtonSetEnum dlgButtons = ksButtonSetOk,
              bool showModal = false)
{
  if (kompasApp)
    kompasApp->ShowMessageDialog(nullptr, message, title, L"", dlgType, dlgButtons, showModal, L"", L"", L"", 0, L"");
}

//------------------------------------------------------------------------------
/**
  Обработчик команд библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  // Получить активный документ трехмерной модели
  if (ksapi::IKompasDocument3DPtr document3d = kompasApp->GetActiveDocument())
  {
    // Получить верхний компонент
    if (ksapi::IPartPtr part = document3d->GetTopPart())
    {
      switch (commandId)
      {
        case 1:
          ConstrAxisConeface(part);
          break; // Ось конической поверхности
        case 2:
          ConstrAxis2Point(part);
          break; // Конструктивная ось по двум точкам
        case 3:
          ConstrAxisEdge(part);
          break; // Конструктивная ось, проходящая через ребро
        case 4:
          ConstrElem(part);
          break; // Смещенная плоскость, ось по двум плоскостям, плоскость под углом к другой пло-ти
        case 5:
          ConstrPlane3Point(part);
          break; // Плоскость через три вершины
      }
    }
  }
  else
  {
    MessageT(LoadResourceString(IDS_WRONG_DOCUMENT), LoadResourceString(IDS_ERROR));
  }

  ksapi::IKompasErrorPtr error = kompasApp->GetKompasError();
  if (error->GetCode() == etError10) // "Ошибка! Вырожденный объект"
    error->Clear();                  // Обнулить ошибки
}

//-------------------------------------------------------------------------------
// Создание отрезка
// ---
void LineSeg(ksapi::IFragmentDocumentPtr fragmentDocument, double x1, double y1, double x2, double y2, int style)
{
  if (!fragmentDocument)
    return;

  if (ksapi::IViewsAndLayersManagerPtr layersMngr = fragmentDocument->GetViewsAndLayersManager())
  {
    if (ksapi::IViewsPtr views = layersMngr->GetViews())
    {
      if (ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView())
      {
        if (ksapi::ILineSegmentsPtr lineSegments = drawingContainer->GetLineSegments())
        {
          if (ksapi::ILineSegmentPtr lineSegment = lineSegments->Add())
          {
            lineSegment->SetX1(x1);
            lineSegment->SetY1(y1);
            lineSegment->SetX2(x2);
            lineSegment->SetY2(y2);
            lineSegment->SetStyle(style);
            lineSegment->Update();
          }
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
//  Создать объект "окружность".
// ---
void Circle(ksapi::IFragmentDocumentPtr fragmentDocument, double xc, double yc, // центр окружности
            double radius,                                                      // радиус окружности
            int32_t style)                                                      // стиль отрисовки окружности
{
  if (ksapi::IViewsAndLayersManagerPtr layersMngr = fragmentDocument->GetViewsAndLayersManager())
  {
    if (ksapi::IViewsPtr views = layersMngr->GetViews())
    {
      if (ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView())
      {
        if (ksapi::ICirclesPtr circles = drawingContainer->GetCircles())
        {
          if (ksapi::ICirclePtr circle = circles->Add())
          {
            circle->SetXc(xc);
            circle->SetYc(yc);
            circle->SetRadius(radius);
            circle->SetStyle(style);
            circle->Update();
          }
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
//  Создать новый эскиз и операцию выдавливания
// ---
void CreateSketchAndExtrusion(ksapi::IPartPtr & part, double depth, double draftValue, double thicknessNormal,
                              double thicknessReverse)
{
  // Создать новый эскиз
  ksapi::IModelContainerPtr modelContainer = part->GetModelContainer();

  if (!modelContainer)
    return;

  ksapi::ISketchsPtr sketchs = modelContainer->GetSketchs();

  if (ksapi::ISketchPtr sketch = sketchs ? sketchs->Add() : nullptr)
  {
    // Получить базовую плоскость XOY
    ksapi::IModelObjectPtr basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY);

    sketch->SetPlane(basePlane);
    sketch->Update();

    // Войти в режим редактирования эскиза
    if (ksapi::IFragmentDocumentPtr fragmentDocument = sketch->BeginEdit(false))
    {
      // Введем новый эскиз - квадрат
      LineSeg(fragmentDocument, 50, 50, -50, 50, 1);
      LineSeg(fragmentDocument, 50, -50, -50, -50, 1);
      LineSeg(fragmentDocument, 50, -50, 50, 50, 1);
      LineSeg(fragmentDocument, -50, -50, -50, 50, 1);
      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    // Базовая операция выдавливания
    if (ksapi::IExtrusionsPtr extrusions = modelContainer->GetExtrusions())
    {
      if (ksapi::IExtrusionPtr extrusion = extrusions->Add(o3d_bossExtrusion))
      {
        extrusion->SetDirection(dtNormal);
        ksapi::IModelObjectPtr depthObject; /// ?????
        extrusion->SetSideParameters(true, etBlind, depth, draftValue, false, depthObject);

        // Изменить параметры тонкой стенки элемента
        if (ksapi::IThinParametersPtr thinParams = extrusion)
          thinParams->SetThinParameters(true, dtBoth, thicknessNormal, thicknessReverse);

        // Эскиз операции выдавливания
        extrusion->SetProfiles({sketch});
        extrusion->Update();
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Ось конической поверхности
// ---
void ConstrAxisConeface(ksapi::IPartPtr & part)
{
  // Создать новый эскиз
  ksapi::IModelContainerPtr modelContainer = part->GetModelContainer();

  if (!modelContainer)
    return;

  ksapi::ISketchsPtr sketchs = modelContainer->GetSketchs();

  if (ksapi::ISketchPtr sketch = sketchs ? sketchs->Add() : nullptr)
  {
    // Получить базовую плоскость XOY
    ksapi::IModelObjectPtr basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY);

    sketch->SetPlane(basePlane);
    sketch->Update();

    // Войти в режим редактирования эскиза
    if (ksapi::IFragmentDocumentPtr fragmentDocument = sketch->BeginEdit(false))
    {
      // Введем новый эскиз
      Circle(fragmentDocument, 20, 0, 10, 1);
      LineSeg(fragmentDocument, 0, 0, 0, 5, 3);
      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    if (ksapi::IRotatedsPtr rotateds = modelContainer->GetRotateds())
    {
      if (ksapi::IRotatedPtr rotate = rotateds->Add(o3d_bossRotated))
      {
        rotate->SetDirection(dtBoth);
        rotate->SetToroidShapeType(true);

        // Изменить параметры тонкой стенки элемента
        if (ksapi::IThinParametersPtr thinParams = rotate)
          thinParams->SetThinParameters(true, dtBoth, 1, 1);

        // Изменить параметры вращения в одном направлении
        rotate->SetAngle(true, 180);
        rotate->SetAngle(false, 180);
        rotate->SetProfile(sketch);
        rotate->Update();

        if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
        {
          if (ksapi::IAxes3DPtr axes = auxGeomContainer->GetAxes3D())
          {
            if (ksapi::IAxis3DByConefacePtr axis = axes->Add(o3d_axisConeFace))
            {
              ksapi::IFeaturePtr feature = rotate->GetOwner();
              std::vector<ksapi::IModelObjectPtr> faces =
                feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_face})
                        : std::vector<ksapi::IModelObjectPtr>();

              if (ksapi::IFacePtr face = faces[0])
              {
                // Установим базовую коническую поверхность
                axis->SetFace(face);
                axis->Update();
                MessageT(LoadResourceString(IDS_AXIS3D_BY_CONEFACE));
              }
            }
          }
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Конструктивная ось по двум точкам
// ---
void ConstrAxis2Point(ksapi::IPartPtr & part)
{
  // Создать новый эскиз и операцию выдавливания
  CreateSketchAndExtrusion(part, 20, 0, 20, 20);

  // Получить массив объектов заданного типа (вершины)
  ksapi::IFeaturePtr feature = part;
  std::vector<ksapi::IModelObjectPtr> vertexes =
    feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_vertex}) : std::vector<ksapi::IModelObjectPtr>();

  if (!vertexes.empty())
  {
    // Создадим ось по двум точкам
    if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
    {
      if (ksapi::IAxes3DPtr axes = auxGeomContainer->GetAxes3D())
      {
        if (ksapi::IAxis3DBy2PointsPtr axis = axes->Add(o3d_axis2Points))
        {
          std::vector<ksapi::IModelObjectPtr> selected = part->SelectByPoint(vertexes, 70.0, 70.0, 20.0);
          if (!selected.empty())
            axis->SetPoint1(selected.front());

          selected = part->SelectByPoint(vertexes, -70.0, -70.0, 0.0);
          if (!selected.empty())
            axis->SetPoint2(selected.front());

          axis->Update();
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Конструктивная ось, проходящая через ребро
// ---
void ConstrAxisEdge(ksapi::IPartPtr & part)
{
  // Создать новый эскиз и операцию выдавливания
  CreateSketchAndExtrusion(part, 20, 0, 20, 20);

  // Получить массив объектов заданного типа (рёбра)
  ksapi::IFeaturePtr feature = part;
  std::vector<ksapi::IModelObjectPtr> edges = feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_edge})
                                                      : std::vector<ksapi::IModelObjectPtr>();

  if (!edges.empty())
  {
    // Создадим ось через грань
    if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
    {
      if (ksapi::IAxes3DPtr axes = auxGeomContainer->GetAxes3D())
      {
        if (ksapi::IAxis3DByEdgePtr axis = axes->Add(o3d_axisEdge))
        {
          std::vector<ksapi::IModelObjectPtr> selected = part->SelectByPoint(edges, 50.0, 70.0, 20.0);
          if (!selected.empty())
            axis->SetEdge(selected.front());

          axis->Update();
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Плоскость через три вершины
// ---
void ConstrPlane3Point(ksapi::IPartPtr & part)
{
  // Создать новый эскиз и операцию выдавливания
  CreateSketchAndExtrusion(part, 20, 30, 10, 10);

  // Получить массив объектов заданного типа (вершины)
  ksapi::IFeaturePtr feature = part;
  std::vector<ksapi::IModelObjectPtr> vertexes =
    feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_vertex}) : std::vector<ksapi::IModelObjectPtr>();

  if (vertexes.size() > 2)
  {
    // Создадим плоскость, проходящюю через три вершины
    if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
    {
      if (ksapi::IPlanes3DPtr planes3d = auxGeomContainer->GetPlanes3D())
      {
        if (ksapi::IPlane3DBy3PointsPtr planeBy3Points = planes3d->Add(o3d_plane3Points))
        {
          std::vector<ksapi::IModelObjectPtr> selected = part->SelectByPoint(vertexes, -60.0, 60.0, 0.0);
          if (!selected.empty())
            planeBy3Points->SetPoint1(selected.front());

          selected = part->SelectByPoint(vertexes, -60.0, -60.0, 0.0);
          if (!selected.empty())
            planeBy3Points->SetPoint2(selected.front());

          selected = part->SelectByPoint(vertexes, 60.0, 60.0, 0.0);
          if (!selected.empty())
            planeBy3Points->SetPoint3(selected.front());

          planeBy3Points->Update();
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Смещенная плоскость, ось по двум плоскостям, плоскость под углом к другой плоскости
// ---
void ConstrElem(ksapi::IPartPtr & part)
{
  // Создадим смещенную плоскость
  if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
  {
    if (ksapi::IAxes3DPtr axes = auxGeomContainer->GetAxes3D())
    {
      if (ksapi::IPlanes3DPtr planes3d = auxGeomContainer->GetPlanes3D())
      {
        if (ksapi::IPlane3DByOffsetPtr planeOffset = planes3d->Add(o3d_planeOffset))
        {
          planeOffset->SetOffset(150);

          // Получить базовую плоскость XOY
          if (ksapi::IModelObjectPtr basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY))
          {
            basePlane->SetName(L"XOX");
            basePlane->Update();
            planeOffset->SetBasePlane(basePlane);
            planeOffset->SetDirection(false);
            planeOffset->SetName(LoadResourceString(IDS_OFFSETPLANE));
            planeOffset->Update();

            MessageT(LoadResourceString(IDS_CHANGE_OFFSETPLANE_PARAM));
            planeOffset->SetOffset(50);
            // Возьмем другую базовую плоскость YOZ
            if (basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeYOZ))
            {
              basePlane->SetName(L"YOZ");
              basePlane->Update();
              planeOffset->SetBasePlane(basePlane);
              planeOffset->SetDirection(true);
              planeOffset->Update();
            }

            MessageT(LoadResourceString(IDS_CREATE_AXIS3D_BY_2PLANES));
            // Возьмем другую базовую плоскость XOY
            if (basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY))
            {
              basePlane->SetName(L"XOY");
              basePlane->Update();

              // Ось на пересечении двух плоскостей
              if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
              {
                if (ksapi::IAxes3DPtr axes = auxGeomContainer->GetAxes3D())
                {
                  if (ksapi::IAxis3DBy2PlanesPtr axis2Planes = axes->Add(o3d_axis2Planes))
                  {
                    axis2Planes->SetPlane1(planeOffset); // Первая базовая плоскость - смещенная плоскость
                    axis2Planes->SetPlane2(basePlane);   // Вторая базовая плоскость - плоскость XOY
                    axis2Planes->SetName(LoadResourceString(IDS_AXIS3D_BY_2PLANES));
                    axis2Planes->Update();

                    MessageT(LoadResourceString(IDS_CHANGE_BASE_PLANE));
                    // Возьмем другую базовую плоскость XOZ
                    if (basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOZ))
                    {
                      basePlane->SetName(L"XOZ");
                      basePlane->Update();

                      axis2Planes->SetPlane2(basePlane); // Вторая базовая плоскость - плоскость XOZ
                      axis2Planes->Update();
                    }


                    MessageT(LoadResourceString(IDS_CREATE_PLANE3D_BY_ANGLE));
                    // Создадим плоскость под углом к другой плоскости
                    if (ksapi::IAuxiliaryGeomContainerPtr auxGeomContainer = part)
                    {
                      if (ksapi::IPlanes3DPtr planes3d = auxGeomContainer->GetPlanes3D())
                      {
                        if (ksapi::IPlane3DByAnglePtr planeByAngle = planes3d->Add(o3d_planeAngle))
                        {
                          planeByAngle->SetAngle(45);
                          planeByAngle->SetBasePlane(planeOffset);
                          planeByAngle->SetBaseLine(axis2Planes);
                          planeByAngle->SetName(LoadResourceString(IDS_PLANE3D_BY_ANGLE));
                          planeByAngle->Update();

                          MessageT(LoadResourceString(IDS_CHANGE_BASE_PLANE));
                          planeByAngle->SetBasePlane(basePlane);
                          planeByAngle->Update();
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Получить номер версии приложения.
*/
//---
void GetApplicationVersion(int32_t & major, int32_t & minor, int32_t & release, int32_t & build)
{
  major = BuildVersionMajor;
  minor = BuildVersionMinor;
  release = BuildVersionPatch;
  build = BuildVersionBuild;
}


//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libaryActions)
{
  libaryActions.AddGetApplicationVersion(GetApplicationVersion);
  libaryActions.AddRunCommandHandler(RunCommand);

  kompasApp = &app;
  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}
