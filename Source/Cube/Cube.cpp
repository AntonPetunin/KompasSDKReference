////////////////////////////////////////////////////////////////////////////////
//
// Кубик (KsAPI)
//
////////////////////////////////////////////////////////////////////////////////
#include "Cube_pch.h"

#include <format>
#include "DocumentFrameEvents.h"
#include "Resource.h"

void * GetModule();
void LoadResources(ksapi::IApplication & app);

void TestDocumentFrameNotify();
void TestCurrentDocumentFrameNotify(int redrawType);

ksapi::IApplication * kompasApp = nullptr;
std::unique_ptr<DocumentFrameEvents> events = nullptr;
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


//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
int32_t GetMenuId()
{
  return IDR_LIB;
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 1:
      TestDocumentFrameNotify();
      break;
    case 2:
    case 3:
    case 4:
      TestCurrentDocumentFrameNotify(commandId - 2); // Обход по граням компонента
      break;
  }

  ksapi::IKompasErrorPtr error = kompasApp->GetKompasError();
  if (error->GetCode() == etError10) // "Ошибка! Вырожденный объект"
    error->Clear();                  // Обнулить ошибки
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

//-----------------------------------------------------------------------------
// Создать новый документ заданного типа
// ---
ksapi::IKompasDocumentPtr CreateNewDocument(DocumentTypeEnum type)
{
  ksapi::IDocumentsPtr documents = kompasApp->GetDocuments();
  return documents ? documents->Add(type, true) : nullptr;
}


//-------------------------------------------------------------------------------
// Оперция выдавливания, работа с экизом
// ---
bool CreateExtrusion(ksapi::IPartPtr & part)
{
  bool res = false;

  // Создать новый эскиз
  ksapi::IModelContainerPtr modelContainer = part->GetModelContainer();

  if (!modelContainer)
    return false;

  ksapi::ISketchsPtr sketchs = modelContainer->GetSketchs();

  if (ksapi::ISketchPtr sketch = sketchs ? sketchs->Add() : nullptr)
  {
    // Получить базовую плоскость XOY
    ksapi::IModelObjectPtr basePlane = part->GetDefaultObject(ksObj3dTypeEnum::o3d_planeXOY);

    // Установить параметры эскиза
    sketch->SetPlane(basePlane);
    sketch->SetAngle(45);
    sketch->Update();

    // Войти в режим редактирования эскиза
    if (ksapi::IFragmentDocumentPtr fragmentDocument = sketch->BeginEdit(false))
    {
      // Введем новый эскиз - квадрат
      LineSeg(fragmentDocument, 50, 50, -50, 50, 1);
      LineSeg(fragmentDocument, 50, -50, -50, -50, 1);
      LineSeg(fragmentDocument, 50, -50, 50, 50, 1);
      LineSeg(fragmentDocument, -50, -50, -50, 50, 1);
      // Выйти из режима редактирования эскиза
      sketch->EndEdit();
    }

    // Базовая операция выдавливания
    if (ksapi::IExtrusionsPtr extrusions = modelContainer->GetExtrusions())
    {
      if (ksapi::IExtrusionPtr extrusion = extrusions->Add(o3d_bossExtrusion))
      {
        extrusion->SetDirection(dtNormal);
        extrusion->SetSideParameters(true, etBlind, 100, 0, true, nullptr);

        // Изменить параметры тонкой стенки элемента
        if (ksapi::IThinParametersPtr thinParams = extrusion)
          thinParams->SetThinParameters(false, dtBoth, 10, 10);

        extrusion->SetName(LoadResourceString(IDS_OPERATION_NAME));

        // Эскиз операции выдавливания
        extrusion->SetProfiles({sketch});
        res = extrusion->Update();
      }
    }
  }
  return res;
}

//-----------------------------------------------------------------------------
//
// ---
void TestDocumentFrameNotify()
{
  if (ksapi::IKompasDocument3DPtr doc3D = CreateNewDocument(DocumentTypeEnum::ksDocumentPart))
  {
    doc3D->SetDrawMode(vm_Shaded);
    doc3D->SetShadedWireframe(true);

    if (ksapi::IPartPtr part = doc3D->GetTopPart())
    {
      if (CreateExtrusion(part))
      {
        part->SetName(LoadResourceString(IDS_TITLE));
        part->Update();

        if (ksapi::IDocumentFramePtr documentFrame = doc3D->GetDocumentFrame())
        {
          events = nullptr;                                                // Отписка от событий
          events.reset(new DocumentFrameEvents(kompasApp, documentFrame)); // Новая подписка на события

          documentFrame->SetGabaritModifying();
          documentFrame->RefreshWindow();
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
//
// ---
void TestCurrentDocumentFrameNotify(int redrawType)
{
  if (kompasApp)
  {
    if (ksapi::IKompasDocumentPtr komDoc = kompasApp->GetActiveDocument())
    {
      if (ksapi::IDocumentFramePtr documentFrame = komDoc->GetDocumentFrame())
      {
        events = nullptr;                                                // Отписка от событий
        events.reset(new DocumentFrameEvents(kompasApp, documentFrame)); // Новая подписка на события
        DocumentFrameEvents::redrawType = redrawType;

        documentFrame->SetGabaritModifying();
        documentFrame->RefreshWindow();
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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetMenuIdHandler(GetMenuId);

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
