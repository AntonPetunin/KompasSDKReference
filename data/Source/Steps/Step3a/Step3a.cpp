////////////////////////////////////////////////////////////////////////////////
//
// Step3a.cpp - Объекты - a
//
// 1.  Контур                            - WorkContour
// 2.  Технические требования            - TDemWork
// 3.  Стрелка вида                      - DrawViewPointer
// 4.  Работа со штампом                 - WorkStamp
// 5.  Таблица                           - TableWork
// 6.  Эквидистанта                      - DrawEquidistant
// 7.  Эллипс                            - DrawEllipse
// 8.  Полилиния                         - DrawPolyline
// 9.  Nurbs                             - DrawNurbs
// 10. Допуск формы                      - WorkTolerance
// 11. Неуказанная шероховатость         - DrawSpecRough
// 12. Вставка фрагмента внешней ссылкой - DrawInsFragment1
// 13. Вставка локального фрагмента      - DrawInsFragment2
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KsAPI.h>
#include <KompasLibraryActions.h>

ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void WorkContour(ksapi::IKompasDocument2D & document);
void TDemWork(ksapi::IKompasDocument2D & document);
void DrawViewPointer(ksapi::IKompasDocument2D & document);
void WorkStamp(ksapi::IKompasDocument2D & document);
void TableWork(ksapi::IKompasDocument2D & document);
void DrawEquidistant(ksapi::IKompasDocument2D & document);
void DrawEllipse(ksapi::IKompasDocument2D & document);
void DrawPolyline(ksapi::IKompasDocument2D & document);
void DrawNurbs(ksapi::IKompasDocument2D & document);
void WorkTolerance(ksapi::IKompasDocument2D & document);
void DrawSpecRough(ksapi::IKompasDocument2D & document);
void DrawInsFragment1(ksapi::IKompasDocument2D & document);
void DrawInsFragment2(ksapi::IKompasDocument2D & document);

ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & document);
ksapi::IRectanglePtr CreateRectangle(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2);
ksapi::IDrawingObjectPtr SelectObject(ksapi::IKompasDocument2D & document);
ksapi::IArcPtr CreateArc(ksapi::IKompasDocument2D & document, double xc, double yc, double r, double x1, double y1, double x2,
                         double y2);
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2);

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

  switch (commandId)
  {
    case 1:
      WorkContour(*document);
      break;
    case 2:
      TDemWork(*document);
      break;
    case 3:
      DrawViewPointer(*document);
      break;
    case 4:
      WorkStamp(*document);
      break;
    case 5:
      TableWork(*document);
      break;
    case 6:
      DrawEquidistant(*document);
      break;
    case 7:
      DrawEllipse(*document);
      break;
    case 8:
      DrawPolyline(*document);
      break;
    case 9:
      DrawNurbs(*document);
      break;
    case 10:
      WorkTolerance(*document);
      break;
    case 11:
      DrawSpecRough(*document);
      break;
    case 12:
      DrawInsFragment1(*document);
      break;
    case 13:
      DrawInsFragment2(*document);
      break;
  }
}

//------------------------------------------------------------------------------
/**
  Построить контур.
*/
//---
void WorkContour(ksapi::IKompasDocument2D & document)
{
  // создание контура
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::IDrawingContoursPtr contours = drawingContainer->GetDrawingContours();
  if (!contours)
    return;
  ksapi::IDrawingContourPtr drawingContour = contours->Add();
  ksapi::IContourPtr contour = drawingContour;
  if (!contour)
    return;
  drawingContour->SetStyle(ksCSNormal);

  // создание кривых контура через метод контура
  ksapi::IContourLineSegmentPtr line1 = contour->AddSegment(ksCSLineSeg);
  ksapi::IContourArcPtr arc1 = contour->AddSegment(ksCSArc);
  if (!line1 || !arc1)
    return;

  line1->SetX1(20);
  line1->SetY1(30);
  line1->SetX2(50);
  line1->SetY2(30);

  arc1->SetXc(50);
  arc1->SetYc(20);
  arc1->SetRadius(10);
  arc1->SetX1(50);
  arc1->SetY1(10);
  arc1->SetX2(50);
  arc1->SetY2(30);
  arc1->SetDirection(false); // против часовой стрелки

  // создание кривых и добавление их в контур
  ksapi::ILineSegmentPtr line2 = CreateLineSegment(document, /*x1*/ 50, /*y1*/ 10, /*x2*/ 20, /*y2*/ 10);
  ksapi::IArcPtr arc2 = CreateArc(document, /*xc*/ 20, /*yc*/ 20, /*r*/ 10, /*x1*/ 20, /*y1*/ 10, /*x2*/ 20, /*y2*/ 30);
  if (!arc2 || !line2)
    return;

  // Добавление кривых в контур
  contour->CopyCurve(line2, /*deleteSource*/ true);
  contour->CopyCurve(arc2, /*deleteSource*/ true);

  drawingContour->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  std::vector<ksapi::IKompasAPIObjectPtr> contourInContainer{drawingContour};
  highlighter->Choose(contourInContainer); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение контура", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->Unchoose(contourInContainer); // снятие подсветки


  // создание группы, добавление в неё контура, и её перемещение
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"");
  if (!group)
    return;
  group->AddObjects({drawingContour});
  document.MoveObjects({group}, /*dx*/ 10, /*dy*/ 10);
}


//------------------------------------------------------------------------------
/**
  Заполнение технических требований.
*/
//---
void TDemWork(ksapi::IKompasDocument2D & document)
{
  // создание объекта технических требований
  ksapi::IDrawingDocumentPtr drawingDocument = &document;
  if (!drawingDocument)
    return;
  ksapi::ITechnicalDemandPtr demand = drawingDocument->GetTechnicalDemand();
  if (!demand)
    return;

  ksapi::ITextPtr text = demand->GetText();
  if (!text)
    return;

  // создание шести строчек с цифрами
  for (uint32_t i = 1111111; i <= 6666666; i += 1111111)
  {
    ksapi::ITextLinePtr line = text->Add();
    if (!line)
      return;
    ksapi::ITextItemPtr item = line->Add();

    item->SetStr(std::to_wstring(i));
    item->Update();
  }

  // Задание габаритов.
  // габариты описывются как вектор координат, представляющих набор прямоугольников.
  // Прямоугольники состоят из 2х точек, левого нижнего и правого верхнего угла
  std::vector<double> gabarits = {/*x1*/ 230, /*y1*/ 65, /*x2*/ 415, /*y2*/ 80,  // первый прямоугольник
                                  /*x1*/ 45,  /*y1*/ 15, /*x2*/ 230, /*y2*/ 60}; // второй прямоугольник
  demand->SetBlocksGabarits(gabarits);
  demand->Update();

  kompasApp->ShowMessageBox(L"Удалим технические требования", L"", ksMessageWarning, ksButtonSetOk, true);
  demand->Delete();
}


//------------------------------------------------------------------------------
/**
  Создание стрелки вида.
*/
//---
void DrawViewPointer(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetDrawingContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IViewPointersPtr viewPointers = symbolsContainer->GetViewPointers();
  if (!viewPointers)
    return;
  ksapi::IViewPointerPtr viewPointer = viewPointers->Add();
  if (!viewPointer)
    return;

  viewPointer->SetX1(55); // Координаты вершины стрелки ( острие )
  viewPointer->SetY1(50); // Координаты вершины стрелки ( острие )
  viewPointer->SetX2(40); // Координаты конечной точки стрелки
  viewPointer->SetY2(50); // Координаты конечной точки стрелки
  viewPointer->SetTextX(40);
  viewPointer->SetTextY(52);
  viewPointer->SetArrowType(ksArrowEnum::ksClosedArrow);

  ksapi::ITextPtr text = viewPointer->GetText();
  if (!text)
    return;
  text->SetStr(L"A");

  viewPointer->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  highlighter->Choose({viewPointer}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение стрелки", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Заполнение основной надписи.
*/
//---
void WorkStamp(ksapi::IKompasDocument2D & document)
{
  ksapi::ILayoutSheetsPtr layoutSheets = document.GetLayoutSheets();
  if (!layoutSheets || layoutSheets->GetCount() < 1)
    return;
  ksapi::ILayoutSheetPtr layoutSheet = layoutSheets->GetItem(0);
  if (!layoutSheet)
    return;
  ksapi::IStampPtr stamp = layoutSheet->GetStamp();
  if (!stamp)
    return;

  ksapi::ITextPtr text = stamp->GetText(2); // получение текста для ячейки с индексом
  if (text)
    text->SetStr(L"1111111");

  stamp->Update();
}


//------------------------------------------------------------------------------
/**
  Создание таблицы.
*/
//---
void TableWork(ksapi::IKompasDocument2D & document)
{
  // создание объекта таблицы
  ksapi::ISymbols2DContainerPtr symbolContainer = GetDrawingContainer(document);
  if (!symbolContainer)
    return;
  ksapi::IDrawingTablesPtr drTables = symbolContainer->GetDrawingTables();
  if (!drTables)
    return;
  ksapi::IDrawingTablePtr drTable = drTables->Add(/*rowsCount*/ 2, /*columnsCount*/ 2, /*rowHeight*/ 10, /*columnsWidth*/ 20,
                                                  ksTableTileLayoutEnum::ksTTLNotCreate);
  ksapi::ITablePtr table = drTable;
  if (!table)
    return;

  // Точка привязки
  drTable->SetX(50);
  drTable->SetY(50);

  // Изменение стиля линий таблицы
  ksapi::ITableRangePtr tableRange = table->GetRange(/*beginRow*/ 0, /*beginColumn*/ 0, /*endRow*/ 2, /*endColumn*/ 2);
  if (!tableRange)
    return;
  ksapi::ICellBoundariesPtr boundaries = tableRange->GetCellsBoundaries();
  if (!boundaries)
    return;
  boundaries->SetLineStyle(ksCBAllBorders, ksCSNormal);

  // присвоение значения ячейкам
  int32_t num = 1;
  for (int32_t i = 0; i < table->GetRowsCount(); ++i)
  {
    for (int32_t j = 0; j < table->GetColumnsCount(); ++j)
    {
      ksapi::ITableCellPtr cell = table->GetCell(i, j);
      if (!cell)
        continue;
      ksapi::ITextPtr text = cell->GetText();
      if (text)
        text->SetStr(std::to_wstring(num++));
    }
  }

  drTable->Update();
}


//------------------------------------------------------------------------------
/**
  Построить эквидистанту.
*/
//---
void DrawEquidistant(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;

  // создание отрезка, который потенциально можно сразу выбрать для создания эсвидистанты
  ksapi::ILineSegmentsPtr lines = drawingContainer->GetLineSegments();
  if (!lines)
    return;
  ksapi::ILineSegmentPtr line = CreateLineSegment(document, /*x1*/ 150, /*y1*/ 150, /*x2*/ 200, /*y2*/ 150);
  ksapi::IChooseManagerPtr chooseMngr = document.GetChooseManager();
  if (!chooseMngr || !line)
    return;

  // получение выбранного пользователем объекта
  while (ksapi::IDrawingObjectPtr selected = SelectObject(document))
  {
    // создание эквидистанты и определение её параметров
    ksapi::IEquidistantsPtr equidistants = drawingContainer->GetEquidistants();
    if (!equidistants)
      return;
    ksapi::IEquidistantPtr equidistant = equidistants->Add();
    if (!equidistant)
      return;
    equidistant->SetBaseObject(selected); // объект, вокруг которого будет строится эквидистанта
    equidistant->SetSide(ksEquidistantTypeEnum::ksETBoth);
    equidistant->SetCutMode(false);           // Тип обхода углов контура ( false - обход срезом, true - обход дугой )
    equidistant->SetDegenerateSegment(false); // Флаг разрешения вырожденных сегментов эквидистанты ( false - вырожденные
                                              // сегменты запрещены, true - вырожденные сегменты разрешены )
    equidistant->SetRightRadius(5);
    equidistant->SetLeftRadius(3);
    equidistant->SetStyle(ksCSNormal);

    equidistant->Update();

    // Подсветить, вывести сообщение, погасить подсветку.
    chooseMngr->Choose({equidistant});
    kompasApp->ShowMessageBox(L"Снять выделение эквидистанты", L"", ksMessageWarning, ksButtonSetOk, true);
    chooseMngr->UnchooseAll();
  }
}


//------------------------------------------------------------------------------
/**
  Создать эллипс.
*/
//---
void DrawEllipse(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::IEllipsesPtr ellipses = drawingContainer->GetEllipses();
  if (!ellipses)
    return;
  ksapi::IEllipsePtr ellipse = ellipses->Add();
  if (!ellipse)
    return;
  ellipse->SetXc(50);        // Координаты центра эллипса
  ellipse->SetYc(40);        // Координаты центра эллипса
  ellipse->SetSemiAxisA(20); // Длина полуосей эллипса
  ellipse->SetSemiAxisB(10); // Длина полуосей эллипса
  ellipse->SetAngle(0);      // Угол наклона оси а эллипса к оси X
  ellipse->SetStyle(ksCSNormal);

  ellipse->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  highlighter->Choose({ellipse}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение эллипса", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Создание полилинии.
*/
//---
void DrawPolyline(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::IPolyLines2DPtr polylines = drawingContainer->GetPolyLines2D();
  if (!polylines)
    return;
  ksapi::IPolyLine2DPtr polyline = polylines->Add();
  if (!polyline)
    return;

  polyline->AddPoint(/*index*/ -1, /*x*/ 10, /*y*/ 10); // Индекс -1 - в конец.
  polyline->AddPoint(/*index*/ -1, /*x*/ 20, /*y*/ 20); // Индекс -1 - в конец.
  polyline->AddPoint(/*index*/ 2, /*x*/ 40, /*y*/ 20);  // В данном случае - тоже в конец.
  polyline->AddPoint(/*index*/ 2, /*x*/ 30, /*y*/ 10);  // На предпоследнее место.
  polyline->SetStyle(ksCSThin);
  polyline->SetClosed(false);
  polyline->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Подсветить, вывести сообщение, снять подсветку
  highlighter->Choose({polyline}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение полилинии", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Создать Nurbs - сплайн.
*/
//---
void DrawNurbs(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::INurbsesPtr nurbses = drawingContainer->GetNurbses();
  if (!nurbses)
    return;
  ksapi::INurbsPtr nurbs = nurbses->Add();
  if (!nurbs)
    return;

  nurbs->AddPoint(/*index*/ -1, /*x*/ 0, /*y*/ 0, /*weight*/ 1);   // index -1 - добавить в конец
  nurbs->AddPoint(/*index*/ -1, /*x*/ 20, /*y*/ 20, /*weight*/ 1); // weight должен быть больше 0
  nurbs->AddPoint(/*index*/ -1, /*x*/ 50, /*y*/ 10, /*weight*/ 1);
  nurbs->AddPoint(/*index*/ -1, /*x*/ 70, /*y*/ 20, /*weight*/ 1);
  nurbs->AddPoint(/*index*/ -1, /*x*/ 100, /*y*/ 0, /*weight*/ 1);
  nurbs->AddPoint(/*index*/ -1, /*x*/ 50, /*y*/ -50, /*weight*/ 1);
  nurbs->SetClosed(false);
  nurbs->SetDegree(3);
  nurbs->SetStyle(ksCSNormal);
  nurbs->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Подсветить, вывести сообщение, снять подсветку
  highlighter->Choose({nurbs}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение сплайна", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Допуск формы.
*/
//---
void WorkTolerance(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetDrawingContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ITolerancesPtr tolerances = symbolsContainer->GetTolerances();
  if (!tolerances)
    return;
  ksapi::ITolerancePtr ifTolerance = tolerances->Add();
  ksapi::IBranchsPtr ifBranchs = ifTolerance;
  ksapi::IToleranceParamPtr ifToleranceParam = ifTolerance;
  if (!ifBranchs || !ifToleranceParam)
    return;

  ksapi::ITablePtr ifTable = ifToleranceParam->GetTable();
  if (!ifTable)
    return;

  // по умолчанию 1 строка и 4 столбца
  ifTable->AddRow(/*row*/ -1, /*down*/ true);
  ifTable->DeleteColumn(/*column*/ 3);

  // Лямбда для установки значения ячейки по индексам.
  auto addStrToCell = [ifTable](int32_t i, int32_t j, const std::wstring & str, int32_t sign)
  {
    ksapi::ITableCellPtr cell = ifTable->GetCell(/*row*/ i, /*column*/ j);
    if (!cell)
      return;

    ksapi::ITextLinePtr line = cell->GetText();
    if (!line)
      return;
    ksapi::ITextItemPtr item = line->Add();
    if (!item)
      return;

    if (sign == -1 && !str.empty())
      item->SetStr(str);
    else
    {
      item->SetItemType(ksTItSpecialSymbol);
      item->SetNumber(sign);
    }
    item->Update();
    return;
  };

  // Заполнение таблицы данными.
  addStrToCell(0, 0, L"", 26);
  addStrToCell(0, 1, L"111", -1);
  addStrToCell(0, 2, L"222", -1);
  addStrToCell(1, 0, L"", 23);
  addStrToCell(1, 1, L"333", -1);
  addStrToCell(1, 2, L"444", -1);

  // общие параметры - базовая точка и ориентация
  ifBranchs->SetX0(40);
  ifBranchs->SetY0(40);
  ifToleranceParam->SetBasePointPos(ksTablePointEnum::ksTPLeftBottom);
  ifToleranceParam->SetVertical(false);

  // параметры первой ветки - точки, тип и место соединения с таблицей
  ifBranchs->AddBranch(/*index*/ 0, /*points*/ {/*x*/ 40, /*y*/ 10});
  ifTolerance->SetToleranceArrowType(/*index*/ 0, ksToleranceArrowType::ksTAArrow);
  ifTolerance->SetBranchPos(/*index*/ 0, ksTablePointEnum::ksTPLeftBottom);

  // параметры второй ветки - точки, тип и место соединения с таблицей
  ifBranchs->AddBranch(/*index*/ 1, /*points*/ {/*x*/ 100, /*y*/ 50, /*x*/ 100, /*y*/ 10});
  ifTolerance->SetToleranceArrowType(/*index*/ 1, ksToleranceArrowType::ksTATriangle);
  ifTolerance->SetBranchPos(/*index*/ 1, ksTablePointEnum::ksTPRightUp);

  ifTolerance->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Подсветить, вывести сообщение, снять подсветку
  highlighter->Choose({ifTolerance}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение допуска формы", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Шероховатость.
*/
//---
void DrawSpecRough(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingDocumentPtr drawingDocument = &document;
  if (!drawingDocument)
    return;
  ksapi::ISpecRoughPtr specRough = drawingDocument->GetSpecRough();
  if (!specRough)
    return;
  specRough->SetSignType(ksRoughSignEnum::ksWithoutDeleteMaterial);
  specRough->SetAddSign(true);
  specRough->SetText(L"Rz40");
  specRough->Update();
}


//------------------------------------------------------------------------------
/**
  Вставка внешнего фрагмента.
*/
//---
void DrawInsFragment1(ksapi::IKompasDocument2D & document)
{
  // выбор файла пользователем
  const std::wstring file = kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, /*defaultExtention*/ L"*.frw",
                                                                           /*filters*/ L"", /*startDirectory*/ L"",
                                                                           ksSystemPathTypeEnum::ksDocumentLibraries,
                                                                           /*preview*/ true);
  if (file.empty())
    return;

  // создание описания фрагмента
  ksapi::IInsertionsManagerPtr insertionsMngr = &document;
  if (!insertionsMngr)
    return;
  ksapi::IInsertionDefinitionPtr insertionDefinition = insertionsMngr->AddDefinition(ksTBodyFragment, L"Fragment", file);
  if (!insertionDefinition)
    return;

  // создание самого фрагмента, и передача ему его описания
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::IInsertionObjectsPtr objects = drawingContainer->GetInsertionObjects();
  if (!objects)
    return;
  ksapi::IInsertionFragmentPtr insertionFragment = objects->Add(insertionDefinition);
  if (!insertionFragment)
    return;

  bool res = insertionFragment->SetPlacement(/*x*/ 30, /*y*/ 40, /*angle*/ 45, /*mirrorSymmetry*/ false);
  insertionFragment->SetScale(2);
  insertionFragment->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Подсветить, вывести сообщение, снять подсветку
  highlighter->Choose({insertionFragment}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение фрагмента", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
}


//------------------------------------------------------------------------------
/**
  Вставка локального фрагмента.
*/
//---
void DrawInsFragment2(ksapi::IKompasDocument2D & document)
{
  // создание описания фрагмента, и открытие связанного с ним документа для редактирвания
  ksapi::IInsertionsManagerPtr insertionsMngr = &document;
  if (!insertionsMngr)
    return;
  ksapi::IInsertionDefinitionPtr insertionDefinition =
    insertionsMngr->AddDefinition(ksInsertionTypeEnum::ksTLocalFragment, L"Local", L"");
  if (!insertionDefinition)
    return;
  ksapi::IKompasDocument2DPtr insertionDocFrg = insertionDefinition->Open(/*visible*/ false, /*readOnly*/ false);
  if (!insertionDocFrg)
    return;
  // добавление примитивов в документ-фрагмент
  CreateLineSegment(*insertionDocFrg, /*x1*/ 0, /*y1*/ 0, /*x2*/ 10, /*y2*/ 0);
  CreateLineSegment(*insertionDocFrg, /*x1*/ 0, /*y1*/ 0, /*x2*/ 0, /*y2*/ 10);
  CreateArc(*insertionDocFrg, /*xc*/ 0, /*yc*/ 0, /*r*/ 10, /*x1*/ 10, /*y1*/ 0, /*x2*/ 0, /*y2*/ 10);
  insertionDocFrg->Close(DocumentCloseOptions::kdDoNotSaveChanges);

  // добавление фрагмента в основной документ
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  ksapi::IInsertionObjectsPtr objects = drawingContainer->GetInsertionObjects();
  if (!objects)
    return;
  ksapi::IInsertionFragmentPtr insertionFragment = objects->Add(insertionDefinition);
  if (!insertionFragment)
    return;

  bool res = insertionFragment->SetPlacement(/*x*/ 30, /*y*/ 40, /*angle*/ 45, /*mirrorSymmetry*/ false);
  insertionFragment->SetScale(2);
  insertionFragment->Update();

  // Этот объект в данной ситуации нужен для подсветки группы
  ksapi::IChooseManagerPtr highlighter = document.GetChooseManager();
  if (!highlighter)
    return;

  // Подсветить, вывести сообщение, снять подсветку
  highlighter->Choose({insertionFragment}); // подсветка
  kompasApp->ShowMessageBox(L"Снять выделение фрагмента", L"", ksMessageWarning, ksButtonSetOk, true);
  highlighter->UnchooseAll(); // снятие подсветки
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
  Получение активного вида, как IDrawingContainer
*/
//---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & document)
{
  ksapi::IViewsAndLayersManagerPtr viewsAndLayersMngr = document.GetViewsAndLayersManager();
  if (!viewsAndLayersMngr)
    return nullptr;
  ksapi::IViewsPtr views = viewsAndLayersMngr->GetViews();
  if (!views)
    return nullptr;
  return views->GetActiveView();
}

//------------------------------------------------------------------------------
/**
  Создание прямоугольника, если где-то понадобится примитив, для уменьшения дублирования кода
*/
//---
ksapi::IRectanglePtr CreateRectangle(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return nullptr;
  ksapi::IRectanglesPtr rectangles = drawingContainer->GetRectangles();
  if (!rectangles)
    return nullptr;
  ksapi::IRectanglePtr rectangle = rectangles->Add();
  if (!rectangle)
    return nullptr;

  rectangle->SetX(x1);
  rectangle->SetY(y1);
  rectangle->SetWidth(x2 - x1);
  rectangle->SetHeight(y2 - y1);
  rectangle->Update();
  return rectangle;
}

/**
  Создание отрезка
*/
//---
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return nullptr;
  ksapi::ILineSegmentsPtr lineSegs = drawingContainer->GetLineSegments();
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
  return lineSeg;
}

/**
  Создание дуги
*/
//---
ksapi::IArcPtr CreateArc(ksapi::IKompasDocument2D & document, double xc, double yc, double r, double x1, double y1, double x2,
                         double y2)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return nullptr;
  ksapi::IArcsPtr arcs = drawingContainer->GetArcs();
  if (!arcs)
    return nullptr;
  ksapi::IArcPtr arc = arcs->Add();
  if (!arc)
    return nullptr;

  arc->SetXc(xc);
  arc->SetYc(yc);
  arc->SetRadius(r);
  arc->SetX1(x1);
  arc->SetY1(y1);
  arc->SetX2(x2);
  arc->SetY2(y2);
  arc->SetDirection(true); // против часовой стрелки
  arc->Update();
  return arc;
}


//------------------------------------------------------------------------------
/**
  Ожидание выбора объекта пользователем
*/
//---
ksapi::IDrawingObjectPtr SelectObject(ksapi::IKompasDocument2D & document)
{
  ksapi::IProcess2DPtr process2d = document.GetLibProcess(ksProcess2DTypeEnum::ksProcess2DCursor);
  ksapi::IProcessPtr process = process2d;
  if (!process)
    return nullptr;
  ksapi::IViewPtr currentView = GetDrawingContainer(document);
  if (!currentView)
    return nullptr;

  process->SetCaption(L"Укажите объект");

  // функция, которая ищет объект по координатам, будет вызываться по событию клика мыши
  ksapi::IDrawingObjectPtr result;
  auto clickCallback = [currentView, &result](double x, double y, double, bool)
  {
    ksapi::IFindObjectParametersPtr params = nullptr;
    result = currentView->FindObject(x, y, 1e-6, params);

    if (!result || !result->IsCurve())
    {
      kompasApp->ShowMessageBox(L"Объект не найден", L"", ksMessageWarning, ksButtonSetOk, true);
      return false;
    }

    // после того как объект найден, вернём false, что бы удалить эту функцию из контейнера
    return false;
  };

  // добавление колбека в контейнер обработчиков событий
  if (ksapi::IProcess2DEventsPtr events = process2d->Events())
    events->AddPlacementChangedHandler(L"Step3a", clickCallback);

  process->Run(/*modal*/ true, /*postMessage*/ false);

  return result;
}
