////////////////////////////////////////////////////////////////////////////////
//
// Step4_API7_3D.cpp - Библиотека на Visual C++
//
////////////////////////////////////////////////////////////////////////////////
#include "Step4_KsAPI_3D_pch.h"
#include "Step4_KsAPI_3D.h"

ksapi::IApplication * application;

//------------------------------------------------------------------------------
/**
  Сообщение от библиотеки
*/
//---
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons,
                        bool showModal)
{
  std::int32_t res = 0;

  if (!message.empty() && application)
    res = application->ShowMessageBox(message, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return res;
}


/// Результат диалога
enum DialogResult
{
  Positive = 6,
  Negative = 7,
  Cancel = 2
};


//------------------------------------------------------------------------------
/**
  Диалог да/нет/отмена
*/
//---
std::int32_t YesNoMessage(const std::wstring & message)
{
  std::int32_t res = LibMessage(message, ksMessageWarning, ksButtonSetYesNoCancel, false);
  return std::int32_t(res == DialogResult::Positive);
}


//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void CreateAndEditPointDrivenPattern(ksapi::IKompasDocumentPtr & doc);
void CreateAndEditCircularPattern(ksapi::IKompasDocumentPtr & doc);
void CreateAndEditMirrorPattern(ksapi::IKompasDocumentPtr & doc);
void CreateAndEditExtrusionSurface(ksapi::IKompasDocumentPtr & doc);
void CreateAndEditMeshPointSurface(ksapi::IKompasDocumentPtr & doc);
void CreateAndEditFilletCurve(ksapi::IKompasDocumentPtr & doc);


//-------------------------------------------------------------------------------
// Задать идентификатор ресурсов
// ---
unsigned int GetMenuId()
{
  return IDR_LIBID;
}


//-------------------------------------------------------------------------------
// Головная функция библиотеки
// ---
void RunCommand(int32_t commandId, ksapi::ksRunCommandModeEnum mode)
{
  if (application)
  {
    // Получаем текущий документ
    ksapi::IKompasDocumentPtr doc(application->GetActiveDocument());

    switch (commandId)
    {
      case 1:
      {
        // Массив по точкам
        CreateAndEditPointDrivenPattern(doc);
        break;
      }

      case 2:
      {
        // Массив по концентрической сетке
        CreateAndEditCircularPattern(doc);
        break;
      }

      case 3:
      {
        // Зеркальный массив
        CreateAndEditMirrorPattern(doc);
        break;
      }

      case 4:
      {
        // Поверхность выдавливания
        CreateAndEditExtrusionSurface(doc);
        break;
      }

      case 5:
      {
        // Поверхность по сети точек
        CreateAndEditMeshPointSurface(doc);
        break;
      }

      case 6:
      {
        // Операция скругления кривых
        CreateAndEditFilletCurve(doc);
        break;
      }
    }
  }
}


extern void LoadResources(ksapi::IApplication & app);

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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetMenuIdHandler(GetMenuId);

  application = &app;
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


//-----------------------------------------------------------------------------
/**
  Получить контейнер модельных объектов
  \param doc - указатель на текущий документ
  \return указатель на интерфейс контейнера модельных объектов
*/
//---
ksapi::IModelContainerPtr GetModelContainer(ksapi::IKompasDocumentPtr & doc)
{
  ksapi::IModelContainerPtr mCont;

  if (doc)
  {
    // Получаем документ 3Д
    if (ksapi::IKompasDocument3DPtr document3D = doc)
    {
      // Получаем контейнер модельных объектов из верхнего компонента
      mCont = document3D->GetTopPart();
    }
  }

  return mCont;
}


//-----------------------------------------------------------------------------
/**
  Получить интерфейс контейнера поверхностей
  \param doc - указатель на текущий документ
  \return указатель на интерфейс контейнера поверхностей
*/
//---
ksapi::ISurfaceContainerPtr GetSurfaceContainer(ksapi::IKompasDocumentPtr & doc)
{
  ksapi::ISurfaceContainerPtr surfCont;

  if (doc)
  {
    // Получаем документ 3Д
    ksapi::IKompasDocument3DPtr document3D(doc);

    if (document3D)
    {
      // Получаем контейнер поверхностей из верхнего компонента
      surfCont = document3D->GetTopPart();
    }
  }

  return surfCont;
}


//-----------------------------------------------------------------------------
/**
  Получить интерфейс контейнера вспомогательной геометрии
  \param doc - указатель на текущий документ
  \return указатель на интерфейс контейнера вспомогательной геометрии
*/
//---
ksapi::IAuxiliaryGeomContainerPtr GetAuxiliaryGeomContainer(ksapi::IKompasDocumentPtr & doc)
{
  ksapi::IAuxiliaryGeomContainerPtr auxCont;

  if (doc)
  {
    // Получаем документ 3D
    ksapi::IKompasDocument3DPtr document3D(doc);

    if (document3D)
    {
      // Получаем контейнер вспомогательной геометрии из верхнего компонента
      auxCont = document3D->GetTopPart();
    }
  }

  return auxCont;
}


void DrawRectangle(const ksapi::IKompasDocument2DPtr & document, double x, double y, double width, double height, short style)
{
  ksapi::IViewsAndLayersManagerPtr view_and_layers_mng = document ? document->GetViewsAndLayersManager() : nullptr;
  ksapi::IViewsPtr views = view_and_layers_mng ? view_and_layers_mng->GetViews() : nullptr;
  ksapi::IDrawingContainerPtr container = views ? views->GetActiveView() : nullptr;

  if (ksapi::IRectanglesPtr rectangles = container ? container->GetRectangles() : nullptr)
  {
    if (ksapi::IRectanglePtr rectangle = rectangles->Add())
    {
      rectangle->SetX(x);
      rectangle->SetY(y);
      rectangle->SetWidth(width);
      rectangle->SetHeight(height);
      rectangle->SetStyle(style);
      rectangle->Update();
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создать эскиз в виде прямоугольника
  \param doc - указатель на текущий документ
  \param x, y - координаты базовой точки прямоугольника
  \param h - высота прямоугольника
  \param w - ширина прямоугольника
  \return указатель на интерфейс эскиза
*/
//---
ksapi::ISketchPtr CreateSketch(ksapi::IKompasDocumentPtr & doc, double x, double y, double h, double w)
{
  ksapi::ISketchPtr sketch;

  if (doc)
  {
    if (ksapi::IModelContainerPtr modelCont = GetModelContainer(doc))
    {
      // Получаем коллекцию эскзиов
      if (ksapi::ISketchsPtr sketchs = modelCont->GetSketchs())
      {
        // Добавляем новый эскиз
        if (sketch = sketchs->Add())
        {
          // Получаем 3D документ
          if (ksapi::IKompasDocument3DPtr doc3D = doc)
          {
            // Получаем верхнюю деталь
            if (ksapi::IPartPtr part = doc3D->GetTopPart())
            {
              // Получаем плоскость XY
              ksapi::IModelObjectPtr planeXY = part->GetDefaultObject(o3d_planeXOY);
              // Задаем базовую плоскость для эскиза
              sketch->SetPlane(planeXY);

              // Редактируем эскиз
              if (ksapi::IKompasDocument2DPtr doc = sketch->BeginEdit(false))
                // Строим прямоугольник
                DrawRectangle(doc, x, y, w, h, ksCSNormal);

              sketch->EndEdit();
            }
          }
          sketch->Update();
        }
      }
    }
  }

  return sketch;
}


//-----------------------------------------------------------------------------
/**
  Создать операцию выдавливания
  \param doc - указатель на текущий документ
  \return указатель на операцию выдавливания
*/
//---
ksapi::IExtrusionPtr CreateExtrusion(ksapi::IKompasDocumentPtr & doc)
{
  ksapi::IExtrusionPtr newExtrusion;

  if (doc)
  {
    // Получаем контейнер модельных объектов
    if (ksapi::IModelContainerPtr modelCont = GetModelContainer(doc))
    {
      // Получаем коллекцию операций выдавливания
      if (ksapi::IExtrusionsPtr extrusions = modelCont->GetExtrusions())
      {
        // Добавляем новую операцию выдавливания
        newExtrusion = extrusions->Add(o3d_bossExtrusion);

        if (newExtrusion)
        {
          // Создаем эскиз для построения операции
          if (ksapi::ISketchPtr sketch = CreateSketch(doc, 0, 0, 25, 25))
            newExtrusion->SetProfiles({sketch});

          // Задаем параметры операции
          newExtrusion->SetDepth(true /*normal*/, 10);
          newExtrusion->SetDirection(dtNormal);
          newExtrusion->SetExtrusionType(true /*normal*/, etBlind);
          newExtrusion->Update();
        }
      }
    }
  }

  return newExtrusion;
}


//-----------------------------------------------------------------------------
/**
  Создать точку
  \param doc - указатель на текущий документ
  \param x, y, z - координаты точки
  \return указатель на интерфейс точки
*/
//---
ksapi::IPoint3DPtr CreatePoint(ksapi::IKompasDocumentPtr & doc, double x, double y, double z)
{
  ksapi::IPoint3DPtr newPoint;

  if (doc)
  {
    // Получаем контейнер модельных объектов
    if (ksapi::IModelContainerPtr modelCont = GetModelContainer(doc))
    {
      // Получаем коллекцию точек
      if (ksapi::IPoints3DPtr pointsColl = modelCont->GetPoints3D())
      {
        // Добавляем новую точку
        if (newPoint = pointsColl->Add())
        {
          // Задаем параметры точки
          newPoint->SetX(x);
          newPoint->SetY(y);
          newPoint->SetZ(z);
          newPoint->SetSymbol(ksDotPoint);
          newPoint->SetParameterType(ksPParamCoord);
          newPoint->Update();
        }
      }
    }
  }

  return newPoint;
}


//-----------------------------------------------------------------------------
/**
  Создать ломаную из одного сегмента
  \param doc - указатель на текущий документ
  \return указатель на интерфейс контейнера модельных объектов
*/
//---
ksapi::IPolyLinePtr CreatePolyLine(ksapi::IKompasDocumentPtr & doc, double x1, double y1, double z1, double x2, double y2,
                                   double z2)
{
  ksapi::IPolyLinePtr polyLine;

  // Получаем контейнер вспомогательной геометрии
  if (ksapi::IAuxiliaryGeomContainerPtr geomCont = GetAuxiliaryGeomContainer(doc))
  {
    // Получаем коллекцию ломаных
    if (ksapi::IPolyLinesPtr coll = geomCont->GetPolyLines())
    {
      // Добавляем ломаную в коллекцию
      if (polyLine = coll->Add())
      {
        // Создаем вершины ломаной
        if (ksapi::ICurveVertexParamPtr vertex1 = polyLine->AddVertex(-1))
          vertex1->SetParamVertex(x1, y1, z1, 1);

        if (ksapi::ICurveVertexParamPtr vertex2 = polyLine->AddVertex(-1))
          vertex2->SetParamVertex(x2, y2, z2, 1);

        polyLine->Update();
      }
    }
  }

  return polyLine;
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование массива по точкам
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditPointDrivenPattern(ksapi::IKompasDocumentPtr & doc)
{
  if (doc)
  {
    // Получаем контейнер модельных объектов
    ksapi::IModelContainerPtr modelCont = GetModelContainer(doc);

    if (modelCont)
    {
      // Получаем коллекцию массивов
      if (ksapi::IFeaturePatternsPtr patternsCol = modelCont->GetFeaturePatterns())
      {
        // Добавляем новую операцию копирования по точкам
        if (ksapi::IPointDrivenPatternPtr newPattern = patternsCol->Add(o3d_PointDrivenPattern))
        {
          // Создаем исходный объект для массива - операцию выдавливания
          ksapi::IModelObjectPtr iniObj(CreateExtrusion(doc));
          // Создаем точки для построения массива
          ksapi::IPoint3DPtr point1(CreatePoint(doc, 27, -39, 13));
          ksapi::IPoint3DPtr point2(CreatePoint(doc, -16, -15, 31));
          ksapi::IPoint3DPtr point3(CreatePoint(doc, -41, 35, 6));

          if (iniObj && point1 && point2 && point3)
          {
            // Устанавливаем исходный объект массива
            newPattern->SetInitialObjects({iniObj});

            // Проверяем пригодность точек для операции
            if (newPattern->IsSuitableDrivenObject(point1) && newPattern->IsSuitableDrivenObject(point2) &&
                newPattern->IsSuitableDrivenObject(point3))
            {
              // Создаем массив точек
              std::vector<ksapi::IModelObjectPtr> points;
              points.emplace_back(point1);
              points.emplace_back(point2);
              points.emplace_back(point3);
              // Задаем массив управляющих объектов
              newPattern->SetDrivenObjects(points);
            }
            newPattern->Update();
          }
        }

        // Редактирование массива
        if (YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Получаем массив из коллекции
          if (ksapi::IPointDrivenPatternPtr pattern = patternsCol->GetFeaturePattern(0))
          {
            // Создаем массив индексов удаляемых элементов
            std::vector<std::int32_t> indices;
            indices.emplace_back(2);
            indices.emplace_back(0);

            // Удаляем элемент
            pattern->SetInstanceDeletedIndexes(indices);
            pattern->Update();
          }
        }
      }
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование массива по концентрической сетке
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditCircularPattern(ksapi::IKompasDocumentPtr & doc)
{
  if (doc)
  {
    // Получаем контейнер модельных объектов
    if (ksapi::IModelContainerPtr modelCont = GetModelContainer(doc))
    {
      // Получаем коллекцию массивов
      if (ksapi::IFeaturePatternsPtr patternsCol = modelCont->GetFeaturePatterns())
      {
        // Добавляем новую операцию копирования по концентрической сетке
        if (ksapi::ICircularPatternPtr newPattern = patternsCol->Add(o3d_circularCopy /*массив операций*/))
        {
          // Создаем исходный объект для массива - операцию выдавливания
          if (ksapi::IModelObjectPtr iniObj = CreateExtrusion(doc))
          {
            // Устанавливаем исходный объект массива
            newPattern->SetInitialObjects({iniObj});

            // Шаг копирования в кольцевом направлении
            newPattern->SetStep2(360);
            // Количество экземпляров в кольцевом направлении
            newPattern->SetCount2(6);
            // Шаг между крайними экземплярами
            newPattern->SetBoundaryInstancesStepFactor2(true);

            // Ось массива
            if (ksapi::IKompasDocument3DPtr document3D = doc)
            {
              if (ksapi::IPartPtr part = document3D->GetTopPart())
              {
                if (ksapi::IModelObjectPtr axisOZ = part->GetDefaultObject(o3d_axisOZ))
                  newPattern->SetAxis(axisOZ);
              }
            }
            newPattern->Update();
          }
        }

        // Редактируем
        if (YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Получаем массив из коллекции
          if (ksapi::ICircularPatternPtr pattern = patternsCol->GetFeaturePattern(0))
          {
            // Шаг копирования в радиальном направлении
            pattern->SetStep1(10);
            // Количество экземпляров в радиальном направлении
            pattern->SetCount1(5);
            // Исходная ориентация
            pattern->SetSaveInitialOrientation(true);
            pattern->Update();
          }
        }
      }
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование зеркального массива
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditMirrorPattern(ksapi::IKompasDocumentPtr & doc)
{
  if (doc)
  {
    ksapi::IKompasDocument3DPtr document3D(doc);
    // Получаем контейнер модельных объектов
    ksapi::IModelContainerPtr modelCont(GetModelContainer(doc));

    if (modelCont && document3D)
    {
      // Получаем коллекцию массивов
      ksapi::IFeaturePatternsPtr patternsCol(modelCont->GetFeaturePatterns());
      ksapi::IPartPtr part(document3D->GetTopPart());

      if (patternsCol && part)
      {
        // Добавляем новый зеркальный массив
        ksapi::IMirrorPatternPtr newPattern = patternsCol->Add(o3d_mirrorOperation);

        if (newPattern)
        {
          // Создаем исходный объект для массива - операцию выдавливания
          if (ksapi::IModelObjectPtr iniObj = CreateExtrusion(doc))
          {
            // Устанавливаем исходный объект массива
            newPattern->SetInitialObjects({iniObj});

            // Получаем плоскость симметрии
            if (ksapi::IModelObjectPtr planeXY = part->GetDefaultObject(o3d_planeXOY))
              newPattern->SetPlane(planeXY);

            // Оставлять исходные объекты
            newPattern->SetSaveInitialObjects(true);
            newPattern->Update();
          }
        }

        // Редактируем
        if (YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Получаем массив из коллекции
          if (ksapi::IMirrorPatternPtr pattern = patternsCol->GetFeaturePattern(0))
          {
            // Меняем плоскость симметрии
            if (ksapi::IModelObjectPtr planeXZ = part->GetDefaultObject(o3d_planeXOZ))
              pattern->SetPlane(planeXZ);

            pattern->Update();
          }
        }
      }
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование поверхности выдавливания
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditExtrusionSurface(ksapi::IKompasDocumentPtr & doc)
{
  // Получаем контейнер поверхностей
  if (ksapi::ISurfaceContainerPtr surfCont = GetSurfaceContainer(doc))
  {
    // Получаем коллекцию поверхностей выдавливания
    if (ksapi::IExtrusionsPtr exrusionsCol = surfCont->GetExtrusionSurfaces())
    {
      ksapi::IExtrusionSurfacePtr surface = exrusionsCol->Add(o3d_ExtrusionSurface);

      // Добавляем новую поверхность выдавливания
      if (ksapi::IExtrusionPtr extrussion = surface)
      {
        // Создаем эскиз
        if (ksapi::ISketchPtr sketch = CreateSketch(doc, -25, -40, 40, 40))
        {
          // Сечение
          extrussion->SetProfiles({sketch});
          // Направляющий объект
          extrussion->SetDirectionObject(sketch);
          // Глубина
          extrussion->SetDepth(true, 10);
          // Признак замкнутости поверхности
          surface->SetClosedShell(true);
          extrussion->Update();
        }
      }
      // Редактируем
      if (YesNoMessage(LoadStr(IDS_EDIT)))
      {
        // Получаем поверхность выдавливания из коллекции
        if (ksapi::IExtrusionSurfacePtr surface = exrusionsCol->GetExtrusion(0))
        {
          // Уклон внутрь
          surface->SetDraftOutward(true, true);
          // Угол уклона
          surface->SetDraftValue(true, 15);
          // Глубина
          surface->SetDepth(true, 30);
          // Признак замкнутости поверхности
          surface->SetClosedShell(false);
          surface->Update();
        }
      }
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование поверхности по сети точек
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditMeshPointSurface(ksapi::IKompasDocumentPtr & doc)
{
  // Получаем контейнер поверхностей
  if (ksapi::ISurfaceContainerPtr surfCont = GetSurfaceContainer(doc))
  {
    // Получаем коллекцию поверхностей по сети точек
    if (ksapi::IMeshPointsSurfacesPtr meshCol = surfCont->GetMeshPointsSurfaces())
    {
      // Добавляем новую поверхность по сети точек
      if (ksapi::IMeshPointsSurfacePtr newSurface = meshCol->Add())
      {
        // 1-й ряд точек
        newSurface->BeginPointsSeries(true, -1);
        newSurface->AddPoint(39, -62, 23, 1, ksPParamCoord, nullptr);
        newSurface->AddPoint(18, -40, 22, 1, ksPParamCoord, nullptr);
        newSurface->AddPoint(-13, -24, 37, 1, ksPParamCoord, nullptr);
        newSurface->EndPointsSeries();

        // 2-й ряд точек
        newSurface->BeginPointsSeries(true, -1);
        newSurface->AddPoint(56, -52, -4, 1, ksPParamCoord, nullptr);
        newSurface->AddPoint(28, -21, 7, 1, ksPParamCoord, nullptr);
        newSurface->AddPoint(1, -11, 10, 1, ksPParamCoord, nullptr);
        newSurface->EndPointsSeries();
        newSurface->Update();
      }

      // Редактируем
      if (YesNoMessage(LoadStr(IDS_EDIT)))
      {
        // Получаем поверхность по сети точек из коллекции
        if (ksapi::IMeshPointsSurfacePtr surface = meshCol->GetMeshPointsSurface(0))
        {
          std::vector<double> coords;
          coords.reserve(9);
          coords.emplace_back(73);
          coords.emplace_back(-59);
          coords.emplace_back(-14);
          coords.emplace_back(56);
          coords.emplace_back(-41);
          coords.emplace_back(-15);
          coords.emplace_back(30);
          coords.emplace_back(-18);
          coords.emplace_back(-13);

          std::vector<double> weights;
          weights.reserve(3);
          weights.emplace_back(1);
          weights.emplace_back(3);
          weights.emplace_back(3);

          // Добавляем ряд точек
          surface->AddPointsSeries(true, -1, coords, weights);
          surface->Update();
        }
      }
    }
  }
}


//-----------------------------------------------------------------------------
/**
  Создание и редактирование операции скругления кривых
  \param doc - указатель на текущий документ
  \return
*/
//---
void CreateAndEditFilletCurve(ksapi::IKompasDocumentPtr & doc)
{
  // Получаем контейнер вспомогательной геометрии
  if (ksapi::IAuxiliaryGeomContainerPtr auxCont = GetAuxiliaryGeomContainer(doc))
  {
    // Получаем коллекцию операций скругления кривых
    if (ksapi::IFilletCurvesPtr filletCol = auxCont->GetFilletCurves())
    {
      // Добавляем новую операцию скругления
      if (ksapi::IFilletCurvePtr newFilletCurve = filletCol->Add())
      {
        // Создаем кривые
        ksapi::IPolyLinePtr polyLine1 = CreatePolyLine(doc, 36, -37, 55, 36, -37, 1);
        ksapi::IPolyLinePtr polyLine2 = CreatePolyLine(doc, 36, -37, 1, -5, -37, 1);

        if (polyLine1 && polyLine2)
        {
          // Задаем кривые
          newFilletCurve->SetCurve1(polyLine1);
          newFilletCurve->SetCurve2(polyLine2);
          // Радиус дуги скругления
          newFilletCurve->SetRadius(10);
          newFilletCurve->Update();
        }
      }
      // Редактируем
      if (YesNoMessage(LoadStr(IDS_EDIT)))
      {
        // Получаем операцию скругления из коллекции
        if (ksapi::IFilletCurvePtr filletCurve = filletCol->GetFilletCurve(0))
        {
          // Направление дуги скругления - обратное
          filletCurve->SetDirection(false);
          // Радиус дуги скругления
          filletCurve->SetRadius(25);
          filletCurve->Update();
        }
      }
    }
  }
}

void * GetModule();

//------------------------------------------------------------------------------
// Загрузить строку из ресурса
// ---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring resStr;

  if (ksapi::ILibraryManagerPtr libMng = application ? application->GetLibraryManager() : nullptr)
  {
    if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Step4_KsAPI_3D"))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strID);
      }
    }
  }

  return resStr;
}
