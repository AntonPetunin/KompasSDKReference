////////////////////////////////////////////////////////////////////////////////
//
// Step1_KsAPI_2D.cpp - Обозначения СПДС.
//
//   1: NewDocument();                - Создать документ
//   2: NewMark();                    - Создать марку без линии-выноски
//   3: AddStrAxis();                 - Прямая ось
//   4: AddCircleAxis();              - Круговая ось
//   5: NewMarkOnLeader();            - Создать марку с линией-выноской
//   6: NewMarkOnLine();              - Марка/поз.обозначение
//   7: NewMultiTextLeader();         - Выносная надпись к многослойным конструкциям
//   8: NewBrace();                   - Фигурная скобка
//
////////////////////////////////////////////////////////////////////////////////

#include <Step1_KsAPI_2D_pch.h>

#include <format>

// Интерфейс приложения
ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
/// Создать документ.
void NewDocument();
/// Создать марку без линии-выноски.
void NewMark();
/// Создать прямую координационную ось.
void AddStrAxis();
/// Создать круговую координационную ось.
void AddCircleAxis();
/// Создать марку с линией-выноской.
void NewMarkOnLeader();
/// Создать марку/позиционное обозначение.
void NewMarkOnLine();
/// Создать выносную надпись к многослойным конструкциям.
void NewMultiTextLeader();
/// Создать фигурную скобку.
void NewBrace();


//-------------------------------------------------------------------------------
// Описания вспомогательных функций
// ---
/// Получить указатель на интерфейс коллекции видов.
ksapi::IViewsPtr GetViewsPtr();
/// Получить указатель на коллекцию отрезков.
ksapi::ILineSegmentsPtr GetILineSegments();


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 1:
      NewDocument();
      break; // Создать документ
    case 2:
      NewMark();
      break; // Создать марку без линии-выноски
    case 3:
      AddStrAxis();
      break; // Прямая ось
    case 4:
      AddCircleAxis();
      break; // Круговая ось
    case 5:
      NewMarkOnLeader();
      break; // Создать марку с линией-выноской
    case 6:
      NewMarkOnLine();
      break; // Марка/поз.обозначение
    case 7:
      NewMultiTextLeader();
      break; // Выносная надпись к многослойным конструкциям
    case 8:
      NewBrace();
      break; // Фигурная скобка
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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);

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
  Создать документ.
*/
//---
void NewDocument()
{
  if (kompasApp)
    if (ksapi::IDocumentsPtr pDocuments = kompasApp->GetDocuments()) //Получить указатель на интерфейс коллекции документов
      //Получить указатель на интерфейс IKompasDocument
      //Вызов Add создаёт документ, (тип указан в списке параметров) и добавляет его в коллекцию
      ksapi::IKompasDocumentPtr pKompasDocument(pDocuments->AddWithDefaultSettings(ksDocumentDrawing, true));
}


//------------------------------------------------------------------------------
/**
  Создать марку без линии-выноски.
*/
//---
void NewMark()
{
  ksapi::IViewsPtr pViews = GetViewsPtr();
  if (!pViews)
    return;

  //Получить указатель на интерфейс активного вида
  ksapi::IViewPtr pView(pViews->GetActiveView());

  //Получить указатель на контейнер объектов СПДС
  ksapi::IBuildingContainerPtr pBuildingContainer(pView);

  if (!pBuildingContainer)
    return;

  // Получить указатель на коллекцию марок
  ksapi::IMarksPtr pMarks(pBuildingContainer->GetMarks());

  if (!pMarks)
    return;

  // Получить указатель на марку
  ksapi::IMarkInsideFormPtr pMarkInF(pMarks->Add(ksDrMarkInsideForm));
  if (!pMarkInF)
    return;

  pMarkInF->SetX(250);
  pMarkInF->SetY(250);
  pMarkInF->SetForm(ksMFormRectangle);
  pMarkInF->SetFormGabarit(45);
  pMarkInF->SetFormHeight(50);
  pMarkInF->SetAngle(45);
  pMarkInF->SetFormStyle(ksCSAxial);

  pMarkInF->Update(); // Обязательный вызов для создания объекта в Компасе.

  // Взять координаты точки привязки.
  double x = pMarkInF->GetX();
  double y = pMarkInF->GetY();

  // Показать сообщение с координатами на экране.
  std::wstring msg = std::format(L"Координаты точки привязки:\nX = {}, Y = {}", x, y);
  kompasApp->ShowMessageBox(msg /*text*/, kLibName /*caption*/, ksMessageWarning, ksButtonSetOk, true /*showModal*/);
}


//------------------------------------------------------------------------------
/**
  Создать прямую координационную ось.
*/
//---
void AddStrAxis()
{
  ksapi::IStraightAxisPtr ax1;
  if (kompasApp)
    // получить 2D документ
    if (ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument())
      // получить менеджер видов и слоев
      if (ksapi::IViewsAndLayersManagerPtr viewMngr = doc2D->GetViewsAndLayersManager())
        // получить коллекцию видов
        if (ksapi::IViewsPtr views = viewMngr->GetViews())
          // Здесь делается QueryInterface с вида на контейнер СПДС
          // (это видно под отладчиком)
          if (ksapi::IBuildingContainerPtr pBuildContainer = views->GetActiveView())
            // получить коллекцию строительных осей
            if (ksapi::IBuildingAxesPtr axes = pBuildContainer->GetBuildingAxes())
              // получить прямую ось
              ax1 = axes->Add(ksDrStraightAxis);
  if (!ax1)
    return;

  ax1->SetAngle(45);
  ax1->SetLength(100);

  ax1->Update(); // Обязательный вызов для создания объекта в Компасе.
}


//------------------------------------------------------------------------------
/**
  Создание круговой координационной оси.
*/
//---
void AddCircleAxis()
{
  ksapi::ICircleAxisPtr axC;

  if (kompasApp)
    // получить 2D документ
    if (ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument())
      // получить менеджер видов и слоев
      if (ksapi::IViewsAndLayersManagerPtr viewsMngr = doc2D->GetViewsAndLayersManager())
        // получить коллекцию видов
        if (ksapi::IViewsPtr views = viewsMngr->GetViews())
          // Здесь делается QueryInterface с вида на контейнер СПДС
          // (это видно под отладчиком)
          if (ksapi::IBuildingContainerPtr pBuildContainer = views->GetActiveView())
            // получить коллекцию строительных осей
            if (ksapi::IBuildingAxesPtr axes = pBuildContainer->GetBuildingAxes())
              // получить круговую ось
              axC = axes->Add(ksDrCircleAxis);
  if (!axC)
    return;

  axC->SetXc(100);
  axC->SetYc(100);
  axC->SetRadius(100);
  axC->SetMarkOn(true);
  axC->SetMarkAngle(45);

  axC->Update(); // Обязательный вызов для создания объекта в Компасе.
}


//------------------------------------------------------------------------------
/**
  Создать марку с линией-выноской.
*/
//---
void NewMarkOnLeader()
{
  ksapi::IViewsPtr ppViews = GetViewsPtr();
  if (!ppViews)
    return;

  // Получить указатель на интерфейс активного вида.
  ksapi::IViewPtr ppView = ppViews->GetActiveView();

  // Получить указатель на контейнер объектов СПДС.
  ksapi::IBuildingContainerPtr ppBuildingContainer = ppView;
  if (!ppBuildingContainer)
    return;

  // Получить указатель на коллекцию марок.
  ksapi::IMarksPtr ppMarks = ppBuildingContainer->GetMarks();
  if (!ppMarks)
    return;

  // Получить указатель на марку.
  ksapi::IMarkOnLeaderPtr pMarkOnL = ppMarks->Add(ksDrMarkOnLeader);
  if (!pMarkOnL)
    return;

  pMarkOnL->SetX(150);
  pMarkOnL->SetY(150);
  pMarkOnL->SetArrowType(ksLeaderArrow);
  pMarkOnL->SetShelfDirection(ksLSLeft);
  pMarkOnL->AddBranchByPoint(1 /*branchIndex*/, false /*branchBegin*/, 100 /*x*/, 100 /*y*/);

  // Получить интерфейс текста справа и установить строку для него.
  ksapi::ITextPtr pTextAfter = pMarkOnL->GetTextAfter();
  pTextAfter->SetStr(L"  текст после");

  // Получить интерфейс текста слева и установить строку для него.
  ksapi::ITextPtr pTextBefore = pMarkOnL->GetTextBefore();
  pTextBefore->SetStr(L"текст до  ");

  // Получить интерфейс текста под полкой и установить строку для него.
  ksapi::ITextPtr pTextUnder = pMarkOnL->GetTextUnder();
  pTextUnder->SetStr(L"Текст под полкой  ");

  pMarkOnL->Update(); // Обязательный вызов для создания объекта в Компасе.

  // Показать сообщение на экране.
  kompasApp->ShowMessageBox(L"Марка с линией-выноской" /*text*/, kLibName /*caption*/, ksMessageWarning, ksButtonSetOk,
                            true /*showModal*/);
}


//------------------------------------------------------------------------------
/**
  Создать марку/позиционное обозначение.
*/
//---
void NewMarkOnLine()
{
  ksapi::IViewsPtr pViews = GetViewsPtr();
  if (!pViews)
    return;

  //Получить указатель на контейнер объектов СПДС (он же - активный вид).
  ksapi::IBuildingContainerPtr ppBuildingContainer = pViews->GetActiveView();
  if (!ppBuildingContainer)
    return;

  //Получить указатель на коллекцию марок
  ksapi::IMarksPtr pMarks(ppBuildingContainer->GetMarks());
  if (!pMarks)
    return;

  //Получить указатель на марку
  ksapi::IMarkOnLinePtr pMarkOnLine(pMarks->Add(ksDrMarkOnLine));
  if (!pMarkOnLine)
    return;

  // Получить интерфейс текста справа и установить строку для него.
  ksapi::ITextPtr pTextAfter(pMarkOnLine->GetTextAfter());
  pTextAfter->SetStr(L"  текст после");


  //построение отрезка - объект для размещения марки

  ksapi::ILineSegmentsPtr pLineSegments = GetILineSegments();
  ksapi::ILineSegmentPtr pLineSegment(pLineSegments ? pLineSegments->Add() : ksapi::ILineSegmentPtr{});
  if (!pLineSegment)
    return;

  pLineSegment->SetX1(100);
  pLineSegment->SetY1(100);
  pLineSegment->SetX2(150);
  pLineSegment->SetY2(110);
  pLineSegment->SetStyle(ksCurveStyleEnum::ksCSNormal);

  pLineSegment->Update(); // Создаём отрезок в Компасе перед добавлением в марку.

  pMarkOnLine->SetLine(pLineSegment);
  pMarkOnLine->SetPosition(ksMTextOnLine);

  pMarkOnLine->Update(); // Обязательный вызов для создания объекта в Компасе.

  // Показать сообщение на экране.
  kompasApp->ShowMessageBox(L"Марка/позиционное обозначение" /*text*/, kLibName /*caption*/, ksMessageWarning, ksButtonSetOk,
                            true /*showModal*/);
}


//------------------------------------------------------------------------------
/**
  Создать выносную надпись к многослойным конструкциям.
*/
//---
void NewMultiTextLeader()
{
  ksapi::IViewsPtr ppViews = GetViewsPtr();
  if (!ppViews)
    return;

  //Получить указатель на интерфейс активного вида
  ksapi::IViewPtr ppView(ppViews->GetActiveView());

  //Получить указатель на контейнер объектов СПДС
  ksapi::IBuildingContainerPtr ppBuildingContainer(ppView);

  if (!ppBuildingContainer)
    return;

  //Получить указатель на коллекцию выносных линий к многослойным объектам (ВЛКМО)
  ksapi::IMultiTextLeadersPtr pMTLs(ppBuildingContainer->GetMultiTextLeaders());
  if (!pMTLs)
    return;

  //Получить указатель на ВЛКМО
  ksapi::IMultiTextLeaderPtr pMTL(pMTLs->Add());
  ksapi::IBranchsPtr pMTLBranchs = pMTL; // Является дополнительным интерфейсом для управления ответвлениями.
  if (!pMTLBranchs)
    return;

  pMTL->SetForm(true);
  pMTL->SetArrowType(ksLeaderArrow);
  pMTL->SetTextDirection(true); // true - вверх
  pMTL->SetShelfDirection(ksLSRight);

  // Координаты начала полки.
  pMTLBranchs->SetX0(150);
  pMTLBranchs->SetY0(150);

  // Добавим ответвления.
  pMTLBranchs->AddBranchByPoint(1 /*index*/, 100 /*x*/, 100 /*y*/);
  pMTLBranchs->AddBranchByPoint(-1 /*index*/, 120 /*x*/, 100 /*y*/); // Индекс -1 - в конец
  pMTLBranchs->AddBranchByPoint(-1 /*index*/, 130 /*x*/, 100 /*y*/);

  // Добавление текста выносной надписи
  ksapi::ITextPtr pText1(pMTL->GetText());
  pText1->SetStr(L"текст выносной надписи1");

  pMTL->Update(); // Обязательный вызов для создания объекта в Компасе.

  // Показать сообщение на экране.
  kompasApp->ShowMessageBox(L"Выносная надпись к многослойным конструкциям" /*text*/, kLibName /*caption*/, ksMessageWarning,
                            ksButtonSetOk, true /*showModal*/);
}


//------------------------------------------------------------------------------
/**
  Создать фигурную скобку.
*/
//---
void NewBrace()
{
  ksapi::IViewsPtr ppViews = GetViewsPtr();
  if (!ppViews)
    return;

  //Получить указатель на интерфейс активного вида
  ksapi::IViewPtr ppView(ppViews->GetActiveView());

  //Получить указатель на контейнер объектов СПДС
  ksapi::IBuildingContainerPtr ppBuildingContainer(ppView);
  if (!ppBuildingContainer)
    return;

  //Получить указатель на коллекцию фигурных скобок
  ksapi::IBracesPtr pBraces(ppBuildingContainer->GetBraces());
  if (!pBraces)
    return;

  //Получить указатель на фигурную скобку
  ksapi::IBracePtr pBrace(pBraces->Add());
  if (!pBrace)
    return;

  pBrace->SetX1(50);
  pBrace->SetY1(200);
  pBrace->SetAlignment(ksATArbitrary);
  pBrace->SetAngle(200);
  pBrace->SetShelfDirection(ksLSRight);
  pBrace->SetDirection(true);
  pBrace->SetLength(100);
  pBrace->SetRadius(25);
  pBrace->SetStyle(7);

  ksapi::ITextPtr pText1(pBrace->GetText());
  pText1->SetStr(L"текст фигурной скобки");

  pBrace->Update(); // Обязательный вызов для создания объекта в Компасе.

  // Показать сообщение на экране.
  kompasApp->ShowMessageBox(L"Фигурная скобка" /*text*/, kLibName /*caption*/, ksMessageWarning, ksButtonSetOk,
                            true /*showModal*/);
}


//------------------------------------------------------------------------------
/**
  Получить указатель на интерфейс коллекции видов.
*/
//---
ksapi::IViewsPtr GetViewsPtr()
{
  if (kompasApp)
    // Получить указатель на интерфейс документа Компас
    if (ksapi::IKompasDocument2DPtr pKompasDocument2D = kompasApp->GetActiveDocument())
      //Получить указатель на интерфейс менеджера видов и слоёв
      if (ksapi::IViewsAndLayersManagerPtr pViewsAndLayersManager = pKompasDocument2D->GetViewsAndLayersManager())
        return pViewsAndLayersManager->GetViews();

  return nullptr; // Преобразуется к KsAPI умному указателю, содержащему nullptr вместо объекта.
}


//------------------------------------------------------------------------------
/**
  Получить указатель на коллекцию отрезков.
*/
//---
ksapi::ILineSegmentsPtr GetILineSegments()
{
  ksapi::ILineSegmentsPtr pLineSegments;

  if (ksapi::IKompasDocument2DPtr pKompasDocument2D = kompasApp->GetActiveDocument())
    if (ksapi::IViewsAndLayersManagerPtr pViewsAndLayersManager = pKompasDocument2D->GetViewsAndLayersManager())
      //Получить указатель на интерфейс коллекции видов
      if (ksapi::IViewsPtr pViews = pViewsAndLayersManager->GetViews())
        // Получить указатель на интерфейс активного вида (и получить интерфейс контейнера).
        if (ksapi::IDrawingContainerPtr pDrawingContainer = pViews->GetActiveView())
          //Получить указатель на коллекцию отрезков
          pLineSegments = pDrawingContainer->GetLineSegments();

  return pLineSegments;
}
