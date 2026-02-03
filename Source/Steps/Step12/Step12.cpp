////////////////////////////////////////////////////////////////////////////////
//
// step12.cpp - Пример создания пользовательской панели свойств
//
////////////////////////////////////////////////////////////////////////////////
#include "Step12.h"

#include "Step12_pch.h"
#include "Resource.h"

ksapi::IApplication * application;                                        ///< Приложение
std::wstring handlerOwner = L"Step12";                                    ///< Имя подписчика
PropertyManagerLayout PropertyManagerSubscriber::m_layout = pmAlignRight; ///< Положение панели
PropertyManagerSubscriber * subscriber = nullptr;                         ///< Подписчик на события документа

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурсов
*/
//---
std::wstring LoadStr(int id)
{
  std::wstring resStr;

  if (application)
  {
    if (ksapi::ILibraryManagerPtr libMng = application->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItem(IDR_LIB))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            return dictionary->LoadAndConvertLangString(GetModule(), id);
        }
      }
    }
  }

  return resStr.c_str();
}


//------------------------------------------------------------------------------
/**
  Вывод сообщения
*/
//---
std::int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  int res = 0;

  if (!str.empty() && application)
    res = application->ShowMessageBox(str, LoadStr(IDR_LIB), dlgType, dlgButtons, showModal);

  return res;
}


//------------------------------------------------------------------------------
/**
  Вывод сообщения
*/
//---
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  return ::LibMessage(LoadStr(strId), dlgType, dlgButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  Получить id библиотеки
*/
//---
int GetMenuId()
{
  return IDR_LIB;
}


//-------------------------------------------------------------------------------
// Головная функция библиотеки
// ---
void RunCommand(int32_t commandId, ksapi::ksRunCommandModeEnum mode)
{
  // Выдача сообщения пользователю
  if (ksapi::IKompasDocumentPtr doc = application ? application->GetActiveDocument() : nullptr)
  {
    if (!subscriber)
      subscriber = new PropertyManagerSubscriber(*doc);

    switch (commandId)
    {
      case IDC_1_MENU:
        // Создать закладку и подписаться
        subscriber->CreateAndSubscriptionPropertyManager(true);
        break;

      case IDC_2_MENU:
        // Отписаться
        subscriber->ClosePropertyManager(true);
        break;
    }
  }
}


void LoadResources(ksapi::IApplication & app);

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
  libaryActions.AddGetMenuIdHandler(GetMenuId);

  application = &app;
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


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerSubscriber::PropertyManagerSubscriber(ksapi::IKompasDocument & document)
  : m_document(&document)
{
  SubscribeEvents(document);
}


//------------------------------------------------------------------------------
/**
  Подписаться на события разных типов документов 
*/
//---
void PropertyManagerSubscriber::SubscribeEvents(ksapi::IKompasDocument & document)
{
  DocumentTypeEnum docType = document.GetDocumentType();

  if (SubscribeOnDocument(&document))
  {
    switch (docType)
    {
      case ksDocumentFragment:
      case ksDocumentDrawing:
        SubscribeOnDocument2D(&document);
        SubscribeOnObject2D(&document, ksAllObj);
        break;
      case ksDocumentPart:
      case ksDocumentAssembly:
      {
        SubscribeOnDocument3D(&document);
        SubscribeOnObject3D(&document, o3d_unknown);
        break;
      }
      case ksDocumentSpecification:
      {
        SubscribeOnSpcDoc(&document);
        break;
      }
      case ksDocumentTextual:
        SubscribeOnStamp(&document);
        break;
    }

    switch (docType)
    {
      case ksDocumentSpecification:
      case ksDocumentAssembly:
      {
        SubscribeOnSpcDescription(&document);
        SubscribeOnSpcObject(&document, ksSpecificationBaseObject);
        break;
      }
    }
  }
}

bool PropertyManagerSubscriber::SubscribeOnDocument(const ksapi::IKompasDocumentPtr & doc)
{
  if (doc)
  {
    ksapi::IKompasDocumentEventsPtr events = doc->Events();

    if (!events)
      return false;

    events->AddCloseDocumentHandler(handlerOwner,
                                    [&]()
                                    {
                                      if (!ClosePropertyManager(false))
                                        UpdateSlideBox(application ? application->GetActiveDocument() : nullptr);
                                    });
    events->AddProcessActivate(handlerOwner,
                               [&](int32_t processId)
                               {
                                 UpdateSlideBox(application ? application->GetActiveDocument() : nullptr);
                                 return true;
                               });
    events->AddProcessDeactivateHandler(handlerOwner, [&](int32_t processId)
                                        { UpdateSlideBox(application ? application->GetActiveDocument() : nullptr); });
    return true;
  }

  return false;
}

void PropertyManagerSubscriber::SubscribeOnDocument3D(const ksapi::IKompasDocument3DPtr & doc)
{
  if (doc)
    doc->Events3D()->AddRebuildHandler(handlerOwner, [&]() { return UpdateSlideBox(); });
}


//------------------------------------------------------------------------------
/**
  Подписаться на события 2D документа
*/
//---
void PropertyManagerSubscriber::SubscribeOnDocument2D(const ksapi::IKompasDocument2DPtr & doc)
{
  if (doc)
    doc->Events2D()->AddRebuildHandler(handlerOwner, [&]() { return UpdateSlideBox(); });
}


//------------------------------------------------------------------------------
/**
  Подписаться на события объектов чертежа
*/
//---
void PropertyManagerSubscriber::SubscribeOnObject2D(const ksapi::IKompasDocument2DPtr & doc, DrawingObjectTypeEnum objType)
{
  if (doc)
  {
    if (m_drawingObjEvents = doc->GetObject2DEvents(handlerOwner))
    {
      auto updateSlide = [&](const std::vector<ksapi::IDrawingObjectPtr> & objects)
      {
        return UpdateSlideBox();
      };

      m_drawingObjEvents->AddObjectsTypes({objType});
      m_drawingObjEvents->AddChangeActiveHandler(updateSlide);
      m_drawingObjEvents->AddDeleteHandler(updateSlide);
      m_drawingObjEvents->AddMoveHandler(updateSlide);
      m_drawingObjEvents->AddRotateHandler(updateSlide);
      m_drawingObjEvents->AddScaleHandler(updateSlide);
      m_drawingObjEvents->AddTransformHandler(updateSlide);
      m_drawingObjEvents->AddTransformHandler(updateSlide);
      m_drawingObjEvents->AddCopyHandler(updateSlide);
      m_drawingObjEvents->AddSymmetryHandler(updateSlide);
      m_drawingObjEvents->AddCreateObjectHandler(updateSlide);
      m_drawingObjEvents->AddUpdateObjectHandler(updateSlide);
      m_drawingObjEvents->AddDestroyObjectHandler(updateSlide);
      m_drawingObjEvents->AddEndProcessHandler([this](DrawingObjectTypeEnum typeObject) { return UpdateSlideBox(); });
    }
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события объектов детали
*/
//---
void PropertyManagerSubscriber::SubscribeOnObject3D(const ksapi::IKompasDocument3DPtr & doc, ksObj3dTypeEnum objType)
{
  if (doc)
  {
    if (m_modelObjEvents = doc->ObjectEvents3D(handlerOwner))
    {
      auto updateSlide = [&](const std::vector<ksapi::IModelObjectPtr> & objects)
      {
        return UpdateSlideBox();
      };
      auto updateSlideWithBool = [&](const std::vector<ksapi::IModelObjectPtr> & objects, bool value)
      {
        return UpdateSlideBox();
      };

      m_modelObjEvents->AddObjectsTypes({objType});
      m_modelObjEvents->AddDeleteObjectHandler(updateSlide);
      m_modelObjEvents->AddExcludedObjectHandler(updateSlideWithBool);
      m_modelObjEvents->AddHiddenObjectHandler(updateSlideWithBool);
      m_modelObjEvents->AddPropertyChangedHandler(updateSlide);
      m_modelObjEvents->AddPlacementChangedHandler(updateSlide);
      m_modelObjEvents->AddEndProcessHandler([&](int32_t processType) { UpdateSlideBox(); });
      m_modelObjEvents->AddCreateObjectHandler(updateSlide);
      m_modelObjEvents->AddUpdateObjectHandler([&](const ksapi::IModelObjectPtr & object) { UpdateSlideBox(); });
      m_modelObjEvents->AddLoadStateChangeHandler(
        [&](const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState) { UpdateSlideBox(); });
    }
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события документа спецификации
*/
//---
void PropertyManagerSubscriber::SubscribeOnSpcDoc(const ksapi::ISpecificationDocumentPtr & doc)
{
  if (doc)
  {
    doc->GetSpecificationDocumentEvents()->AddSpcStyleChangeHandler(handlerOwner,
                                                                    [&](const std::wstring & sheetName, int32_t formNumber)
                                                                    {
                                                                      UpdateSlideBox();
                                                                      return true;
                                                                    });
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события объекта спецификации
*/
//---
void PropertyManagerSubscriber::SubscribeOnSpcObject(const ksapi::ISpecificationDocumentPtr & doc,
                                                     ksSpecificationObjectTypeEnum objType)
{
  if (doc)
  {
    if (m_spcObjEvents = doc->GetSpecificationObjectEvents(handlerOwner))
    {
      auto updateSlide = [&](const ksapi::ISpecificationObjectPtr & obj)
      {
        UpdateSlideBox();
        return true;
      };

      m_spcObjEvents->AddDeleteHandler(updateSlide);
      m_spcObjEvents->AddUpdateHandler(updateSlide);
      m_spcObjEvents->AddEndProcessHandler(
        [this](ksSpecificationObjectTypeEnum specificationObjectType)
        {
          UpdateSlideBox();
          return true;
        });
    }
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события описаний спецификации
*/
//---
void PropertyManagerSubscriber::SubscribeOnSpcDescription(const ksapi::ISpecificationDocumentPtr & doc)
{
  if (doc)
  {
    if (ksapi::ISpecificationDescriptionsPtr descriptions = doc->GetSpecificationDescriptions())
    {
      auto updateSlide = [&](const ksapi::ISpecificationDescriptionPtr & description)
      {
        UpdateSlideBox();
      };

      ksapi::ISpecificationDescriptionEventsPtr events = descriptions->Events();

      if (!events)
        return;

      events->AddChangeCurrentSpcDescriptionHandler(handlerOwner, updateSlide);
      events->AddSpcDescriptionAddHandler(handlerOwner, updateSlide);
      events->AddSpcDescriptionRemoveHandler(handlerOwner, updateSlide);
      events->AddTuningSpcStyleChangeHandler(handlerOwner, [&](const ksapi::ISpecificationDescriptionPtr & description,
                                                               bool wasChanged) { UpdateSlideBox(); });
      events->AddCalcPositionsHandler(handlerOwner, [&]() { UpdateSlideBox(); });
    }
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события штампа
*/
//---
void PropertyManagerSubscriber::SubscribeOnStamp(const ksapi::IKompasDocumentPtr & doc)
{
  if (doc)
  {
    ksapi::ILayoutSheetsPtr sheets = doc->GetLayoutSheets();
    ksapi::ILayoutSheetPtr sheet = sheets ? sheets->GetItem(0) : nullptr;

    if (ksapi::IStampPtr stamp = sheet ? sheet->GetStamp() : nullptr)
      stamp->Events()->AddEndEditStampHandler(handlerOwner, [&](bool wasChanged) { UpdateSlideBox(); });
  }
}


//------------------------------------------------------------------------------
/**
  Подписаться на события панели свойств и параметров процесса
*/
//---
void PropertyManagerSubscriber::SubscribeOnPropMng()
{
  if (m_propMng)
  {
    ksapi::IPropertiesManagerEventsPtr events = m_propMng->Events();

    if (!events)
      return;

    events->AddChangeControlValueHandler(handlerOwner,
                                         [this](const ksapi::IPropertyControlPtr & ctrl) { ChangeControlValue(ctrl); });
    events->AddButtonClickHandler(handlerOwner, [this](std::int32_t buttonID) { return ButtonClick(buttonID); });
    events->AddControlCommandHandler(handlerOwner, [this](const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
                                     { return ControlCommand(ctrl, buttonID); });
    events->AddButtonUpdateHandler(handlerOwner, [this](int32_t buttonId, bool & enable, int & checked)
                                   { return ButtonUpdate(buttonId, enable, checked); });
  }
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void PropertyManagerSubscriber::UnadviseEvents()
{
  if (m_document)
  {
    ksapi::IKompasDocumentEventsPtr events = m_document->Events();

    if (!events)
      return;

    events->RemoveAllHandlers(handlerOwner);
    ;
    if (m_propMng)
    {
      ksapi::IPropertiesManagerEventsPtr events = m_propMng->Events();
      events->RemoveAllHandlers(handlerOwner);
    }

    if (ksapi::ISpecificationDescriptionsPtr descriptions = m_document->GetSpecificationDescriptions())
      descriptions->Events()->RemoveAllHandlers(handlerOwner);

    ksapi::ILayoutSheetsPtr sheets = m_document->GetLayoutSheets();
    ksapi::ILayoutSheetPtr sheet = sheets ? sheets->GetItem(0) : nullptr;

    if (ksapi::IStampPtr stamp = sheet ? sheet->GetStamp() : nullptr)
      stamp->Events()->RemoveAllHandlers(handlerOwner);

    if (ksapi::IKompasDocument2DPtr doc2d = m_document)
    {
      doc2d->Events2D()->RemoveAllHandlers(handlerOwner);

      if (m_drawingObjEvents)
        m_drawingObjEvents->RemoveAllHandlers();
    }

    if (ksapi::IKompasDocument3DPtr doc3d = m_document)
    {
      doc3d->Events3D()->RemoveAllHandlers(handlerOwner);

      if (m_modelObjEvents)
        m_modelObjEvents->RemoveAllHandlers();
    }

    if (ksapi::ISpecificationDocumentPtr spcDoc = m_document)
    {
      spcDoc->GetSpecificationDocumentEvents()->RemoveAllHandlers(handlerOwner);

      if (m_spcObjEvents)
        m_spcObjEvents->RemoveAllHandlers();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать закладку и подписаться
*/
//---
void PropertyManagerSubscriber::CreateAndSubscriptionPropertyManager(bool mes)
{
  if (!m_propMng && application)
  {
    m_propMng = application->CreatePropertyManager(true /*FALSE*/);
    m_propMng->SetLayout(m_layout);
    m_propMng->SetCaption(L"Панель из библиотеки");
    m_propMng->SetSpecToolbar(pnEnterEscCreateSaveSearchHelp);
    SubscribeOnPropMng(); // Подписываемся на события процесса
    ksapi::IPropertyTabPtr tab = m_propMng->GetPropertyTabs()->Add(L"Закладка из библиотеки");

    // Получаем коллекцию контролов
    ksapi::IPropertyControlsPtr collection = tab->GetPropertyControls();

    if (ksapi::IKompasDocumentPtr doc = application->GetActiveDocument())
    {
      // Добавляем окно просмотра
      m_slideBox = collection->Add(ksControlSlideBox);
      m_slideBox->SetDocument(doc);
      m_slideBox->SetCheckBoxVisibility(!!doc);
      m_slideBox->SetId(10000);
      m_slideBox->SetName(L"Окно просмотра");
    }

    ksapi::IPropertyEditPtr edit = collection->Add(ksControlEditInt);
    edit->SetName(L"IPropertyEdit");
    edit->SetId(100100);

    m_propMng->ShowTabs();
    m_propMng->SetCaption(L"Панель из библиотеки1");
  }
  else
  {
    if (m_propMng)
      m_propMng->SetVisible(true); // Если панель была скрыта пользователем по крестику востановим ее

    if (mes)
      LibMessage(L"Панель уже загружена");
  }
}


//------------------------------------------------------------------------------
/**
  Остановить работу с панелью свойствт и отписаться
*/
//---
bool PropertyManagerSubscriber::ClosePropertyManager(bool mes)
{
  bool res = false;

  if (m_propMng)
  {
    // Запоминаем положение панели и гасим ее
    m_layout = m_propMng->GetLayout();
    UnadviseEvents();
    m_propMng->HideTabs();
    subscriber = nullptr;
    res = true;
    delete this;
  }
  else
  {
    if (mes)
      LibMessage(L"Панель уже выгружена");
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Обновление окна
*/
//---
bool PropertyManagerSubscriber::UpdateSlideBox(const ksapi::IKompasDocumentPtr & document)
{
  if (m_slideBox)
  {
    m_slideBox->SetDocument(document);
    m_slideBox->SetCheckBoxVisibility(!!document);
    m_slideBox->UpdateParam();
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Обновление окна
*/
//---
bool PropertyManagerSubscriber::UpdateSlideBox()
{
  if (m_slideBox)
    m_slideBox->UpdateParam();
  return true;
}


//------------------------------------------------------------------------------
/**
  Событие изменения значения контрола
*/
//---
bool PropertyManagerSubscriber::ChangeControlValue(const ksapi::IPropertyControlPtr & ctrl)
{
  LibMessage(L"PropertyManagerEvent::ChangeControlValue");
  return true;
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки
*/
//---
bool PropertyManagerSubscriber::ButtonClick(std::int32_t buttonID)
{
  if (buttonID == pbEnter || buttonID == pbEsc)
    ClosePropertyManager(false);

  return true;
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки контрола
*/
//---
bool PropertyManagerSubscriber::ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
{
  LibMessage(L"PropertyManagerEvent::ControlCommand");
  return true;
}


//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели.
*/
//---
bool PropertyManagerSubscriber::ButtonUpdate(int32_t buttonId, bool & enable, int & checked)
{
  checked = 0;
  enable = true;
  return true;
}
