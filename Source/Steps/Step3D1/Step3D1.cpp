////////////////////////////////////////////////////////////////////////////////
//
// Step3D1.cpp - Операции
//
// 1. Базовая операция выдавливания - CreateExtrusion
// 2. Операции вращения             - OperationRotated
// 3. Операции по сечениям          - OperationLoft
// 4. Поверхность по сечениям       - LoftSurface
// 5. Создание фаски и скругления   - CreateFilletAndChamfer
// 6. Операции : оболочка, уклон, сечение плоскостью, сечение эскизом"
//                                  - CreateNextOper
////////////////////////////////////////////////////////////////////////////////
#include <KsAPI.h>
#include <KompasLibraryActions.h>

using namespace ksapi;

static IApplication * kompasApp = nullptr;
constexpr auto libname = L"Step3D1";

// Ресурсы
#define IDS_COMBINEUNDO         100
#define IDS_DOC_ERROR           101
#define IDS_EXTRUSION           102
#define IDS_EXTRUSION_MODIFIED  103
#define IDS_SKETCH_MODIFIED     104
#define IDS_EXTRUSION_BOSS      105
#define IDS_EXTRUSION_CUT       106
#define IDS_FILLET              107
#define IDS_CHAMFER             108
#define IDS_ROTATED             109
#define IDS_ROTATED_BOSS        110
#define IDS_ROTATED_CUT         111
#define IDS_OFFSETPLANE         112
#define IDS_OFFSETPLANE2        113
#define IDS_OFFSETPLANE3        114
#define IDS_OFFSETPLANE4        115
#define IDS_LOFT                116
#define IDS_LOFT_BOSS           117
#define IDS_LOFT_CUT            118
#define IDS_HANDLE              119
#define IDS_HANDGUARD           120
#define IDS_WORKINGSURFACE      121
#define IDS_LOFTSURFACE         122
#define IDS_EXTRUSION_MADE      123
#define IDS_SHELL_MAKE          124
#define IDS_SHELL_MODIFY        125
#define IDS_INCLINE_MAKE        126
#define IDS_INCLINE_MODIFY      127
#define IDS_CUTBYSURFACE_MAKE   128
#define IDS_CUTBYSURFACE_MODIFY 129
#define IDS_CUTBYSKETCH_MAKE    130

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(libname);

  STRING_ID(IDS_COMBINEUNDO)
  STRING_ID(IDS_DOC_ERROR)
  STRING_ID(IDS_EXTRUSION)
  STRING_ID(IDS_EXTRUSION_MODIFIED)
  STRING_ID(IDS_SKETCH_MODIFIED)
  STRING_ID(IDS_EXTRUSION_BOSS)
  STRING_ID(IDS_EXTRUSION_CUT)
  STRING_ID(IDS_FILLET)
  STRING_ID(IDS_CHAMFER)
  STRING_ID(IDS_ROTATED)
  STRING_ID(IDS_ROTATED_BOSS)
  STRING_ID(IDS_ROTATED_CUT)
  STRING_ID(IDS_OFFSETPLANE)
  STRING_ID(IDS_OFFSETPLANE2)
  STRING_ID(IDS_OFFSETPLANE3)
  STRING_ID(IDS_OFFSETPLANE4)
  STRING_ID(IDS_LOFT)
  STRING_ID(IDS_LOFT_BOSS)
  STRING_ID(IDS_LOFT_CUT)
  STRING_ID(IDS_HANDLE)
  STRING_ID(IDS_HANDGUARD)
  STRING_ID(IDS_WORKINGSURFACE)
  STRING_ID(IDS_LOFTSURFACE)
  STRING_ID(IDS_EXTRUSION_MADE)
  STRING_ID(IDS_SHELL_MAKE)
  STRING_ID(IDS_SHELL_MODIFY)
  STRING_ID(IDS_INCLINE_MAKE)
  STRING_ID(IDS_INCLINE_MODIFY)
  STRING_ID(IDS_CUTBYSURFACE_MAKE)
  STRING_ID(IDS_CUTBYSURFACE_MODIFY)
  STRING_ID(IDS_CUTBYSKETCH_MAKE)

  resourceModule->End();
}


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
void CreateExtrusion(IPartPtr & part, IKompasDocument3DPtr & doc3D);
void OperationRotated(IPartPtr & part);
void OperationLoft(IPartPtr & part);
void LoftSurface(IPartPtr & part);
void CreateFilletAndChamfer(IPartPtr & part);
void CreateNextOper(IPartPtr & part);


//------------------------------------------------------------------------------
/**
  Обработчик команд библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksRunCommandModeEnum mode)
{
  if (kompasApp)
  {
    IKompasDocument3DPtr doc3D = kompasApp->GetActiveDocument(); // Получить указатель на активный документ трехмерной модели
    if (doc3D /*&& doc3D->GetDocumentType() == ksDocumentPart*/)
    {
      doc3D->SetTreeNeedRebuild(false);
      bool addUndoContainer = false;
      if (kompasApp->ShowMessageBox(LoadResourceString(IDS_COMBINEUNDO), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
          ksButtonYes)
      {
        doc3D->SetUndoContainer(true);
        addUndoContainer = true;
      }

      // Получить указатель на интерфейс компонента в соответствии с заданным типом ( pNew_Part - новая деталь,
      // pEdit_Part - редактируемая деталь, pTop_Part - верхний компонент ) либо номером компонента
      IPartPtr part = doc3D->GetTopPart();
      if (part)
      {
        switch (commandId)
        {
          case 1:
            CreateExtrusion(part, doc3D);
            break; // Базовая операция выдавливания
          case 2:
            OperationRotated(part);
            break; // Операции вращения
          case 3:
            OperationLoft(part);
            break; // Операции по сечениям
          case 4:
            LoftSurface(part);
            break; // Поверхность по сечениям
          case 5:
            CreateFilletAndChamfer(part);
            break; // Создание фаски и скругления
          case 6:
            CreateNextOper(part);
            break; // Операции : оболочка, уклон, сечение плоскостью, сечение эскизом
        }
      }

      doc3D->SetTreeNeedRebuild(true);
      if (doc3D && addUndoContainer)
        doc3D->SetUndoContainer(false);
    }
    else
    {
      kompasApp->ShowMessageBox(LoadResourceString(IDS_DOC_ERROR), L"", ksMessageWarning, ksButtonSetOk, false);
    }
    IKompasErrorPtr error = kompasApp->GetKompasError();
    if (error->GetCode() == etError10) // "Ошибка! Вырожденный объект"
      error->Clear();                  // Обнулить ошибки
  }
}


//-------------------------------------------------------------------------------
// Удалить все объекты из текущего эскиза
// ---
void ClearCurrentSketch()
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  std::vector<IDrawingObjectPtr> objects = drawCont->GetObjects({ksAllObj});
  for (IDrawingObjectPtr & obj : objects)
  {
    if (obj)
      obj->Delete();
  }
}


//-------------------------------------------------------------------------------
//  Создать объект "дуга" по центру и углам.
// ---
void ArcByAngle(double xc, double yc, // центр дуги
                double radius,        // радиус дуги
                double a1, double a2, // начальный и конечный угол
                bool direction,       // направление отрисовки дуги
                int32_t style)        // стиль отрисовки дуги
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  IArcPtr arc = drawCont->GetArcs()->Add();
  arc->SetXc(xc);
  arc->SetYc(yc);
  arc->SetRadius(radius);
  arc->SetAngle1(a1);
  arc->SetAngle2(a2);
  arc->SetDirection(direction);
  arc->SetStyle(style);
  arc->Update();
}


//-------------------------------------------------------------------------------
//  Создать объект "дуга" по центру и углам.
// ---
void ArcBy3Points(double x1, double y1,           // координаты начальной точки на дуге
                  double xMiddle, double yMiddle, // координаты средней точки на дуге
                  double x2, double y2,           // координаты конечной точки на дуге
                  int32_t style)                  // стиль отрисовки дуги
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  IArcPtr arc = drawCont->GetArcs()->Add();
  arc->SetX1(x1);
  arc->SetY1(y1);
  arc->SetX2(x2);
  arc->SetY2(y2);
  arc->SetX3(xMiddle);
  arc->SetY3(yMiddle);
  arc->SetStyle(style);
  arc->Update();
}


//-------------------------------------------------------------------------------
//  Создать объект "отрезок".
// ---
void LineSeg(double x1, double y1, // первая точка отрезка
             double x2, double y2, // вторая точка отрезка
             int32_t style)        // стиль отрисовки отрезка
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  ILineSegmentPtr lineSegment = drawCont->GetLineSegments()->Add();
  lineSegment->SetX1(x1);
  lineSegment->SetY1(y1);
  lineSegment->SetX2(x2);
  lineSegment->SetY2(y2);
  lineSegment->SetStyle(style);
  lineSegment->Update();
}


//-------------------------------------------------------------------------------
//  Создать объект "окружность".
// ---
void Circle(double xc, double yc, // центр окружности
            double radius,        // радиус окружности
            int32_t style)        // стиль отрисовки окружности
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  ICirclePtr circle = drawCont->GetCircles()->Add();
  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(radius);
  circle->SetStyle(style);
  circle->Update();
}


//-------------------------------------------------------------------------------
//  Создать объект "прямоугольник".
// ---
void Rectangle(double x, double y,          // координаты базовой точки прямоугольника - одной из его вершин
               double height, double width, // высота и ширина прямоугольника
               int32_t style)               // стиль отрисовки прямоугольника
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  IDrawingContainerPtr drawCont = doc2D->GetViewsAndLayersManager()->GetViews()->GetActiveView();
  IRectanglePtr rectangle = drawCont->GetRectangles()->Add();
  rectangle->SetX(x);
  rectangle->SetY(y);
  rectangle->SetHeight(height);
  rectangle->SetWidth(width);
  rectangle->SetStyle(style);
  rectangle->Update();
}


//-------------------------------------------------------------------------------
// Оперция выдавливания, работа с экизом
// ---
void CreateExtrusion(IPartPtr & part, IKompasDocument3DPtr & doc3D)
{
  IModelContainerPtr modelCont = part;
  // Создадим новый эскиз
  ISketchsPtr sketchs = modelCont->GetSketchs();
  ISketchPtr sketch = sketchs->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
    sketch->SetAngle(45.0);      // Угол поворота эскиза

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз - квадрат
      LineSeg(50.0, 50.0, -50.0, 50.0, 1);
      LineSeg(50.0, -50.0, -50.0, -50.0, 1);
      LineSeg(50.0, -50.0, 50.0, 50.0, 1);
      LineSeg(-50.0, -50.0, -50.0, 50.0, 1);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    // Оперция выдавливания
    IExtrusionsPtr extrusions = modelCont->GetExtrusions();
    IExtrusionPtr extrusion = extrusions->Add(o3d_bossExtrusion);
    if (extrusion)
    {
      // Установка параметров операции выдавливания
      extrusion->SetDirection(dtNormal); // Направление выдавливания ( dtNormal  - прямое
                                         // направление, для тонкой стенки - наружу,
                                         // dtReverse  - обратное направление, для тонкой стенки - внутрь
                                         // dtBoth - в обе стороны, dtMiddlePlane от средней плоскости )
                                         // Изменить параметры выдавливания в одном направлении

      extrusion->SetSideParameters(true,    // Направление выдавливания ( true - прямое направление,
                                            // false - обратное направление )
                                   etBlind, // Тип выдавливания ( etBlind - строго на глубину,
                                            // etThroughAll - через всю деталь, etUpToVertexTo - на расстояние до вершины,
                                            // etUpToVertexFrom - на расстояние за вершину, etUpToSurfaceTo - на
                                            // расстояние до поверхности, etUpToSurfaceFrom - на расстояние за поверхность,
                                            // etUpToNearSurface  - до ближайшей поверхности )
                                   200.0,   // Глубина выдавливания
                                   0.0,     // Угол уклона
                                   false,   // Направление уклона ( true - уклон наружу, false - уклон внутрь )
                                   nullptr);

      // Изменить параметры тонкой стенки
      IThinParametersPtr thinParams = extrusion;
      if (thinParams)
        thinParams->SetThinParameters(true,   // Признак тонкостенной операции
                                      dtBoth, // Направление построения тонкой стенки
                                      10.0,   // Толщина стенки в прямом направлении
                                      10.0);  // Толщина стенки в обратном направлении
      extrusion->SetProfiles({sketch});       // Эскиз операции выдавливания

      // Создать операцию выдавливания
      extrusion->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_EXTRUSION), L"", ksMessageWarning, ksButtonSetOk, true);

      // Изменим параметры операции выдавливания
      extrusion->SetDirection(dtBoth); // Направление выдавливания ( dtNormal  - прямое
                                       // направление, для тонкой стенки - наружу,
                                       // dtReverse  - обратное направление, для тонкой стенки - внутрь
                                       // dtBoth - в обе стороны, dtMiddlePlane от средней плоскости )
      // Изменить параметры выдавливания в одном направлении
      extrusion->SetSideParameters(false,   // Направление выдавливания ( true - прямое направление,
                                            // false - обратное направление )
                                   etBlind, // Тип выдавливания ( etBlind - строго на глубину,
                                            // etThroughAll - через всю деталь, etUpToVertexTo - на расстояние до вершины,
                                            // etUpToVertexFrom - на расстояние за вершину, etUpToSurfaceTo - на
                                            // расстояние до поверхности, etUpToSurfaceFrom - на расстояние за поверхность,
                                            // etUpToNearSurface  - до ближайшей поверхности )
                                   150.0,   // Глубина выдавливания
                                   0.0,     // Угол уклона
                                   false,   // Направление уклона ( true - уклон наружу, false - уклон внутрь )
                                   nullptr);

      // Обновить параметры
      extrusion->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_EXTRUSION_MODIFIED), L"", ksMessageWarning, ksButtonSetOk, true);

      // Войти в режим редактирования эскиза
      if (sketch->BeginEdit(false))
      {
        // Удалить все объекты из текущего эскиза
        ClearCurrentSketch();
        // Введем в эскиз окружность
        Circle(0, 0, 100, 1);
        // Выйти из режима редактирования эскиза
        sketch->EndEdit();
      }

      // Обновить параметры эскиза
      sketch->Update();
      // Обновить параметры операции выдавливания
      extrusion->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_SKETCH_MODIFIED), L"", ksMessageWarning, ksButtonSetOk, true);

      // Создадим новый эскиз
      ISketchPtr sketch2 = sketchs->Add();
      if (sketch2)
      {
        // Установка параметров эскиза
        sketch2->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
        sketch2->SetAngle(45.0);      // Повернем эскиз на 45 град.

        // Войти в режим редактирования эскиза
        if (sketch2->BeginEdit(false))
        {
          // Введем новый эскиз - окружность
          Circle(0, 0, 150, 1);
          // Выйти из режима редактирования эскиза
          sketch2->EndEdit();
        }

        // Приклеим выдавливанием
        IExtrusionPtr bossExtrusion = extrusions->Add(o3d_bossExtrusion);
        IThinParametersPtr thinParam = bossExtrusion; // Интерфейс структуры параметров тонкой стенки
        if (bossExtrusion && thinParam)
        {
          // Установка параметров элемента выдавливания
          bossExtrusion->SetProfiles({sketch2}); // Эскиз операции выдавливания

          bossExtrusion->SetDirection(dtNormal);          // Направление выдавливания ( dtNormal - прямое
                                                          // направление, для тонкой стенки - наружу,
                                                          // dtReverse - обратное направление, для тонкой стенки - внутрь
                                                          // dtBoth - в обе стороны, dtMiddlePlane от средней плоскости )
          bossExtrusion->SetExtrusionType(true, etBlind); // Тип выдавливания ( etBlind - строго на глубину,
                                                          // etThroughAll - через всю деталь, etUpToVertexTo - на расстояние до вершины,
                                                          // etUpToVertexFrom - на расстояние за вершину, etUpToSurfaceTo - на расстояние
                                                          // до поверхности, etUpToSurfaceFrom - на расстояние за поверхность,
                                                          // etUpToNearSurface - до ближайшей поверхности )
          bossExtrusion->SetDepth(true, 100.0);           // Глубина выдавливания
          thinParam->SetThin(false);                      // Признак формирования тонкой стенки элемента ( true  - тонкая стенка,
                                                          // формируется, false - тонкая стенка не формируется )
          bossExtrusion->Update();                        // Приклеим выдавливанием
          kompasApp->ShowMessageBox(LoadResourceString(IDS_EXTRUSION_BOSS), L"", ksMessageWarning, ksButtonSetOk, true);
        }

        // Создадим новый эскиз
        ISketchPtr sketch3 = sketchs->Add();
        if (sketch3)
        {
          // Установка параметров эскиза
          sketch3->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
          sketch3->SetAngle(45.0);      // Повернем эскиз на 45 град.

          // Войти в режим редактирования эскиза
          if (sketch3->BeginEdit(false))
          {
            // Введем новый эскиз - квадрат
            LineSeg(50, 50, -50, 50, 1);
            LineSeg(50, -50, -50, -50, 1);
            LineSeg(50, -50, 50, 50, 1);
            LineSeg(-50, -50, -50, 50, 1);

            // Выйти из режима редактирования эскиза
            sketch3->EndEdit();
          }

          // Вырежем выдавливанием
          IExtrusionPtr cutExtrusion = extrusions->Add(o3d_cutExtrusion);
          if (cutExtrusion)
          {
            // Установка параметров операции выдавливания
            cutExtrusion->SetDirection(dtReverse);   // Направление выдавливания ( dtNormal  - прямое
                                                     // направление, для тонкой стенки - наружу,
                                                     // dtReverse - обратное направление, для тонкой стенки - внутрь
                                                     // dtBoth - в обе стороны, dtMiddlePlane от средней плоскости )
                                                     // Изменить параметры выдавливания в одном направлении
            cutExtrusion->SetSideParameters(false,   // Направление выдавливания ( true - прямое направление,
                                                     // false - обратное направление )
                                            etBlind, // Тип выдавливания ( etBlind - строго на глубину,
                                                     // etThroughAll - через всю деталь, etUpToVertexTo - на расстояние до вершины,
                                                     // etUpToVertexFrom - на расстояние за вершину, etUpToSurfaceTo - на
                                                     // расстояние до поверхности, etUpToSurfaceFrom - на расстояние за поверхность,
                                                     // etUpToNearSurface - до ближайшей поверхности )
                                            50.0,    // Глубина выдавливания
                                            0.0,     // Угол уклона
                                            false,   // Направление уклона ( true - уклон наружу, false - уклон внутрь )
                                            nullptr);
            // Изменить параметры тонкой стенки
            IThinParametersPtr thinParams = cutExtrusion;
            if (thinParams)
              thinParams->SetThinParameters(false,    // Признак тонкостенной операции
                                            dtNormal, // Направление построения тонкой стенки
                                            0.0,      // Толщина стенки в прямом направлении
                                            0.0);     // Толщина стенки в обратном направлении
            cutExtrusion->SetProfiles({sketch3});     // Эскиз операции выдавливания

            // Вырежем выдавливанием
            cutExtrusion->Update();
            kompasApp->ShowMessageBox(LoadResourceString(IDS_EXTRUSION_CUT), L"", ksMessageWarning, ksButtonSetOk, true);
          }
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Cоздание фаски и скругления
// ---
void CreateFilletAndChamfer(IPartPtr & part)
{
  IModelContainerPtr modelCont = part;
  // Создадим новый эскиз
  ISketchPtr sketch = modelCont->GetSketchs()->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
    sketch->SetAngle(45.0);      // Угол поворота эскиза

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз - квадрат
      LineSeg(50.0, 50.0, -50.0, 50.0, 1);
      LineSeg(50.0, -50.0, -50.0, -50.0, 1);
      LineSeg(50.0, -50.0, 50.0, 50.0, 1);
      LineSeg(-50.0, -50.0, -50.0, 50.0, 1);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    // Оперция выдавливания
    IExtrusionPtr extrusion = modelCont->GetExtrusions()->Add(o3d_bossExtrusion);
    if (extrusion)
    {
      // Установка параметров операции выдавливания
      extrusion->SetDirection(dtNormal); // Направление выдавливания ( dtNormal - прямое
                                         // направление, для тонкой стенки - наружу,
                                         // dtReverse - обратное направление, для тонкой стенки - внутрь
                                         // dtBoth - в обе стороны, dtMiddlePlane от средней плоскости )
      // Изменить параметры выдавливания в одном направлении
      extrusion->SetSideParameters(true,    // Направление выдавливания ( true - прямое направление,
                                            // false - обратное направление )
                                   etBlind, // Тип выдавливания ( etBlind - строго на глубину,
                                            // etThroughAll - через всю деталь, etUpToVertexTo - на расстояние до вершины,
                                            // etUpToVertexFrom - на расстояние за вершину, etUpToSurfaceTo - на
                                            // расстояние до поверхности, etUpToSurfaceFrom - на расстояние за поверхность,
                                            // etUpToNearSurface - до ближайшей поверхности )
                                   100.0,   // Глубина выдавливания
                                   0.0,     // Угол уклона
                                   false,   // Направление уклона ( true - уклон наружу, false - уклон внутрь )
                                   nullptr);
      // Изменить параметры тонкой стенки
      IThinParametersPtr thinParams = extrusion;
      if (thinParams)
        thinParams->SetThinParameters(false,    // Признак тонкостенной операции
                                      dtNormal, // Направление построения тонкой стенки
                                      0.0,      // Толщина стенки в прямом направлении
                                      0.0);     // Толщина стенки в обратном направлении
      extrusion->SetProfiles({sketch});         // Эскиз операции выдавливания

      // Создать операцию выдавливания
      extrusion->Update();

      // Формирует массив объектов и возвращает указатель на его интерфейс - массив граней компонента
      IFindObject3DParametersPtr findParams = kompasApp->GetActiveDocument()->GetInterface(ksObjectFindObject3DParameters);
      findParams->SetModelObjectType(o3d_face);
      std::vector<IModelObjectPtr> faces =
        part->FindObjectsByPointWithParam(0.0, 0.0, 0.0, true, -1.0, findParams); // Выбор по точке
      if (faces.size())
      {
        // Скругление
        IFilletPtr fillet = modelCont->GetFillets()->Add();
        if (fillet)
        {
          // Установка параметров скругления
          fillet->SetRadius1(10.0);  // Радиус скругления
          fillet->SetTangent(false); // Продолжить по касательной
          fillet->SetBaseObjects(faces);

          // Создать операцию скругления
          fillet->Update();
          kompasApp->ShowMessageBox(LoadResourceString(IDS_FILLET), L"", ksMessageWarning, ksButtonSetOk, true);
        }
      }

      // Формирует массив объектов и возвращает указатель на его интерфейс - массив граней компонента
      std::vector<IModelObjectPtr> faces2 =
        part->FindObjectsByPointWithParam(0.0, 0.0, 100.0, true, -1.0, findParams); // Выбор по точке
      if (faces2.size())
      {
        // Фаска
        IChamferPtr chamfer = modelCont->GetChamfers()->Add();
        if (chamfer)
        {
          // Установка параметров фаски
          chamfer->SetTangent(false);  // Продолжить по касательной
          chamfer->SetDirection(true); // Признак направления фаски
          chamfer->SetDistance1(10.0); // Размер первого катета фаски
          chamfer->SetDistance2(10.0); // Размер второго катета фаски
          chamfer->SetBaseObjects(faces2);

          // Создание фаски
          chamfer->Update();
          kompasApp->ShowMessageBox(LoadResourceString(IDS_CHAMFER), L"", ksMessageWarning, ksButtonSetOk, true);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Операции вращения
// ---
void OperationRotated(IPartPtr & part)
{
  IModelContainerPtr modelCont = part;
  // Создадим новый эскиз
  ISketchsPtr sketchs = modelCont->GetSketchs();
  ISketchPtr sketch = sketchs->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз
      ArcByAngle(0.0, 0.0, 20.0, -90.0, 90.0, false, 1);
      LineSeg(0.0, -20.0, 0.0, 20.0, 3);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    // Операции вращения
    IRotatedPtr rotated = modelCont->GetRotateds()->Add(o3d_bossRotated);
    if (rotated)
    {
      rotated->SetToroidShapeType(false); // Признак тороида ( true - тороид, false - сфероид )
      rotated->SetDirection(dtBoth);      // Направление вращения ( dtNormal - прямое направление, для тонкой стенки - наружу
                                          // dtReverse - обратное направление, для тонкой стенки - внутрь, dtBoth - в обе стороны,
                                          // dtMiddlePlane - от средней плоскости )
      // Изменить параметры тонкой стенки
      IThinParametersPtr thinParams = rotated;
      if (thinParams)
        thinParams->SetThinParameters(true,   // Признак тонкостенной операции
                                      dtBoth, // Направление построения тонкой стенки
                                      1.0,    // Толщина стенки в прямом направлении
                                      1.0);   // Толщина стенки в обратном направлении
      // Изменить параметры выдавливания в одном направлении
      rotated->SetAngle(true,   // Направление вращения ( true - прямое, false - обратное )
                        180.0); // Угол вращения
      // Изменить параметры выдавливания в одном направлении
      rotated->SetAngle(false,     // Направление вращения ( true - прямое, false - обратное )
                        180.0);    // Угол вращения
      rotated->SetProfile(sketch); // Эскиз операции выдавливания

      // Создать операцию
      rotated->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_ROTATED), L"", ksMessageWarning, ksButtonSetOk, true);
    }
  }

  // Создадим новый эскиз
  ISketchPtr sketch2 = sketchs->Add();
  if (sketch2)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch2->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch2->BeginEdit(false))
    {
      // Введем новый эскиз
      ArcByAngle(15.0, 0.0, 10.0, -90.0, 90.0, false, 1);
      LineSeg(15.0, -10.0, 15.0, 10.0, 3);

      // Выйти из режима редактирования эскиза
      sketch2->EndEdit();
    }

    // Операция приклеивания вращением
    IRotatedPtr rotated2 = modelCont->GetRotateds()->Add(o3d_bossRotated);
    if (rotated2)
    {
      rotated2->SetToroidShapeType(false); // Признак тороида ( true - тороид, false - сфероид )
      rotated2->SetDirection(dtNormal);    // Направление вращения ( dtNormal - прямое направление, для тонкой стенки - наружу
                                           // dtReverse - обратное направление, для тонкой стенки - внутрь, dtBoth - в обе стороны,
                                           // dtMiddlePlane - от средней плоскости )
      // Изменить параметры выдавливания в одном направлении
      rotated2->SetAngle(true,   // Направление вращения ( true - прямое, false - обратное )
                         360.0); // Угол вращения
      // Изменить параметры тонкой стенки
      IThinParametersPtr thinParams2 = rotated2;
      if (thinParams2)
        thinParams2->SetThinParameters(true,   // Признак тонкостенной операции
                                       dtBoth, // Направление построения тонкой стенки
                                       1.0,    // Толщина стенки в прямом направлении
                                       1.0);   // Толщина стенки в обратном направлении
      rotated2->SetProfile(sketch2);           // Эскиз операции выдавливания

      // Операция приклеивания вращением
      rotated2->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_ROTATED_BOSS), L"", ksMessageWarning, ksButtonSetOk, true);
    }
  }

  // Создадим новый эскиз
  ISketchPtr sketch3 = sketchs->Add();
  if (sketch3)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch3->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch3->BeginEdit(false))
    {
      // Введем новый эскиз
      ArcByAngle(20.0, 0.0, 20.0, 90.0, 270.0, false, 1);
      LineSeg(20.0, -20.0, 20.0, 20.0, 3);

      // Выйти из режима редактирования эскиза
      sketch3->EndEdit();
    }

    // Операция вырезания вращением
    IRotatedPtr cutRotated = modelCont->GetRotateds()->Add(o3d_cutRotated);
    if (cutRotated)
    {
      cutRotated->SetToroidShapeType(false); // Признак тороида ( true - тороид, false - сфероид )
      cutRotated->SetDirection(dtNormal);    // Направление вращения ( dtNormal - прямое направление, для тонкой стенки - наружу
                                             // dtReverse - обратное направление, для тонкой стенки - внутрь, dtBoth - в обе стороны,
                                             // dtMiddlePlane - от средней плоскости )
      // Изменить параметры выдавливания в одном направлении
      cutRotated->SetAngle(true,  // Направление вращения ( true - прямое, false - обратное )
                           90.0); // Угол вращения
      // Изменить параметры тонкой стенки
      IThinParametersPtr cutThinParams = cutRotated;
      if (cutThinParams)
        cutThinParams->SetThinParameters(true,   // Признак тонкостенной операции
                                         dtBoth, // Направление построения тонкой стенки
                                         5.0,    // Толщина стенки в прямом направлении
                                         7.0);   // Толщина стенки в обратном направлении
      cutRotated->SetProfile(sketch3);           // Эскиз операции выдавливания

      // Операция вырезания вращением
      cutRotated->Update();
      kompasApp->ShowMessageBox(LoadResourceString(IDS_ROTATED_CUT), L"", ksMessageWarning, ksButtonSetOk, true);
    }
  }
}


//-------------------------------------------------------------------------------
// Операции по сечениям
// ---
void OperationLoft(IPartPtr & part)
{
  IModelContainerPtr modelCont = part;
  // Создадим новый эскиз
  ISketchsPtr sketchs = modelCont->GetSketchs();
  ISketchPtr sketch = sketchs->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    // Установка параметров эскиза
    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза

    // Создадим эскиз
    sketch->SetHidden(false);

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 4.5, 1);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }
  }

  IAuxiliaryGeomContainerPtr auxGeomCont = part;
  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlanes3DPtr planes = auxGeomCont->GetPlanes3D();
  IPlane3DByOffsetPtr offsetPlane = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch2 = sketchs->Add();
  if (offsetPlane && sketch2)
  {
    // Установка параметров смещенной плоскости
    offsetPlane->SetOffset(30); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane->SetBasePlane(basePlane);                      // Установим плоскость XOY базовой для плоскости
    offsetPlane->SetName(LoadResourceString(IDS_OFFSETPLANE)); // Имя для смещенной плоскости
    offsetPlane->SetHidden(true);                              // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane->Update();

    // Установка параметров эскиза
    sketch2->SetPlane(offsetPlane); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch2->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 8.0, 1);

      // Выйти из режима редактирования эскиза
      sketch2->EndEdit();
    }
  }

  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlane3DByOffsetPtr offsetPlane2 = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch3 = sketchs->Add();
  if (offsetPlane2 && sketch3)
  {
    // Установка параметров смещенной плоскости
    offsetPlane2->SetOffset(60.0); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane2->SetBasePlane(basePlane);                       // Установим плоскость XOY базовой для плоскости
    offsetPlane2->SetName(LoadResourceString(IDS_OFFSETPLANE2)); // Имя для смещенной плоскости
    offsetPlane2->SetHidden(true);                               // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane2->Update();

    // Установка параметров эскиза
    sketch3->SetPlane(offsetPlane2); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch3->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 1.5, 1);

      // Выйти из режима редактирования эскиза
      sketch3->EndEdit();
    }
  }

  // Создадим базовую операцию по сечениям
  ILoftsPtr lofts = modelCont->GetLofts();
  ILoftPtr loft = lofts->Add(o3d_bossLoft);
  if (loft)
  {
    loft->SetProfiles({sketch, sketch2, sketch3});
    loft->SetName(LoadResourceString(IDS_HANDLE)); // Имя для операции по сечениям

    // Изменить параметры цвета и визуальных свойств объекта
    IColorParamPtr colorParam = loft;
    if (colorParam)
      colorParam->SetAdvancedColor(12345678, // Цвет
                                   0.8,      // Общий свет
                                   0.8,      // Диффузия
                                   0.8,      // Зеркальность
                                   0.8,      // Блеск
                                   1.0,      // Прозрачность
                                   0.8);     // Излучение

    // Cоздать операцию по сечениям
    loft->Update();
    kompasApp->ShowMessageBox(LoadResourceString(IDS_LOFT), L"", ksMessageWarning, ksButtonSetOk, true);
  }

  // Создадим эскиз в уже созданной смещенной плоскости
  ISketchPtr sketch4 = sketchs->Add();
  if (sketch4)
  {
    // Установка параметров эскиза
    sketch4->SetPlane(offsetPlane2); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch4->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 1.5, 1);

      // Выйти из режима редактирования эскиза
      sketch4->EndEdit();
    }
  }

  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlane3DByOffsetPtr offsetPlane3 = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch5 = sketchs->Add();
  if (offsetPlane3 && sketch5)
  {
    // Установка параметров смещенной плоскости
    offsetPlane3->SetOffset(120.0); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane3->SetBasePlane(basePlane);                       // Установим плоскость XOY базовой для плоскости
    offsetPlane3->SetName(LoadResourceString(IDS_OFFSETPLANE3)); // Имя для смещенной плоскости
    offsetPlane3->SetHidden(true);                               // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane3->Update();

    // Установка параметров эскиза
    sketch5->SetPlane(offsetPlane3); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch5->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 1.8, 1);

      // Выйти из режима редактирования эскиза
      sketch5->EndEdit();
    }
  }

  // Создадим операцию приклеивания по сечениям
  ILoftPtr loft2 = lofts->Add(o3d_bossLoft);
  if (loft2)
  {
    loft2->SetProfiles({sketch4, sketch5});
    loft2->SetName(LoadResourceString(IDS_HANDGUARD)); // Имя для операции по сечениям
    // Изменить параметры цвета и визуальных свойств объекта
    IColorParamPtr colorParam2 = loft2;
    if (colorParam2)
      colorParam2->SetAdvancedColor(1234567890, // Цвет
                                    0.8,        // Общий свет
                                    0.8,        // Диффузия
                                    0.8,        // Зеркальность
                                    0.8,        // Блеск
                                    1.0,        // Прозрачность
                                    0.8);       // Излучение

    // Cоздать операцию приклеивания по сечениям
    loft2->Update();
    kompasApp->ShowMessageBox(LoadResourceString(IDS_LOFT_BOSS), L"", ksMessageWarning, ksButtonSetOk, true);
  }

  // Создадим эскиз в уже созданной смещенной плоскости
  ISketchPtr sketch6 = sketchs->Add();
  if (sketch6)
  {
    // Установка параметров эскиза
    sketch6->SetPlane(offsetPlane3); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch6->BeginEdit(false))
    {
      // Введем новый эскиз - прямоугольник
      Rectangle(-1.8, -0.4, 0.8, 3.6, 1);

      // Выйти из режима редактирования эскиза
      sketch6->EndEdit();
    }
  }

  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlane3DByOffsetPtr offsetPlane4 = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch7 = sketchs->Add();
  if (offsetPlane4 && sketch7)
  {
    // Установка параметров смещенной плоскости
    offsetPlane4->SetOffset(110.0); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane4->SetBasePlane(basePlane);                       // Установим плоскость XOY базовой для плоскости
    offsetPlane4->SetName(LoadResourceString(IDS_OFFSETPLANE4)); // Имя для смещенной плоскости
    offsetPlane4->SetHidden(true);                               // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane4->Update();

    // Установка параметров эскиза
    sketch7->SetPlane(offsetPlane4); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch7->BeginEdit(false))
    {
      // Введем новый эскиз - прямоугольник
      Rectangle(-1.8, -1.8, 3.6, 3.6, 1);

      // Выйти из режима редактирования эскиза
      sketch7->EndEdit();
    }
  }

  // Создадим операцию вырезания по сечениям
  ILoftPtr cutLoft = lofts->Add(o3d_cutLoft);
  if (cutLoft)
  {
    cutLoft->SetProfiles({sketch6, sketch7});

    // Изменить параметры тонкой стенки
    IThinParametersPtr thinParams = cutLoft;
    if (thinParams)
      thinParams->SetThinParameters(true,                     // Признак тонкостенной операции
                                    dtNormal,                 // Направление построения тонкой стенки
                                    3.0,                      // Толщина стенки в прямом направлении
                                    0.0);                     // Толщина стенки в обратном направлении
    cutLoft->SetName(LoadResourceString(IDS_WORKINGSURFACE)); // Имя для операции по сечениям

    // Изменить параметры цвета и визуальных свойств объекта
    IColorParamPtr cutColorParam = cutLoft;
    if (cutColorParam)
      cutColorParam->SetAdvancedColor(1234, // Цвет
                                      0.8,  // Общий свет
                                      0.8,  // Диффузия
                                      0.8,  // Зеркальность
                                      0.8,  // Блеск
                                      1.0,  // Прозрачность
                                      0.8); // Излучение

    // Cоздать операцию вырезания по сечениям
    cutLoft->Update();
    kompasApp->ShowMessageBox(LoadResourceString(IDS_LOFT_CUT), L"", ksMessageWarning, ksButtonSetOk, true);
  }
}


//-------------------------------------------------------------------------------
// Операции по сечениям
// ---
void LoftSurface(IPartPtr & part)
{
  IModelContainerPtr modelCont = part;
  // Создадим новый эскиз
  ISketchsPtr sketchs = modelCont->GetSketchs();
  ISketchPtr sketch = sketchs->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
    sketch->SetHidden(false);

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 4.5, 1);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }
  }

  IAuxiliaryGeomContainerPtr auxGeomCont = part;
  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlanes3DPtr planes = auxGeomCont->GetPlanes3D();
  IPlane3DByOffsetPtr offsetPlane = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch2 = sketchs->Add();
  if (offsetPlane && sketch2)
  {
    // Установка параметров смещенной плоскости
    offsetPlane->SetOffset(30.0); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane->SetBasePlane(basePlane);                      // Установим плоскость XOY базовой для плоскости
    offsetPlane->SetName(LoadResourceString(IDS_OFFSETPLANE)); // Имя для смещенной плоскости
    offsetPlane->SetHidden(true);                              // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane->Update();

    // Установка параметров эскиза
    sketch2->SetPlane(offsetPlane); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch2->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 8.0, 1);

      // Выйти из режима редактирования эскиза
      sketch2->EndEdit();
    }
  }

  // Создадим смещенную плоскость ( для создания на ней эскиза )
  IPlane3DByOffsetPtr offsetPlane2 = planes->Add(o3d_planeOffset);
  // Создадим новый эскиз
  ISketchPtr sketch3 = sketchs->Add();
  if (offsetPlane2 && sketch3)
  {
    // Установка параметров смещенной плоскости
    offsetPlane2->SetOffset(60.0); // Расстояние от базовой плоскости

    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);

    offsetPlane2->SetBasePlane(basePlane);                       // Установим плоскость XOY базовой для плоскости
    offsetPlane2->SetName(LoadResourceString(IDS_OFFSETPLANE2)); // Имя для смещенной плоскости
    offsetPlane2->SetHidden(true);                               // Скрыть смещенную плоскость

    // Создать смещенную плоскость
    offsetPlane2->Update();

    // Установка параметров эскиза
    sketch3->SetPlane(offsetPlane2); // Установим смещенную плоскость базовой для эскиза

    // Войти в режим редактирования эскиза
    if (sketch3->BeginEdit(false))
    {
      // Введем новый эскиз - окружность
      Circle(0.0, 0.0, 1.5, 1);

      // Выйти из режима редактирования эскиза
      sketch3->EndEdit();
    }
  }

  ISurfaceContainerPtr surfCont = part;
  // Создадим базовую операцию по сечениям
  ILoftPtr loftSurface = surfCont->GetLoftSurfaces()->Add(o3d_LoftSurface);
  if (loftSurface)
  {
    loftSurface->SetProfiles({sketch, sketch2, sketch3});
    loftSurface->SetName(LoadResourceString(IDS_HANDLE)); // Имя для операции по сечениям

    // Изменить параметры цвета и визуальных свойств объекта
    IColorParamPtr colorParam = loftSurface;
    if (colorParam)
      colorParam->SetAdvancedColor(12345678, // Цвет
                                   0.8,      // Общий свет
                                   0.8,      // Диффузия
                                   0.8,      // Зеркальность
                                   0.8,      // Блеск
                                   1.0,      // Прозрачность
                                   0.8);     // Излучение

    // Cоздать операцию по сечениям
    loftSurface->Update();
    kompasApp->ShowMessageBox(LoadResourceString(IDS_LOFTSURFACE), L"", ksMessageWarning, ksButtonSetOk, true);
  }
}


//-----------------------------------------------------------------------------------
// Операции : оболочка, уклон, сечение плоскостью, сечение эскизом
//---
void CreateNextOper(IPartPtr & part)
{
  IModelContainerPtr modelCont = part;
  // Создадим эскиз для базовой операции
  ISketchsPtr sketchs = modelCont->GetSketchs();
  ISketchPtr sketch = sketchs->Add();
  if (sketch)
  {
    // Получим интерфейс базовой плоскости XOY
    IModelObjectPtr basePlane = part->GetDefaultObject(o3d_planeXOY);
    sketch->SetPlane(basePlane); // Установим плоскость XOY базовой для эскиза
    sketch->SetAngle(0);         // Угол поворота эскиза

    // Войти в режим редактирования эскиза
    if (sketch->BeginEdit(false))
    {
      // Введем новый эскиз - квадрат
      LineSeg(50, 50, -50, 50, 1);
      LineSeg(50, -50, -50, -50, 1);
      LineSeg(50, -50, 50, 50, 1);
      LineSeg(-50, -50, -50, 50, 1);

      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    kompasApp->ShowMessageBox(LoadResourceString(IDS_EXTRUSION_MADE), L"", ksMessageWarning, ksButtonSetOk, true);

    // Создаем операцию выдавливания
    IExtrusionPtr extrusion = modelCont->GetExtrusions()->Add(o3d_bossExtrusion);
    if (extrusion)
    {
      extrusion->SetDirection(dtNormal); // направление выдавливания
      extrusion->SetSideParameters(true /*прямое направление*/, etBlind /*строго на глубину*/, 200.0, 0.0, false, nullptr);
      IThinParametersPtr thinParams = extrusion;
      if (thinParams)
        thinParams->SetThinParameters(true, dtBoth, 10.0, 10.0); // тонкая стенка в два направления
      extrusion->SetProfiles({sketch});                          // эскиз операции выдавливания
      extrusion->Update();                                       // создать операцию

      bool update = false; // если update = true, то параметры операции изменены
      if (kompasApp->ShowMessageBox(LoadResourceString(IDS_SHELL_MAKE), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
          ksButtonYes)
      {
        // Создаем операцию оболочка
        IShellPtr shell = modelCont->GetShells()->Add();
        if (shell)
        {
          IFindObject3DParametersPtr findParams = kompasApp->GetActiveDocument()->GetInterface(ksObjectFindObject3DParameters);
          findParams->SetModelObjectType(o3d_face);
          shell->SetThickness(8.0);      // толщиина оболочки
          shell->SetThinType(dtReverse); // направление оболочки внутрь
          std::vector<IModelObjectPtr> faces =
            part->FindObjectsByPointWithParam(50.0, 0.0, 0.0, true, -1.0, findParams); // выбор поверхности по точке
          shell->SetDeletedFaces({faces[0]}, false);                                   // добавим в массив граней для операции грань с индексом = 0
          shell->Update();                                                             // создать операцию

          if (kompasApp->ShowMessageBox(LoadResourceString(IDS_SHELL_MODIFY), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
              ksButtonYes)
          {
            shell->SetThickness(25.0);    // толщиина оболочки
            shell->SetThinType(dtNormal); // направление оболочки наружу
            std::vector<IModelObjectPtr> faces2 =
              part->FindObjectsByPointWithParam(60.0, 0.0, 10.0, true, -1.0, findParams); // Выбор поверхности по точке
            shell->SetDeletedFaces({faces[0], faces2[0]}, false);                         // добавим в массив граней для операции грань с индексом = 0
            shell->Update();                                                              // перестроим операцию
            update = true;
          }
        }
      }

      if (kompasApp->ShowMessageBox(LoadResourceString(IDS_INCLINE_MAKE), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
          ksButtonYes)
      {
        // создадим операцию уклон
        IInclinePtr incline = modelCont->GetInclines()->Add();
        if (incline)
        {
          IFindObject3DParametersPtr findParams = kompasApp->GetActiveDocument()->GetInterface(ksObjectFindObject3DParameters);
          findParams->SetModelObjectType(o3d_face);

          incline->SetDirection(true);  // Направление уклона - наружу
          incline->SetAngle(3.0);       // Угол уклона
          incline->SetPlane(basePlane); // базовая плоскость
          std::vector<IModelObjectPtr> faces = part->FindObjectsByPointWithParam(0.0, update ? 85.0 : 60.0, 10.0, true, -1.0,
                                                                                 findParams); // Выбор поверхности по точке
          incline->SetFaces(faces);                                                           // добавим в массив граней для операции грань с индексом = 0
          incline->Update();                                                                  // создать операцию

          if (kompasApp->ShowMessageBox(LoadResourceString(IDS_INCLINE_MODIFY), L"", ksMessageQuestion, ksButtonSetYesNo,
                                        false) == ksButtonYes)
          {
            incline->SetDirection(false); // Направление уклона - внутрь
            incline->SetAngle(25.0);      // Угол уклона
            std::vector<IModelObjectPtr> faces2 = part->FindObjectsByPointWithParam(0.0, update ? -85.0 : -60.0, 10.0, true, -1.0,
                                                                                    findParams); // Выбор поверхности по точке
            incline->SetFaces({faces[0], faces2[0]});                                            // добавим в массив граней для операции грань с индексом = 0
            incline->Update();
          }
        }
      }

      if (kompasApp->ShowMessageBox(LoadResourceString(IDS_CUTBYSURFACE_MAKE), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
          ksButtonYes)
      {
        // создадим операцию сечение плоскостью
        ICutPtr cut = modelCont->GetCuts()->Add();
        if (cut)
        {
          IModelObjectPtr basePlaneXOZ = part->GetDefaultObject(o3d_planeXOZ);
          cut->SetCutObject(basePlaneXOZ); // задаем плоскость сечения
          cut->SetDirection(false);        // направление сечения - обратное
          cut->Update();                   // создать операцию

          if (kompasApp->ShowMessageBox(LoadResourceString(IDS_CUTBYSURFACE_MODIFY), L"", ksMessageQuestion, ksButtonSetYesNo,
                                        false) == ksButtonYes)
          {
            cut->SetDirection(true); // направление сечения - прямое
            cut->Update();           // перестроить операцию
          }
        }
      }

      if (kompasApp->ShowMessageBox(LoadResourceString(IDS_CUTBYSKETCH_MAKE), L"", ksMessageQuestion, ksButtonSetYesNo, false) ==
          ksButtonYes)
      {
        // создадим эскиз для операции сечение эскизом
        ISketchPtr sketch2 = sketchs->Add();

        // получим интерфейс базовой плоскости YOZ
        IModelObjectPtr basePlaneYOZ = part->GetDefaultObject(o3d_planeYOZ);

        sketch2->SetPlane(basePlaneYOZ); // установим плоскость yoz базовой для эскиза
        sketch2->SetAngle(45.0);         // угол поворота эскиза

        // Войти в режим редактирования эскиза
        if (sketch2->BeginEdit(false))
        {
          // Введем новый эскиз
          ArcBy3Points(-200.0, 45.0, -150.0, 10.0, -50.0, 10.0, 1);
          LineSeg(-200.0, 45.0, -300.0, 20.0, 1);
          LineSeg(-50.0, 10.0, 60.0, 10.0, 1);

          // Выйти из режима редактирования эскиза
          sketch2->EndEdit();
        }

        // создадим Операцию сечение эскизом
        ICutPtr cut2 = modelCont->GetCuts()->Add();
        if (cut2)
        {
          cut2->SetCutObject(sketch2);
          cut2->SetDirection(true); // задаем направление сечения
          cut2->Update();           // создать операцию
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
APP_EXP_FUNC(bool) LoadKompasLibrary(IApplication & app, IKompasLibraryActions & libaryActions)
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
