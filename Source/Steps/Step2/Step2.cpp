////////////////////////////////////////////////////////////////////////////////
//
// Step2.cpp - Математика
//
// 1. Пересечь прямые                      - Intersect2Line
// 2. Пересечь кривые                      - Intersect2Curve
// 3. Пересечь отрезок и дугу              - IntersectLineSegArc
// 4. Касательная из точки                 - TanLinePointCircle
// 5. Касательная под углом                - TanLineAngCircle
// 6. Поворот точки                        - RotatePoint
// 7. Симметрия точки                      - SymmetryPoint
// 8. Сопрягающие окружности к двум прямым - Couplin2Lines
// 9. Перепендикуляр                       - Perpendicular
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KsAPI.h>
#include <KompasLibraryActions.h>

static ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
/// Пересечь прямые.
void Intersect2Line(ksapi::IDrawingContainer & drawingContainer);
/// Пересечь кривые.
void Intersect2Curve(ksapi::IDrawingContainer & drawingContainer);
/// Пересечь отрезок и дугу.
void IntersectLineSegArc(ksapi::IDrawingContainer & drawingContainer);
/// Касательная из точки.
void TanLinePointCircle(ksapi::IDrawingContainer & drawingContainer);
/// Касательная под углом.
void TanLineAngCircle(ksapi::IDrawingContainer & drawingContainer);
/// Поворот точки.
void RotatePoint(ksapi::IDrawingContainer & drawingContainer);
/// Симметрия точки.
void SymmetryPoint(ksapi::IDrawingContainer & drawingContainer);
/// Сопрягающие окружности к двум прямым.
void Couplin2Lines(ksapi::IDrawingContainer & drawingContainer);
/// Перепендикуляр.
void Perpendicular(ksapi::IDrawingContainer & drawingContainer);

//-------------------------------------------------------------------------------
// Описания вспомогательных функций.
// ---
/// Создать линию по точке и углу и вернуть математическую кривую для неё.
ksapi::ICurve2DPtr CreateLine(ksapi::IDrawingContainer & drawingContainer, double x, double y, double angle);
/// Создать линию по двум точкам и вернуть математическую кривую для неё.
ksapi::ICurve2DPtr CreateLine(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2);
/// Создать отрезок по двум точкам и вернуть математическую кривую для неё.
ksapi::ICurve2DPtr CreateLineSegment(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2);
/// Создать точку.
ksapi::IPointPtr CreatePoint(ksapi::IDrawingContainer & drawingContainer, double x, double y,
                             ksAnnotationSymbolEnum style = ksDotPoint);
/// Создать кривую Безье и вернуть математическую кривую для неё.
ksapi::ICurve2DPtr CreateBezier(ksapi::IDrawingContainer & drawingContainer, const std::vector<double> & points);
/// Создать дугу и вернуть математическую кривую для неё.
ksapi::ICurve2DPtr CreateArc(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double r, double x1, double y1,
                             double x2, double y2, bool dir);
/// Создать окружность по координатам центра и радиусу и вернуть математическую кривую для него.
ksapi::ICurve2DPtr CreateCircle(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double r,
                                ksCurveStyleEnum style = ksCSNormal);
/// Создать окружность по координатам центра и точки на окружности.
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double x, double y,
                               ksCurveStyleEnum style = ksCSNormal);
/// Создать набор точек и вывести на экран их параметры.
void CreateAndPrintPoints(ksapi::IDrawingContainer & drawingContainer, const std::vector<double> & rawPoints,
                          ksAnnotationSymbolEnum style = ksDotPoint);

//------------------------------------------------------------------------------
/**
  Обработчик команд библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  if (!kompasApp)
    return;

  ksapi::IKompasDocument2DPtr document = kompasApp->GetActiveDocument();
  if (!document)
  {
    kompasApp->ShowMessageBox(L"Документ не активизирован или\nне является листом/фрагментом", L"", ksMessageWarning,
                              ksButtonSetOk, true);
    return;
  }

  ksapi::IViewsAndLayersManagerPtr layersMngr = document->GetViewsAndLayersManager();
  if (!layersMngr)
    return;

  ksapi::IViewsPtr views = layersMngr->GetViews();
  if (!views)
    return;

  ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView();
  if (!drawingContainer)
    return;

  switch (commandId)
  {
    case 1:
      Intersect2Line(*drawingContainer);
      break; // Пересечь прямые
    case 2:
      Intersect2Curve(*drawingContainer);
      break; // Пересечь кривые
    case 3:
      IntersectLineSegArc(*drawingContainer);
      break; // Пересечь отрезок и дугу
    case 4:
      TanLinePointCircle(*drawingContainer);
      break; // Касательная из точки
    case 5:
      TanLineAngCircle(*drawingContainer);
      break; // Касательная под углом
    case 6:
      RotatePoint(*drawingContainer);
      break; // Поворот точки
    case 7:
      SymmetryPoint(*drawingContainer);
      break; // Симметрия точки
    case 8:
      Couplin2Lines(*drawingContainer);
      break; // Сопрягающие окружности к двум прямым
    case 9:
      Perpendicular(*drawingContainer);
      break; // Перепендикуляр
  }
}

//------------------------------------------------------------------------------
/**
  Пересечь прямые.
*/
//---
void Intersect2Line(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание линий и получение интерфейса математических кривых для них.
  ksapi::ICurve2DPtr lineCrv1 = CreateLine(drawingContainer, /*x1*/ 10, /*y1*/ 10, /*angle*/ 0);
  ksapi::ICurve2DPtr lineCrv2 = CreateLine(drawingContainer, /*x1*/ 15, /*y1*/ 5, /*angle*/ 90);
  if (!lineCrv1 || !lineCrv2)
    return;

  // Поиск точек пересечения для математических кривых.
  std::vector<double> intersectionPoints = lineCrv1->Intersect(lineCrv2);

  // Отрисовка найденых точек.
  CreateAndPrintPoints(drawingContainer, intersectionPoints);
}


//-------------------------------------------------------------------------------
/**
  Пересечь кривые.
*/
//---
void Intersect2Curve(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание кривых и получение интерфейса математических кривых для них.
  ksapi::ICurve2DPtr beizer1 = CreateBezier(drawingContainer, {/*x1*/ 0, /*y1*/ 10, /*x2*/ 10, /*y2*/ 20, /*x3*/ 20, /*y3*/ 10,
                                                               /*x4*/ 10, /*y4*/ 0, /*x5*/ 0, /*y5*/ 10});
  ksapi::ICurve2DPtr beizer2 = CreateBezier(drawingContainer, {/*x1*/ 0, /*y1*/ 20, /*x2*/ 10, /*y2*/ 30, /*x3*/ 20, /*y3*/ 20,
                                                               /*x4*/ 10, /*y4*/ 10, /*x5*/ 0, /*y5*/ 20});
  if (!beizer1 || !beizer2)
    return;

  // Поиск точек пересечения для математических кривых.
  std::vector<double> intersectionPoints = beizer1->Intersect(beizer2);

  // Отрисовка найденых точек.
  CreateAndPrintPoints(drawingContainer, intersectionPoints);
}


//-------------------------------------------------------------------------------
/**
  Пересечь отрезок и дугу.
*/
//---
void IntersectLineSegArc(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание отрезка и дуги и получение интерфейса математических кривых для них.
  ksapi::ICurve2DPtr line = CreateLineSegment(drawingContainer, /*x1*/ 0, /*y1*/ 40, /*x2*/ 100, /*y2*/ 40);
  ksapi::ICurve2DPtr arc =
    CreateArc(drawingContainer, /*xc*/ 50, /*yc*/ 40, /*r*/ 20, /*x1*/ 30, /*y1*/ 40, /*x2*/ 70, /*y2*/ 40, /*dir*/ false);
  if (!line || !arc)
    return;

  // Поиск точек пересечения для математических кривых.
  std::vector<double> intersectionPoints = line->Intersect(arc);

  // Отрисовка найденых точек.
  CreateAndPrintPoints(drawingContainer, intersectionPoints);
}


//-------------------------------------------------------------------------------
/**
  Касательная из точки.
*/
//---
void TanLinePointCircle(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание точки и окружности и получение интерфейса математической кривой для окружности.
  ksapi::IPointPtr point = CreatePoint(drawingContainer, /*x*/ 10, /*y*/ 50);
  ksapi::ICurve2DPtr circle = CreateCircle(drawingContainer, /*xc*/ 50, /*yc*/ 10, /*r*/ 40);
  if (!point || !circle)
    return;

  // Получение координат касания для созданной окружности и прямой из созданной точки.
  std::vector<double> tanLinePoints = circle->TangentLinePoint(point->GetX(), point->GetY());

  // Отрисовка касательных линий по двум точкам.
  for (size_t i = 1; i < tanLinePoints.size(); i += 2)
    CreateLine(drawingContainer, /*x*/ 10, /*y*/ 50, tanLinePoints[i - 1], tanLinePoints[i]);

  // Отрисовка точек касания и вывод на экран их координат
  CreateAndPrintPoints(drawingContainer, tanLinePoints);
}


//-------------------------------------------------------------------------------
/**
  Касательная под углом.
*/
//---
void TanLineAngCircle(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание отрезка.
  CreateLineSegment(drawingContainer, /*x1*/ 0, /*y1*/ 40, /*x2*/ 100, /*y2*/ 40);

  // Создание окружности и получение интерфейса её математической кривой.
  ksapi::ICurve2DPtr circle = CreateCircle(drawingContainer, /*x*/ 50, /*y*/ 10, /*r*/ 40);
  if (!circle)
    return;

  double x = 0;
  double y = 0;
  circle->PointOn(/*t*/ (circle->GetParamMax() - circle->GetParamMin()) * 7 / 8, x, y);

  ksapi::ICurve2DPtr line = CreateLine(drawingContainer, x, y, /*angle*/ 45);
  if (!line)
    return;

  CreateAndPrintPoints(drawingContainer, {x, y});
}


//-------------------------------------------------------------------------------
/**
  Поворот точки.
*/
//---
void RotatePoint(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание точек, point1 будем вращать, point2 - центр вращения.
  ksapi::IPointPtr point1 = CreatePoint(drawingContainer, /*x*/ 60, /*y*/ 50);
  ksapi::IPointPtr point2 = CreatePoint(drawingContainer, /*x*/ 50, /*y*/ 50, /*style*/ ksPlusPoint);

  // Для вращения точки относительно центра получаем интерфейс математики.
  ksapi::IMath2DPtr math = kompasApp->GetMath2D();
  if (!math || !point1 || !point2)
    return;

  // Вращаемая точка.
  double resX = point1->GetX();
  double resY = point2->GetY();

  // Результат вращения будет записан в исходные переменные.
  if (math->Rotate(resX, resY, point2->GetX(), point2->GetY(), /*angle*/ 180))
    // Создать точку-результат.
    CreateAndPrintPoints(drawingContainer, {resX, resY}, ksXPoint);
}


//-------------------------------------------------------------------------------
/**
  Симметрия точки.
*/
//---
void SymmetryPoint(ksapi::IDrawingContainer & drawingContainer)
{
  // Координаты. по ним создадим точку, и их же и будем передавать в метод вращения.
  double ptX = 55;
  double ptY = 60;

  // Создание исходной точки.
  ksapi::IPointPtr point = CreatePoint(drawingContainer, /*x*/ ptX, /*y*/ ptY);

  // Интерфейс математики - через него расчёт симметричной точки.
  ksapi::IMath2DPtr math = kompasApp->GetMath2D();
  if (!math || !point)
    return;

  // Создать линию - ось вращения.
  ksapi::ICurve2DPtr line = CreateLineSegment(drawingContainer, /*x1*/ 60, /*y1*/ 50, /*x2*/ 50, /*y2*/ 50);
  if (!line)
    return;

  // Вычисление координат симметричной точки и её создание.
  if (math->Symmetry(ptX, ptY, line))
    CreateAndPrintPoints(drawingContainer, {ptX, ptY}, ksPlusPoint);
}


//-------------------------------------------------------------------------------
/**
  Сопрягающие окружности к двум прямым.
*/
//---
void Couplin2Lines(ksapi::IDrawingContainer & drawingContainer)
{
  // Создание линий - для которых будем вычислять сопрягающие окружности.
  ksapi::ICurve2DPtr line1 = CreateLine(drawingContainer, /*x*/ 100, /*y*/ 100, /*angle*/ 45);
  ksapi::ICurve2DPtr line2 = CreateLine(drawingContainer, /*x*/ 100, /*y*/ 100, /*angle*/ -45);

  // Массив, полученный в результате вычисления сопрягающих окружностей, включает в себя наборы по 6 double значений,
  // идущих друг за другом. Xci, Yci - координаты центра, X1i, Y1i и X2i Y2i - координаты точек сопряжения
  const double radius = 20;                                              // Радиус сопрягаемых окружностей
  std::vector<double> points = line1->CouplingCurveCurve(line2, radius); // Координаты центров и точек касания
  const size_t oneCouplingCoordsCnt = 6;                                 // Количество элементов массива на одно сопряжение
  size_t couplingsCount = points.size() / oneCouplingCoordsCnt;          // Количество сопряжений
  if (couplingsCount != 4)
    return;

  // Перебираем полученные координаты и строим по ним сопрягающие окружности и точки касания.
  int32_t style = ksDotPoint; // Стиль точек касания
  for (size_t i = 0; i + oneCouplingCoordsCnt <= points.size(); i += oneCouplingCoordsCnt)
  {
    CreateCircle(drawingContainer, /*xc*/ points[i], /*yc*/ points[i + 1], /*x*/ points[i + 2], /*y*/ points[i + 3], ksCSThin);
    CreatePoint(drawingContainer, points[i + 2], points[i + 3], static_cast<ksAnnotationSymbolEnum>(style));
    CreatePoint(drawingContainer, points[i + 4], points[i + 5], static_cast<ksAnnotationSymbolEnum>(style++));
  }

  // Вывод сообщения на экран.
  const std::wstring msg =
    std::format(L"count = {}, con[0].x1 = {:4.2f}, con[0].y1 = {:4.2f},\ncon[0].x2 = {:4.2f}, con[0].y2 = {:4.2f} ...",
                couplingsCount, points[2], points[3], points[4], points[5]);
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
}


//-------------------------------------------------------------------------------
/**
  Перепендикуляр.
*/
//---
void Perpendicular(ksapi::IDrawingContainer & drawingContainer)
{
  // Исходная точка для построения перпендикуляра.
  ksapi::IPointPtr point = CreatePoint(drawingContainer, /*x*/ 50, /*y*/ 50, ksDotPoint);

  // Создание отрезка - на который строится перпендикуляр.
  double x1 = 60;
  double y1 = 10;
  double x2 = 100;
  double y2 = 10;
  CreateLineSegment(drawingContainer, x1, y1, x2, y2);

  // Создание линии, на которой лежит созданный отрезок, через интерфейс математики - объект не будет добавлен в чертёж.
  ksapi::IMath2DPtr math = kompasApp->GetMath2D();
  if (!point || !math)
    return;
  double angle = math->GetLineAngle(x1, y1, x2, y2);
  double xn, yn, t;
  ksapi::ICurve2DPtr line = math->Line(x1, y1, angle);

  // Вычисление точки - проекции на продолжение отрезка
  if (!line || !line->PointProjection(point->GetX(), point->GetY(), xn, yn, t, angle))
    return;

  // Отрисовка точки проекции.
  CreateAndPrintPoints(drawingContainer, /*rawpoints*/ {xn, yn}, ksPlusPoint);
}


//-------------------------------------------------------------------------------
/**
  Создать линию по точке и углу и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateLine(ksapi::IDrawingContainer & drawingContainer, double x, double y, double angle)
{
  ksapi::ILinesPtr lines = drawingContainer.GetLines();
  if (!lines)
    return nullptr;
  ksapi::ILinePtr line = lines->Add();
  if (!line)
    return nullptr;

  line->SetX1(x);
  line->SetY1(y);
  line->SetAngle(angle);

  line->Update();

  return line->GetCurve2D();
}


//-------------------------------------------------------------------------------
/**
  Создать линию по двум точкам и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateLine(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2)
{
  ksapi::ILinesPtr lines = drawingContainer.GetLines();
  if (!lines)
    return nullptr;
  ksapi::ILinePtr line = lines->Add();
  if (!line)
    return nullptr;
  line->SetX1(x1);
  line->SetY1(y1);
  line->SetX2(x2);
  line->SetY2(y2);
  line->Update();

  return line->GetCurve2D();
}

//-------------------------------------------------------------------------------
/**
  Создать отрезок по двум точкам и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateLineSegment(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2)
{
  ksapi::ILineSegmentsPtr lineSegs = drawingContainer.GetLineSegments();
  if (!lineSegs)
    return nullptr;
  ksapi::ILineSegmentPtr lineSeg = lineSegs->Add();
  if (!lineSeg)
    return nullptr;
  lineSeg->SetX1(x1);
  lineSeg->SetY1(y1);
  lineSeg->SetX2(x2);
  lineSeg->SetY2(y2);
  lineSeg->Update();

  return lineSeg->GetCurve2D();
}


//-------------------------------------------------------------------------------
/**
  Создать точку.
*/
//---
ksapi::IPointPtr CreatePoint(ksapi::IDrawingContainer & drawingContainer, double x, double y, ksAnnotationSymbolEnum style)
{
  ksapi::IPointsPtr pointsContainer = drawingContainer.GetPoints();
  if (!pointsContainer)
    return nullptr;
  ksapi::IPointPtr point = pointsContainer->Add();
  if (!point)
    return nullptr;
  point->SetX(x);
  point->SetY(y);
  point->SetStyle(style);
  point->Update();

  return point;
}

//-------------------------------------------------------------------------------
/**
  Создать кривую Безье и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateBezier(ksapi::IDrawingContainer & drawingContainer, const std::vector<double> & points)
{
  ksapi::IBeziersPtr beizersContainer = drawingContainer.GetBeziers();
  if (!beizersContainer)
    return nullptr;
  ksapi::IBezierPtr bezier = beizersContainer->Add();
  if (!bezier)
    return nullptr;

  bezier->SetPoints(/*allPoints=*/false, points);
  bezier->SetStyle(ksCSThin);
  bezier->Update();

  return bezier->GetCurve2D();
}

//-------------------------------------------------------------------------------
/**
  Создать дугу и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateArc(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double r, double x1, double y1,
                             double x2, double y2, bool dir)
{
  ksapi::IArcsPtr arcsContainer = drawingContainer.GetArcs();
  if (!arcsContainer)
    return nullptr;
  ksapi::IArcPtr arc = arcsContainer->Add();
  if (!arc)
    return nullptr;

  arc->SetXc(xc);
  arc->SetYc(yc);
  arc->SetRadius(r);
  arc->SetX1(x1);
  arc->SetY1(y1);
  arc->SetX2(x2);
  arc->SetY2(y2);
  arc->SetDirection(dir);
  arc->Update();

  return arc->GetCurve2D();
}

//-------------------------------------------------------------------------------
/**
  Создать окружность по координатам центра и радиусу и вернуть математическую кривую для неё.
*/
//---
ksapi::ICurve2DPtr CreateCircle(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double r,
                                ksCurveStyleEnum style)
{
  ksapi::ICirclesPtr circlesContainer = drawingContainer.GetCircles();
  if (!circlesContainer)
    return nullptr;
  ksapi::ICirclePtr circle = circlesContainer->Add();
  if (!circle)
    return nullptr;


  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(r);
  circle->SetStyle(style);
  circle->Update();

  return circle->GetCurve2D();
}

//-------------------------------------------------------------------------------
/**
  Создать окружность по координатам центра и радиусу.
*/
//---
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainer & drawingContainer, double xc, double yc, double x, double y,
                               ksCurveStyleEnum style)
{
  ksapi::ICirclesPtr circlesContainer = drawingContainer.GetCircles();
  if (!circlesContainer)
    return nullptr;
  ksapi::ICirclePtr circle = circlesContainer->Add();
  if (!circle)
    return nullptr;

  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetX(x);
  circle->SetY(y);
  circle->SetStyle(style);
  circle->Update();

  return circle;
}

//-------------------------------------------------------------------------------
/**
  Создать набор точек и вывести на экран их параметры.
*/
//---
void CreateAndPrintPoints(ksapi::IDrawingContainer & drawingContainer, const std::vector<double> & rawPoints,
                          ksAnnotationSymbolEnum style)
{
  if (rawPoints.size() < 2)
    return;

  const size_t pointsCnt = rawPoints.size() / 2;
  std::wstring msg = std::format(L"count = {}", pointsCnt);
  for (size_t i = 0; i < pointsCnt; ++i)
  {
    ksapi::IPointPtr point = CreatePoint(drawingContainer, rawPoints[i * 2], rawPoints[i * 2 + 1], style);
    if (!point)
      return;
    msg += std::format(L", x[{}] = {:4.2f}, y[{}] = {:4.2f}", i, point->GetX(), i, point->GetY());
  }

  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
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
