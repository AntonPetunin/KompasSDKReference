#include <KsAPI.h>
#include <KompasLibraryActions.h>
#include "Resource.h"

#include <format>

static ksapi::IApplication * kompasApp = nullptr;

void * GetModule();
void LoadResources(ksapi::IApplication & app);

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
void CreateDocument3D();
void ChoiceAndOpenFiles();
void ChangeModelObjectColor();
void GetSetPartName(ksapi::IKompasDocument3DPtr pDocument3d);
void FixAndStandartComponent(ksapi::IKompasDocument3DPtr pDocument3d);
void GetSetColorProperty(ksapi::IKompasDocument3DPtr pDocument3d);
void GetSetArrayVariable(ksapi::IKompasDocument3DPtr pDocument3d);
void GetSetPlacmentComponent(ksapi::IKompasDocument3DPtr pDocument3d);
void GetDefaultModelObject(ksapi::IKompasDocument3DPtr pDocument3d);
void CreateSketch(ksapi::IKompasDocument3DPtr pDocument3d);
void GetArraySketch(ksapi::IKompasDocument3DPtr pDocument3d);
void GetSetUserParamComponent(ksapi::IKompasDocument3DPtr pDocument3d);

//------------------------------------------------------------------------------
/**
  Сообщение пользователю.
*/
//---
void MessageT(const std::wstring & message, const std::wstring & title = LoadResourceString(IDS_MSG_TITTLE),
              ksMessageTypeEnum dlgType = ksMessageInformation, ksMessageButtonSetEnum dlgButtons = ksButtonSetOk,
              bool showModal = false)
{
  if (kompasApp)
    kompasApp->ShowMessageDialog(nullptr, message, title, L"", dlgType, dlgButtons, showModal, LoadResourceString(IDS_YES),
                                 LoadResourceString(IDS_NO), LoadResourceString(IDS_CANCEL), 0, L"");
}

//------------------------------------------------------------------------------
/**
  Обработчик команд библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 10:
      CreateDocument3D();
      break; // Создание документа
    case 11:
      ChoiceAndOpenFiles();
      break; // Открыть указанные документы
    case 12:
      ChangeModelObjectColor();
      break; // Изменить параметры цвета объекта трехмерной модели
    default:
    {
      // Получить активный документ трехмерной модели
      ksapi::IKompasDocument3DPtr document3d = kompasApp->GetActiveDocument();

      if (document3d)
      {
        switch (commandId)
        {
          case 1:
            GetSetPartName(document3d);
            break; // Получить/установить имя компонента
          case 2:
            FixAndStandartComponent(document3d);
            break; // Фиксирование и установка стандартного компонента
          case 3:
            GetSetColorProperty(document3d);
            break; // Получить/установить параметры цвета компонента
          case 4:
            GetSetArrayVariable(document3d);
            break; // Получить/установить внешние переменные компонента
          case 5:
            GetSetPlacmentComponent(document3d);
            break; // Получить/установить место расположения детали в сборке
          case 6:
            GetDefaultModelObject(document3d);
            break; // Получить объект, создаваемый системой по умолчанию и поменять параметры
          case 7:
            CreateSketch(document3d);
            break; // Создать эскиз
          case 8:
            GetArraySketch(document3d);
            break; // Получить массив объектов (эскизов)
          case 9:
            GetSetUserParamComponent(document3d);
            break; // Получение/установка пользовательских параметров
        }
      }
      else
      {
        MessageT(LoadResourceString(IDS_WRONG_DOCUMENT), LoadResourceString(IDS_ERROR));
      }

      ksapi::IKompasErrorPtr error = kompasApp->GetKompasError();
      if (error->GetCode() == etError10) // "Ошибка! Вырожденный объект"
        error->Clear();                  // Обнулить ошибки
    }
  }
}

//-------------------------------------------------------------------------------
// Определить, является ли модель деталью
// ---
bool IsDetail(ksapi::IKompasDocument3DPtr & pDocument3d)
{
  if (!pDocument3d)
    return false;

  ksapi::IPartPtr part = pDocument3d->GetTopPart();
  return part && part->IsDetail();
}

//-------------------------------------------------------------------------------
// Получить компонент в сборке по индексу
// ---
ksapi::IPartPtr GetPartByIndex(ksapi::IKompasDocument3DPtr & pDocument3d, int32_t index)
{
  ksapi::IPartPtr part;

  if (ksapi::IPartPtr topPart = pDocument3d->GetTopPart())
  {
    if (ksapi::IPartsPtr parts = topPart->GetParts())
      part = parts->GetItem(index);
  }

  return part;
}

//-------------------------------------------------------------------------------
// Вывод информации о документе
// ---
void PrintDocumentInfo(ksapi::IKompasDocument3DPtr pDocument3d)
{
  if (!pDocument3d)
    return;

  MessageT(LoadResourceString(IDS_DOC_AUTHOR) + pDocument3d->GetAuthor());
  MessageT(LoadResourceString(IDS_DOC_COMMENT) + pDocument3d->GetComment());
  MessageT(LoadResourceString(IDS_FILENAME) + pDocument3d->GetName());
}

//-------------------------------------------------------------------------------
// Создание документа 3D
// ---
void CreateDocument3D()
{
  if (ksapi::IDocumentsPtr documents = kompasApp->GetDocuments())
  {
    // Создать документ трехмерной модели
    if (ksapi::IKompasDocument3DPtr pDocument3d = documents->AddWithDefaultSettings(ksDocumentPart, true))
    {
      // Устаноить информацию о документе
      pDocument3d->SetAuthor(LoadResourceString(IDS_AUTHOR));
      pDocument3d->SetComment(LoadResourceString(IDS_DOC3D));

      // Сохранить документ
      pDocument3d->SaveAs(L"C:\\Example.m3d");
      MessageT(LoadResourceString(IDS_SAVEAS));
      // Сохранить документ под другим именем
      pDocument3d->SaveAs(L"C:\\Example2.m3d");

      PrintDocumentInfo(pDocument3d);

      pDocument3d->Close(DocumentCloseOptions::kdDoNotSaveChanges);
    }
  }
}

//-------------------------------------------------------------------------------
// Открыть указанные документы
// ---
void ChoiceAndOpenFiles()
{
  ksapi::IApplicationDialogsPtr applicationDialogs = kompasApp->GetApplicationDialogs();

  if (!applicationDialogs)
    return;

  ksapi::IDocumentsPtr documents = kompasApp->GetDocuments();

  // Массив имён файлов, котрытых документов
  std::vector<std::wstring> rArDocFileName = applicationDialogs->ChoiceFiles(nullptr, L"*.m3d", LoadResourceString(IDS_FILTER),
                                                                             L"C://", ksSystemPathTypeEnum::ksSystemFiles, true);

  if (documents && !rArDocFileName.empty())
  {
    // Откроем все файлы указанные пользователем
    for (const auto & fileName : rArDocFileName)
      documents->Open(fileName, true, false);

    // Сообщаем количество открытых файлов
    MessageT(LoadResourceString(IDS_OPEN_FILES) + std::to_wstring(rArDocFileName.size()));

    // Итерируемся по открытым 3D документам
    for (auto docCounter = 0, count = documents->GetCount(); docCounter < count; ++docCounter)
    {
      if (ksapi::IKompasDocument3DPtr pDocument3d = documents->GetItem(docCounter))
      {
        PrintDocumentInfo(pDocument3d);

        DocumentTypeEnum documentType = pDocument3d->GetDocumentType();
        std::wstring documentTypeStr = documentType == ksDocumentPart     ? LoadResourceString(IDS_PART)
                                     : documentType == ksDocumentAssembly ? LoadResourceString(IDS_ASSEMBLY)
                                                                          : LoadResourceString(IDS_OTHER);
        MessageT(LoadResourceString(IDS_DOCTYPE) + documentTypeStr);
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Изменить параметры цвета объекта трехмерной модели
// ---
void ChangeModelObjectColor()
{
  // Получить активный документ трехмерной модели
  if (ksapi::IKompasDocument3DPtr pDocument3d = kompasApp->GetActiveDocument())
  {
    // Получить верхний компонент
    if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
    {
      ksapi::IModelContainerPtr modelContainer = part->GetModelContainer();

      if (!modelContainer)
        return;

      // Получить массив объектов заданного типа (поверхности)
      std::vector<ksapi::IModelObjectPtr> modelObjects = modelContainer->GetObjects(ksObj3dTypeEnum::o3d_face);

      if (!modelObjects.empty())
      {
        int countPlane = 0; // Количество плоских поверхностей
        int countCone = 0;  // Количество конических поверхностей

        for (auto & modelObject : modelObjects)
        {
          // Получить поверхность
          ksapi::IFacePtr face = modelObject;
          // Параметры цвета поверхности
          ksapi::IColorParamPtr colorParam = modelObject;

          if (face)
          {
            if (face->IsCone() || face->IsCylinder())
            {
              if (colorParam)
                colorParam->SetColor(0x0000FFFF);

              ++countCone; // Считаем количество конических поверхностей
            }

            if (face->IsPlanar())
            {
              if (colorParam)
                colorParam->SetColor(0x00FFFF00);

              ++countPlane; // Считаем количество плоских поверхностей
            }

            face->Update();
          }
        }

        MessageT(std::vformat(LoadResourceString(IDS_FIND_FACES_RESULT), std::make_wformat_args(countCone, countPlane)));
      }
      else
      {
        MessageT(LoadResourceString(IDS_FACES_NOT_FOUND));
      }

      // Получить массив объектов заданного типа (грани/рёбра)
      ksapi::IFeaturePtr feature = part;
      modelObjects = feature ? feature->GetModelObjects(std::vector<int32_t>{ksObj3dTypeEnum::o3d_edge})
                             : std::vector<ksapi::IModelObjectPtr>{};

      if (!modelObjects.empty())
      {
        int countRectilinear = 0; // Количество прямых ребер
        int countCurvilinear = 0; // Количество криволинейных ребер

        for (auto & modelObject : modelObjects)
        {
          // Получить ребро
          if (ksapi::IEdgePtr edge = modelObject)
          {
            if (edge->IsStraight())
              ++countRectilinear; // Считаем количество прямых ребер
            else
              ++countCurvilinear; // Считаем количество криволинейных ребер
          }
        }

        MessageT(
          std::vformat(LoadResourceString(IDS_FIND_EDGES_RESULT), std::make_wformat_args(countRectilinear, countCurvilinear)));
      }
      else
      {
        MessageT(LoadResourceString(IDS_EDGES_NOT_FOUND));
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Получить/установить имя детали или подсборки в составе сборки
// ---
void GetSetPartName(ksapi::IKompasDocument3DPtr pDocument3d)
{
  // Получить верхний компонент
  if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
  {
    // Получить имя детали или подсборки в составе сборки
    MessageT(LoadResourceString(IDS_COMPONENT_NAME) + part->GetName());

    // Сменить имя детали или подсборки в составе сборки
    part->SetName(LoadResourceString(IDS_NEW_COMPONENT_NAME));
    part->Update();
  }
}

//-------------------------------------------------------------------------------
// Фиксирование и установка стандартного компонент
// ---
void FixAndStandartComponent(ksapi::IKompasDocument3DPtr pDocument3d)
{
  if (IsDetail(pDocument3d))
  {
    // Если документ не является сборкой
    MessageT(LoadResourceString(IDS_DOC_NOT_ASSY), LoadResourceString(IDS_ERROR), ksMessageWarning);
    return;
  }

  // Получить компонент по индексу
  if (ksapi::IPartPtr part = GetPartByIndex(pDocument3d, 0))
  {
    // Получить признак фиксации компонента (если объект зафиксирован, то нельзя изменить его местоположение)
    bool is_fixed = part->IsFixed();
    MessageT(LoadResourceString(is_fixed ? IDS_COMP_FIXED : IDS_COMP_NOT_FIXED));

    // Получить признак стандартного компонента (стандратный не имеет дерева построения)
    bool is_standard = part->IsStandard();
    MessageT(LoadResourceString(is_standard ? IDS_COMP_STANDART : IDS_COMP_NOT_STANDART));

    part->SetFixed(!is_fixed);
    part->SetStandard(!is_standard);

    part->Update();
  }
}

//-------------------------------------------------------------------------------
// Вывод параметров цвета
// ---
void PrintColorParamByObj(ksapi::IColorParamPtr & colorParam)
{
  ksapi::Color color = colorParam->GetColor();    // Цвет
  double ambient = colorParam->GetAmbient(),      // Общий цвет
    diffuse = colorParam->GetDiffuse(),           // Диффузия
    specularity = colorParam->GetSpecularity(),   // Зеркальность
    shininess = colorParam->GetShininess(),       // Блеск
    transparency = colorParam->GetTransparency(), // Прозрачность
    emission = colorParam->GetEmission();         // Излучение
  MessageT(std::vformat(LoadResourceString(IDS_COLOR_PARAMS),
                        std::make_wformat_args(color, ambient, diffuse, specularity, shininess, transparency, emission)));
}

//-------------------------------------------------------------------------------
// Получить/установить параметры цвета компонента
// ---
void GetSetColorProperty(ksapi::IKompasDocument3DPtr pDocument3d)
{
  // Получить верхний компонент
  if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
  {
    // Вывод параметров цвета по значениям
    auto print_color_param = [](ksapi::Color & color, double & ambient, double & diffuse, double & specularity,
                                double & shininess, double & transparency, double & emission)
    {
      MessageT(std::vformat(LoadResourceString(IDS_COLOR_PARAMS),
                            std::make_wformat_args(color, ambient, diffuse, specularity, shininess, transparency, emission)));
    };

    // Получить параметры цвета объекта
    if (ksapi::IColorParamPtr colorParam = part)
    {
#pragma region Первый вариант
      PrintColorParamByObj(colorParam);

      // Изменение параметров цвета объекта
      colorParam->SetColor(5421504);
      colorParam->SetTransparency(0.5);
      colorParam->SetAmbient(0.1);
      colorParam->SetDiffuse(0.1);

      part->Update(); // Обновить параметры компонента

      PrintColorParamByObj(colorParam);
#pragma endregion

#pragma region Второй вариант
      ksapi::Color color;   // Цвет
      double ambient = 0.0, // Общий цвет
        diffuse = 0.0,      // Диффузия
        specularity = 0.0,  // Зеркальность
        shininess = 0.0,    // Блеск
        transparency = 0.0, // Прозрачность
        emission = 0.0;     // Излучение

      // Получить значения параметров цвета и визуальных свойств объекта
      colorParam->GetAdvancedColor(color, ambient, diffuse, specularity, shininess, transparency, emission);
      print_color_param(color, ambient, diffuse, specularity, shininess, transparency, emission);

      // Изменение параметров цвета объекта
      color = 9421504;
      transparency = 0.5;
      ambient = 0.1;
      diffuse = 0.1;

      // Установить значения параметров цвета и визуальных свойств объекта
      colorParam->SetAdvancedColor(color, ambient, diffuse, specularity, shininess, transparency, emission);

      part->Update(); // Обновить параметры компонента

      // Получить  значения параметров цвета и визуальных свойств объекта
      colorParam->GetAdvancedColor(color, ambient, diffuse, specularity, shininess, transparency, emission);
      print_color_param(color, ambient, diffuse, specularity, shininess, transparency, emission);
#pragma endregion
    }
  }
}

//-------------------------------------------------------------------------------
// Получить/установить внешние переменные объекта
// ---
void GetSetArrayVariable(ksapi::IKompasDocument3DPtr pDocument3d)
{
  if (IsDetail(pDocument3d))
  {
    // Если документ не является сборкой
    MessageT(LoadResourceString(IDS_DOC_NOT_ASSY), LoadResourceString(IDS_ERROR), ksMessageWarning);
    return;
  }

  // Получить объект по индексу
  if (ksapi::IPartPtr part = GetPartByIndex(pDocument3d, 0))
  {
    if (ksapi::IFeaturePtr feature = part)
    {
      for (int i = 0, count = feature->GetVariablesCount(true, false); i < count; ++i)
      {
        // Получить переменную
        if (ksapi::IVariablePtr variable = feature->GetVariable(true, false, i))
        {
          if (i == 0)
          {
            // Установить комментарий переменной
            variable->SetNote(LoadResourceString(IDS_NEW_VALUE));
            double d = 0;
            ksapi::IApplicationDialogsPtr applicationDialogs = kompasApp->GetApplicationDialogs();
            // Ввод переменной пользователем
            applicationDialogs->ReadDouble(nullptr, LoadResourceString(IDS_ENTER_VALUE), 10, 0, 100, d);
            // Установить значение переменной
            variable->SetValue(d);
          }

          std::wstring name = variable->GetName();
          double value = variable->GetValue();
          std::wstring note = variable->GetNote();
          MessageT(std::vformat(LoadResourceString(IDS_VARIABLE_PARAMS), std::make_wformat_args(i, name, value, note)));
        }
      }
    }

    // Перестроить модель в соответствии с новыми значениями внешних переменных
    part->RebuildModel(true);
  }
}

//-------------------------------------------------------------------------------
// Получить/установить место расположения детали в сборке
// ---
void GetSetPlacmentComponent(ksapi::IKompasDocument3DPtr pDocument3d)
{
  if (IsDetail(pDocument3d))
  {
    // Если документ не является сборкой
    MessageT(LoadResourceString(IDS_DOC_NOT_ASSY), LoadResourceString(IDS_ERROR), ksMessageWarning);
    return;
  }

  // Получить компонент в сборке по индексу
  if (ksapi::IPartPtr part = GetPartByIndex(pDocument3d, 0))
  {
    if (ksapi::IPlacement3DPtr placement = part->GetPlacement())
    {
      double x = 0.0, y = 0.0, z = 0.0;
      // Получить координаты начала локальной системы координат
      placement->GetOrigin(x, y, z);

      MessageT(std::format(L"x = {}\ny = {}\nz = {}", x, y, z));

      // Изменить координаты начала локальной системы координат
      placement->SetOrigin(20, 20, 20);
      part->UpdatePlacement(true);
      part->Update();
    }
  }
}

//-------------------------------------------------------------------------------
// Получить объект, создаваемый системой по умолчанию и поменять параметры
// ---
void GetDefaultModelObject(ksapi::IKompasDocument3DPtr pDocument3d)
{
  // Получить верхний компонент
  if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
  {
    // Получить объект, создаваемый системой по умолчанию (плоскость XOY)
    if (ksapi::IModelObjectPtr modelObject = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY))
    {
      MessageT(modelObject->GetName());

      modelObject->SetName(L"Plane");
      modelObject->Update();
    }
  }
}

//-------------------------------------------------------------------------------
// Создание отрезка
// ---
void LineSeg(ksapi::IFragmentDocumentPtr fragmentDocument, double x1, double y1, double x2, double y2, int style)
{
  if (!fragmentDocument)
    return;

  if (ksapi::IViewsAndLayersManagerPtr layersMngr = fragmentDocument->GetViewsAndLayersManager())
  {
    if (ksapi::IViewsPtr views = layersMngr->GetViews())
    {
      if (ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView())
      {
        if (ksapi::ILineSegmentsPtr lineSegments = drawingContainer->GetLineSegments())
        {
          if (ksapi::ILineSegmentPtr lineSegment = lineSegments->Add())
          {
            lineSegment->SetX1(x1);
            lineSegment->SetY1(y1);
            lineSegment->SetX2(x2);
            lineSegment->SetY2(y2);
            lineSegment->SetStyle(style);
            lineSegment->Update();
          }
        }
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Создание эскиза
// ---
void CreateSketch(ksapi::IKompasDocument3DPtr pDocument3d)
{
  // Получить верхний компонент
  if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
  {
    // Создать новый эскиз
    ksapi::IModelContainerPtr modelContainer = part->GetModelContainer();

    if (!modelContainer)
      return;

    ksapi::ISketchsPtr sketchs = modelContainer->GetSketchs();

    if (ksapi::ISketchPtr sketch = sketchs ? sketchs->Add() : nullptr)
    {
      // Получить базовую плоскость XOY
      ksapi::IModelObjectPtr basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY);

      // Установить параметров эскиза
      sketch->SetPlane(basePlane);
      sketch->SetAngle(45.0);

      // Войти в режим редактирования эскиза
      if (ksapi::IFragmentDocumentPtr fragmentDocument = sketch->BeginEdit(false))
      {
        LineSeg(fragmentDocument, 50.0, 50.0, -50.0, 50.0, 1);
        LineSeg(fragmentDocument, 50.0, -50.0, -50.0, -50.0, 1);
        LineSeg(fragmentDocument, 50.0, -50.0, 50.0, 50.0, 1);
        LineSeg(fragmentDocument, -50.0, -50.0, -50.0, 50.0, 1);

        // Выйти из режима редактирования эскиза
        sketch->EndEdit();
      }
    }
  }
}

//-------------------------------------------------------------------------------
// Получить массив объектов (эскизов)
// ---
void GetArraySketch(ksapi::IKompasDocument3DPtr pDocument3d)
{
  // Получить верхний компонент
  if (ksapi::IPartPtr part = pDocument3d->GetTopPart())
  {
    if (ksapi::IModelContainerPtr modelContainer = part)
    {
      if (ksapi::ISketchsPtr sketchs = modelContainer->GetSketchs())
      {
        for (auto & obj : sketchs->GetObjects())
        {
          if (ksapi::ISketchPtr sketch = obj)
            MessageT(sketch->GetName());
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Получение/установка пользовательских параметров
*/
//---
void GetSetUserParamComponent(ksapi::IKompasDocument3DPtr pDocument3d)
{
  if (IsDetail(pDocument3d))
  {
    // Если документ не является сборкой
    MessageT(LoadResourceString(IDS_DOC_NOT_ASSY), LoadResourceString(IDS_ERROR), ksMessageWarning);
    return;
  }

  // Получить компонент в сборке по индексу
  if (ksapi::IPartPtr part = GetPartByIndex(pDocument3d, 0))
  {
    if (ksapi::IModelContainerPtr modelContainer = part)
    {
      if (ksapi::IUserObjects3DPtr userObjects = modelContainer->GetUserObjects())
      {
        if (ksapi::IUserObject3DPtr userObject = userObjects->Add(ksObj3dTypeEnum::o3d_UserObjectOperation))
        {
          if (ksapi::IUserParametersPtr userParams = userObject)
          {
            // Установка пользовательских параметров
            std::wstring data = L"param.d1 = 12.12\nparam.d2 = 21.21\nparam.i1 = 888\nparam.i2 = 999;";
            size_t byteLen = data.size() * sizeof(std::wstring::value_type);
            std::vector<uint8_t> vData;
            vData.reserve(byteLen);
            const uint8_t * pItem = reinterpret_cast<const uint8_t *>(data.c_str());

            for (size_t i = 0; i < byteLen; i++)
              vData.push_back(*pItem++);

            MessageT(LoadResourceString(IDS_USER_DATA_SIZE) + std::to_wstring(vData.size()));

            userParams->SetUserParams(vData);
            part->Update();

            // Чтение пользовательских параметров
            vData = userParams->GetUserParams();
            data.clear();

            for (int i = 0; i < vData.size(); i = i + 2)
            {
              wchar_t wch = static_cast<wchar_t>(vData[i + 1]);
              wch = wch << 8;
              wch = wch | static_cast<wchar_t>(vData[i]);
              data += wch;
            }

            MessageT(LoadResourceString(IDS_USER_DATA) + data);
          }
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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libaryActions)
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
