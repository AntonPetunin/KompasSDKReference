////////////////////////////////////////////////////////////////////////////////
//
// step7.cpp - Hавигация по модели
//
// 1. Хождение по виду                    - WalkFromView
// 2. Хождение по макроэлементу           - WalkFromMacro
// 3. Хождение по документам              - WalkFromDoc
// 4. Хождение по видам                   - WalkViewDoc
// 5. Хождение по группам                 - WalkGroup
// 6. Хождение по слоям                   - WalkLayer
// 7. Хождение по группе                  - WalkFromGroup
// 8. Хождение по элементам с атрибутом   - WalkFromDocWithAttr
// 9. Хождение по атрибутам объекта       - WalkFromObjWithAttr
// 10.Хождение по квалитетам              - WalkFromQuality
// 11.Получить отклонения                 - QualityDefects
//
////////////////////////////////////////////////////////////////////////////////

#include <format>

#include <KsAPI.h>
#include <KompasLibraryActions.h>

ksapi::IApplication * kompasApp = nullptr;

//-------------------------------------------------------------------------------
// Описания используемых функций.
// ---
void WalkFromView(ksapi::IKompasDocument2D & document);
void WalkFromMacro(ksapi::IKompasDocument2D & document);
void WalkFromDoc();
void WalkViewDoc(ksapi::IKompasDocument2D & document);
void WalkGroup(ksapi::IKompasDocument2D & document);
void WalkLayer(ksapi::IKompasDocument2D & document);
void WalkFromGroup(ksapi::IKompasDocument2D & document);
void WalkFromDocWithAttr(ksapi::IKompasDocument2D & document);
void WalkFromObjWithAttr(ksapi::IKompasDocument2D & document);
void WalkFromQuality(ksapi::IKompasDocument2D & document);
void QualityDefects(ksapi::IKompasDocument2D & document);


//-------------------------------------------------------------------------------
// Описания Вспомогательных функций.
// ---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & kDoc2D);
template<typename KsApiObjectPtr>
void ShowObjectsToUser(ksapi::IKompasDocument2D & document, const std::vector<KsApiObjectPtr> & objects,
                       const std::wstring & oneObjectName);
void ShowObjectToUser(ksapi::IChooseManager & chooseManager, ksapi::IKompasAPIObjectPtr object, const std::wstring & objName);
ksapi::IKompasDocument2DPtr CreateDocument(ksapi::IDocuments & documents, const std::wstring & name);
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IDrawingContainerPtr drawingContainer, double x1, double y1, double x2,
                                         double y2);
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r);
ksapi::IArcPtr CreateArc(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r, double angle1,
                         double angle2);
ksapi::IViewPtr CreateView(ksapi::IViews & views, int32_t num);
ksapi::ILayerPtr CreateLayer(ksapi::IView & view, int32_t num);
ksapi::IHatchPtr CreateHatch(ksapi::IDrawingContainerPtr drawingContainer, const std::vector<ksapi::IDrawingObjectPtr> & objects);
template<typename Quality, bool kIsStringQuality = std::is_same_v<Quality, std::wstring>>
void PrintQuality(Quality quality, ksQualitySystemEnum systemType = ksQualSystemHole);

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
  if (commandId != 3)
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
      WalkFromView(*document);
      break;
    case 2:
      WalkFromMacro(*document);
      break;
    case 3:
      WalkFromDoc();
      break;
    case 4:
      WalkViewDoc(*document);
      break;
    case 5:
      WalkGroup(*document);
      break;
    case 6:
      WalkLayer(*document);
      break;
    case 7:
      WalkFromGroup(*document);
      break;
    case 8:
      WalkFromDocWithAttr(*document);
      break;
    case 9:
      WalkFromObjWithAttr(*document);
      break;
    case 10:
      WalkFromQuality(*document);
      break;
    case 11:
      QualityDefects(*document);
      break;
  }
}

//-------------------------------------------------------------------------------
/**
  Перебор документов.
  Создаются 3 документа.
  Перебираются через коллекцию и в каждом рисуется свой примитив.
*/
// ---
void WalkFromDoc()
{
  ksapi::IDocumentsPtr documents = kompasApp->GetDocuments();
  if (!documents)
    return;

  // Создать 3 документа "Чертёж".
  CreateDocument(*documents, L"a.cdw");
  CreateDocument(*documents, L"b.cdw");
  CreateDocument(*documents, L"c.cdw");

  // В цикле по массиву объектов коллекции создаётся примитив в кажджом из документов
  int32_t i = 0;
  for (ksapi::IKompasDocument2DPtr document : documents->GetObjects())
  {
    if (!document)
      continue;

    document->SetActive();

    // Для каждого документа свой DrawingContainer - активный вид.
    ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(*document);

    switch (++i)
    {
      case 1: // В первом документе отрезок.
        CreateLineSegment(drawingContainer, /*x1*/ 20, /*y1*/ 10, /*x2*/ 40, /*y2*/ 10);
        break;
      case 2: // Во втором документе окружность.
        CreateCircle(drawingContainer, /*xc*/ 50, /*yc*/ 50, /*r*/ 20);
        break;
      case 3: // В третьем документе дуга.
        CreateArc(drawingContainer, /*xc*/ 50, /*yc*/ 50, /*r*/ 20, /*angle1*/ 45, /*angle2*/ 135);
        break;
    }
  }

  kompasApp->ShowMessageBox(std::format(L"Count = {}", i), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор элементов вида.
*/
// ---
void WalkFromView(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;
  std::vector<ksapi::IDrawingObjectPtr> objects = drawingContainer->GetObjects(/*objTypes*/ {DrawingObjectTypeEnum::ksAllObj});

  ShowObjectsToUser(document, objects, L"объекта");

  kompasApp->ShowMessageBox(std::format(L"Count = {}", objects.size()), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор макроэлементов.
*/
// ---
void WalkFromMacro(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
  if (!drawingContainer || !chooseManager)
    return;

  // Получение только макрообъектов вида.
  std::vector<ksapi::IDrawingObjectPtr> objects = drawingContainer->GetObjects(/*objTypes*/ {DrawingObjectTypeEnum::ksDrMacro});

  size_t macroCount = 0;
  size_t objectsCount = 0;
  for (ksapi::IMacroObjectPtr macroObject : objects)
  {
    // Макрообъект сам является DrawingContainer-ом, как и вид.
    ksapi::IDrawingContainerPtr macroDrawingContainer = macroObject;
    if (!macroDrawingContainer)
      continue;

    // Показать весь макрообъект.
    ShowObjectToUser(*chooseManager, macroObject, L"макроэлемента");

    // Показать принадлежащие макрообъекту объекты.
    std::vector<ksapi::IDrawingObjectPtr> nestedOjects = macroDrawingContainer->GetObjects({DrawingObjectTypeEnum::ksAllObj});
    ++macroCount;
    objectsCount += nestedOjects.size();
    ShowObjectsToUser(document, nestedOjects, L"объекта");
  }

  kompasApp->ShowMessageBox(std::format(L"count = {}, count1 = {}", macroCount, objectsCount), L"", ksMessageWarning,
                            ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор видов.
*/
// ---
void WalkViewDoc(ksapi::IKompasDocument2D & document)
{
  ksapi::IViewsAndLayersManagerPtr viewsAndLayerManager = document.GetViewsAndLayersManager();
  if (!viewsAndLayerManager)
    return;
  ksapi::IViewsPtr views = viewsAndLayerManager->GetViews();
  if (!views)
    return;

  // Создать 3 вида (если вид с заданным номером существует, то не создастся).
  CreateView(*views, /*num*/ 1);
  CreateView(*views, /*num*/ 2);
  CreateView(*views, /*num*/ 3);

  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
  if (!chooseManager)
    return;

  // Пройти в массиве по всем существующим видам документа и в созданых отобразить примитивы.
  int32_t i = 0;
  for (ksapi::IViewPtr view : views->GetObjects())
  {
    ksapi::IDrawingContainerPtr drawingContainer = view;
    if (!drawingContainer)
      continue;

    view->SetCurrent(true); // Активизируем вид

    // Нулевой должен быть системный
    switch (i++)
    {
      case 1: // В первом виде отрезок
        CreateLineSegment(drawingContainer, /*x1*/ 20, /*y1*/ 20, /*x2*/ 40, /*y2*/ 20);
        break;
      case 2: // Во втором окружность
        CreateCircle(drawingContainer, /*xc*/ 40, /*yc*/ 20, /*r*/ 30);
        break;
      case 3: // В третье дуга
        CreateArc(drawingContainer, /*xc*/ 50, /*yc*/ 50, /*r*/ 20, /*angle1*/ 45, /*angle2*/ 135);
        break;
    }
  }

  kompasApp->ShowMessageBox(std::format(L"count = {}", i), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор групп.
*/
// ---
void WalkGroup(ksapi::IKompasDocument2D & document)
{
  // Показать существующие именованные группы пользователю
  ksapi::IDrawingGroupsPtr namedGroups = document.GetNamedGroups();
  if (!namedGroups)
    return;
  ShowObjectsToUser(document, namedGroups->GetObjects(), L"группы");

  kompasApp->ShowMessageBox(std::format(L"count = {}", namedGroups->GetCount()), L"", ksMessageWarning, ksButtonSetOk, true);

  // В активном виде документа создать примитивы и штриховку внутри окружностей.
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  ksapi::ICirclePtr circle1 = CreateCircle(drawingContainer, /*xc*/ 30, /*yc*/ 30, /*r*/ 20);
  ksapi::ICirclePtr circle2 = CreateCircle(drawingContainer, /*xc*/ 30, /*yc*/ 30, /*r*/ 10);
  ksapi::IHatchPtr hatch = CreateHatch(drawingContainer, {circle1, circle2});

  // Создать группу. поместить туда созданные объекты
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"");
  if (!group)
    return;
  group->AddObjects({circle1, circle2, hatch});

  // Показать объекты группы
  ShowObjectsToUser(document, groups->GetObjects(), L"группы");

  kompasApp->ShowMessageBox(std::format(L"count1 = {}", groups->GetCount()), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор слоёв.
*/
// ---
void WalkLayer(ksapi::IKompasDocument2D & document)
{
  ksapi::IViewPtr view = GetDrawingContainer(document);
  if (!view)
    return;

  // Создать 3 слоя, и в каждом по окружности.
  for (int32_t i = 0; i < 5; ++i)
  {
    ksapi::ILayerPtr layer = CreateLayer(*view, i);
    ksapi::ICirclePtr circle = CreateCircle(view, /*xc*/ 30, /*yc*/ 30, /*r*/ 5 + i * 10);
    if (layer && circle)
    {
      circle->SetLayerNumber(i);
      circle->Update();
    }
  }

  // Показать созданные окружности
  ksapi::ILayersPtr layers = view->GetLayers();
  if (!layers)
    return;

  ShowObjectsToUser(document, layers->GetObjects(), L"слоя");

  kompasApp->ShowMessageBox(std::format(L"count = {}", layers->GetCount()), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор объектов группы.
*/
// ---
void WalkFromGroup(ksapi::IKompasDocument2D & document)
{
  ksapi::IDrawingContainerPtr drawingContainer = GetDrawingContainer(document);
  if (!drawingContainer)
    return;

  // Создать и добавить в массив некоторое количество объектов.
  std::vector<ksapi::IDrawingObjectPtr> groupObjects;
  groupObjects.emplace_back(CreateLineSegment(drawingContainer, /*x1*/ 10, /*y1*/ 50, /*x2*/ 50, /*y2*/ 50));
  groupObjects.emplace_back(CreateLineSegment(drawingContainer, /*x1*/ 10, /*y1*/ 10, /*x2*/ 50, /*y2*/ 10));
  groupObjects.emplace_back(CreateLineSegment(drawingContainer, /*x1*/ 10, /*y1*/ 10, /*x2*/ 10, /*y2*/ 50));
  groupObjects.emplace_back(CreateLineSegment(drawingContainer, /*x1*/ 50, /*y1*/ 10, /*x2*/ 50, /*y2*/ 50));
  groupObjects.emplace_back(CreateCircle(drawingContainer, /*xc*/ 30, /*yc*/ 30, /*r*/ 20));
  groupObjects.emplace_back(CreateCircle(drawingContainer, /*xc*/ 30, /*yc*/ 30, /*r*/ 10));
  groupObjects.emplace_back(CreateHatch(drawingContainer, {groupObjects[4], groupObjects[5]}));

  // Создать группу и добавить в неё массив созданых объектов.
  ksapi::IDrawingGroupsPtr groups = document.GetDrawingGroups();
  if (!groups)
    return;
  ksapi::IDrawingGroupPtr group = groups->Add(/*temp*/ false, L"");
  if (!group)
    return;
  group->AddObjects(groupObjects);

  // Показать все объекты группы.
  ShowObjectsToUser(document, group->GetObjects({ksAllObj}), L"объекта");

  kompasApp->ShowMessageBox(std::format(L"count = {}", groupObjects.size()), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор элементов документа с определенным атрибутом.
*/
// ---
void WalkFromDocWithAttr(ksapi::IKompasDocument2D & document)
{
  // Получение всех атрибутов с key1 == 10.
  const std::vector<ksapi::IAttributePtr> attributes =
    document.GetAttributes(/*key1*/ 10, /*key2*/ 0, /*key3*/ 0, /*key4*/ 0, /*number*/ 0, /*objectTypes*/ {});

  // Для каждого атрибута показать количество строк, столбцов и объекты с этим атрибутом.
  int32_t i = 0;
  for (ksapi::IAttributePtr attribute : attributes)
  {
    if (!attribute)
      continue;

    const std::vector<ksapi::IKompasAPIObjectPtr> objectsWithAttribute = attribute->GetObjects();

    // Менеджер подсветки объектов
    ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
    if (!chooseManager)
      return;

    // Подсветка объектов с атрибутом
    chooseManager->Choose(objectsWithAttribute);

    kompasApp->ShowMessageBox(std::format(L"count = {}, countRows = {}, columnsCount = {}", ++i, attribute->GetRowsCount(),
                                          attribute->GetColumnsCount()),
                              L"", ksMessageWarning, ksButtonSetOk, true);

    chooseManager->UnchooseAll();
  }

  kompasApp->ShowMessageBox(std::format(L"count = {}", i), L"", ksMessageWarning, ksButtonSetOk, true);
}

//-------------------------------------------------------------------------------
/**
  Перебор атрибутов объекта.
*/
// ---
void WalkFromObjWithAttr(ksapi::IKompasDocument2D & document)
{
  // Процесс для обработки нажатия мыши
  ksapi::IProcess2DPtr process2d =
    document.GetLibProcess(ksProcess2DTypeEnum::ksProcess2DCursor); // Через этот интерфейс получим контейнер обработчиков.
  ksapi::IProcessPtr process = process2d;                           // Через этот интерфейс установим заголовок и запустим процесс.

  // Текущий вид передадим в обработчик события клика мыши для поиска объектов.
  ksapi::IViewPtr currentView = GetDrawingContainer(document);

  // Менеджер подсветки объектов передадим в обработчик события клика мыши для подсветки найденых объектов.
  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();

  if (!process || !currentView || !chooseManager)
    return;

  process->SetCaption(L"Укажите объект"); // Заголовок процесса

  // Обработчик события клика мыши - выполняет поиск объекта по координатам (СК активного вида) клика, и перебирает атрибуты объекта.
  auto findAndShowObjectAndAttributes = [currentView, &document, chooseManager](double x, double y, double, bool)
  {
    // Поиск объекта
    ksapi::IDrawingObjectPtr selected = currentView->FindObject(x, y, /*limit*/ 1e-6, /*params*/ nullptr);
    if (!selected)
      return true; // что бы не завершился процесс нужно вернуть true.

    chooseManager->Choose({selected}); // Подсветка найденого объекта.

    // Получение атрибутов объекта с key1 == 10.
    const std::vector<ksapi::IAttributePtr> attributes =
      document.GetAttributes(/*key1*/ 10, /*key2*/ 0, /*key3*/ 0, /*key4*/ 0, /*number*/ 0, /*objects*/ {selected});

    int32_t i = 0;
    for (ksapi::IAttributePtr attribute : attributes)
    {
      if (!attribute)
        continue;
      std::wstring typeName;
      if (ksapi::IAttributeTypePtr attributeType = attribute->GetAttributeType())
        typeName = attributeType->GetTypeName();

      kompasApp->ShowMessageBox(std::format(L"count = {}, countRows = {}, columnsCount = {}", ++i, attribute->GetRowsCount(),
                                            attribute->GetColumnsCount()),
                                L"", ksMessageWarning, ksButtonSetOk, true);
    }

    chooseManager->UnchooseAll(); // Убрать подсветку объекта.

    return true; // Возврат true - не убирать обработчик из контейнера и не завершать процесс.
  };


  // добавление колбека в контейнер обработчиков событий.
  if (ksapi::IProcess2DEventsPtr events = process2d->Events())
    events->AddPlacementChangedHandler(L"step7", findAndShowObjectAndAttributes);

  // Запуск процесса.
  process->Run(/*modal*/ true, /*postMessage*/ false);
}

//-------------------------------------------------------------------------------
/**
  Перебор квалитетов.
*/
// ---
void WalkFromQuality(ksapi::IKompasDocument2D & document)
{
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();
  if (!dialogs)
    return;

  // Запрос имени квалитета у пользователя.
  std::wstring qualityName = L"H7";
  bool readed = dialogs->ReadString(/*windowHandle*/ nullptr,
                                    /*caption*/ L"Укажите поле допуска",
                                    /*maxLength*/ 5,
                                    /*value*/ qualityName);
  if (!readed)
    return;

  if (!qualityName.empty())
  {
    PrintQuality(qualityName);
    return;
  }

  // Если пользователь не указал имени, выведем квалитеты для вала, первые 5 штук.
  ksapi::IQualityManagerPtr qualityManager = kompasApp->GetQualityManager();
  if (!qualityManager)
    return;

  for (int32_t i = 0; i < 5 && i < qualityManager->GetQualityCount(ksQualSystemHole, /*withLimitation*/ true); ++i)
    PrintQuality(i, ksQualitySystemEnum::ksQualSystemShaft);
}

//-------------------------------------------------------------------------------
/**
  Получить отклонения. 
  Сначала выводятся имена квалитетов, удовлетворяющих условиям, 
  затем выводятся отклонения для указанного пользователем квалитета
*/
// ---
void QualityDefects(ksapi::IKompasDocument2D & document)
{
  ksapi::IApplicationDialogsPtr dialogs = kompasApp->GetApplicationDialogs();
  if (!dialogs)
    return;

  // Запрос имени квалитета у пользователя.
  std::wstring qualityName = L"H7";
  bool readed = dialogs->ReadString(/*windowHandle*/ nullptr,
                                    /*caption*/ L"Укажите поле допуска",
                                    /*maxLength*/ 5,
                                    /*value*/ qualityName);
  if (!readed)
    return;

  ksapi::IQualityManagerPtr qualityManager = kompasApp->GetQualityManager();
  if (!qualityManager)
    return;

  const std::vector<std::wstring> namesShaftSystem =
    qualityManager->GetQualityNames(/*dimensionValue*/ 100, /*maxDeviation*/ 0.2, /*minDeviation*/ 0.1,
                                    ksQualitySystemEnum::ksQualSystemShaft, /*withLimitation*/ true);


  // Вывод в сообщение квалитетов.
  std::wstring msg;
  size_t step = namesShaftSystem.size() / 5;
  for (size_t i = 0; i < namesShaftSystem.size(); i += step)
    msg += namesShaftSystem[i] + L'\n';
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);

  double maxDeviation, minDeviation;
  const double dimensionValue = 100;
  qualityManager->GetQualityDefects(qualityName, dimensionValue, /*display*/ false, maxDeviation, minDeviation);
  kompasApp->ShowMessageBox(std::format(L"dimValue = {}, high = {}, low = {}", dimensionValue, maxDeviation, minDeviation), L"",
                            ksMessageWarning, ksButtonSetOk, true);
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
  Получить активный IDrawingContainer.
*/
//---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2D & kDoc2D)
{
  ksapi::IViewsAndLayersManagerPtr viewsAndLayerManager = kDoc2D.GetViewsAndLayersManager();
  if (!viewsAndLayerManager)
    return nullptr;
  ksapi::IViewsPtr views = viewsAndLayerManager->GetViews();
  if (!views)
    return nullptr;
  return views->GetActiveView();
}

//------------------------------------------------------------------------------
/**
  Подсветить объекты поочереди и вывести диалоговое окно для каждого подсвеченного объекта.
*/
//---
template<typename KsApiObjectPtr>
void ShowObjectsToUser(ksapi::IKompasDocument2D & document, const std::vector<KsApiObjectPtr> & objects,
                       const std::wstring & oneObjectName)
{
  if (objects.empty())
    return;

  // ChooseManager отвечает за подсветку объектов.
  ksapi::IChooseManagerPtr chooseManager = document.GetChooseManager();
  if (!chooseManager)
    return;

  for (ksapi::IKompasAPIObjectPtr object : objects)
    ShowObjectToUser(*chooseManager, object, oneObjectName);
}

//------------------------------------------------------------------------------
/**
  Подсветить объект и вывести диалоговое окно.
*/
//---
void ShowObjectToUser(ksapi::IChooseManager & chooseManager, ksapi::IKompasAPIObjectPtr object, const std::wstring & objName)
{
  if (!chooseManager.Choose({object}))
    return;

  kompasApp->ShowMessageBox(std::format(L"Снять выделение {}", objName), L"", ksMessageWarning, ksButtonSetOk, true);

  chooseManager.Unchoose({object});
}


//------------------------------------------------------------------------------
/**
  Создать документ и задать параметры для его листа.
*/
//---
ksapi::IKompasDocument2DPtr CreateDocument(ksapi::IDocuments & documents, const std::wstring & name)
{
  ksapi::IKompasDocument2DPtr document = documents.Add(ksDocumentDrawing, /*visible*/ true);
  if (!document)
    return nullptr;

  // Установка параметров документа. Для установки имени документа, требуется его сохранить.
  document->SetAuthor(L"User");
  document->SetComment(L"Create document");
  document->SaveAs(name);

  ksapi::ILayoutSheetsPtr layoutSheets = document->GetLayoutSheets();
  if (!layoutSheets)
    return nullptr;

  // Получаем автоматически созданный лист документа для изменения его параметров
  ksapi::ILayoutSheetPtr layoutSheet = layoutSheets->GetItem(0);
  if (!layoutSheet)
    return nullptr;
  ksapi::ISheetFormatPtr sheetFormat = layoutSheet->GetFormat();
  if (!sheetFormat)
    return nullptr;

  sheetFormat->SetFormat(ksDocumentFormatEnum::ksFormatA3); // Формат листа
  sheetFormat->SetFormatMultiplicity(1);                    // Кратность размеров листа стандартному формату
  sheetFormat->SetVerticalOrientation(true);                // Ориентация листа, расположение штампа

  layoutSheet->SetLayoutLibraryFileName(/*fileName*/ L"");            // Имя библиотеки оформления, пустая строка - библиотека "Graphic.lyt"
  layoutSheet->SetSheetType(ksSheetTypeEnum::ksFrontAdditionalSheet); // Тип штампа из указанной библиотеки
  layoutSheet->Update();

  return document;
}


//------------------------------------------------------------------------------
/**
  Создать отрезок.
*/
//---
ksapi::ILineSegmentPtr CreateLineSegment(ksapi::IDrawingContainerPtr drawingContainer, double x1, double y1, double x2, double y2)
{
  if (!drawingContainer)
    return nullptr;
  ksapi::ILineSegmentsPtr lines = drawingContainer->GetLineSegments();
  if (!lines)
    return nullptr;
  ksapi::ILineSegmentPtr line = lines->Add();
  if (!line)
    return nullptr;

  line->SetX1(x1);
  line->SetY1(y1);
  line->SetX2(x2);
  line->SetY2(y2);
  line->Update();

  return line;
}


//------------------------------------------------------------------------------
/**
  Создать окружность.
*/
//---
ksapi::ICirclePtr CreateCircle(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r)
{
  if (!drawingContainer)
    return nullptr;
  ksapi::ICirclesPtr circles = drawingContainer->GetCircles();
  if (!circles)
    return nullptr;
  ksapi::ICirclePtr circle = circles->Add();
  if (!circle)
    return nullptr;

  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(r);
  circle->Update();

  return circle;
}

//------------------------------------------------------------------------------
/**
  Создать дугу.
*/
//---
ksapi::IArcPtr CreateArc(ksapi::IDrawingContainerPtr drawingContainer, double xc, double yc, double r, double angle1,
                         double angle2)
{
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
  arc->SetDirection(false);
  arc->SetRadius(r);
  arc->SetAngle1(angle1);
  arc->SetAngle2(angle2);

  arc->Update();

  return arc;
}

//------------------------------------------------------------------------------
/**
  Создать вид.
*/
//---
ksapi::IViewPtr CreateView(ksapi::IViews & views, int32_t num)
{
  // Если вид с таким номером уже есть, вернуть его.
  if (ksapi::IViewPtr view = views.GetViewByNumber(num))
    return view;

  ksapi::IViewPtr view = views.Add(LtViewType::vt_Normal);
  if (!view)
    return nullptr;

  view->SetNumber(num);
  view->SetX(10);
  view->SetY(20);
  view->SetScale(1);
  view->SetColor(0x0A140A);
  view->SetBackground(false);
  view->SetName(L"User view");
  view->Update();
  return view;
}

//------------------------------------------------------------------------------
/**
  Создать вид.
*/
//---
ksapi::ILayerPtr CreateLayer(ksapi::IView & view, int32_t num)
{
  ksapi::ILayersPtr layers = view.GetLayers();
  if (!layers)
    return nullptr;

  // Если слой с таким номером уже есть, вернуть его.
  if (ksapi::ILayerPtr layer = layers->GetLayerByNumber(num))
    return layer;

  ksapi::ILayerPtr layer = layers->Add();
  layer->SetLayerNumber(num);
  layer->Update();

  return layer;
}


//------------------------------------------------------------------------------
/**
  Создать штриховку.
*/
//---
ksapi::IHatchPtr CreateHatch(ksapi::IDrawingContainerPtr drawingContainer, const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ksapi::IHatchesPtr hatches = drawingContainer->GetHatches();
  if (!hatches)
    return nullptr;
  ksapi::IHatchPtr hatch = hatches->Add();
  ksapi::IBoundariesObjectPtr hatchBoundaries = hatch;
  ksapi::IHatchParamPtr hatchParam = hatchBoundaries;
  if (!hatchParam)
    return nullptr;

  hatch->SetX(0);
  hatch->SetY(0);

  hatchParam->SetStep(2);
  hatchParam->SetHatchAngle(45);

  // Границы штриховки определяются по массиву примитивов.
  hatchBoundaries->AddBoundaries(objects, /*deleteSource*/ false);

  hatch->Update();
  return hatch;
}


//------------------------------------------------------------------------------
/**
  Получить и вывести на экран информацию о квалитете.
  Квалитет может быть указан в виде имени и в виде индекса с системой.
*/
//---
template<typename Quality, bool kIsStringQuality>
void PrintQuality(Quality quality, ksQualitySystemEnum systemType)
{
  // Получение информации о квалитете.
  ksapi::IQualityManagerPtr qualityManager = kompasApp->GetQualityManager();
  ksQualityTypeEnum qualityType;
  std::vector<double> qualityParams;
  std::wstring qualityName;

  if constexpr (kIsStringQuality) // При получении квалитета по имени, возвращается тип системы для него
  {
    qualityName = quality;
    qualityParams =
      qualityManager->GetQualityContensParam(quality, /*dimValueFilter*/ 0, /*display*/ true, systemType, qualityType);
  }
  else // При получении квалитета по индексу необходимо указать тип системы
  {
    qualityParams = qualityManager->GetQualityContensParamByIndex(/*index*/ quality, qualityName, systemType,
                                                                  /*withLimitation*/ true,
                                                                  /*display*/ true, qualityType);
  }

  // Печать информации о квалитете. Выведем первые 5 параметров.
  const std::wstring systemName = systemType == ksQualitySystemEnum::ksQualSystemHole ? L"Отверстия" : L"Вала";

  std::wstring msg =
    std::format(L"Система {}, Тип квалитета {}, Поле допуска {}\n", systemName, static_cast<int32_t>(qualityType), qualityName);

  size_t step = qualityParams.size() / (5 * 4); // покажем каждый 5й параметр, по 4 элемента массива в каждом
  for (size_t i = 0; i + 3 < qualityParams.size(); i += 4 * step)
  {
    msg += std::format(L"Значение: {:.5} < X < {:.5}, high = {:.5}, low = {:.5}\n", qualityParams[i], qualityParams[i + 1],
                       qualityParams[i + 3], qualityParams[i + 2]);
  }
  kompasApp->ShowMessageBox(msg, L"", ksMessageWarning, ksButtonSetOk, true);
}
