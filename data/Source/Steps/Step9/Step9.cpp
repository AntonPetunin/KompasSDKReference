////////////////////////////////////////////////////////////////////////////////
//
// step9.cpp - Размеры и другие технологические объекты
//
// 1.  Линейный размер             - DrawLineDimension
// 2.  Угловой размер              - DrawAngleDimension
// 3.  Шероховатость               - DrawRough
// 4.  Линия выноски               - DrawLeader
// 5.  Позиционная линия выноски   - DrawPosLeader
// 6.  Клеймение                   - DrawBrandLeader
// 7.  Маркирование                - DrawMarkerLeader
// 8.  Обозначение базы            - DrawBase
// 9.  Линия разреза/cечения       - DrawCutLine
// 10. Диаметральный размер        - DrawDiametralDimension
// 11. Радиальный размер           - DrawRadialDimension
// 12. Радиальный размер c изломом - DrawRadBreakDimension
// 13. Стрелка вида                - DrawViewPointer
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KompasLibraryActions.h>
#include <KsAPI.h>

ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void DrawLineDimension(ksapi::IKompasDocument2D & document);      // Линейный размер
void DrawAngleDimension(ksapi::IKompasDocument2D & document);     // Угловой размер
void DrawRough(ksapi::IKompasDocument2D & document);              // Шероховатость
void DrawLeader(ksapi::IKompasDocument2D & document);             // Линия выноски
void DrawPosLeader(ksapi::IKompasDocument2D & document);          // Позиционная линия выноски
void DrawBrandLeader(ksapi::IKompasDocument2D & document);        // Клеймение
void DrawMarkerLeader(ksapi::IKompasDocument2D & document);       // Маркирование
void DrawBase(ksapi::IKompasDocument2D & document);               // Обозначение базы
void DrawCutLine(ksapi::IKompasDocument2D & document);            // Линия разреза/cечения
void DrawDiametralDimension(ksapi::IKompasDocument2D & document); // Диаметральный размер
void DrawRadialDimension(ksapi::IKompasDocument2D & document);    // Радиальный размер
void DrawRadBreakDimension(ksapi::IKompasDocument2D & document);  // Радиальный размер c изломом
void DrawViewPointer(ksapi::IKompasDocument2D & document);        // Стрелка вида

//-------------------------------------------------------------------------------
// Перечисление для показа определённых параметров функциями ShowDimension
// ---
enum class EParams
{
  All,
  Draw,
  Snap,
  Text
};

//-------------------------------------------------------------------------------
// Описания вспомогательных функций
// ---
template<EParams kParams = EParams::All>
void ShowLineDimension(const std::wstring & caption, ksapi::ILineDimensionPtr lineDim);
template<EParams kParams = EParams::All>
void ShowAngleDimension(const std::wstring & caption, ksapi::IAngleDimensionPtr angleDim);
template<typename IDiametralRadialDimensionPtr>
void ShowDiametralRadialDimension(const std::wstring & caption, IDiametralRadialDimensionPtr diamDim);
void ShowBreakRadialDimension(const std::wstring & caption, ksapi::IBreakRadialDimensionPtr diamDim);

ksapi::ISymbols2DContainerPtr GetSymbolsContainer(ksapi::IKompasDocument2D & document);
void ShowDimensionTextParam(const std::wstring & caption, ksapi::IDimensionTextPtr textParams);
void ShowObjects(ksapi::IKompasDocument2D & document, const std::vector<ksapi::IKompasAPIObjectPtr> & objects);


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
      DrawLineDimension(*document);
      break;
    case 2:
      DrawAngleDimension(*document);
      break;
    case 3:
      DrawRough(*document);
      break;
    case 4:
      DrawLeader(*document);
      break;
    case 5:
      DrawPosLeader(*document);
      break;
    case 6:
      DrawBrandLeader(*document);
      break;
    case 7:
      DrawMarkerLeader(*document);
      break;
    case 8:
      DrawBase(*document);
      break;
    case 9:
      DrawCutLine(*document);
      break;
    case 10:
      DrawDiametralDimension(*document);
      break;
    case 11:
      DrawRadialDimension(*document);
      break;
    case 12:
      DrawRadBreakDimension(*document);
      break;
    case 13:
      DrawViewPointer(*document);
      break;
  }
}

//------------------------------------------------------------------------------
/**
  Линейный размер.
*/
//---
void DrawLineDimension(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;

  ksapi::ILineDimensionsPtr lineDimensions = symbolsContainer->GetLineDimensions();
  if (!lineDimensions)
    return;
  ksapi::ILineDimensionPtr lineDimension = lineDimensions->Add();
  ksapi::IDimensionParamsPtr dimensionParams = lineDimension;
  ksapi::IDimensionTextPtr dimensionText = dimensionParams;
  if (!dimensionText)
    return;

  lineDimension->SetX1(50); // Первая точка размера.
  lineDimension->SetY1(50);
  lineDimension->SetX2(70); // Вторая точка размера.
  lineDimension->SetY2(60);
  lineDimension->SetX3(50); // Точка, определяющая положение размерной линии.
  lineDimension->SetY3(30);
  lineDimension->SetOrientation(ksLinDHorizontal);

  dimensionParams->SetRemoteLine1(/*draw*/ true);  // Признак отрисовки первой выносной линии.
  dimensionParams->SetRemoteLine2(/*draw*/ true);  // Признак отрисовки второй выносной линии.
  dimensionParams->SetArrowType1(ksArrow);         // Тип стрелки у первой выносной линии.
  dimensionParams->SetArrowType2(ksArrow);         // Тип стрелки у второй выносной линии.
  dimensionParams->SetShelfDirection(ksLSNone);    // Тип полки
  dimensionParams->SetShelfAngle(-30);             // Угол наклона ножки выносной полки.
  dimensionParams->SetShelfLength(20);             // Длина ножки выносной полки.
  dimensionParams->SetArrowPos(ksDimArrowOutside); // Стрелки снаружи размера

  dimensionText->SetStyle(0);                                // Cтиль текста размера (0 - стиль по умолчанию).
  dimensionText->SetSign(0);                                 // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                                                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).
  dimensionText->SetAutoNominalValue(true);                  // Автоматически определять номинальное значение.
  dimensionText->SetDeviationOn(true);                       // Включить в текст отклонения.
  dimensionText->SetDeviationType(ksDimDeviation);           // Тип отображаемого отклонения.
  dimensionText->InitDeviations(/*high*/ 0.5, /*low*/ -0.5); // Значения отклонений.

  // Инициализация остальных текстовых значений.
  if (ksapi::ITextLinePtr suffix = dimensionText->GetSuffix())
    suffix->SetStr(L"suffix");
  dimensionText->GetPrefix();
  if (ksapi::ITextLinePtr prefix = dimensionText->GetPrefix())
    prefix->SetStr(L"prefix");
  if (ksapi::ITextLinePtr unit = dimensionText->GetUnit())
    unit->SetStr(L"mm"); // Единицы измерения.

  dimensionParams->SetTextBase(ksDimBaseP2); // Параметр отрисовки текста
  dimensionParams->SetTextPos(10);           // Положение текста (0 - автоматическое размещение текста,
                                             // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                             // < 0 - на указанное расстояние в направлении от второй точки к первой).

  lineDimension->Update();

  ShowLineDimension(L"Создание размера", lineDimension);

  dimensionParams->SetShelfDirection(ksLSNone);    // Убрать полку.
  dimensionParams->SetRemoteLine1(/*draw*/ false); // Признак отрисовки первой выносной линии.
  dimensionParams->SetRemoteLine2(/*draw*/ false); // Признак отрисовки второй выносной линии.
  dimensionParams->SetArrowPos(ksDimArrowInside);  // Стрелки внутри размера
  dimensionParams->SetTextBase(ksDimBaseP2);       // Точка привязки текста.
  dimensionParams->SetTextPos(1);                  // Положение текста (0 - автоматическое размещение текста,
                                                   // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                                   // < 0 - на указанное расстояние в направлении от второй точки к первой).

  lineDimension->Update();

  ShowLineDimension<EParams::Draw>(L"Редактирование размера", dimensionParams);
}

//------------------------------------------------------------------------------
/**
  Угловой размер.
*/
//---
void DrawAngleDimension(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;

  ksapi::IAngleDimensionsPtr angleDimensions = symbolsContainer->GetAngleDimensions();
  if (!angleDimensions)
    return;
  ksapi::IAngleDimensionPtr angleDimension = angleDimensions->Add(DrawingObjectTypeEnum::ksDrADimension);
  ksapi::IDimensionParamsPtr dimensionParams = angleDimension;
  ksapi::IDimensionTextPtr dimensionText = dimensionParams;
  if (!dimensionText)
    return;

  angleDimension->SetXc(40); // Координаты центра размерной дуги.
  angleDimension->SetYc(0);
  angleDimension->SetX1(40); // Координаты точки выхода первой выносной линии.
  angleDimension->SetY1(40);
  angleDimension->SetX2(60); // Координаты точки выхода второй выносной линии.
  angleDimension->SetY2(20);
  angleDimension->SetAngle1(0); // Начальный угол размерной дуги (0 - используются точки выхода выносной линии).
  angleDimension->SetAngle2(0); // Конечный угол размерной дуги (0 - используются точки выхода выносной линии).
  angleDimension->SetRadius(50);
  angleDimension->SetDimensionType(ksADMaxAngle);
  angleDimension->SetDirection(false);

  dimensionParams->SetRemoteLine1(/*draw*/ true); // Признак отрисовки первой выносной линии.
  dimensionParams->SetRemoteLine2(/*draw*/ true); // Признак отрисовки второй выносной линии.
  dimensionParams->SetArrowType1(ksArrow);        // Не рисовать стрелку у первой выносной линии.
  dimensionParams->SetArrowType2(ksArrow);        // Не рисовать стрелку у второй выносной линии.
  dimensionParams->SetShelfDirection(ksLSNone);   // Тип полки.
  dimensionParams->SetArrowPos(ksDimArrowInside); // Стрелки внутри размера
  dimensionParams->SetShelfAngle(30);             // Угол наклона ножки выносной полки
  dimensionParams->SetShelfLength(20);            // Длина ножки выносной полки

  dimensionText->SetStyle(0);                                // Cтиль текста размера (0 - стиль по умолчанию).
  dimensionText->SetSign(0);                                 // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                                                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).
  dimensionText->SetAutoNominalValue(true);                  // Автоматически определять номинальное значение.
  dimensionText->SetDeviationOn(true);                       // Включить в текст отклонения.
  dimensionText->SetDeviationType(ksDimDeviation);           // Тип отображаемого отклонения.
  dimensionText->InitDeviations(/*high*/ 0.3, /*low*/ -0.3); // Значения отклонений.

  // Инициализация остальных текстовых значений.
  if (ksapi::ITextLinePtr suffix = dimensionText->GetSuffix())
    suffix->SetStr(L"suffix");
  dimensionText->GetPrefix();
  if (ksapi::ITextLinePtr prefix = dimensionText->GetPrefix())
    prefix->SetStr(L"prefix");
  if (ksapi::ITextLinePtr unit = dimensionText->GetUnit())
    unit->SetStr(L"гр"); // Единицы измерения.

  dimensionParams->SetTextBase(ksDimBaseCenter); // Параметр отрисовки текста
  dimensionParams->SetTextPos(-10);              // Положение текста (0 - автоматическое размещение текста,
                                                 // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                                 // < 0 - на указанное расстояние в направлении от второй точки к первой).
  angleDimension->Update();

  ShowAngleDimension(L"Создание размера", angleDimension);

  angleDimension->SetXc(40);                      // Координаты центра размерной дуги.
  angleDimension->SetYc(60);                      // Координаты центра размерной дуги.
  angleDimension->SetX1(10);                      // Координаты точки выхода первой выносной линии.
  angleDimension->SetY1(60);                      // Координаты точки выхода первой выносной линии.
  angleDimension->SetX2(20);                      // Координаты точки выхода второй выносной линии.
  angleDimension->SetY2(80);                      // Координаты точки выхода второй выносной линии.
  angleDimension->SetAngle1(0);                   // Начальный угол размерной дуги (0 - используются точки выхода выносной линии).
  angleDimension->SetAngle2(0);                   // Конечный угол размерной дуги (0 - используются точки выхода выносной линии).
  angleDimension->SetRadius(50);                  // Радиус размерной дуги
  angleDimension->SetDimensionType(ksADMinAngle); // Тип угла из возможных.
  angleDimension->SetDirection(true);

  angleDimension->Update();

  ShowAngleDimension<EParams::Snap>(L"Редактирование размера", angleDimension);
}

//------------------------------------------------------------------------------
/**
  Шероховатость.
*/
//---
void DrawRough(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IRoughsPtr roughs = symbolsContainer->GetRoughs();
  if (!roughs)
    return;
  ksapi::IRoughPtr rough = roughs->Add();
  ksapi::IRoughParamsPtr roughParams = rough;
  if (!roughParams)
    return;

  roughParams->SetShelfDirection(ksLSNone);     // Полка отсутствует
  roughParams->SetLeaderAngle(130);             // Угол наклона линии-выноски.
  roughParams->SetLeaderLength(20);             // Длина линии-выноски.
  roughParams->SetProcessingByContour(false);   // Не применять для контура.
  roughParams->SetSignType(ksNoProcessingType); // Без указания типа обработки на знаке

  // Параметры шероховатости.
  if (ksapi::ITextPtr text = roughParams->GetRoughParamText())
    text->SetStr(L"00_100_2");

  // Способ обработки поверхности и др. указания.
  if (ksapi::ITextPtr text = roughParams->GetProcessText())
    text->SetStr(L"11_111_2");

  // Базовая длина (под знаком, посередине).
  if (ksapi::ITextPtr text = roughParams->GetBaseLengthText())
    text->SetStr(L"22_122_2");

  // Направление неровностей (под знаком слева).
  if (ksapi::ITextPtr text = roughParams->GetTrendText())
    text->SetStr(L"33_1");

  // Тока привязки обозначения шероховатости.
  rough->SetShelfX(50);
  rough->SetShelfY(50);

  rough->Update();

  ShowObjects(document, {rough});
}

//------------------------------------------------------------------------------
/**
  Линия выноски.
*/
//---
void DrawLeader(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ILeadersPtr leaders = symbolsContainer->GetLeaders();
  if (!leaders)
    return;
  // Линия выноски реализуется несколькими интерфейсами.
  ksapi::ILeaderPtr leader = leaders->Add(DrawingObjectTypeEnum::ksDrLeader);
  ksapi::IBaseLeaderPtr baseLeader = leader;
  ksapi::IBranchsPtr branchsLeader = baseLeader;
  if (!branchsLeader)
    return;

  baseLeader->SetArrowType(ksArrowEnum::ksLeaderPoint); // Точка вместо стрелки.

  leader->SetSignType(ksLSignNone);     // Без знака.
  leader->SetShelfDirection(ksLSRight); // Направление полки - влево.
  leader->SetArround(false);            // Не применять для контура.
  leader->SetSignType(ksLSignNone);     // Знак отсутствует

  if (ksapi::ITextPtr text = leader->GetTextOnShelf()) // Текст над полкой.
    text->SetStr(L"00_1");
  if (ksapi::ITextPtr text = leader->GetTextUnderShelf()) // Текст под полкой.
    text->SetStr(L"11_1");
  if (ksapi::ITextPtr text = leader->GetTextOnBranch()) // Текст над ножкой.
    text->SetStr(L"22_1");
  if (ksapi::ITextPtr text = leader->GetTextUnderBranch()) // Текст под ножкой.
    text->SetStr(L"33_1");

  branchsLeader->SetX0(50); // Базовая точка линии выноски (начало ответвлений).
  branchsLeader->SetY0(50);
  // Ветки от базовой точки, через массив указанных точек (points). Индекс -1 - добавить ветку в конец массива.
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {10, 10});
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {30, 10});

  baseLeader->Update();

  ShowObjects(document, {leader});
}

//------------------------------------------------------------------------------
/**
  Позиционная линия выноски 
*/
//---
void DrawPosLeader(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ILeadersPtr leaders = symbolsContainer->GetLeaders();
  if (!leaders)
    return;
  ksapi::IPositionLeaderPtr posLeader = leaders->Add(DrawingObjectTypeEnum::ksDrPosLeader);
  ksapi::IBaseLeaderPtr baseLeader = posLeader;
  ksapi::IBranchsPtr branchsLeader = baseLeader;
  if (!branchsLeader)
    return;

  baseLeader->SetArrowType(ksArrowEnum::ksLeaderPoint); // Точка вместо стрелки.

  posLeader->SetShelfDirection(ksShelfDirectionEnum::ksLSLeft); // Направление полки - вправо.

  if (ksapi::ITextPtr text = posLeader->GetPositions())
    text->SetStr(L"12");

  branchsLeader->SetX0(50); // Базовая точка линии выноски (начало ответвлений).
  branchsLeader->SetY0(50);
  // Ветки от базовой точки, через массив указанных точек (points). Индекс -1 - добавить ветку в конец массива.
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {10, 10});
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {30, 10});

  baseLeader->Update();

  ShowObjects(document, {posLeader});
}

//------------------------------------------------------------------------------
/**
  Клеймение.
*/
//---
void DrawBrandLeader(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ILeadersPtr leaders = symbolsContainer->GetLeaders();
  if (!leaders)
    return;
  ksapi::IBrandLeaderPtr brandLeader = leaders->Add(DrawingObjectTypeEnum::ksDrBrandLeader);
  ksapi::IBaseLeaderPtr baseLeader = brandLeader;
  ksapi::IBranchsPtr branchsLeader = baseLeader;
  if (!branchsLeader)
    return;

  baseLeader->SetArrowType(ksArrowEnum::ksLeaderPoint);

  brandLeader->SetDirection(true); // Расположение знака относительно точки привязки

  // Текст обозначения.
  if (ksapi::ITextPtr text = brandLeader->GetDesignation())
    text->SetStr(L"п.11");
  // Текст над ножкой.
  if (ksapi::ITextPtr text = brandLeader->GetTextOnBranch())
    text->SetStr(L"Ну");
  // Текст под ножкой.
  if (ksapi::ITextPtr text = brandLeader->GetTextUnderBranch())
    text->SetStr(L"Ту");

  branchsLeader->SetX0(50); // Базовая точка линии выноски (начало ответвлений).
  branchsLeader->SetY0(50);
  // Ветки от базовой точки, через массив указанных точек (points). Индекс -1 - добавить ветку в конец массива.
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {10, 10});
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {30, 10});

  baseLeader->Update();

  ShowObjects(document, {brandLeader});
}

//------------------------------------------------------------------------------
/**
  Маркирование.
*/
//---
void DrawMarkerLeader(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ILeadersPtr leaders = symbolsContainer->GetLeaders();
  if (!leaders)
    return;
  ksapi::IMarkLeaderPtr markLeader = leaders->Add(DrawingObjectTypeEnum::ksDrMarkerLeader);
  ksapi::IBaseLeaderPtr baseLeader = markLeader;
  ksapi::IBranchsPtr branchsLeader = baseLeader;
  if (!branchsLeader)
    return;

  baseLeader->SetArrowType(ksArrowEnum::ksArrow);

  // Текст обозначения.
  if (ksapi::ITextPtr text = markLeader->GetDesignation())
    text->SetStr(L"п.11");
  // Текст над ножкой.
  if (ksapi::ITextPtr text = markLeader->GetTextOnBranch())
    text->SetStr(L"Ну");
  // Текст под ножкой.
  if (ksapi::ITextPtr text = markLeader->GetTextUnderBranch())
    text->SetStr(L"Ту");

  branchsLeader->SetX0(50); // Базовая точка линии выноски (начало ответвлений).
  branchsLeader->SetY0(50);
  // Ветки от базовой точки, через массив указанных точек (points). Индекс -1 - добавить ветку в конец массива.
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {10, 10});
  branchsLeader->AddBranch(/*index*/ -1, /*points*/ {30, 10});

  baseLeader->Update();

  ShowObjects(document, {markLeader});
}

//------------------------------------------------------------------------------
/**
  Обозначение базы.
*/
//---
void DrawBase(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IBasesPtr bases = symbolsContainer->GetBases();
  if (!bases)
    return;
  ksapi::IBasePtr base = bases->Add();
  if (!base)
    return;

  // Координаты базовой точки (начало ножки).
  base->SetX0(50);
  base->SetY0(50);
  // Координаты конечной точки ножки.
  base->SetBranchX(50);
  base->SetBranchY(30);

  if (ksapi::ITextPtr text = base->GetText())
    text->SetStr(L"ABC");

  base->Update();

  ShowObjects(document, {base});
}

//------------------------------------------------------------------------------
/**
  Линия разреза/cечения
*/
//---
void DrawCutLine(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::ICutLinesPtr cutLines = symbolsContainer->GetCutLines();
  if (!cutLines)
    return;
  ksapi::ICutLinePtr cutLine = cutLines->Add();
  if (!cutLine)
    return;

  cutLine->SetArrowPos(false); // Положение стрелок - справа по направлению ломаной.
  cutLine->SetX1(30);          // Координаты надписи у первого участка.
  cutLine->SetY1(65);
  cutLine->SetX2(95); // Координаты надписи у второго участка.
  cutLine->SetY2(15);

  cutLine->SetPoints({50, 50, 50, 30, 80, 30}); // координаты точек линий разреза

  if (ksapi::ITextPtr text = cutLine->GetText())
    text->SetStr(L"A$;1$");

  cutLine->Update();

  ShowObjects(document, {cutLine});
}

//------------------------------------------------------------------------------
/**
  Диаметральный размер.
*/
//---
void DrawDiametralDimension(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IDiametralDimensionsPtr diametralDimensions = symbolsContainer->GetDiametralDimensions();
  if (!diametralDimensions)
    return;
  ksapi::IDiametralDimensionPtr diametralDimension = diametralDimensions->Add();
  ksapi::IDimensionParamsPtr dimensionParams = diametralDimension;
  ksapi::IDimensionTextPtr dimensionText = dimensionParams;
  if (!dimensionText)
    return;

  // Привязка диаметрального размера
  diametralDimension->SetXc(50);
  diametralDimension->SetYc(50);
  diametralDimension->SetRadius(70);
  diametralDimension->SetDimensionType(false); // Не от центра

  dimensionParams->SetRemoteLine1(/*draw*/ true);  // Признак отрисовки первой выносной линии.
  dimensionParams->SetRemoteLine2(/*draw*/ false); // Признак отрисовки второй выносной линии.
  dimensionParams->SetArrowType1(ksArrow);         // Рисовать стрелку у первой выносной линии.
  dimensionParams->SetArrowPos(ksDimArrowOutside); // Положение стрекли
  dimensionParams->SetShelfDirection(ksLSRight);   // Тип полки.
  dimensionParams->SetShelfAngle(-30);             // Угол наклона ножки выносной полки

  dimensionText->SetStyle(0);                                // Cтиль текста размера (0 - стиль по умолчанию).
  dimensionText->SetSign(0);                                 // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                                                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).
  dimensionText->SetAutoNominalValue(true);                  // Автоматически определять номинальное значение.
  dimensionText->SetDeviationOn(true);                       // Включить в текст отклонения.
  dimensionText->SetDeviationType(ksDimDeviation);           // Тип отображаемого отклонения.
  dimensionText->InitDeviations(/*high*/ 0.5, /*low*/ -0.5); // Значения отклонений.

  // Инициализация остальных текстовых значений.
  if (ksapi::ITextLinePtr suffix = dimensionText->GetSuffix())
    suffix->SetStr(L"suffix");
  dimensionText->GetPrefix();
  if (ksapi::ITextLinePtr prefix = dimensionText->GetPrefix())
    prefix->SetStr(L"prefix");
  if (ksapi::ITextLinePtr unit = dimensionText->GetUnit())
    unit->SetStr(L"mm"); // Единицы измерения.

  dimensionParams->SetTextBase(ksDimBaseCenter); // Параметр отрисовки текста
  dimensionParams->SetTextPos(75);

  diametralDimension->Update();

  ShowDiametralRadialDimension(L"Создание размера", diametralDimension);

  // Привязка размера.
  diametralDimension->SetXc(30);
  diametralDimension->SetYc(30);
  diametralDimension->SetRadius(20);

  dimensionText->SetSign(1); // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).

  dimensionParams->SetShelfAngle(0);              // Угол наклона ножки выносной полки
  dimensionParams->SetArrowPos(ksDimArrowInside); // Положение стрекли
  dimensionParams->SetShelfDirection(ksLSRight);  // Тип полки.
  dimensionParams->SetTextBase(ksDimBaseCenter);  // Параметр отрисовки текста
  dimensionParams->SetTextPos(0);                 // Положение текста (0 - автоматическое размещение текста,
                                                  // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                                  // < 0 - на указанное расстояние в направлении от второй точки к первой).
  diametralDimension->Update();

  ShowDiametralRadialDimension(L"Редактирование размера", diametralDimension);
}

//------------------------------------------------------------------------------
/**
  Радиальный размер.
*/
//---
void DrawRadialDimension(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IRadialDimensionsPtr radialDimensions = symbolsContainer->GetRadialDimensions();
  if (!radialDimensions)
    return;
  ksapi::IRadialDimensionPtr radialDimension = radialDimensions->Add();
  ksapi::IDimensionParamsPtr dimensionParams = radialDimension;
  ksapi::IDimensionTextPtr dimensionText = dimensionParams;
  if (!dimensionText)
    return;

  // Привязка радиального размера
  radialDimension->SetXc(50);
  radialDimension->SetYc(50);
  radialDimension->SetRadius(70);
  radialDimension->SetDimensionType(false); // Не от центра

  dimensionParams->SetRemoteLine1(/*draw*/ true);  // Признак отрисовки первой выносной линии.
  dimensionParams->SetRemoteLine2(/*draw*/ false); // Признак отрисовки второй выносной линии.
  dimensionParams->SetArrowType1(ksArrow);         // Рисовать стрелку у первой выносной линии.
  dimensionParams->SetArrowPos(ksDimArrowOutside); // Положение стрекли
  dimensionParams->SetShelfDirection(ksLSRight);   // Тип полки.
  dimensionParams->SetShelfAngle(30);              // Угол наклона ножки выносной полки

  dimensionText->SetStyle(0);                                // Cтиль текста размера (0 - стиль по умолчанию).
  dimensionText->SetSign(0);                                 // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                                                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).
  dimensionText->SetAutoNominalValue(true);                  // Автоматически определять номинальное значение.
  dimensionText->SetDeviationOn(true);                       // Включить в текст отклонения.
  dimensionText->SetDeviationType(ksDimDeviation);           // Тип отображаемого отклонения.
  dimensionText->InitDeviations(/*high*/ 0.5, /*low*/ -0.5); // Значения отклонений.

  // Инициализация остальных текстовых значений.
  if (ksapi::ITextLinePtr suffix = dimensionText->GetSuffix())
    suffix->SetStr(L"suffix");
  dimensionText->GetPrefix();
  if (ksapi::ITextLinePtr prefix = dimensionText->GetPrefix())
    prefix->SetStr(L"prefix");
  if (ksapi::ITextLinePtr unit = dimensionText->GetUnit())
    unit->SetStr(L"mm"); // Единицы измерения.

  dimensionParams->SetTextBase(ksDimBaseCenter); // Параметр отрисовки текста
  dimensionParams->SetTextPos(75);               // Положение текста (0 - автоматическое размещение текста,
                                                 // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                                 // < 0 - на указанное расстояние в направлении от второй точки к первой).

  radialDimension->Update();

  ShowDiametralRadialDimension(L"Создание размера", radialDimension);

  // Привязка размера.
  radialDimension->SetXc(30);
  radialDimension->SetYc(30);
  radialDimension->SetRadius(30);

  dimensionText->SetSign(3); // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).

  dimensionParams->SetShelfAngle(0);              // Угол наклона ножки выносной полки
  dimensionParams->SetArrowPos(ksDimArrowInside); // Положение стрекли
  dimensionParams->SetShelfDirection(ksLSNone);   // Тип полки.
  dimensionParams->SetTextPos(0);                 // Положение текста (0 - автоматическое размещение текста,
                                                  // > 0 - на указанное расстояние в направлении от первой точки ко второй,
                                                  // < 0 - на указанное расстояние в направлении от второй точки к первой).
  radialDimension->Update();

  ShowDiametralRadialDimension(L"Редактирование размера", radialDimension);
}

//------------------------------------------------------------------------------
/**
  Радиальный размер c изломом.
*/
//---
void DrawRadBreakDimension(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IBreakRadialDimensionsPtr breakRadialDimensions = symbolsContainer->GetBreakRadialDimensions();
  if (!breakRadialDimensions)
    return;
  ksapi::IBreakRadialDimensionPtr breakRadialDimension = breakRadialDimensions->Add();
  ksapi::IDimensionParamsPtr dimensionParams = breakRadialDimension;
  ksapi::IDimensionTextPtr dimensionText = dimensionParams;
  if (!dimensionText)
    return;

  // Привязка радиального размера
  breakRadialDimension->SetXc(50);
  breakRadialDimension->SetYc(50);
  breakRadialDimension->SetRadius(70);
  breakRadialDimension->SetTextOnLine(ksDimTextParallelOnLine);

  dimensionParams->SetArrowType1(ksArrow);         // Рисовать стрелку у первой выносной линии.
  dimensionParams->SetArrowPos(ksDimArrowOutside); // Положение стрекли
  dimensionParams->SetShelfDirection(ksLSNone);    // Тип полки.
  dimensionParams->SetTextBase(ksDimBaseCenter);

  dimensionText->SetStyle(0);                                // Cтиль текста размера (0 - стиль по умолчанию).
  dimensionText->SetSign(0);                                 // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
                                                             // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).
  dimensionText->SetAutoNominalValue(true);                  // Автоматически определять номинальное значение.
  dimensionText->SetDeviationOn(true);                       // Включить в текст отклонения.
  dimensionText->SetDeviationType(ksDimDeviation);           // Тип отображаемого отклонения.
  dimensionText->InitDeviations(/*high*/ 0.5, /*low*/ -0.5); // Значения отклонений.

  // Инициализация остальных текстовых значений.
  if (ksapi::ITextLinePtr suffix = dimensionText->GetSuffix())
    suffix->SetStr(L"suffix");
  dimensionText->GetPrefix();
  if (ksapi::ITextLinePtr prefix = dimensionText->GetPrefix())
    prefix->SetStr(L"prefix");
  if (ksapi::ITextLinePtr unit = dimensionText->GetUnit())
    unit->SetStr(L"mm"); // Единицы измерения.

  //breakRadialDimension->SetAngle(-30);

  breakRadialDimension->SetBreakX1(44);
  breakRadialDimension->SetBreakY1(55);

  breakRadialDimension->SetBreakX2(35);
  breakRadialDimension->SetBreakY2(47);

  breakRadialDimension->SetAngle(30);

  breakRadialDimension->Update();

  ShowBreakRadialDimension(L"Создание размера", breakRadialDimension);

  dimensionParams->SetArrowPos(ksDimArrowInside); // Положение стрекли

  // Привязка размера.
  breakRadialDimension->SetXc(30);
  breakRadialDimension->SetYc(30);
  breakRadialDimension->SetRadius(40);

  breakRadialDimension->SetBreakX1(5);
  breakRadialDimension->SetBreakY1(19);

  breakRadialDimension->SetBreakX2(-3);
  breakRadialDimension->SetBreakY2(16);

  breakRadialDimension->SetAngle(30);

  dimensionText->SetSign(3); // Номер условного значка перед номиналом (0 - нет значка, 1 - диаметр, 2 - квадрат,
  // 3 - радиус, > 3 - номер значка из шрифта Symbol type A).

  dimensionParams->SetRemoteLine1(/*draw*/ false); // Признак отрисовки первой выносной линии.

  breakRadialDimension->Update();

  ShowBreakRadialDimension(L"Редактирование размера", breakRadialDimension);
}

//------------------------------------------------------------------------------
/**
  Стрелка вида.
*/
//---
void DrawViewPointer(ksapi::IKompasDocument2D & document)
{
  ksapi::ISymbols2DContainerPtr symbolsContainer = GetSymbolsContainer(document);
  if (!symbolsContainer)
    return;
  ksapi::IViewPointersPtr viewPointers = symbolsContainer->GetViewPointers();
  if (!viewPointers)
    return;
  ksapi::IViewPointerPtr viewPointer = viewPointers->Add();
  if (!viewPointer)
    return;

  // Координаты начала стрелки.
  viewPointer->SetX1(50);
  viewPointer->SetY1(50);
  // Координаты конца стрелки.
  viewPointer->SetX2(20);
  viewPointer->SetY2(50);
  // Координаты текста.
  viewPointer->SetTextX(25);
  viewPointer->SetTextY(55);

  if (ksapi::ITextPtr text = viewPointer->GetText())
    text->SetStr(L"AAA");

  viewPointer->Update();

  ShowObjects(document, {viewPointer});
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


//-------------------------------------------------------------------------------
// Получить контейнер условных обозначений.
// ---
ksapi::ISymbols2DContainerPtr GetSymbolsContainer(ksapi::IKompasDocument2D & document)
{
  ksapi::IViewsAndLayersManagerPtr viewsAndLayerManager = document.GetViewsAndLayersManager();
  if (!viewsAndLayerManager)
    return nullptr;
  ksapi::IViewsPtr views = viewsAndLayerManager->GetViews();
  if (!views)
    return nullptr;

  return views->GetActiveView();
}

//-------------------------------------------------------------------------------
// Вывести на экран значения текста размера.
// ---
void ShowDimensionTextParam(const std::wstring & caption, ksapi::IDimensionTextPtr textParams)
{
  if (!textParams)
    return;
  ksapi::ITextLinePtr text = textParams->GetNominalText();
  if (!text)
    return;

  std::wstring msg = caption + L'\n';

  if (ksapi::ITextLinePtr prefix = textParams->GetPrefix())
    msg += prefix->GetStr() + L" ";

  if (ksapi::ITextLinePtr highDeviation = textParams->GetHighDeviation())
    msg += highDeviation->GetStr() + L" ";

  if (ksapi::ITextLinePtr lowDeviation = textParams->GetLowDeviation())
    msg += lowDeviation->GetStr() + L" ";

  if (ksapi::ITextLinePtr units = textParams->GetUnit())
    msg += units->GetStr() + L" ";

  if (ksapi::ITextLinePtr suffix = textParams->GetSuffix())
    msg += suffix->GetStr();

  msg += std::format(L"\nПараметры надписи\nsign = {},\ndeviation = {}", textParams->GetSign(), textParams->IsDeviationOn());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
// Вывести на экран информацию о линейном размере.
// ---
template<EParams kParams>
void ShowLineDimension(const std::wstring & caption, ksapi::ILineDimensionPtr lineDim)
{
  if constexpr (kParams == EParams::All || kParams == EParams::Text)
    ShowDimensionTextParam(caption, lineDim);

  std::wstring msg;
  if constexpr (kParams == EParams::All || kParams == EParams::Snap)
  {
    // параметры привязки линейного размера
    msg = caption + L"\nПараметры привязки\n";
    ksapi::IDimension2DPtr dimension = lineDim;

    double x = 0, y = 0;
    dimension->GetDimensionPoint(ksDimensionBasePoint, 1, x, y);
    msg += std::format(L"x1 = {:3.2f}, y1 = {:3.2f},\n", x, y);

    dimension->GetDimensionPoint(ksDimensionBasePoint, 0, x, y);
    msg += std::format(L"x2 = {:3.2f}, y2 = {:3.2f},\n", x, y);

    dimension->GetDimensionPoint(ksDimensionLinePoint, 2, x, y);
    msg += std::format(L"x3 = {:3.2f}, y3 = {:3.2f},\n", x, y);
    ksLineDimensionOrientationEnum orientation = lineDim->GetOrientation();
    msg += L"ps = " + std::to_wstring(static_cast<int32_t>(orientation));

    kompasApp->ShowMessageBox(msg, caption + L"", ksMessageWarning, ksButtonSetOk, true);
  }

  if constexpr (kParams == EParams::All || kParams == EParams::Draw)
  {
    // Парметры отрисовки линейного размера
    msg = caption + L"\nПараметры отрисовки\n";
    ksapi::IDimensionParamsPtr params = lineDim;
    msg += std::
      format(L"pl1 = {}, pl2 = {},\npt1 = {}, pt2 = {},\ntextPos = {}, textBase = {},\nshelfDir = {}, ang = {:3.2f}, length = {}",
             params->IsRemoteLine1(), params->IsRemoteLine2(), static_cast<int32_t>(params->GetArrowType1()),
             static_cast<int32_t>(params->GetArrowType2()), params->GetTextPos(), static_cast<int32_t>(params->GetTextBase()),
             static_cast<int32_t>(params->GetShelfDirection()), params->GetShelfAngle(), params->GetShelfLength());
    kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
  }
}

//-------------------------------------------------------------------------------
// Вывести на экран информацию о угловом размере.
// ---
template<EParams kParams>
void ShowAngleDimension(const std::wstring & caption, ksapi::IAngleDimensionPtr angleDim)
{
  if constexpr (kParams == EParams::All || kParams == EParams::Text)
    ShowDimensionTextParam(caption, angleDim);

  std::wstring msg;
  if constexpr (kParams == EParams::All || kParams == EParams::Snap)
  {
    // Параметры привязки углового размера
    msg = caption + L"\nПараметры привязки\n";
    msg += std::format(L"xc = {:3.2f}, yc = {:3.2f},\nx1 = {:3.2f}, y1 = {:3.2f},\n"
                       "x2 = {:3.2f}, y2 = {:3.2f},\nang1 = {:3.2f}, ang2 = {:3.2f},\nrad = {:3.2f}, dir = {}",
                       angleDim->GetXc(), angleDim->GetYc(), angleDim->GetX1(), angleDim->GetY1(), angleDim->GetX2(),
                       angleDim->GetY2(), angleDim->GetAngle1(), angleDim->GetAngle2(), angleDim->GetRadius(),
                       angleDim->GetDirection());
    kompasApp->ShowMessageBox(msg, caption + L"", ksMessageWarning, ksButtonSetOk, true);
  }

  if constexpr (kParams == EParams::All || kParams == EParams::Draw)
  {
    // параметры отрисовки углового размера
    msg = caption + L"\nПараметры отрисовки\n";
    ksapi::IDimensionParamsPtr params = angleDim;
    msg += std::
      format(L"pl1 = {}, pl2 = {},\npt1 = {}, pt2 = {},\ntextPos = {}, textBase = {},\nshelfDir = {}, ang = {:3.2f}, length = {}",
             params->IsRemoteLine1(), params->IsRemoteLine2(), static_cast<int32_t>(params->GetArrowType1()),
             static_cast<int32_t>(params->GetArrowType2()), params->GetTextPos(), static_cast<int32_t>(params->GetTextBase()),
             static_cast<int32_t>(params->GetShelfDirection()), params->GetShelfAngle(), params->GetShelfLength());
    kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
  }
}

//-------------------------------------------------------------------------------
// Вывести на экран информацию о диаметральном или радиальном размере.
// ---
template<typename IDiametralRadialDimensionPtr>
void ShowDiametralRadialDimension(const std::wstring & caption, IDiametralRadialDimensionPtr diamDim)
{
  ShowDimensionTextParam(caption, diamDim);

  std::wstring msg;
  // Параметры привязки диаметрального размера
  msg = caption + L"\nПараметры привязки\n";
  msg += std::format(L"xc = {:3.2f}, yc = {:3.2f},\nrad = {:3.2f}", diamDim->GetXc(), diamDim->GetYc(), diamDim->GetRadius());
  kompasApp->ShowMessageBox(msg, caption + L"", ksMessageWarning, ksButtonSetOk, true);

  // параметры отрисовки диаметрального размера
  msg = caption + L"\nПараметры отрисовки\n";
  ksapi::IDimensionParamsPtr params = diamDim;
  msg += std::format(L"pl1 = {}, pl2 = {},\n\ntextPos = {}, shelfDir = {},\n ang = {:3.2f}", params->IsRemoteLine1(),
                     params->IsRemoteLine2(), params->GetTextPos(), static_cast<int32_t>(params->GetShelfDirection()),
                     diamDim->GetAngle());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
}


//-------------------------------------------------------------------------------
// Вывести на экран информацию о радиальном размере с изломом размере.
// ---
void ShowBreakRadialDimension(const std::wstring & caption, ksapi::IBreakRadialDimensionPtr radBreakDim)
{
  ShowDimensionTextParam(caption, radBreakDim);

  std::wstring msg;
  // Параметры привязки диаметрального размера
  msg = caption + L"\nПараметры привязки\n";
  msg += std::format(L"xc = {:3.2f}, yc = {:3.2f},\nrad = {:3.2f}", radBreakDim->GetXc(), radBreakDim->GetYc(),
                     radBreakDim->GetRadius());
  kompasApp->ShowMessageBox(msg, caption + L"", ksMessageWarning, ksButtonSetOk, true);

  // Параметры отрисовки размера
  msg = caption + L"\nПараметры отрисовки\n";
  ksapi::IDimensionParamsPtr params = radBreakDim;
  msg += std::format(L"pt = {},\nang = {:3.2f}, pb = {}", static_cast<int32_t>(params->GetArrowType1()), radBreakDim->GetAngle(),
                     radBreakDim->GetBreakLength());
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
}


//-------------------------------------------------------------------------------
// Подсветить объект.
// ---
void ShowObjects(ksapi::IKompasDocument2D & document, const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  if (objects.empty())
    return;
  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
  if (!chooseManager)
    return;

  chooseManager->Choose(objects);
  kompasApp->ShowMessageBox(std::format(L"Снять выделение объект{}", (objects.size() == 1 ? L"а" : L"ов")), L"", ksMessageWarning,
                            ksButtonSetOk, true);
  chooseManager->Unchoose(objects);
}
