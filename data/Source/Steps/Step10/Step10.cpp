////////////////////////////////////////////////////////////////////////////////
//
// step10.cpp - Работа с объектами спецификации
//
// 1. Создать шаблон обозначения 'Болт'                - CreateTypeAttrBolt
// 2. Создать объект для раздела 'Детали'              - CreateDet
// 3. Создать объект для раздела 'Стандартные изделия' - CreateStandart
// 4. Конвертировать спецификацию во фрагмент          - DecomposeSpc
// 5. Просмотреть текущую спецификацию                 - ShowSpc
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KompasLibraryActions.h>
#include <KsAPI.h>
#include "Resource.h"

ksapi::IApplication * kompasApp = nullptr;

extern void LoadResources(ksapi::IApplication & app);
extern void * GetResourceModule();

constexpr int32_t kKey1 = 100;
constexpr int32_t kKey2 = 5;
constexpr int32_t kKey3 = 1;

constexpr ksapi::NumericId kBoltAttrTypeID = 313277777065;

const std::wstring kTypesLibraryName = L"graphic.lyt";
const std::wstring kAttrTypesLibraryName = L"spc.lat"; // Имя библиотеки типов атрибутов.

//-------------------------------------------------------------------------------
// Пример создания шаблона обозначения для стандартного изделия "Болт"
// Шаблон обозначения можно создать визуальными средствами, предоставленными графиком для
// создания типа атрибута. Этот способ не требует непосредственного программирования,
// но влечет к затратам времени на создание шалона, а при потере данных в библиотеке
// потребует повторного создания утраченного шаблона.
// Можно создать шаблон обозначения средствами apptool из библиотеки.

// Полное обозначение болта :
//   Болт 1 М12 х 1.25 -6g x 60.58.35X.16 ГОСТ 7808 - 70

// Представим это обозначение в виде компонент :

// Номер | Имя компоненты | Умолчательное значение | Тип     | Очередность сортировки
//       |                |                        |         |
// 1     | Имя элем.      | Болт                   | строка  | 1
// 2     | Исполнение     | 1                      | uint    | 3
// 3     | Резьба         | М                      | строка  | 0       ( не сортируется )
// 4     | Диаметр        | 12                     | uint    | 4 или 3 ( сортирует подряд )
// 5     | Разделитель    | х                      | строка  | 0       ( не сортируется )
// 6     | Шаг            | 1.25                   | float   | 5 или 3 ( сортирует подряд )
// 7     | Поле допуска   | -6g                    | строка  | 6 или 3 ( сортирует подряд )
// 8     | Разделитель    | х                      | строка  | 0       ( не сортируется )
// 9     | Длина          | 60                     | uint    | 7 или 3 ( сортирует подряд )
// 10    | Кл. прочности  | .58                    | строка  | 0       ( не сортируется )
// 11    | Материал       | .35Х                   | строка  | 0       ( не сортируется )
// 12    | Покрытие       | .16                    | строка  | 0       ( не сортируется )
// 13    | ГОСТ           | ГОСТ                   | строка  | 0       ( не сортируется )
// 14    | Номер          | 7808                   | uint    | 2
// 15    | Разделитель    | -                      | строка  | 0       ( не сортируется )
// 16    | Год            | 70                     | строка  | 0       ( не сортируется )

//-------------------------------------------------------------------------------
// Структура для хранения информации о болте
// ---
struct Bolt
{
  float dr = -1;           // Диаметр резьбы
  float p = -1;            // Шаг резьбы
  float s = -1;            // Размер под ключ
  float h = -1;            // Высота головки
  float D = -1;            // Диаметр описанной окружности
  float d3 = -1;           // Диаметр отверстия в стержне
  float h2 = -1;           // Высота подголовка
  float L = -1;            // Ддлина стержня
  float l1 = -1;           // Расстояние до отверстия в стержне
  float b = -1;            // Длина резьбовой части
  float d2 = -1;           // Диаметр под головкой
  float z4 = -1;           // Величина фаски конца
  short k = -1;            // Расположение резьбы ( 1 - длина резьбы ниже ломманной( резьба только на b ),
                           //                       2 - выше ломанной ( резьба до головки и на в ) )
  unsigned short f = 0;    // Битовые маски
  short cl = -1;           // Класс точности
  unsigned short gost = 0; // Номер госта
};

enum EBoltFlags : int32_t
{
  EMBODIMENT = 0x80,
  PITCH = 0x2
};

//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void CreateTypeAttrBolt();                              /// Создать шаблон обозначения 'Болт'.
void CreateDet(ksapi::IKompasDocument2D & kDoc2D);      /// Создать объект для раздела 'Детали'.
void CreateStandart(ksapi::IKompasDocument2D & kDoc2D); /// Создать объект для раздела 'Стандартные изделия'.
void DecomposeSpc();                                    /// Конвертировать спецификацию во фрагмент
void ShowSpc();                                         /// Просмотреть текущую спецификацию.

//-------------------------------------------------------------------------------
// Описания вспомогательных функций
// ---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & document);
ksapi::IRectanglePtr CreateRectangle(ksapi::IKompasDocument2D & kDoc2D);
ksapi::ISpecificationBaseObjectPtr CreateSpecificationObject(ksapi::IKompasDocument2D & kDoc2D,
                                                             ksapi::IKompasAPIObjectPtr geometry, int32_t sectionNumber,
                                                             ksapi::NumericId attrTypeNum);
void FillSpecificationBolt(ksapi::ISpecificationBaseObject & specObj, const Bolt & bolt);
ksapi::IPositionLeaderPtr GetPosLeaderFromUser(ksapi::IKompasDocument2D & kDoc2D);
ksapi::IDrawingObjectPtr SelectObjectOnCurrentView(ksapi::IKompasDocument2D & document, const std::wstring & caption);

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
  if ((commandId == 2 || commandId == 3) && !document)
  {
    kompasApp->ShowMessageBox(L"Документ не активизирован или\nне является листом/фрагментом", L"", ksMessageWarning,
                              ksButtonSetOk, true);
    return;
  }

  switch (commandId)
  {
    case 1:
      CreateTypeAttrBolt();
      break;
    case 2:
      CreateDet(*document);
      break;
    case 3:
      CreateStandart(*document);
      break;
    case 4:
      DecomposeSpc();
      break;
    case 5:
      ShowSpc();
      break;
  }
}


//-------------------------------------------------------------------------
// Создать шаблон обозначения 'Болт'.
// ---
void CreateTypeAttrBolt()
{
  ksapi::IAttrTypeMngPtr attrTypeManager = kompasApp->GetAttrTypeMng(kAttrTypesLibraryName);
  if (!attrTypeManager)
    return;
  ksapi::IAttributeTypePtr attrType = attrTypeManager->CreateAttrType();
  if (!attrType)
    return;

  attrType->SetTypeName(L"Болт");
  attrType->SetRowsCount(1);
  attrType->SetKeysInfo(kKey1, kKey2, kKey3, /*key4*/ 0);

  auto createColumn = [&attrType](ksValueTypeEnum type, const std::wstring & name, const std::wstring & defaultValue, int32_t key)
  {
    ksapi::IColumnInfoPtr column = attrType->AddColumn(/*index*/ -1, type, name);
    if (!column)
      return;

    column->SetDefaultValue(defaultValue);
    column->SetKey(key); // Дополнительный признак, очерёдность сортировки.
  };

  createColumn(ksValueTypeEnum::ksValueTypeString, L"Имя элем.", L"Болт", 1);      // Колонка 1 - "Имя элемента".
  createColumn(ksValueTypeEnum::ksValueTypeInteger, L"Исполнение", L"1", 3);       // Колонка 2 - "Исполнение".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Резьба", L"М", 0);            // Колонка 3 - "Резьба".
  createColumn(ksValueTypeEnum::ksValueTypeInteger, L"Диаметр", L"12", 3);         // Колонка 4 - "Диаметр".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"", L"x", 0);                  // Колонка 5 - "Разделитель".
  createColumn(ksValueTypeEnum::ksValueTypeFloat, L"Шаг", L"1.25", 3);             // Колонка 6 - "Шаг".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Поле допуска", L"-6g", 3);    // Колонка 7 - "Поле допуска".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"", L"x", 0);                  // Колонка 8 - "Разделитель".
  createColumn(ksValueTypeEnum::ksValueTypeInteger, L"Длина", L"60", 3);           // Колонка 9 - "Длина".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Класс прочности", L".58", 0); // Колонка 10 - "Класс прочности".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Материал", L".35Х", 0);       // Колонка 11 - "Материал".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Покрытие", L".16", 0);        // Колонка 12 - "Покрытие".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"ГОСТ", L"ГОСТ", 0);           // Колонка 13 - "ГОСТ".
  createColumn(ksValueTypeEnum::ksValueTypeInteger, L"Номер", L"7808", 2);         // Колонка 14 - "Номер".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"", L"-", 0);                  // Колонка 15 - "Разделитель".
  createColumn(ksValueTypeEnum::ksValueTypeString, L"Год", L"70", 0);              // Колонка 15 - "Год".

  attrType->Update(/*password*/ L"");
}

//-------------------------------------------------------------------------
// Создать объект для раздела 'Детали'.
// ---
void CreateDet(ksapi::IKompasDocument2D & kDoc2D)
{
  ksapi::IRectanglePtr rectangle = CreateRectangle(kDoc2D);
  if (!rectangle)
    return;

  ksapi::IKompasAPIObjectPtr geometry = kDoc2D.GetEditMacroObject();
  if (!geometry)
    geometry = rectangle;

  ksapi::ISpecificationBaseObjectPtr specObject =
    CreateSpecificationObject(kDoc2D, geometry, /*sectionNumber*/ 20, /*attrTypeNum*/ 0);
  if (!specObject)
    return;

  ksapi::ISpecificationColumnsPtr specColumns = specObject->GetColumns();
  if (!specColumns)
    return;
  ksapi::ISpecificationColumnPtr specColumn = specColumns->GetColumn(ksSColumnName, /*columnNumber*/ 1, /*blockNumber*/ 0);
  if (!specColumn)
    return;
  ksapi::ISpecificationColumnItemsPtr items = specColumn->GetColumnItems();
  if (!items)
    return;
  ksapi::ISpecificationColumnItemPtr item = items->GetItem(/*index*/ 0);
  if (!item)
    return;
  item->SetValue(L"Втулка");

  specObject->Edit(); // Дать возможность пользователю отредактировать объект.

  // Добавление позиционной линии-выноски к объекту спецификации.
  ksapi::IPositionLeaderPtr leader = GetPosLeaderFromUser(kDoc2D);
  if (!leader)
    return;

  specObject->IncludeGeometry({leader}, /*fillText*/ false);
  specObject->Update();
}


//-------------------------------------------------------------------------
// Создать объект для раздела 'Стандартные изделия'.
// ---
void CreateStandart(ksapi::IKompasDocument2D & kDoc2D)
{
  // Если в режиме редактирования - передать редактируемый объект, иначе - nullptr
  ksapi::IKompasAPIObjectPtr geometry = kDoc2D.GetEditMacroObject();

  // Создание объекта спецификации "Стандартные изделия"
  ksapi::ISpecificationBaseObjectPtr specObject =
    CreateSpecificationObject(kDoc2D, /*geometry*/ geometry, /*sectionNumber*/ 25, kBoltAttrTypeID);
  if (!specObject)
    return;

  // Структура для наполнения объекта спецификации.
  Bolt bolt{
    .dr = 20,    // Диаметр резьбы
    .s = 27,     // Размер под ключ
    .h = 11.f,   // Высота головки
    .D = 30.1f,  // Диаметр описанной окружности
    .h2 = 10.f,  // Высота подголовка
    .L = 55.f,   // Длина стержня
    .l1 = 49.f,  // Расстояние до отверстия в стержне
    .b = 46.f,   // Длина резьбовой части
    .d2 = 22.5f, // Диаметр под головкой
    .k = 2,      // Расположение резьбы ( 1 - длина резьбы ниже ломманной( резьба только на b ),
                 //                       2 - выше ломанной ( резьба до головки и на в ) )
    .cl = 1,     // Класс точности (B)
    .gost = 7808 // Номер госта
  };

  // Наполним объект спецификации.
  FillSpecificationBolt(*specObject, bolt);

  // Войти в режим редактирования. Если пользователь нажмёт "Отмена", удалим созданный объект спецификации.
  if (!specObject->Edit())
  {
    specObject->Delete();
    return;
  }

  // Добавление позиционной линии-выноски к объекту спецификации.
  ksapi::IPositionLeaderPtr leader = GetPosLeaderFromUser(kDoc2D);
  if (!leader)
    return;
  specObject->IncludeGeometry({leader}, /*fillText*/ false);

  specObject->Update();
}


//-------------------------------------------------------------------------
// Конвертировать спецификацию во фрагмент.
// ---
void DecomposeSpc()
{
  ksapi::ISpecificationDocumentPtr docSpec = kompasApp->GetActiveDocument();
  if (!docSpec)
  {
    kompasApp->ShowMessageBox(L"Спецификация должна быть текущей", L"", ksMessageWarning, ksButtonSetOk, false);
    return;
  }
  double x1, x2, y1, y2;
  if (!docSpec->GetDocumentGabarit(x1, y1, x2, y2))
    return;

  ksapi::ILayoutSheetsPtr specificationSheets = docSpec->GetLayoutSheets();
  if (!specificationSheets)
    return;

  ksapi::IDocumentsPtr documents = kompasApp->GetDocuments();
  if (!documents)
    return;
  ksapi::IFragmentDocumentPtr docFragment = documents->Add(DocumentTypeEnum::ksDocumentFragment, /*visible*/ true);
  if (!docFragment)
    return;

  // Для каждой страницы спецификации необходимо вызвать метод разбиения документа.
  int32_t decomposed = 0;
  for (int32_t i = 0; i < specificationSheets->GetCount(); ++i)
  {
    ksapi::IDrawingGroupPtr drGroup =
      docFragment->DecomposeSpecificationDocument(docSpec, ksDecomposeObjectLevelEnum::ksDOLSegmentsArcsTextsPoints,
                                                  /*arrow*/ 0.4, i + 1);
    if (!drGroup || !drGroup->Store())
      continue;
    // Координаты листов, в ряд по 3.
    int32_t column = decomposed % 3;
    int32_t row = decomposed / 3;
    double x = (x2 - x1 + 5) * static_cast<double>(column);
    double y = -(y2 - y1 + 5) * static_cast<double>(row);

    if (docFragment->MoveObjects({drGroup}, x, y))
      ++decomposed;
  }

  kompasApp->ShowMessageBox(std::format(L"Декомпозировано {} листов спецификации", decomposed), L"", ksMessageInformation,
                            ksButtonSetOk, true);
}


//-------------------------------------------------------------------------
// Просмотреть текущую спецификацию.
// ---
void ShowSpc()
{
  // Получение коллекции спецификаций из 2D документа, или документа-спецификации.
  ksapi::ISpecificationDescriptionsPtr specDescriptions;
  if (ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument())
    specDescriptions = doc2D->GetSpecificationDescriptions();
  else if (ksapi::ISpecificationDocumentPtr specDoc = kompasApp->GetActiveDocument())
    specDescriptions = specDoc->GetSpecificationDescriptions();
  if (!specDescriptions)
    return;

  // Получение объектов активной спецификации.
  ksapi::ISpecificationDescriptionPtr specDescr = specDescriptions->GetActive();
  if (!specDescr)
    return;
  ksapi::ISpecificationBaseObjectsPtr specObjectsCollection = specDescr->GetBaseObjects();
  if (!specObjectsCollection)
    return;
  std::vector<ksapi::IKompasAPIObjectPtr> specObjects = specObjectsCollection->GetObjects();

  // Пройтись по всем объектам и вывести текст для каждой колонки каждого объекта спецификации
  for (ksapi::ISpecificationBaseObjectPtr specObject : specObjects)
  {
    if (!specObject)
      continue;

    ksapi::ISpecificationColumnsPtr columnsCollection = specObject->GetColumns();
    if (!columnsCollection)
      continue;
    std::vector<ksapi::IKompasAPIObjectPtr> columns = columnsCollection->GetObjects();

    int32_t i = 1;
    // Вывод текста каждой колонки объекта спецификации
    for (ksapi::ISpecificationColumnPtr column : columns)
    {
      ksapi::ITextPtr columnText = column->GetText();
      if (!columnText)
        continue;

      kompasApp->ShowMessageBox(std::format(L"{},\ncolumnType = {}, block = {},\ni = {}, colNumb = {}", columnText->GetStr(),
                                            static_cast<int32_t>(column->GetColumnType()), column->GetBlockNumber(), i++,
                                            column->GetNumber() + 1),
                                L"", ksMessageWarning, ksButtonSetOk, true);
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

  LoadResources(app);

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
  Создание прямоугольника.
*/
//---
ksapi::IRectanglePtr CreateRectangle(ksapi::IKompasDocument2D & kDoc2D)
{
  ksapi::IViewsAndLayersManagerPtr viewAndLayerManager = kDoc2D.GetViewsAndLayersManager();
  if (!viewAndLayerManager)
    return nullptr;
  ksapi::IViewsPtr views = viewAndLayerManager->GetViews();
  if (!views)
    return nullptr;
  ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView();
  if (!drawingContainer)
    return nullptr;
  ksapi::IRectanglesPtr rectangles = drawingContainer->GetRectangles();
  if (!rectangles)
    return nullptr;
  ksapi::IRectanglePtr rectangle = rectangles->Add();
  if (!rectangle)
    return nullptr;

  rectangle->SetX(20);
  rectangle->SetY(30);
  rectangle->SetHeight(50);
  rectangle->SetWidth(50);
  rectangle->SetStyle(ksCSThin);
  rectangle->Update();

  return rectangle;
}

//------------------------------------------------------------------------------
/**
  Создать объект спецификации для заданного графического объекта.
*/
//---
ksapi::ISpecificationBaseObjectPtr CreateSpecificationObject(ksapi::IKompasDocument2D & kDoc2D,
                                                             ksapi::IKompasAPIObjectPtr geometry, int32_t sectionNumber,
                                                             ksapi::NumericId attrTypeNum)
{
  ksapi::ISpecificationDescriptionsPtr specDescriptions = kDoc2D.GetSpecificationDescriptions();
  if (!specDescriptions)
    return nullptr;

  ksapi::ISpecificationDescriptionPtr specDescription = specDescriptions->GetActive();
  if (!specDescription)
    specDescription = specDescriptions->Add(/*layoutName*/ kTypesLibraryName, /*styleId*/ 1, L"");
  if (!specDescription)
    return nullptr;
  ksapi::ISpecificationBaseObjectsPtr specObjects = specDescription->GetBaseObjects();
  if (!specObjects)
    return nullptr;
  std::vector<ksapi::ISpecificationBaseObjectPtr> findedObjects =
    specObjects->GetSpecificationObjectsForGeom(/*geometry*/ {geometry}, /*equal*/ true, sectionNumber, attrTypeNum);

  ksapi::ISpecificationBaseObjectPtr specObject;
  if (findedObjects.empty() || !findedObjects[0])
  {
    specObject = specObjects->Add(sectionNumber, attrTypeNum);
    if (!specObject)
      return nullptr;
    specObject->SetGeometry({geometry});
    specObject->SetIncrementPosition(true);
    specObject->Update();
  }
  else
    specObject = findedObjects[0];

  return specObject;
}

//------------------------------------------------------------------------------
/**
  Ожидание выбора объекта пользователем
*/
//---
ksapi::IDrawingObjectPtr SelectObjectOnCurrentView(ksapi::IKompasDocument2D & document, const std::wstring & caption)
{
  ksapi::IProcess2DPtr process2d = document.GetLibProcess(ksProcess2DTypeEnum::ksProcess2DCursor);
  ksapi::IProcessPtr process = process2d;
  if (!process)
    return nullptr;
  ksapi::IProcess2DEventsPtr proc2DEvents = process2d->Events();
  if (!proc2DEvents)
    return nullptr;
  ksapi::IViewPtr currentView = GetDrawingContainer(document);
  if (!currentView)
    return nullptr;

  // Признак, какой пункт меню выбрал пользователь. 3 варианта - "создать новую", "выбрать существующую" и ничего не выбрал
  std::optional<bool> choosePosLeader;

  // функция, которая ищет объект по координатам, будет вызываться по событию клика мыши
  ksapi::IDrawingObjectPtr result;
  auto clickCallback = [currentView, &result, &choosePosLeader](double x, double y, double, bool)
  {
    if (!choosePosLeader && !*choosePosLeader)
      return false;

    ksapi::IFindObjectParametersPtr params = nullptr;
    result = currentView->FindObject(x, y, 1e-6, params);

    // после того как объект найден, вернём false, что бы удалить эту функцию из контейнера
    if (result)
      return false;

    kompasApp->ShowMessageBox(L"Объект не позиционная линия выноски!", L"", ksMessageInformation, ksButtonSetOk, true);
    return false;
  };

  // добавление колбека в контейнер обработчиков событий
  proc2DEvents->AddPlacementChangedHandler(L"Step10", clickCallback);

  process->SetCaption(caption);
  ksapi::IMenuPtr menu = kompasApp->CreateMenu();
  menu->Load(MENU_POS_LEADER, GetResourceModule());
  process->SetMenu(menu);

  // функция, ждёт нажатия кнопки меню. возвращает нажатую кнопку
  auto menuCallback = [&choosePosLeader, process](int32_t id)
  {
    choosePosLeader = id == CHOOSE_EXITING;
    return false;
  };

  proc2DEvents->AddExecuteCommandHandler(L"Step10", menuCallback);

  process->Run(/*modal*/ true, /*postMessage*/ false);
  process->SetMenu(nullptr);

  if (!choosePosLeader)
  {
    proc2DEvents->RemoveAllHandlers(L"Step10");
    return nullptr;
  }

  // пользователь пожелал создать новую линию выноски
  if (*choosePosLeader)
    process->Run(/*modal*/ true, /*postMessage*/ false); // Повторный запуск для выбора линии-выноски
  else
    result = document.RunCreateObjectProcess(ProcessTypeEnum::prPositionLeader);

  proc2DEvents->RemoveAllHandlers(L"Step10");

  return result;
}

//------------------------------------------------------------------------------
/**
  Получение активного вида, как IDrawingContainer.
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
  Получение от пользователя линии-выноски (создание новой или указание существующей).
*/
//---
ksapi::IPositionLeaderPtr GetPosLeaderFromUser(ksapi::IKompasDocument2D & kDoc2D)
{
  return SelectObjectOnCurrentView(kDoc2D, L"Укажите позиционную линию выноски");
}


//------------------------------------------------------------------------------
/**
  Наполнение объекта спецификации данными болта.
*/
//---
void FillSpecificationBolt(ksapi::ISpecificationBaseObject & specObj, const Bolt & bolt)
{
  ksapi::ISpecificationColumnsPtr specColumns = specObj.GetColumns();
  if (!specColumns)
    return;
  ksapi::ISpecificationColumnPtr specColumn = specColumns->GetColumn(ksSColumnName, /*columnNumber*/ 1, /*blockNumber*/ 0);
  if (!specColumn)
    return;
  ksapi::ISpecificationColumnItemsPtr items = specColumn->GetColumnItems();
  if (!items)
    return;

  // Название
  if (ksapi::ISpecificationColumnItemPtr name = items->GetItem(/*index*/ 0))
    name->SetValue(L"Болт");

  // Исполнение
  if (ksapi::ISpecificationColumnItemPtr embodiment = items->GetItem(/*index*/ 1))
  {
    // Если флаг наличия исполнения не установлен - делаем его невимдимым, если установлен - устанавливаем значение.
    if ((bolt.f & EBoltFlags::EMBODIMENT) == 0)
      embodiment->SetVisible(false);
    else
    {
      embodiment->SetVisible(true);
      embodiment->SetValue(2);
    }
  }

  // Диаметр
  if (ksapi::ISpecificationColumnItemPtr diameter = items->GetItem(/*index*/ 3))
  {
    diameter->SetVisible(bolt.dr < 0 ? false : true);
    diameter->SetValue(bolt.dr);
  }

  // Шаг резьбы
  ksapi::ISpecificationColumnItemPtr step = items->GetItem(/*index*/ 4);
  ksapi::ISpecificationColumnItemPtr stepSeparator = items->GetItem(/*index*/ 5);
  if (step && stepSeparator)
  {
    // Если флаг наличия шага резьбы не установлен - делаем его и разделитель невимдимым, если установлен - устанавливаем значение.
    if ((bolt.f & EBoltFlags::PITCH) == 0 || bolt.p < 0)
    {
      step->SetVisible(false);
      stepSeparator->SetVisible(false);
    }
    else
    {
      step->SetVisible(true);
      stepSeparator->SetVisible(true);
      step->SetValue(bolt.p);
    }
  }

  // Поле допуска - выключаем
  if (ksapi::ISpecificationColumnItemPtr toleranceField = items->GetItem(/*index*/ 6))
    toleranceField->SetVisible(false);

  // Длина.
  if (ksapi::ISpecificationColumnItemPtr length = items->GetItem(/*index*/ 8))
  {
    length->SetVisible(bolt.L < 0 ? false : true);
    length->SetValue(static_cast<int32_t>(bolt.L));
  }

  // Класс прочности - выключаем.
  if (ksapi::ISpecificationColumnItemPtr strengthClass = items->GetItem(/*index*/ 9))
    strengthClass->SetVisible(false);

  // Материал - выключаем.
  if (ksapi::ISpecificationColumnItemPtr material = items->GetItem(/*index*/ 10))
    material->SetVisible(false);

  // Покрытие - выключаем.
  if (ksapi::ISpecificationColumnItemPtr coating = items->GetItem(/*index*/ 11))
    coating->SetVisible(false);

  // ГОСТ.
  if (ksapi::ISpecificationColumnItemPtr gost = items->GetItem(/*index*/ 13))
  {
    gost->SetVisible(true);
    gost->SetValue(static_cast<int32_t>(bolt.gost));
  }

  specObj.Update();
}
