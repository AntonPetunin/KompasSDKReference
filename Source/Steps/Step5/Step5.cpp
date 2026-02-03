////////////////////////////////////////////////////////////////////////////////
//
// Step5.cpp - Редактирование
//
// 1.  Трансформация объекта                      - DrawTransform
// 2.  Копия объекта                              - DrawCopy
// 3.  Симметрия объекта                          - DrawSymmetry
// 4.  Просмотр/редактирование допуска формы      - EditTolerance
// 5.  Просмотр/редактирование таблицы            - EditTable
// 6.  Взять тексты граф и редактировать штамп    - EditStamp
// 7.  Получить текст технических требований      - GetTextTechnicalDemand
// 8.  Редактировать технические требования       - ChangeTechnicalDemand
// 9.  Вставка фрагмента из библиотеки фрагментов - ShowInsertFragment
// 10. Вставка фрагмента россыпью                 - ShowInsertFragment1
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KsAPI.h>
#include <KompasLibraryActions.h>

ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций.
// ---
void DrawTransform(ksapi::IKompasDocument2D & kDoc2D);                   /// Трансформация объекта по матрице
void DrawCopy(ksapi::IKompasDocument2D & kDoc2D);                        /// Копирование объекта
void DrawSymmetry(ksapi::IKompasDocument2D & kDoc2D);                    /// Симметрия объекта
void EditTolerance(ksapi::IKompasDocument2D & kDoc2D);                   /// Просмотр и редактирование допуска формы
void EditStamp(ksapi::IKompasDocument2D & kDoc2D);                       /// Взять тексты граф и редактировать штамп
void GetTextTechnicalDemand(ksapi::IKompasDocument2DPtr kDoc2D);         /// Получить текст технических требований
void ChangeTechnicalDemand(ksapi::IKompasDocument2DPtr kDoc2D);          /// Редактирование техничеческих требований
void ShowInsertFragment(ksapi::IKompasDocument2DPtr kDoc2D);             /// Вставка фрагмента
void ShowInsertFragmentKompasProcess(ksapi::IKompasDocument2D & kDoc2D); /// Вставка фрагмента через встроенный процесс
void ShowInsertFragment1(ksapi::IKompasDocument2DPtr kDoc2D);            /// Вставка фрагмента россыпью
void EditTable(ksapi::IKompasDocument2D & kDoc2D);                       /// Просмотр и редактирование таблицы

//-------------------------------------------------------------------------------
// Описания вспомогательных функций.
// ---
ksapi::IRectanglePtr CreateRectangle(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2);
ksapi::IHatchPtr CreateHatch(ksapi::IDrawingContainer & drawingContainer,
                             const std::vector<ksapi::IDrawingObjectPtr> & boundaries);
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2);
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & kDoc2D, bool createNew);
ksapi::IDrawingObjectPtr SelectObject(ksapi::IKompasDocument2D & document, const std::wstring & caption);
bool PrintToleranceParams(ksapi::IToleranceParamPtr tolerance);
bool PrintTable(ksapi::ITablePtr table);
void PrintTextLine(ksapi::ITextLinePtr textLine, int32_t index);
void PrintText(ksapi::ITextPtr text);
void PrintTextItem(ksapi::ITextItemPtr item, int32_t index);
void ReplaceTableCells(ksapi::ITablePtr table, const std::wstring & value);


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
      DrawTransform(*document);
      break;
    case 2:
      DrawCopy(*document);
      break;
    case 3:
      DrawSymmetry(*document);
      break;
    case 4:
      EditTolerance(*document);
      break;
    case 5:
      EditTable(*document);
      break;
    case 6:
      EditStamp(*document);
      break;
    case 7:
      GetTextTechnicalDemand(document);
      break;
    case 8:
      ChangeTechnicalDemand(document);
      break;
    case 9:
      ShowInsertFragment(document);
      // ShowInsertFragmentKompasProcess(ksapi::IKompasDocument2D & kDoc2D);
      break;
    case 10:
      ShowInsertFragment1(document);
      break;
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
/**
  Трансформация объектов.
  Создаётся заштрихованный прямоугольник.
  Добавляется матрица в документ.
  Прямоугольник трансформируется по матрице.
  Добавляются матрицы для обратной трансформации.
  Прямоугольник трансформируется в начальное положение.
*/
// ---
void DrawTransform(ksapi::IKompasDocument2D & kDoc2D)
{
  // Получение IDrawingContainer, для создания прямоугольника и штриховки.
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(kDoc2D, /*createNew*/ false);
  if (!drawingContainer)
    return;

  // Создание прямоугольника и штриховки внутри него
  ksapi::IRectanglePtr rectangle = CreateRectangle(*drawingContainer, /*x1*/ 30, /*y1*/ 30, /*x2*/ 60, /*y2*/ 60);
  ksapi::IHatchPtr hatch = CreateHatch(*drawingContainer, std::vector<ksapi::IDrawingObjectPtr>{rectangle});
  if (!rectangle || !hatch)
    return;

  // Создание группы и помещение туда прямоугольника и штриховки
  ksapi::IDrawingGroupsPtr groups = kDoc2D.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(false, L"");
  if (!group)
    return;
  group->AddObjects({rectangle, hatch});
  const std::vector<ksapi::IKompasAPIObjectPtr> groupOnContainer{group};

  kompasApp->ShowMessageBox(L"Cоздали матрицу 20, 20, 45, 2", L"", ksMessageWarning, ksButtonSetOk, true);

  // Создание матрицы и трансформирование группы по ней и удаление матрицы из документа.
  kDoc2D.CreateMatrix(/*x*/ 20, /*y*/ 20, /*angle*/ 45, /*scaleX*/ 2, /*scaleY*/ 2);
  bool ret = kDoc2D.TransformObjects(groupOnContainer);
  kDoc2D.DeleteMatrix();

  if (ret)
    kompasApp->ShowMessageBox(L"Вернём обратно", L"", ksMessageWarning, ksButtonSetOk, true);

  // Создание трёх матриц. Преобразование объектов будет происходить по всем добавленным матрицам последовательно.
  kDoc2D.CreateMatrix(/*x*/ 0, /*y*/ 0, /*angle*/ 0, /*scaleX*/ 0.5, /*scaleY*/ 0.5); // масштаб
  kDoc2D.CreateMatrix(/*x*/ 0, /*y*/ 0, /*angle*/ -45, /*scaleX*/ 1, /*scaleY*/ 1);   // угол
  kDoc2D.CreateMatrix(/*x*/ -20, /*y*/ -20, /*angle*/ 0, /*scaleX*/ 1, /*scaleY*/ 1); // смещение

  // Преобразование объектов по матрицам документа.
  ret = kDoc2D.TransformObjects(groupOnContainer);

  // Удаление всех матриц
  kDoc2D.DeleteMatrix();
  kDoc2D.DeleteMatrix();
  kDoc2D.DeleteMatrix();
}

//-------------------------------------------------------------------------------
/**
  Копирование объектов.
  Создаётся новый вид.
  В новом виде создаётся прямоугольник.
  Задаются параметры копирования и по ним копируется вид с прямоугольником.
*/
// ---
void DrawCopy(ksapi::IKompasDocument2D & kDoc2D)
{
  // Создание нового вида.
  ksapi::IViewPtr view = GetDrawingContainer(kDoc2D, /*createNew*/ true);

  if (!view)
  {
    kompasApp->ShowMessageBox(L"В документе не предусмотрена работа с видами.", L"", ksMessageWarning, ksButtonSetOk, true);
    return;
  }

  view->SetName(L"User view");
  view->SetX(20);
  view->SetY(60);
  view->SetScale(1);
  view->SetColor(0x0A140A);
  view->SetNumber(5);
  view->SetVisible(true);
  view->SetCurrent(true);
  view->Update();

  // Создание прямоугольника.
  ksapi::IDrawingContainerPtr drawingContainer = view;
  if (!drawingContainer)
    return;
  ksapi::IRectanglePtr rectangle = CreateRectangle(*drawingContainer, /*x1*/ 20, /*y1*/ 10, /*x2*/ 40, /*y2*/ 30);
  if (!rectangle)
    return;

  // Создание параметров копирования.
  ksapi::ICopyObjectParamPtr params = kDoc2D.GetInterface(KompasAPIObjectTypeEnum::ksObjectCopyObjectParam);
  if (!params)
    return;
  params->SetXOld(20);
  params->SetYOld(60);
  params->SetXNew(40);
  params->SetYNew(80);
  params->SetScale(1);
  params->SetAngle(0);
  params->SetAttributeCopy(true);
  params->SetDimensionLineScale(true);

  // Копирование вида содержащего прямоугольник по заданным параметрам.
  kDoc2D.CopyObjects({view}, params);
}


//-------------------------------------------------------------------------------
/**
  Пример использования симметрии объектов.
*/
// ---
void DrawSymmetry(ksapi::IKompasDocument2D & kDoc2D)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(kDoc2D, /*createNew*/ false);
  if (!drawingContainer)
    return;

  const std::vector<ksapi::IKompasAPIObjectPtr> toSymmetry{CreateLineSegment(kDoc2D, /*x1*/ 20, /*y1*/ 10, /*x2*/ 20, /*y2*/ 30),
                                                           CreateLineSegment(kDoc2D, /*x1*/ 20, /*y1*/ 30, /*x2*/ 40, /*y2*/ 30),
                                                           CreateLineSegment(kDoc2D, /*x1*/ 40, /*y1*/ 30, /*x2*/ 40, /*y2*/ 10),
                                                           CreateLineSegment(kDoc2D, /*x1*/ 40, /*y1*/ 10, /*x2*/ 20, /*y2*/ 10)};


  kDoc2D.SymmetryObjects(toSymmetry, /*x1*/ 80, /*y1*/ 10, /*x2*/ 80, /*y2*/ 20, /*copy*/ true, /*textSymmetry*/ true,
                         /*copyAttributes*/ true);
}

//-------------------------------------------------------------------------------
/**
  Просмотр параметров и редактирование допуска формы.
*/
// ---
void EditTolerance(ksapi::IKompasDocument2D & kDoc2D)
{
  // Функция ждёт выбора объекта пользователем.
  ksapi::IDrawingObjectPtr selected = SelectObject(kDoc2D, L"Укажите допуск формы.");
  if (!selected)
    return;
  ksapi::ITolerancePtr tolerance = selected;
  if (!tolerance)
  {
    kompasApp->ShowMessageBox(L"Объект не допуск формы.", L"", ksMessageWarning, ksButtonSetOk, true);
    return;
  }

  // Каст к нужным интерфейсам.
  ksapi::IToleranceParamPtr toleranceParam = tolerance;
  ksapi::IBranchsPtr toleranceBranchs = toleranceParam;
  ksapi::ITablePtr table = toleranceBranchs;
  if (!table)
    return;

  // Вывод значений пользовательской таблицы допуска формы.
  if (!PrintToleranceParams(toleranceParam))
    return;

  // Смещение допуска формы.
  toleranceBranchs->SetX0(toleranceBranchs->GetX0() + 10);
  toleranceBranchs->SetY0(toleranceBranchs->GetY0() + 10);

  // Установка новых параметров.
  // Разделим 3ю ячейку на две.
  ksapi::ITableCellPtr cell3 = table->GetCellById(3);
  if (!cell3)
    return;
  ksapi::ITableRangePtr tableRange = table->GetRange(/*beginRow*/ cell3->GetRow(), /*beginColumne*/ cell3->GetColumn(),
                                                     /*endRow*/ cell3->GetRow(), /*endColumne*/ cell3->GetColumn());
  if (!tableRange)
    return;
  tableRange->DivideCells(/*columnCount*/ 2, /*rowCount*/ 1, /*prepareCombine*/ false);
  tolerance->Update();

  // Очистка таблицы
  table->Clear();

  // Устаноить текст 2 и 4 ячейки
  if (ksapi::ITableCellPtr cell2 = table->GetCellById(2))
    if (ksapi::ITextLinePtr textLine = cell2->GetText())
      textLine->SetStr(L"Вторая ячейка");

  if (ksapi::ITableCellPtr cell4 = table->GetCellById(4))
    if (ksapi::ITextLinePtr textLine = cell4->GetText())
      textLine->SetStr(L"4");

  tolerance->Update();
}

//-------------------------------------------------------------------------------
/**
  Просмотр параметров и редактирование таблицы.
  Для корректной работы требуется таблица, не меньше чем 2х2.
*/
// ---
void EditTable(ksapi::IKompasDocument2D & kDoc2D)
{
  // Функция ждёт выбора объекта пользователем.
  ksapi::IDrawingObjectPtr selected = SelectObject(kDoc2D, L"Укажите таблицу");
  if (!selected)
    return;
  ksapi::ITablePtr table = selected;
  ksapi::IDrawingTablePtr drawingTable = table;
  if (!drawingTable)
  {
    kompasApp->ShowMessageBox(L"Объект не таблица", L"", ksMessageWarning, ksButtonSetOk, true);
    return;
  }

  // Вывод содержимого таблицы.
  if (!PrintTable(table))
    return;

  // Установка новых параметров.
  // Разделим ячейкку (1, 0) на две
  ksapi::ITableRangePtr tableRange = table->GetRange(/*beginRow*/ 1, /*beginColumn*/ 0, /*endRow*/ 1, /*endColumn*/ 0);
  if (!tableRange)
    return;
  // После деления, ячейки с индексом 2 не будет в таблице - для подсчёта индексов следует "продлить" линию деления
  // ячейки на всю таблицу.
  tableRange->DivideCells(/*columnCount*/ 2, /*rowCount*/ 1, /*prepareCombine*/ false);

  drawingTable->Update();

  // Очистим таблицу от значений
  table->Clear();

  // Установим стиль линии, которой мы разделили ячейку
  tableRange = table->GetRange(/*beginRow*/ 1, /*beginColumn*/ 0, /*endRow*/ 1, /*endColumn*/ 1);
  if (!tableRange)
    return;
  ksapi::ICellBoundariesPtr dividedCellBoundaries = tableRange->GetCellsBoundaries();
  if (!dividedCellBoundaries)
    return;
  dividedCellBoundaries->SetLineStyle(ksCBInternalBorders, ksCSThin);

  // Установим стиль верхней границы первой ячейки
  ksapi::ITableCellPtr cell1 = table->GetCellById(1);
  ksapi::ICellBoundariesPtr cell1Boundaries = cell1;
  if (!cell1Boundaries)
    return;
  cell1Boundaries->SetLineStyle(ksCBTopBorder, ksCSNormal);

  // Установка текста для 2й ячейки.
  // 2й индекс присутствует при расчёте индексов,
  // однако фактически отсутствует в таблице (см. комментарий в месте деления ячейки)
  // Поэтому 2я отображаемая ячейка имеет индекс 3
  if (ksapi::ITableCellPtr tableCell = table->GetCellById(/*cellId*/ 3))
    if (ksapi::ITextPtr text = tableCell->GetText())
      text->SetStr(L"Вторая ячейка");

  // Установка значения 4й ячейки.
  // Будем считать разделённую ячейку за одну. Однако, их теперь 2, и каждая имеет свой индекс.
  // Поэтому, если изначальная таблица была шириной 2, индекс должен быть увеличен ещё на 1.
  int32_t index4 = table->GetColumnsCount() == 3 ? 6 : 5;
  if (ksapi::ITableCellPtr tableCell = table->GetCellById(/*cellId*/ index4))
    if (ksapi::ITextPtr text = tableCell->GetText())
      text->SetStr(L"4");

  drawingTable->Update();
}

//-------------------------------------------------------------------------------
/**
  Просмотр и редактирование штампа.
  Выбор таблицы пользователем.
  Деление ячеек верхней строки таблицы.
  Изменение стиля внутренних границ ячеек верхних трех строк.
  Установка значений 4-й и 1-й ячейке.
*/
// ---
void EditStamp(ksapi::IKompasDocument2D & kDoc2D)
{
  // Получение текущего штампа.
  ksapi::ILayoutSheetsPtr layoutSheets = kDoc2D.GetLayoutSheets();

  if (!layoutSheets)
  {
    kompasApp->ShowMessageBox(L"Штамп не найден.", L"", ksMessageWarning, ksButtonSetOk, false);
    return;
  }

  ksapi::ILayoutSheetPtr layoutSheet = layoutSheets->GetItem(0);
  if (!layoutSheet)
    return;
  ksapi::IStampPtr stamp = layoutSheet->GetStamp();
  if (!stamp)
    return;

  // Цикл по всем ячейкам штампа имеющим значение, и их вывод.
  for (int32_t i = stamp->GetNextColumnId(-1); i != -1; i = stamp->GetNextColumnId(i))
  {
    ksapi::ITextPtr text = stamp->GetText(i);
    if (!text)
      continue;

    std::wstring msg;
    kompasApp->ShowMessageBox(std::format(L"number = {}", i), L"", ksMessageWarning, ksButtonSetOk, true);
    PrintText(text);
  }

  stamp->Clear(/*id*/ 2); // Индекс поля штампа для очиски. 0 - весь штамп.

  // Установка значения ячейки штампа с индексом 2.
  ksapi::ITextPtr text = stamp->GetText(2);
  text->SetStr(L"Графа 2");
  stamp->Update();
}

//-------------------------------------------------------------------------------
/**
  Просмотр значений технических требований.
*/
// ---
void GetTextTechnicalDemand(ksapi::IKompasDocument2DPtr kDoc2D)
{
  if (ksapi::IDrawingDocumentPtr drawDoc = kDoc2D)
  {
    if (ksapi::ITechnicalDemandPtr techDemand = drawDoc->GetTechnicalDemand())
    {
      if (!techDemand || !techDemand->IsCreated())
      {
        kompasApp->ShowMessageBox(L"Технические требования отсутствуют", L"", ksMessageWarning, ksButtonSetOk, true);
        return;
      }

      ksapi::ITextPtr text = techDemand->GetText();
      if (!text)
        return;
      kompasApp->ShowMessageBox(std::format(L"style = {}, число страниц = {}", text->GetStyle(),
                                            techDemand->GetBlocksStartLineNumbers().size()),
                                L"", ksMessageWarning, ksButtonSetOk, true);
      PrintText(text);
    }
  }
  else
  {
    kompasApp->ShowMessageBox(L"В документе не предусмотрена работа с техническими требованиями", L"", ksMessageWarning,
                              ksButtonSetOk, false);
  }
}

//-------------------------------------------------------------------------------
/**
  Получение фрагмента из библиотеки фрагментов.
  Вставка фрагмента с помощью процесса Компаса.
  Не используется в данном приложении. Реализовано в качестве демонстрации.
*/
// ---
void ShowInsertFragmentKompasProcess(ksapi::IKompasDocument2D & kDoc2D)
{
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();
  if (!dialogs)
    return;

  // Получение имени файла библиотеки.
  const std::wstring libraryFragment =
    dialogs->ChoiceFile(/*windowsHandle*/ nullptr,
                        /*defaultExtention*/ L"*.lfr",
                        /*filter*/ L"Библиотки фрагментов( *.lfr )|*.lfr|Все файлы ( *.* )|*.*|",
                        /*startDirectory*/ L"",
                        /*directoryType*/ ksSystemPathTypeEnum::ksApplications,
                        /*preview*/ true);

  // Получение имени фрагмента.
  ksChoiceFromLibraryResultEnum choiceResult;
  const std::wstring fragmentName = dialogs->ChoiceDocumentFromLibrary(/*windowHandle*/ nullptr,
                                                                       /*title*/ L"Выберите фрагмент",
                                                                       /*libraryPath*/ libraryFragment, /*currentDocument*/ L"",
                                                                       /*filterInsertType*/ ksInsertionFragment, choiceResult);
  if (choiceResult != ksChoiceFromLibraryDocument)
  {
    kompasApp->ShowMessageBox(L"Имя вставки не определено", L"", ksMessageWarning, ksButtonSetOk, true);
    return;
  }

  // Запуск процесса вставки фрагмента (не блокирующий).
  kDoc2D.ExecuteProcessOfDocumentFromFile(fragmentName, prInsertFragment);
}

//-------------------------------------------------------------------------------
/**
  Вставка фрагмента из библиотеки фрагментов.
  Процесс вставки происходит в выбранную пользователем точку.
*/
// ---
void ShowInsertFragment(ksapi::IKompasDocument2DPtr kDoc2D)
{
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();
  if (!dialogs)
    return;

  // Получение имени файла библиотеки.
  const std::wstring libraryFragment =
    dialogs->ChoiceFile(/*windowsHandle*/ nullptr,
                        /*defaultExtention*/ L"*.lfr",
                        /*filter*/ L"Библиотки фрагментов( *.lfr )|*.lfr|Все файлы ( *.* )|*.*|",
                        /*startDirectory*/ L"",
                        /*directoryType*/ ksSystemPathTypeEnum::ksApplications,
                        /*preview*/ true);

  // Получение нужных интерфейсов
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(*kDoc2D, /*createNew*/ false);
  ksapi::IDrawingGroupsPtr groups = kDoc2D->GetDrawingGroups();
  ksapi::IProcess2DPtr process2D = kDoc2D->GetLibProcess(ksProcess2DPlacement);
  ksapi::IProcessPtr process = process2D;
  ksapi::IInsertionsManagerPtr insertionMngr = kDoc2D;
  if (!groups || !drawingContainer || !process)
    return;
  ksapi::IPhantom2DPtr phantom = process2D->GetPhantom2D();
  ksapi::IInsertionObjectsPtr insertionObjects = drawingContainer->GetInsertionObjects();
  if (!phantom || !insertionObjects)
    return;

  // Инициализация обработчика нажатий клавиш.
  ksapi::IDrawingGroupPtr phantomGroup; // Фантомная группа, отображается у курсора, пока точка не выбрана
  bool repeat = true;                   // Флаг, означающий успешную вставку фрагмента.
  auto clickCallback = [&repeat, &phantomGroup, &kDoc2D](double x, double y, double angle, bool dynamic)
  {
    // Смещение группы с фрагментом на выбранную точку и угол.
    kDoc2D->MoveObjects({phantomGroup}, x, y);
    kDoc2D->RotateObjects({phantomGroup}, x, y, angle);

    // Вставка временной группы в Компас.
    phantomGroup->Store();
    phantomGroup->Clear(/*deleteTmp*/ true);

    // Признак того, что надо повторить процесс.
    repeat = true;

    // Возвращаем false, иначе процесс не завершится и будет ждать новой точки от пользователя.
    return false;
  };


  // Повторение, пока пользователь указывает фрагмент
  while (true)
  {
    // Получение имени файла фрагмента.
    ksChoiceFromLibraryResultEnum choiceResult;
    const std::wstring fragmentName = dialogs->ChoiceDocumentFromLibrary(/*windowHandle*/ nullptr,
                                                                         /*title*/ L"Выберите фрагмент",
                                                                         /*libraryPath*/ libraryFragment, /*currentDocument*/ L"",
                                                                         /*filterInsertType*/ ksInsertionFragment, choiceResult);
    if (choiceResult != ksChoiceFromLibraryDocument || fragmentName.empty())
      return;


    // Повторение, пока выбирается точка вставки. При отмене процесса не вызовется обработчик и не переключится флаг repeat
    repeat = true;
    while (repeat)
    {
      // Фантомная группа для отображения у курсора во время процесса.
      phantomGroup = groups->Add(/*tmp*/ true, /*name*/ L"");
      if (!phantomGroup)
        return;

      // Вставка фрагмента во временную группу
      // Создать описания фрагмента
      ksapi::IInsertionDefinitionPtr definition =
        insertionMngr->AddDefinition(ksTBodyFragment, L"Внешний документ", fragmentName);
      // Создать объект-вставку
      ksapi::IInsertionFragmentPtr insertionFragment = insertionObjects->Add(definition);
      insertionFragment->SetPlacement(/*x*/ 0, /*y*/ 0, /*angle*/ 0, /*mirrorSymmetry*/ false);
      insertionFragment->SetScale(1);
      insertionFragment->SetMultiLayer(true);
      // Засисываем во временную группу.
      phantomGroup->Open();
      insertionFragment->Update();
      phantomGroup->Close();

      phantom->SetPhantomType(ksMoveGroupPhantom);
      phantom->SetPhantomGroup(phantomGroup);
      phantom->Show();

      // Установка обработчика нажатия кнопки мыши.
      process2D->Events()->AddPlacementChangedHandler(L"Step5", clickCallback);

      // По-умолчанию итак false. Если true, то обработчик будет вызываться на изменение положения курсора в том числе.
      process->SetDynamic(false);

      repeat = false;

      // Запуск процесса. Блокирует поток, пока процесс не завершится.
      process->Run(/*modal*/ true, /*postMessage*/ false);

      process2D->Events()->RemoveAllHandlers(L"Step5");
    }
  }
}

//-------------------------------------------------------------------------------
/**
  Вставка фрагмента россыпью.
  Процесс вставки происходит в выбранную пользователем точку.
*/
// ---
void ShowInsertFragment1(ksapi::IKompasDocument2DPtr kDoc2D)
{
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(*kDoc2D, /*createNew*/ false);
  ksapi::IDrawingGroupsPtr groups = kDoc2D->GetDrawingGroups();
  ksapi::IProcess2DPtr process2D = kDoc2D->GetLibProcess(ksProcess2DPlacement);
  ksapi::IProcessPtr process = process2D;
  if (!dialogs || !groups || !drawingContainer || !process)
    return;
  ksapi::IPhantom2DPtr phantom = process2D->GetPhantom2D();
  ksapi::IProcess2DEventsPtr events = process2D->Events();
  if (!phantom || !events)
    return;

  // Инициализация обработчика нажатий клавиш.
  ksapi::IDrawingGroupPtr phantomGroup; // Фантомная группа, отображается у курсора, пока точка не выбрана
  bool repeat = true;                   // Флаг, означающий успешную вставку фрагмента.
  auto clickCallback = [&repeat, &phantomGroup, &kDoc2D](double x, double y, double angle, bool dynamic)
  {
    // Смещение группы с фрагментом на выбранную точку и угол.
    kDoc2D->MoveObjects({phantomGroup}, x, y);
    kDoc2D->RotateObjects({phantomGroup}, x, y, angle);

    // Вставка временной группы в Компас.
    phantomGroup->Store();
    phantomGroup->Clear(/*deleteTmp*/ true);

    // Признак того, что надо повторить процесс.
    repeat = true;

    // Возвращаем false, иначе процесс не завершится и будет ждать новой точки от пользователя.
    return false;
  };


  // Повторение, пока пользователь указывает фрагмент
  while (true)
  {
    // Получение имени файла фрагмента.
    std::wstring fragmentName = dialogs->ChoiceFile(/*windowsHandle*/ nullptr,
                                                    /*defaultExtention*/ L"*.frw",
                                                    /*filter*/ L"фрагменты( *.frw )|*.frw|Все файлы ( *.* )|*.*|",
                                                    /*startDirectory*/ L"",
                                                    /*directoryType*/ ksApplications,
                                                    /*preview*/ true);
    if (fragmentName.empty()) // Файл не выбран
      return;

    // Повторение, пока выбирается точка вставки. При отмене процесса не вызовется обработчик и не переключится флаг repeat
    repeat = true;
    while (repeat)
    {
      // Фантомная группа для отображения у курсора во время процесса.
      phantomGroup = groups->Add(/*tmp*/ true, /*name*/ L"");
      if (!phantomGroup)
        return;

      // Чтение фрагмента из выбранного файла.
      phantomGroup->ReadFragment(fragmentName, /*currentLayer*/ true, /*xBase*/ 0, /*yBase*/ 0, /*scale*/ 1, /*angle*/ 0,
                                 /*scaleProjLineSize*/ true);

      phantom->SetPhantomType(ksMoveGroupPhantom);
      phantom->SetPhantomGroup(phantomGroup);
      phantom->Show();

      // Установка обработчика нажатия кнопки мыши.
      events->AddPlacementChangedHandler(L"Step5", clickCallback);

      // По-умолчанию итак false. Если true, то обработчик будет вызываться на изменение положения курсора в том числе.
      process->SetDynamic(false);

      repeat = false;

      // Запуск процесса. Блокирует поток, пока процесс не завершится.
      process->Run(/*modal*/ true, /*postMessage*/ false);

      events->RemoveAllHandlers(L"Step5");
    }
#ifdef TESTS
    return;
#endif
  }
}


//-------------------------------------------------------------------------------
/**
  Редактирование технических требований.
*/
// ---
void ChangeTechnicalDemand(ksapi::IKompasDocument2DPtr kDoc2D)
{
  if (ksapi::IDrawingDocumentPtr drawDoc = kDoc2D)
  {
    if (ksapi::ITechnicalDemandPtr technicalDemand = drawDoc->GetTechnicalDemand())
    {
      if (!technicalDemand || !technicalDemand->IsCreated())
      {
        kompasApp->ShowMessageBox(L"Технические требования отсутствуют", L"", ksMessageWarning, ksButtonSetOk, true);
        return;
      }

      ksapi::ITextPtr text = technicalDemand->GetText();
      if (!text)
        return;

      kompasApp->ShowMessageBox(std::format(L"Число строк = {}", text->GetCount()), L"", ksMessageWarning, ksButtonSetOk, true);

      for (ksapi::ITextLinePtr textLine : text->GetTextLines())
      {
        if (!textLine)
          continue;

        // Добавим "!!!" к существующей строке
        const std::wstring str = textLine->GetStr();
        textLine->SetStr(str + L"!!!");
      }

      technicalDemand->Update();
    }
    else
    {
      kompasApp->ShowMessageBox(L"В документе не предусмотрена работа с техническими требованиями", L"", ksMessageWarning,
                                ksButtonSetOk, true);
    }
  }
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция создания прямоугольника.
*/
// ---
ksapi::IRectanglePtr CreateRectangle(ksapi::IDrawingContainer & drawingContainer, double x1, double y1, double x2, double y2)
{
  ksapi::IRectanglesPtr rectangles = drawingContainer.GetRectangles();
  if (!rectangles)
    return nullptr;
  ksapi::IRectanglePtr rectangle = rectangles->Add();
  if (!rectangle)
    return nullptr;

  rectangle->SetX(x1);
  rectangle->SetY(y1);
  rectangle->SetHeight(y2 - y1);
  rectangle->SetWidth(x2 - x1);

  rectangle->Update();

  return rectangle;
}

//-------------------------------------------------------------------------------
/**
  Вспомогательная функция получения IDrawingContainer.
  По флагу создаётся новый, или возвращается активный.
*/
// ---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & kDoc2D, bool createNew)
{
  ksapi::IViewsAndLayersManagerPtr viewsAndLayersManager = kDoc2D.GetViewsAndLayersManager();
  if (!viewsAndLayersManager)
    return nullptr;
  ksapi::IViewsPtr views = viewsAndLayersManager->GetViews();
  if (!views)
    return nullptr;
  return createNew ? views->Add(LtViewType::vt_Normal) : views->GetActiveView();
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция создания штриховки в заданных примитивами границах.
*/
// ---
ksapi::IHatchPtr CreateHatch(ksapi::IDrawingContainer & drawingContainer,
                             const std::vector<ksapi::IDrawingObjectPtr> & boundaries)
{
  ksapi::IHatchesPtr hatches = drawingContainer.GetHatches();
  if (!hatches)
    return nullptr;
  ksapi::IHatchPtr hatch = hatches->Add();
  ksapi::IBoundariesObjectPtr hatchBoundaries = hatch;
  ksapi::IHatchParamPtr hatchParam = hatchBoundaries;
  if (!hatchParam)
    return nullptr;

  hatchParam->SetStep(2);
  hatchBoundaries->AddBoundaries(boundaries, /*deleteSource*/ false);
  hatch->Update();
  return hatch;
}

//------------------------------------------------------------------------------
/**
  Ожидание выбора объекта пользователем
*/
//---
ksapi::IDrawingObjectPtr SelectObject(ksapi::IKompasDocument2D & document, const std::wstring & caption)
{
  ksapi::IProcess2DPtr process2d = document.GetLibProcess(ksProcess2DTypeEnum::ksProcess2DCursor);
  ksapi::IProcessPtr process = process2d;
  if (!process)
    return nullptr;
  ksapi::IViewsAndLayersManagerPtr viewsAndLayerMngr = document.GetViewsAndLayersManager();
  if (!viewsAndLayerMngr)
    return nullptr;
  ksapi::IViewsPtr views = viewsAndLayerMngr->GetViews();
  if (!views)
    return nullptr;

  process->SetCaption(caption);

  // функция, которая ищет объект по координатам, будет вызываться по событию клика мыши
  ksapi::IDrawingObjectPtr result;
  auto clickCallback = [views, &result](double xView, double yView, double, bool)
  {
    ksapi::IViewPtr currentView = views->GetActiveView();
    if (!currentView)
      return false; // ошибка - прервём выполнение.

    if (result = currentView->FindObject(xView, yView, 1e-6, nullptr))
      return false; // объект нашли - возвращаем false, что бы удалить обработчик из контейнера.

    kompasApp->ShowMessageBox(L"Нет объекта", L"", ksMessageWarning, ksButtonSetOk, true);
    return false;
  };

  // добавление колбека в контейнер обработчиков событий
  ksapi::IProcess2DEventsPtr events = process2d->Events();
  if (events)
    events->AddPlacementChangedHandler(L"Step5", clickCallback);

  process->Run(/*modal*/ true, /*postMessage*/ false);

  return result;
}


//-------------------------------------------------------------------------------
/**
  Вывод значений таблицы допуска формы.
*/
// ---
bool PrintToleranceParams(ksapi::IToleranceParamPtr toleranceParam)
{
  ksapi::IBranchsPtr toleranceBranchs = toleranceParam;
  ksapi::ITolerancePtr tolerance = toleranceBranchs;
  if (!tolerance)
    return false;

  // Общие параметры.
  kompasApp->ShowMessageBox(std::format(L"tBase = {}, style = {}, расположение - {},\nx = {:4.2f}, y = {:4.2f}",
                                        static_cast<int>(toleranceParam->GetBasePointPos()), tolerance->GetStyle(),
                                        std::wstring(toleranceParam->IsVertical() ? L"вертикальное" : L"горизонтальное"),
                                        toleranceBranchs->GetX0(), toleranceBranchs->GetY0()),
                            L"", ksMessageWarning, ksButtonSetOk, true);

  ksapi::ITablePtr toleranceTable = toleranceParam;
  if (!toleranceTable)
    return false;

  // Циклы по ячейкам таблицы. Для каждой ячейки выводится текст поэлементно.
  for (int32_t i = 0; i < toleranceTable->GetRowsCount(); ++i)
  {
    for (int32_t j = 0; j < toleranceTable->GetColumnsCount(); ++j)
    {
      ksapi::ITableCellPtr cell = toleranceTable->GetCell(i, j);
      if (!cell)
        continue;

      ksapi::ITextLinePtr textLine = cell->GetText();
      if (!textLine)
        continue;
      // Вывод информации о ячейке.
      kompasApp->ShowMessageBox(std::format(L"number = {}, style = {}", cell->GetCellId(), textLine->GetStyle()), L"",
                                ksMessageWarning, ksButtonSetOk, true);

      // В цикле вывод каждого элемента текста ячейки.
      int32_t itemsCount = 0;
      for (ksapi::ITextItemPtr item : textLine->GetTextItems())
        PrintTextItem(item, ++itemsCount);
    }
  }
  return true;
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция вывода значений таблицы.
*/
// ---
bool PrintTable(ksapi::ITablePtr table)
{
  if (!table)
    return false;

  for (int32_t i = 1; ksapi::ITableCellPtr cell = table->GetCellById(i); ++i)
  {
    kompasApp->ShowMessageBox(std::format(L"number = {}", cell->GetCellId()), L"", ksMessageWarning, ksButtonSetOk, true);
    ksapi::ITextPtr text = cell->GetText();
    if (!text)
      continue;
    for (int32_t j = 0; j < text->GetCount(); ++j)
    {
      ksapi::ITextLinePtr textLine = text->GetTextLine(j);
      if (!textLine)
        continue;
      kompasApp->ShowMessageBox(std::format(L"i = {} style = {}", j + 1, textLine->GetStyle()), L"", ksMessageWarning,
                                ksButtonSetOk, true);
      for (int32_t k = 0; k < textLine->GetCount(); ++k)
      {
        ksapi::ITextItemPtr textItem = textLine->GetTextItem(k);
        if (!textItem)
          continue;
        PrintTextItem(textItem, k);
      }
    }
  }


  return true;
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция вывода элементов текста.
*/
// ---
void PrintTextItem(ksapi::ITextItemPtr item, int32_t index)
{
  ksapi::ITextFontPtr textFont = item;
  if (!textFont)
    return;

  if (item->GetItemType() == ksTItString)
  {
    kompasApp->ShowMessageBox(
      std::format(
        L"i = {}, font.height = {:4.2f},\ns = {},\nfont.fontName = {},\nfont.bold = {}, font.italic = {}, font.underlined = {}",
        index, textFont->GetHeight(), item->GetStr(), textFont->GetFontName(), textFont->IsBold(), textFont->IsItalic(),
        textFont->IsUnderline()),
      L"", ksMessageWarning, ksButtonSetOk, true);
  }
  else
  {
    kompasApp->ShowMessageBox(std::format(L"i = {}, tip = {}, iSNumb = {}", index, static_cast<int32_t>(item->GetItemType()),
                                          item->GetNumber()),
                              L"", ksMessageWarning, ksButtonSetOk, true);
  }
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция вывода элементов текста.
*/
// ---
void PrintText(ksapi::ITextPtr text)
{
  if (!text)
    return;

  int32_t i = 0;
  for (ksapi::ITextLinePtr textLine : text->GetTextLines())
    PrintTextLine(textLine, i++);
}

//-------------------------------------------------------------------------------
/**
  Вспомогательная функция вывода элементов строки текста.
*/
// ---
void PrintTextLine(ksapi::ITextLinePtr textLine, int32_t index)
{
  if (!textLine)
    return;

  kompasApp->ShowMessageBox(std::format(L"i = {} style = {}", index, textLine->GetStyle()), L"", ksMessageWarning, ksButtonSetOk,
                            true);

  int32_t i = 0;
  for (ksapi::ITextItemPtr textItem : textLine->GetTextItems())
    PrintTextItem(textItem, i++);
}


//-------------------------------------------------------------------------------
/**
  Вспомогательная функция замены значений ячеек таблицы на заданное.
*/
// ---
void ReplaceTableCells(ksapi::ITablePtr table, const std::wstring & value)
{
  if (!table)
    return;

  table->Clear();
  for (int32_t i = 0; i < table->GetRowsCount(); ++i)
  {
    for (int32_t j = 0; j < table->GetColumnsCount(); ++j)
    {
      ksapi::ITableCellPtr cell = table->GetCell(/*row*/ i, /*column*/ j);
      if (!cell)
        continue;
      if (ksapi::ITextPtr text = cell->GetText())
        text->SetStr(value);
    }
  }

  if (ksapi::IDrawingObjectPtr obj = table)
    obj->Update();
}

//-------------------------------------------------------------------------------
/**
  Вспомогательная функция cоздания отрезка.
*/
// ---
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IKompasDocument2D & document, double x1, double y1, double x2, double y2)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document, /*createNew*/ false);
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
