////////////////////////////////////////////////////////////////////////////////
//
// Step3.cpp - Объекты
//
// 1.  Создать документ - WorkDocument
// 2.  Виды             - DrawView
// 3.  Слои             - DrawLayer
// 4.  Группы           - DrawGroup
// 5.  Именная группа   - WorkNameGroup
// 6.  Отрезки          - DrawLineSeg
// 7.  Дуги             - DrawArc
// 8.  Линии            - DrawLine
// 9.  Окружности       - DrawCircle
// 10. Точки            - DrawPoint
// 11. Bezier-сплайны   - DrawBezier
// 12. Штриховка        - DrawHatch
// 13. Текст            - DrawText
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KsAPI.h>
#include <KompasLibraryActions.h>

ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void WorkDocument();
void DrawView(ksapi::IKompasDocument2D & document);
void DrawLayer(ksapi::IKompasDocument2D & document);
void DrawGroup(ksapi::IKompasDocument2D & document);
void WorkNameGroup(ksapi::IKompasDocument2D & document);
void DrawLineSeg(ksapi::IKompasDocument2D & document);
void DrawArc(ksapi::IKompasDocument2D & document);
void DrawLine(ksapi::IKompasDocument2D & document);
void DrawCircle(ksapi::IKompasDocument2D & document);
void DrawPoint(ksapi::IKompasDocument2D & document);
void DrawBezier(ksapi::IKompasDocument2D & document);
void DrawHatch(ksapi::IKompasDocument2D & document);
void DrawText(ksapi::IKompasDocument2D & document);

/// Получить активный вид.
ksapi::IDrawingContainerPtr GetActiveView(ksapi::IKompasDocument2D & document);

/// Создать отрезок по двум точкам и вернуть математическую кривую для неё.
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IDrawingContainerPtr drawingContainer, double x1, double y1, double x2, double y2,
                                         ksCurveStyleEnum style);

/// Создать окружность по координатам центра и радиусу.
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r,
                               ksCurveStyleEnum style = ksCSNormal);

/// Создать элемент текста.
ksapi::ITextItemPtr CreateTextItem(ksapi::ITextLine & textLine, ksTextItemEnum type, ksTextSizeEnum size,
                                   const std::wstring & text, bool italic, bool bold, bool newLine);

/// Вывод параметров строки.
void PrintTextLine(ksapi::ITextLinePtr textLine);


//------------------------------------------------------------------------------
/**
  Обработчик команд библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  if (!kompasApp)
    return;

  ksapi::IKompasDocument2DPtr document = nullptr;
  if (commandId != 1)
  {
    document = kompasApp->GetActiveDocument();
    if (!document)
    {
      kompasApp->ShowMessageBox(L"Документ не активизирован или\nне является листом/фрагментом", L"", ksMessageWarning,
                                ksButtonSetOk, true);
      return;
    }
  }

  switch (commandId)
  {
    case 1:
      WorkDocument();
      break;
    case 2:
      DrawView(*document);
      break;
    case 3:
      DrawLayer(*document);
      break;
    case 4:
      DrawGroup(*document);
      break;
    case 5:
      WorkNameGroup(*document);
      break;
    case 6:
      DrawLineSeg(*document);
      break;
    case 7:
      DrawArc(*document);
      break;
    case 8:
      DrawLine(*document);
      break;
    case 9:
      DrawCircle(*document);
      break;
    case 10:
      DrawPoint(*document);
      break;
    case 11:
      DrawBezier(*document);
      break;
    case 12:
      DrawHatch(*document);
      break;
    case 13:
      DrawText(*document);
      break;
  }
}

//-------------------------------------------------------------------------------
// Работа с документом. Создание и сохранение
// ---
void WorkDocument()
{
  // создание документа
  ksapi::IDocumentsPtr documentsArr = kompasApp->GetDocuments();
  if (!documentsArr)
    return;
  ksapi::IKompasDocument2DPtr document = documentsArr->Add(DocumentTypeEnum::ksDocumentDrawing, /*visible*/ true);
  if (!document)
    return;

  // определение параметров и сохранение
  document->SetComment(L"Create document");
  document->SetAuthor(L"User");
  document->SaveAs(L"1.cdw");

  // Параметры оформления документа
  ksapi::ILayoutSheetsPtr layoutSheets = document->GetLayoutSheets();
  if (!layoutSheets)
    return;
  ksapi::ILayoutSheetPtr layoutSheet = layoutSheets->GetItem(0);
  if (!layoutSheet)
    return;
  ksapi::ISheetFormatPtr sheetFormat = layoutSheet->GetFormat();
  if (!sheetFormat)
    return;
  sheetFormat->SetFormat(ksFormatA3);
  sheetFormat->SetFormatMultiplicity(1);
  sheetFormat->SetVerticalOrientation(true);
  layoutSheet->SetLayoutStyleNumber(1);
  layoutSheet->Update();

  // Создание вида с номером 2
  ksapi::IViewsAndLayersManagerPtr viewsAndLayerMngr = document->GetViewsAndLayersManager();
  if (!viewsAndLayerMngr)
    return;
  ksapi::IViewsPtr views = viewsAndLayerMngr->GetViews();
  if (!views)
    return;
  ksapi::IViewPtr view = views->Add(vt_Normal);
  if (!view)
    return;
  view->SetX(10);
  view->SetY(20);
  view->SetScale(0.5);
  view->SetAngle(45);
  view->SetColor(ksapi::Color{0x000A140A}); // цвет вида в активном состоянии.
  view->SetBackground(false);
  view->SetCurrent(true);
  view->SetName(L"User view");
  view->SetNumber(2);
  view->Update();

  // Слой с номером 5
  ksapi::ILayersPtr layers = view->GetLayers();
  if (!layers)
    return;
  ksapi::ILayerPtr layer = layers->Add();
  if (!layer)
    return;
  layer->Update();

  // Создание отрезков
  CreateLineSegment(view, /*x1*/ 20, /*y1*/ 10, /*x2*/ 40, /*y2*/ 10, ksCSNormal);
  CreateLineSegment(view, /*x1*/ 40, /*y1*/ 10, /*x2*/ 40, /*y2*/ 30, ksCSNormal);
  CreateLineSegment(view, /*x1*/ 40, /*y1*/ 30, /*x2*/ 20, /*y2*/ 30, ksCSNormal);
  CreateLineSegment(view, /*x1*/ 20, /*y1*/ 30, /*x2*/ 20, /*y2*/ 10, ksCSNormal);

  // Вывод параметров на экран
  const std::wstring msg = std::format(L"type = {}, sheet format = {},\nsheet multiply = {}, sheet direct = {},\n "
                                       "fileName = {},\ncomment = {},\nauthor = {}",
                                       static_cast<int32_t>(document->GetDocumentType()),
                                       static_cast<int32_t>(sheetFormat->GetFormat()), sheetFormat->GetFormatMultiplicity(),
                                       sheetFormat->IsVerticalOrientation() ? 0 : 1, document->GetName(), document->GetComment(),
                                       document->GetAuthor());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);

  // Закрыть документ
  document->Close(kdDoNotSaveChanges);
}


//------------------------------------------------------------------------------
/**
  Создать вид.
*/
//---
void DrawView(ksapi::IKompasDocument2D & document)
{
  if (document.GetDocumentType() != DocumentTypeEnum::ksDocumentDrawing)
    return;

  // Создание объектов и добавление их в группу
  ksapi::IViewPtr systemView = GetActiveView(document);
  if (!systemView)
    return;
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"");
  group->Open(); // Создаваемые объект между Open и Close попадают в группу.
  CreateLineSegment(systemView, /*x1*/ 20, /*y1*/ 10, /*x2*/ 20, /*y2*/ 30, ksCSNormal);
  CreateLineSegment(systemView, /*x1*/ 20, /*y1*/ 30, /*x2*/ 40, /*y2*/ 30, ksCSNormal);
  CreateLineSegment(systemView, /*x1*/ 40, /*y1*/ 30, /*x2*/ 40, /*y2*/ 10, ksCSNormal);
  CreateLineSegment(systemView, /*x1*/ 40, /*y1*/ 10, /*x2*/ 20, /*y2*/ 10, ksCSNormal);
  group->Close(); // Создаваемые объект между Open и Close попадают в группу.

  // Создание вида с номером 5
  ksapi::IViewsAndLayersManagerPtr viewsAndLayersMngr = document.GetViewsAndLayersManager();
  if (!viewsAndLayersMngr)
    return;
  ksapi::IViewsPtr views = viewsAndLayersMngr->GetViews();
  if (!views)
    return;
  ksapi::IViewPtr view5 = views->Add(LtViewType::vt_Normal);
  if (!view5)
    return;

  // определение параметров вида
  view5->SetName(L"User view");
  view5->SetX(10);                           // точка привязки вида
  view5->SetY(20);                           // точка привязки вида
  view5->SetScale(0.5);                      // Масштаб вида
  view5->SetAngle(45);                       // Угол поворота вида
  view5->SetColor(ksapi::Color{0x000A140A}); // Цвет вида в активном состоянии. 3 младших байта - RGB. R - младший
  view5->SetCurrent(true);                   // Состояние видa
  view5->SetVisible(true);
  view5->SetNumber(5);
  view5->Update();

  // Добавление вида в группу
  kompasApp->ShowMessageBox(L"Добавим вид в группу", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);
  group->AddObjects({view5});

  // Отчёт об ошибке на экран
  if (ksapi::IKompasErrorPtr error = kompasApp->GetKompasError())
    error->Report();

  // Создание отрезков в созданном виде
  ksapi::ILineSegmentPtr line = CreateLineSegment(view5, /*x1*/ 10, /*y1*/ 10, /*x2*/ 30, /*y2*/ 30, ksCSNormal);
  group->AddObjects({line});
  kompasApp->ShowMessageBox(L"добавили эл в группу", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);

  // Отчёт об ошибке на экран
  if (ksapi::IKompasErrorPtr error = kompasApp->GetKompasError())
    error->Report();


  // Поворот группы на 45 градусов
  document.RotateObjects({group}, /*x*/ 0, /*y*/ 0, /*angle*/ -45);

  kompasApp->ShowMessageBox(
    std::format(
      L"x = {:4.2f}, y = {:4.2f}, scale = {:4.2f}, ang = {:4.2f}, color = {:x}, current = {}, active = {}, name = {}, number = {}",
      view5->GetX(), view5->GetY(), view5->GetScale(), view5->GetAngle(), view5->GetColor(), view5->IsCurrent(),
      !view5->IsBackground(), view5->GetName(), view5->GetNumber()),
    L"", ksMessageWarning, ksButtonSetOk, true);

  // сделаем активным первый вид
  systemView->SetCurrent(true);
  systemView->Update();

  kompasApp->ShowMessageBox(L"Изменить состояние вида", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);
  view5->SetBackground(true);
  view5->SetCurrent(false);
  view5->Update();
}

//------------------------------------------------------------------------------
/**
  Поиск слоя по номеру, создание при необходимости. Определение параметров слоя
*/
//---
void DrawLayer(ksapi::IKompasDocument2D & document)
{
  // Чтение из поля ввода числа
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();

  if (!dialogs)
    return;

  int32_t layerNum = 1;
  bool readed = dialogs->ReadInt(/*windowHandle*/ nullptr, L"Введите номер создаваемого слоя", /*value*/ 1, /*minValue*/ 0,
                                 /*maxValue*/ 255,
                                 /*newValue*/ layerNum);
  if (!readed)
    return;

  // поиск существующего слоя и создание нового, если его нет
  ksapi::IViewPtr view = GetActiveView(document);
  if (!view)
    return;
  ksapi::ILayersPtr layers = view->GetLayers();
  if (!layers)
    return;
  ksapi::ILayerPtr layer = layers->GetLayerByNumber(layerNum);
  if (!layer)
  {
    if (layer = layers->Add()) // создание нового
    {
      layer->SetLayerNumber(layerNum);
      layer->SetCurrent();
    }
    else
      return;
  }
  layer->Update();

  // Создание отрезков и преобразование их с помощью матрицы.
  document.CreateMatrix(/*x*/ 20, /*y*/ 15, /*angle*/ 0, /*scaleX*/ 1, /*scaleY*/ 1);
  if (ksapi::ILineSegmentPtr line = CreateLineSegment(view, /*x1*/ -10, /*y1*/ 0, /*x2*/ 10, /*y2*/ 0, ksCSNormal))
    line->SetLayerNumber(layerNum);
  if (ksapi::ILineSegmentPtr line = CreateLineSegment(view, /*x1*/ 10, /*y1*/ 0, /*x2*/ 10, /*y2*/ 20, ksCSNormal))
    line->SetLayerNumber(layerNum);
  if (ksapi::ILineSegmentPtr line = CreateLineSegment(view, /*x1*/ 10, /*y1*/ 20, /*x2*/ -10, /*y2*/ 20, ksCSNormal))
    line->SetLayerNumber(layerNum);
  if (ksapi::ILineSegmentPtr line = CreateLineSegment(view, /*x1*/ -10, /*y1*/ 20, /*x2*/ -10, /*y2*/ 0, ksCSNormal))
    line->SetLayerNumber(layerNum);
  document.DeleteMatrix();

  // подсветить слой
  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
  if (chooseManager)
    chooseManager->Choose({layer});

  // параметры перед изменением
  kompasApp->ShowMessageBox(std::format(L"number = {},\ncurrent = {}, active = {}, color = {:x}, name = {}",
                                        layer->GetLayerNumber(), layer->IsCurrent(), !layer->IsBackground(), layer->GetColor(),
                                        layer->GetName()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  kompasApp->ShowMessageBox(L"Изменим параметры слоя", L"", ksMessageWarning, ksButtonSetOk, true);

  // Параметры текущего слоя изменить нельзя поэтому переключаемся на нулевой слой
  ksapi::ILayerPtr zeroLayer = layers->GetLayer(0);
  zeroLayer->SetCurrent();
  zeroLayer->Update();

  // определение параметров слоя
  layer->SetColor(ksapi::Color{0x0000FF00});
  layer->SetBackground(false);
  layer->SetName(L"Зеленый");
  layer->Update();

  kompasApp->ShowMessageBox(std::format(L"number = {},\ncurrent = {}, active = {}, color = {:x}, name = {}",
                                        layer->GetLayerNumber(), layer->IsCurrent(), !layer->IsBackground(), layer->GetColor(),
                                        layer->GetName()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  // Снимем выделение слоя.
  if (chooseManager)
    chooseManager->UnchooseAll();

  // Изменить состояние слоя ( активизировать слой )
  kompasApp->ShowMessageBox(L"Изменить состояние слоя", L"", ksMessageWarning, ksButtonSetOk, true);
  layer->SetCurrent();
  layer->Update();
}

//------------------------------------------------------------------------------
/**
  Работа с группой.
*/
//---
void DrawGroup(ksapi::IKompasDocument2D & document)
{
  // создание группы
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"");
  ksapi::IViewPtr view = GetActiveView(document);
  if (!group || !view)
    return;

  // Создание и добавление отрезка в группу между вызовами Open и Close
  group->Open();
  CreateLineSegment(view, /*x1*/ 10, /*y1*/ 10, /*x2*/ 10, /*y2*/ 20, ksCSThin);
  group->Close();

  // Перемещение/вращение группы
  document.MoveObjects({group}, /*dx*/ 10, /*dy*/ 0);
  kompasApp->ShowMessageBox(L"Cдвинули группу на 10 мм", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);

  document.RotateObjects({group}, /*x*/ 20, /*y*/ 10, 45);
  kompasApp->ShowMessageBox(L"Повернули группу на 45 гр", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);

  document.RotateObjects({group}, /*x*/ 20, /*y*/ 10, -45);
  kompasApp->ShowMessageBox(L"Повернули группу на -45 гр", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);

  document.MoveObjects({group}, /*dx*/ -10, /*dy*/ 0);
  kompasApp->ShowMessageBox(L"Cдвинули группу на -10 мм", L"", ksMessageWarning, ksButtonSetOk, /*showModal*/ true);

  kompasApp->ShowMessageBox(L"Очистили группу ( объекты исключаются из группы )", L"", ksMessageWarning, ksButtonSetOk,
                            /*showModal*/ true);
  group->Clear(/*clearTmp*/ true);

  // Создание и добавление отрезка в группу через интерфейс группы
  group->AddObjects({CreateLineSegment(view, /*x1*/ 10, /*y1*/ 10, /*x2*/ 20, /*y2*/ 10, ksCSThin)});

  if (ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager())
    chooseManager->Choose({group});
  kompasApp->ShowMessageBox(L"Снять выделение группы", L"", ksMessageWarning, ksButtonSetOk,
                            /*showModal*/ true);
  if (ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager())
    chooseManager->Unchoose({group});

  group->Delete(); // Удалим  группу (объекты удалятся тоже)
}


//------------------------------------------------------------------------------
/**
  Работа с именованой группой.
*/
//---
void WorkNameGroup(ksapi::IKompasDocument2D & document)
{
  // создание группы
  ksapi::IDrawingGroupsPtr groups = document.GetNamedGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"Group");
  ksapi::IViewPtr view = GetActiveView(document);
  if (!group || !view)
    return;

  group->AddObjects({
    CreateLineSegment(view, /*x1*/ 20, /*y1*/ 20, /*x2*/ 40, /*y2*/ 20, ksCSNormal),
    CreateLineSegment(view, /*x1*/ 40, /*y1*/ 20, /*x2*/ 40, /*y2*/ 40, ksCSNormal),
    CreateLineSegment(view, /*x1*/ 40, /*y1*/ 40, /*x2*/ 20, /*y2*/ 40, ksCSNormal),
    CreateLineSegment(view, /*x1*/ 20, /*y1*/ 40, /*x2*/ 20, /*y2*/ 20, ksCSNormal),
  });

  // Получение группы по имени
  group = nullptr;
  group = groups->GetItemByName(L"Group");
  if (!group)
    return;

  // Получение менеджера для подсветки
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Метод Choose/Unchoose принимает вектор объектов, поэтому сразу создадим его
  const std::vector<ksapi::IKompasAPIObjectPtr> groupHolder{group};

  // Добавим окружность в группу
  ksapi::ICirclePtr circle = CreateCircle(view, /*xc*/ 30, /*yc*/ 30, /*r*/ 10, ksCSNormal);
  group->AddObjects({circle});

  highlighter->Choose(groupHolder); // подсветка группы
  kompasApp->ShowMessageBox(L"Был добавлен объект в именную группу", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // убираем подсветку

  // Исключить объект из группы
  group->DetachObjects({circle}, /*deleteTmp*/ false);

  highlighter->Choose(groupHolder); // подсветка группы
  kompasApp->ShowMessageBox(L"Объект был исключен из именной группы", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // убираем подсветку
}


//------------------------------------------------------------------------------
/**
  Создать отрезок.
*/
//---
void DrawLineSeg(ksapi::IKompasDocument2D & document)
{
  // создание отрезка
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;

  // Создание отрезка и трансформация его с помощью матрицы.
  document.CreateMatrix(/*x*/ 30, /*y*/ 20, /*angle*/ 45, /*scaleX*/ 1, /*scaleY*/ 1);
  ksapi::ILineSegmentPtr lineSeg = CreateLineSegment(drawingContainer, /*x1*/ 30, /*y1*/ 20, /*x2*/ 60, /*y2*/ 20, ksCSNormal);
  document.DeleteMatrix();

  kompasApp->ShowMessageBox(std::format(L"x1 = {:4.2f}, y1 = {:4.2f}, x2 = {:4.2f}, y2 = {:4.2f}, style = {}", lineSeg->GetX1(),
                                        lineSeg->GetY1(), lineSeg->GetX2(), lineSeg->GetY2(), lineSeg->GetStyle(),
                                        lineSeg->GetAngle()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  // изменение параметров
  lineSeg->SetX2(30);
  lineSeg->SetY2(60);
  lineSeg->SetStyle(ksCSThin);

  lineSeg->Update();
}

//------------------------------------------------------------------------------
/**
  Создать дугу.
*/
//---
void DrawArc(ksapi::IKompasDocument2D & document)
{
  // создание дуги
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::IArcsPtr arcs = drawingContainer->GetArcs();
  if (!arcs)
    return;
  ksapi::IArcPtr arc = arcs->Add();
  if (!arc)
    return;

  // Создание дуги в Компасе и преобразование с помощью матрицы.
  document.CreateMatrix(/*x*/ 10, /*y*/ 10, /*angle*/ 0, /*scaleX*/ 1, /*scaleY*/ 1);
  // определение параметров дуги
  arc->SetXc(30);
  arc->SetYc(20);
  arc->SetRadius(20);
  arc->SetAngle1(45);
  arc->SetAngle2(135);
  arc->SetDirection(false);
  arc->SetStyle(ksCSNormal);
  arc->Update();
  document.DeleteMatrix();

  kompasApp
    ->ShowMessageBox(std::
                       format(L"xc = {:4.2f}, yc = {:4.2f}, rad = {:4.2f},\nang1 = {:4.2f}, ang2 = {:4.2f}, dir = {}, style = {}",
                              arc->GetXc(), arc->GetYc(), arc->GetRadius(), arc->GetAngle1(), arc->GetAngle2(),
                              arc->GetDirection(), arc->GetStyle()),
                     L"", ksMessageWarning, ksButtonSetOk, true);

  // изменение параметров дуги
  arc->SetXc(40);
  arc->SetYc(30);
  arc->SetRadius(10);
  arc->SetX1(50); // Начальная точка дуги
  arc->SetY1(30); // Начальная точка дуги
  arc->SetX2(40); // Конечная точка дуги
  arc->SetY2(20); // Конечная точка дуги
  arc->SetStyle(ksCSThin);

  if (arc->Update())
    kompasApp->ShowMessageBox(L"Изменили объект", L"", ksMessageWarning, ksButtonSetOk, true);
  else if (ksapi::IKompasErrorPtr error = kompasApp->GetKompasError())
    error->Report();
}


//------------------------------------------------------------------------------
/**
  Создать вспомогательную линию.
*/
//---
void DrawLine(ksapi::IKompasDocument2D & document)
{
  // создание линии
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::ILinesPtr lines = drawingContainer->GetLines();
  if (!lines)
    return;
  ksapi::ILinePtr line = lines->Add();
  if (!line)
    return;

  // Создание линии в Компасе и преобразование с помощью матрицы.
  document.CreateMatrix(/*x*/ 0, /*y*/ 0, /*angle*/ 45, /*scaleX*/ 1, /*scaleY*/ 1);
  // определение параметров линии
  line->SetX1(30);
  line->SetY1(20);
  line->SetAngle(0);
  line->Update();
  document.DeleteMatrix();

  kompasApp->ShowMessageBox(std::format(L"x = {:4.2f}, y = {:4.2f}, ang = {:4.2f}", line->GetX1(), line->GetY1(),
                                        line->GetAngle()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  // изменение параметров линии
  line->SetAngle(90);

  line->Update();
}


//------------------------------------------------------------------------------
/**
  Создать окружность.
*/
//---
void DrawCircle(ksapi::IKompasDocument2D & document)
{
  // создание окружности
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;

  document.CreateMatrix(/*x*/ 0, /*y*/ 0, /*angle*/ 0, /*scaleX*/ 2, /*scaleY*/ 2);
  ksapi::ICirclePtr circle = CreateCircle(drawingContainer, /*xc*/ 30, /*yc*/ 20, /*r*/ 10, ksCSNormal);
  document.DeleteMatrix();

  kompasApp->ShowMessageBox(std::format(L"xc = {:4.2f}, yc = {:4.2f}, rad = {:4.2f}, style = {}", circle->GetXc(),
                                        circle->GetYc(), circle->GetRadius(), circle->GetStyle()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  // изменение параметров окружности
  circle->SetRadius(20);
  circle->SetStyle(ksCSThin);

  circle->Update();
}


//------------------------------------------------------------------------------
/**
  Cоздать точки.
*/
//---
void DrawPoint(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::IPointsPtr points = drawingContainer->GetPoints();
  if (!points)
    return;

  ksapi::IPointPtr firstPoint;

  document.CreateMatrix(/*x*/ 10, /*y*/ 10, /*angle*/ 0, /*scaleX*/ 1, /*scaleY*/ 1);
  for (int32_t i = 0; i < 8; ++i)
  {
    ksapi::IPointPtr point = points->Add();
    if (!point)
      return;
    point->SetX(30 + i * 10);
    point->SetY(40);
    point->SetStyle(static_cast<ksAnnotationSymbolEnum>(i + 1));
    point->Update();

    if (i == 0)
      firstPoint = point;
  }
  document.DeleteMatrix();

  kompasApp->ShowMessageBox(std::format(L"x = {:4.2f} y = {:4.2f} style = {}", firstPoint->GetX(), firstPoint->GetY(),
                                        firstPoint->GetStyle()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  // изменение параметров точки
  firstPoint->SetX(20);
  firstPoint->SetY(30);
  firstPoint->SetStyle(ksStrikeSquarePoint);

  firstPoint->Update();
}


//------------------------------------------------------------------------------
/**
  Создать Bezier сплайн.
*/
//---
void DrawBezier(ksapi::IKompasDocument2D & document)
{
  // создание сплайна
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::IBeziersPtr beizers = drawingContainer->GetBeziers();
  if (!beizers)
    return;
  ksapi::IBezierPtr bezier = beizers->Add();
  if (!bezier)
    return;

  // точки сплайна задаются вектором
  std::vector<double> points{/*x*/ 0, /*y*/ 0, /*x*/ 20, /*y*/ 20, /*x*/ 50, /*y*/ 10, /*x*/ 70, /*y*/ 20};

  // определение параметров сплайна
  bezier->SetPoints(/*allPoints*/ false, points); // флаг false означает, что точка задаётся только координатами
  bezier->SetClosed(false);                       // признак замкнутости
  bezier->SetStyle(ksCSNormal);
  bezier->Update();

  // Вывод параметров сплайна на экран
  kompasApp->ShowMessageBox(std::format(L"count = {}, closed = {}, style = {}", bezier->GetPointsCount(), bezier->IsClosed(),
                                        bezier->GetStyle()),
                            L"", ksMessageWarning, ksButtonSetOk, true);
  const std::vector<double> receivedPoints = bezier->GetPoints(/*allPoints*/ false);
  for (size_t i = 1; i < receivedPoints.size(); i += 2)
  {
    kompasApp->ShowMessageBox(std::format(L"x[{}] = {:4.2f}, y[{}] = {:4.2f}", i / 2, receivedPoints[i - 1], i / 2,
                                          receivedPoints[i]),
                              L"", ksMessageWarning, ksButtonSetOk, true);
  }

  // Добавление ещё 2х точек и смещение точек по Y
  points.insert(points.end(), {/*x*/ 100, /*y*/ 0, /*x*/ 50, /*y*/ -50});
  for (size_t i = 1; i < points.size(); i += 2)
    points[i] += 10;

  // изменение параметров сплайна
  bezier->SetStyle(ksCSThin);
  bezier->SetClosed(true);
  bezier->SetPoints(/*allPoints*/ false, points);

  bezier->Update();
}


//------------------------------------------------------------------------------
/**
  Создать штриховку.
*/
//---
void DrawHatch(ksapi::IKompasDocument2D & document)
{
  // создание прямоугольника и штриховки
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::IHatchesPtr hatches = drawingContainer->GetHatches();
  ksapi::IRectanglesPtr rects = drawingContainer->GetRectangles();
  if (!hatches || !rects)
    return;

  // Создание временной группы
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*true*/ true, /*name*/ L"");
  if (!group)
    return;

  ksapi::IRectanglePtr rectangle = rects->Add();
  ksapi::IHatchPtr hatch = hatches->Add(); // штриховка управляется тремя интерфейсами
  ksapi::IHatchParamPtr hatchParam = hatch;
  ksapi::IBoundariesObjectPtr hatchBounds = hatch;
  if (!rectangle || !hatchBounds)
    return;

  // Матрица преобразования координат ( 30 по оси OX, 20 по оси OY, маштаб 1:2 )
  document.CreateMatrix(/*x*/ 30, /*y*/ 20, /*angle*/ 0, /*scaleX*/ 0.5, /*scaleY*/ 0.5);
  // определение параметров прямоугольника
  rectangle->SetX(20);
  rectangle->SetY(30);
  rectangle->SetWidth(50);
  rectangle->SetHeight(50);
  rectangle->SetStyle(ksCSThin);

  // Объекты будут созданы как временные (между Open и Close временной группы),
  // таким образом штриховка не будет ассоциирована с границами
  group->Open();

  rectangle->Update();

  // определение параметров штриховки
  hatchBounds->AddBoundaries({rectangle}, /*deleteSource*/ false); // границы штриховки определяются графическими примитивами
  hatchParam->SetStyle(ksHatchMetal);                              // Стиль из перечисления ksHatchStyleEnum (или пользовательский)
  hatchParam->SetStep(2);                                          // Шаг штриховки
  hatchParam->SetHatchAngle(45);                                   // Угол наклона штриховки
  hatchParam->SetWidth(0);                                         // Ширина полосы штриховки ( 0 - штриховать всю область )
  hatchParam->SetHatchType(true);                                  // Штриховать всю область
  hatch->SetX(0);                                                  // Базовая точка
  hatch->SetY(0);
  hatch->Update();

  group->Close();

  document.DeleteMatrix(); // Отключение матрицы преобразования координат

  group->Store(); // Вставили группу в документ.
  group->Clear(/*deleteTmp*/ true);
  group->Delete();

  std::wstring msg = std::format(L"style = {}, ang = {:4.2f}, step = {:4.2f}\nwidth = {:4.2f}, x = {:4.2f}, y = {:4.2f}",
                                 hatchParam->GetStyle(), hatchParam->GetHatchAngle(), hatchParam->GetStep(),
                                 hatchParam->GetWidth(), hatch->GetX(), hatch->GetY());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);

  // Матрица преобразования координат ( маштаб 2:1 )
  document.CreateMatrix(/*x*/ 0, /*y*/ 0, /*angle*/ 0, /*scaleX*/ 2, /*scaleY*/ 2);
  // измененеие параметров штриховки
  hatch->SetSide(true);
  hatch->SetX(0.8);
  hatch->Update();
  document.TransformObjects({hatch});
  document.DeleteMatrix(); // Отключение матрицы преобразования координат
}


//------------------------------------------------------------------------------
/**
  Текст.
*/
//---
void DrawText(ksapi::IKompasDocument2D & document)
{
  // создание текста
  ksapi::IDrawingContainerPtr drawingContainer = GetActiveView(document);
  if (!drawingContainer)
    return;
  ksapi::IDrawingTextsPtr texts = drawingContainer->GetDrawingTexts();
  if (!texts)
    return;
  ksapi::IDrawingTextPtr drawingText = texts->Add();
  ksapi::ITextPtr text = drawingText;
  if (!text)
    return;

  // определение общих параметров текста
  drawingText->SetStyle(ksTSDefault);    // Стиль текста из ksTextStyleEnum (или пользовательский)
  drawingText->SetX(30);                 // Координаты привязки текста
  drawingText->SetY(30);                 // Координаты привязки текста
  drawingText->SetAngle(0);              // угол наклона текста
  drawingText->SetHFormat(ksHFormatNot); // Признак горизонтального форматирования
  drawingText->SetVFormat(false);        // Признак вертикального форматирования. При true будет изменятся шаг строк
  drawingText->SetHeight(25);            // высота блока форматирования
  drawingText->SetWidth(20);             // Ширина блока форматирования

  // Создание строки.
  ksapi::ITextLinePtr textLine = text->Add();
  if (!textLine)
    return;

  // пример создания дроби.
  if (!CreateTextItem(*textLine, ksTItString, ksTextDefault, L"Дробь ", /*italic*/ false, /*bold*/ false, /*newLine*/ true))
    return;
  // Числитель, наклон.
  if (!CreateTextItem(*textLine, ksTItNumerator, ksTextBig, L"111", /*italic*/ true, /*bold*/ false, /*newLine*/ false))
    return;
  // Знаменатель, утолщение.
  if (!CreateTextItem(*textLine, ksTItDenominator, ksTextBig, L"222", /*italic*/ true, /*bold*/ true,
                      /*newLine*/ false))
    return;
  // Конец дроби, снятие утолщения, снятие наклона и текст после дроби.
  if (!CreateTextItem(*textLine, ksTItFractionEnd, ksTextDefault, L" 333", /*italic*/ false, /*bold*/ false, /*newLine*/ false))
    return;

  // Создание новой строки.
  textLine = text->Add();
  if (!textLine)
    return;
  // Пример задания дроби, нижнего и верхнего отклонения.
  if (!CreateTextItem(*textLine, ksTItString, ksTextDefault, L"Дробь и отклонения ", /*italic*/ false, /*bold*/ false,
                      /*newLine*/ true))
    return;
  // Числитель, наклон.
  if (!CreateTextItem(*textLine, ksTItNumerator, ksTextDefault, L"111 ", /*italic*/ true, /*bold*/ false, /*newLine*/ false))
    return;
  // Базовая строка для отклонения.
  if (!CreateTextItem(*textLine, ksTItSBase, ksTextDefault, L"222", /*italic*/ true, /*bold*/ false,
                      /*newLine*/ false))
    return;
  // Верхнее отклонение.
  if (!CreateTextItem(*textLine, ksTItSUpperIndex, ksTextDefault, L"Верх", /*italic*/ true, /*bold*/ false,
                      /*newLine*/ false))
    return;
  // Нижнее отклонение.
  if (!CreateTextItem(*textLine, ksTItSLowerIndex, ksTextDefault, L"Низ", /*italic*/ true, /*bold*/ false,
                      /*newLine*/ false))
    return;
  // Конец отклонения.
  if (!CreateTextItem(*textLine, ksTItSEnd, ksTextDefault, L" 333", /*italic*/ true, /*bold*/ false,
                      /*newLine*/ false))
    return;
  // Знаменатель.
  if (!CreateTextItem(*textLine, ksTItDenominator, ksTextDefault, L"444", /*italic*/ true, /*bold*/ false,
                      /*newLine*/ false))
    return;
  // Конец дроби, снятие утолщения, снятие наклона и текст после дроби.
  if (!CreateTextItem(*textLine, ksTItFractionEnd, ksTextDefault, L" 555", /*italic*/ false, /*bold*/ false, /*newLine*/ false))
    return;

  // Создание новой строки.
  textLine = text->Add();
  if (!textLine)
    return;
  // Пример задания спецсимвола.
  if (!CreateTextItem(*textLine, ksTItString, ksTextDefault, L"Спецсимвол ", /*italic*/ false, /*bold*/ false,
                      /*newLine*/ true))
    return;
  // Сам спецсимвол.
  ksapi::ITextItemPtr item = textLine->Add();
  if (!item)
    return;
  item->SetItemType(ksTItSpecialSymbol);
  item->SetNumber(65); // Номер спецзнака - шероховатость
  item->Update();
  // Конец спецсимвола и строка после него.
  if (!CreateTextItem(*textLine, ksTItSpecialSymbolEnd, ksTextDefault, L" 111", /*italic*/ false, /*bold*/ false,
                      /*newLine*/ false))
    return;

  // Создание новой строки.
  textLine = text->Add();
  if (!textLine)
    return;
  // Пример задания символа из сушествующего шрифта.
  if (!CreateTextItem(*textLine, ksTItString, ksTextDefault, L"Шрифт Arial ", /*italic*/ false, /*bold*/ false,
                      /*newLine*/ true))
    return;

  // Сам символ.
  ksapi::ITextFontPtr font = item = textLine->Add();
  if (!font)
    return;
  item->SetItemType(ksTItFontSymbol); // Важно установить тип символа перед установкой шрифта
  item->SetNumber(64);                // Номер символа из шрифта - @
  font->SetBold(true);
  item->SetSymbolFontName(L"Arial");
  item->Update();

  // Конец символа.
  if (!CreateTextItem(*textLine, ksTItSpecialSymbolEnd, ksTextDefault, L"", /*italic*/ false, /*bold*/ true,
                      /*newLine*/ false))
    return;

  // Создание текста.
  drawingText->Update();

  // Вывод параметров строки.
  PrintTextLine(text->GetTextLine(0));

  // Диалоговое окно "Изменять параметры текста ?".
  ksMessageButtonEnum btn =
    kompasApp->ShowMessageDialog(/*parentWindow*/ nullptr, /*text*/ L"Изменять параметры текста ?", /*caption*/ L"",
                                 /*explanation*/ L"", ksMessageWarning, ksButtonSetYesNoCancel, /*showModal*/ true,
                                 /*positiveButton*/ L"Да", /*negativeButton*/ L"Нет", /*cancelButton*/ L"Отмена", /*helpId*/ 0,
                                 /*helpFileName*/ L"");
  if (btn != ksButtonYes) // Ответ пользователя
    return;

  // Изменение параметров созданной ранее строки.
  textLine = text->GetTextLine(0);
  if (!textLine)
    return;

  font = item = textLine->GetTextItem(0);
  if (!font)
    return;
  font->SetColor(ksapi::Color{0x0000FF00});
  item->Update();

  font = item = textLine->GetTextItem(1);
  if (!font)
    return;
  font->SetBold(true);
  font->SetItalic(false);
  item->Update();

  drawingText->Update(); // Применение параметров
  PrintTextLine(textLine);
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


//------------------------------------------------------------------------------
/**
  Создать отрезок по двум точкам.
*/
//---
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IDrawingContainerPtr drawingContainer, double x1, double y1, double x2, double y2,
                                         ksCurveStyleEnum style)
{
  if (!drawingContainer)
    return {};
  ksapi::ILineSegmentsPtr lines = drawingContainer->GetLineSegments();
  if (!lines)
    return {};
  ksapi::ILineSegmentPtr line = lines->Add();
  if (!line)
    return {};

  line->SetX1(x1);
  line->SetY1(y1);
  line->SetX2(x2);
  line->SetY2(y2);
  line->SetStyle(style);
  line->Update();

  return line;
}

//-------------------------------------------------------------------------------
/**
  Создать окружность по координатам центра и радиусу.
*/
//---
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r,
                               ksCurveStyleEnum style)
{
  ksapi::ICirclesPtr circlesContainer = drawingContainer->GetCircles();
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

  return circle;
}

//-------------------------------------------------------------------------------
/**
  Создать элемент текста.
*/
//---
ksapi::ITextItemPtr CreateTextItem(ksapi::ITextLine & textLine, ksTextItemEnum type, ksTextSizeEnum size,
                                   const std::wstring & text, bool italic, bool bold, bool newLine)
{
  ksapi::ITextItemPtr textItem = textLine.Add();
  ksapi::ITextFontPtr font = textItem;
  if (!font)
    return nullptr;

  font->SetItalic(italic); // Наклон
  font->SetBold(bold);     // Жирный шрифт
  textItem->SetItemType(type);
  textItem->SetSizeFactor(size);
  textItem->SetStr(text);
  textItem->SetNewLine(newLine);
  textItem->Update();

  return textItem;
}

//-------------------------------------------------------------------------------
/**
  Получить активный вид.
*/
//---
ksapi::IDrawingContainerPtr GetActiveView(ksapi::IKompasDocument2D & document)
{
  if (ksapi::IViewsAndLayersManagerPtr mngr = document.GetViewsAndLayersManager())
    if (ksapi::IViewsPtr views = mngr->GetViews())
      return views->GetActiveView();
  return nullptr;
}


//-------------------------------------------------------------------------------
/**
  Вывод параметров строки.
*/
//---
void PrintTextLine(ksapi::ITextLinePtr textLine)
{
  if (!textLine)
    return;

  std::wstring msg = std::format(L"style = {}", textLine->GetStyle());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);

  // Цикл по компонентам строки
  for (int32_t i = 0; i < textLine->GetCount(); ++i)
  {
    // Получить значение элемента массива
    ksapi::ITextItemPtr item = textLine->GetTextItem(i);
    ksapi::ITextFontPtr font = item;
    if (!font)
      continue;

    std::wstring msg = std::format(L"i = {}, font.height = {:4.2f}, s = {},\nfont.fontName = {}", i, font->GetHeight(),
                                   item->GetStr(), font->GetFontName());
    kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
  }
}
