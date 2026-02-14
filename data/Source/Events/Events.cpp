////////////////////////////////////////////////////////////////////////////////
//
// События (KsAPI)
//
////////////////////////////////////////////////////////////////////////////////
#include <format>
#include <fstream>

#include "BaseEvent.h"
#include "Events_pch.h"
#include "IniFile.h"
#include "LibFunctions.h"

void LoadResources(ksapi::IApplication & app);

ksapi::IApplication * kompasApp = nullptr;

// Настройки приложения
AppConfig theApp;

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
int32_t GetMenuId()
{
  return ID_LIB;
}


//-------------------------------------------------------------------------------
// Выбор 2D объекта для подписки/отписки
// ---
int SelectObj2DType()
{
  int32_t typeObj = -1;

  static const std::wstring & kHandlersOwner = L"SelectObj2DType";

  ksapi::IKompasDocument2DPtr doc = kompasApp->GetActiveDocument();

  if (!doc)
    return typeObj;

  ksapi::IProcess2DPtr process2d = doc->GetLibProcess(ksProcess2DCursor);
  ksapi::IProcessPtr process = process2d;

  if (process && process2d)
  {
    process->SetCaption(LoadResourceString(IDS_SELECT_OBJECTS));
    process->SetPrompt(LoadResourceString(IDS_SELECT_OBJECTS_FOR_ADVISE));

    // Обработчик события клика в процессе - переключает флаг, если был хоть 1 клик.
    // Если клика не было - вернуть false из метода.
    double x = 0.0, y = 0.0;
    bool selected = false;
    auto clickCallback = [&selected, &x, &y](double userX, double userY, double, bool)
    {
      x = userX;
      y = userY;
      selected = true;
      return false;
    };

    // Регитрация обработчика и запуск процесса.
    process2d->Events()->AddPlacementChangedHandler(kHandlersOwner, clickCallback);
    process->Run(/*modal*/ true, /*post*/ false);

    // Был выбран объект (совершён клик)
    if (selected)
    {
      ksapi::IFindObjectParametersPtr params = doc->GetInterface(KompasAPIObjectTypeEnum::ksObjectFindObjectParameters);

      if (ksapi::IDrawingObjectPtr obj = params ? doc->FindObject(x, y, doc->GetCursorLimit(), params) : nullptr)
        typeObj = obj->GetDrawingObjectType();
    }
    else
    {
      kompasApp->GetApplicationDialogs()->ReadInt(nullptr, LoadResourceString(IDS_ENTER_OBJ_TYPE_NUM), 0, 0 /*Все*/, 123 /*Вид*/,
                                                  typeObj);
    }

    process2d->Events()->RemoveAllHandlers(kHandlersOwner);
  }

  return typeObj;
}


//-------------------------------------------------------------------------------
// Выбор 3D объекта для подписки/отписки
// ---
bool SelectObj3DType(ksapi::IModelObjectPtr & obj, ksObj3dTypeEnum & objType)
{
  objType = o3d_unknown;
  bool res = false;
  ksapi::IKompasDocument3DPtr doc3D = kompasApp->GetActiveDocument();

  if (!doc3D)
    return res;

  ksapi::IProcess3DPtr process3d = doc3D->GetLibProcess(ksProcess3DSelectEntity);
  ksapi::IProcessPtr process = process3d;
  ksapi::IKompasAPIObjectPtr selectedObject;

  if (process && process3d)
  {
    process->SetCaption(LoadResourceString(IDS_SELECT_OBJECTS));
    process->SetPrompt(LoadResourceString(IDS_SELECT_OBJECTS_FOR_ADVISE));

    // Обработчик события клика в процессе - переключает флаг, если был хоть 1 клик.
    // Если клика не было - вернуть false из метода.
    bool selected = false;
    auto clickCallback = [&selected, &selectedObject](const ksapi::IKompasAPIObjectPtr & object)
    {
      selectedObject = object;
      selected = true;
      return false;
    };

    // Региcтрация обработчика и запуск процесса.
    process3d->Events()->AddPlacementChangedHandler(L"SelectObj3DType", clickCallback);
    process->Run(/*modal*/ true, /*post*/ false);

    // Был выбран объект (совершён клик)
    if (selected)
    {
      ksapi::IModelObjectPtr modelObject = selectedObject;
      ksObj3dTypeEnum modelObjType = modelObject ? modelObject->GetModelObjectType() : o3d_unknown;

      if (modelObjType == o3d_face || modelObjType == o3d_edge || modelObjType == o3d_vertex)
      {
        ksMessageButtonEnum resYesNo = YesNoCancelDialog(LoadResourceString(IDS_CHOOSE_OTHER_TYPE));
        switch (resYesNo)
        {
          case ksMessageButtonEnum::ksButtonYes:
            obj = modelObject->GetPart();
            objType = o3d_part;
            res = true;
            break;
          case ksMessageButtonEnum::ksButtonNo:
            obj = modelObject->GetOwner();
            objType = o3d_feature;
            res = true;
            break;
        }
      }
      else
      {
        obj = modelObject;
        res = true;
      }
    }

    process3d->Events()->RemoveAllHandlers(L"SelectObj3DType");
  }


  return res;
}

//-------------------------------------------------------------------------------
// Выбор объекта спецификации для подписки/отписки
// ---
bool SelectSpcObjType(int32_t & objType, bool byType)
{
  bool res = false;
  ksapi::IKompasDocumentPtr doc = kompasApp->GetActiveDocument();

  if (!doc)
    return res;

  DocumentTypeEnum type = doc->GetDocumentType();
  ksSpecificationObjectTypeEnum spcObjType{objType};

  switch (type)
  {
    case DocumentTypeEnum::ksDocumentSpecification:
    case DocumentTypeEnum::ksDocumentDrawing:
    case DocumentTypeEnum::ksDocumentAssembly:
    case DocumentTypeEnum::ksDocumentTechnologyAssembly:
    case DocumentTypeEnum::ksDocumentPart:
    {
      if (!byType)
      {
        // Подписываемся на тип текущего объекта
        if (ksapi::ISpecificationDescriptionsPtr spcDescrs = doc->GetSpecificationDescriptions())
        {
          if (ksapi::ISpecificationDescriptionPtr activeDescr = spcDescrs->GetActive())
          {
            if (ksapi::ISpecificationObjectPtr currentObj = activeDescr->GetCurrentObject())
            {
              objType = currentObj->GetObjectType();
            }
          }
        }
      }

      res = byType || (objType == ksSpecificationObjectTypeEnum::ksSpecificationBaseObject ||
                       objType == ksSpecificationObjectTypeEnum::ksSpecificationComment);
      break;
    }
    default:
      LibMessage(IDS_SPC_WRONG_DOC_TYPE);
      break;
  }

  return res;
}

//-------------------------------------------------------------------------------
// Подписка 2D объекта по типу
// ---
void AdviseObj2D()
{
  if (ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument())
  {
#ifndef TESTS
    int32_t typeObj = SelectObj2DType();
#else
    int32_t typeObj = 0;
#endif
    if (typeObj != -1)
    {
      if (!EventContainer::FindEvents(EventType::Object2D, doc2D, typeObj, nullptr))
      {
        if (EventContainer::NewEvent(EventType::Object2D, doc2D, typeObj, nullptr))
        {
          LibMessage(FormatResourceString(IDS_OBJECT_2D_ADVISED_TYPE, typeObj));
        }
        else
          LibMessage(IDS_ADVISE_FAIL);
      }
      else
        LibMessage(IDS_ADVISE_EXISTS);
    }
  }
  else
    LibMessage(IDS_NO_ACTIVE_DOC_2D);
}


//-------------------------------------------------------------------------------
// Отписка 2D объекта
// ---
void UnAdviseObj2D()
{
  if (ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument())
  {
    int typeObj = SelectObj2DType();
    if (typeObj != -1)
    {
      EventContainer::UnAdviseFromObject(EventType::Object2D, doc2D, typeObj, nullptr);
      if (!EventContainer::FindEvents(EventType::Object2D, doc2D, typeObj, nullptr))
        LibMessage(IDS_UNADVISE);
      else
        LibMessage(IDS_UNADVISE_FAIL);
    }
  }
  else
    LibMessage(IDS_NO_ACTIVE_DOC_2D);
}


//------------------------------------------------------------------------------
/**
  Получить вид по указанному виду
*/
//---
ksapi::IViewPtr GetViewByNumber()
{
  ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  if (!doc2D || doc2D->GetDocumentType() == DocumentTypeEnum::ksDocumentFragment)
  {
    LibMessage(IDS_NO_ACTIVE_DOC_2D);
    return nullptr;
  }
  int viewNumber = 0;
  ksapi::IViewPtr view;

  if (kompasApp->GetApplicationDialogs()->ReadInt(nullptr, LoadResourceString(IDS_ENTER_VIEW_NUM), 0, 0 /*системный*/, 255,
                                                  viewNumber))
  {
    if (ksapi::IViewsAndLayersManagerPtr viewsMngr = doc2D->GetViewsAndLayersManager())
    {
      ksapi::IViewsPtr views = viewsMngr->GetViews();
      view = views ? views->GetViewByNumber(viewNumber) : nullptr;

      if (!view)
        LibMessage(IDS_VIEW_NOT_EXISTS);
    }
  }

  return view;
}


//------------------------------------------------------------------------------
/**
  Получить слой по указанному виду
*/
//---
ksapi::ILayerPtr GetLayerByNumber()
{
  ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  if (!doc2D || doc2D->GetDocumentType() == DocumentTypeEnum::ksDocumentFragment)
  {
    LibMessage(IDS_NO_ACTIVE_DOC_2D);
    return nullptr;
  }
  int layerNumber = 0;
  ksapi::ILayerPtr layer;

  if (kompasApp->GetApplicationDialogs()->ReadInt(nullptr, LoadResourceString(IDS_ENTER_LAYER_NUM), 0 /*системный*/, 0 /*Все*/,
                                                  254, layerNumber))
  {
    if (ksapi::IViewsAndLayersManagerPtr layersMngr = doc2D->GetViewsAndLayersManager())
    {
      if (ksapi::IViewsPtr views = layersMngr->GetViews())
      {
        if (ksapi::IViewPtr view = views->GetActiveView())
        {
          ksapi::ILayersPtr layers = view->GetLayers();
          layer = layers ? layers->GetLayerByNumber(layerNumber) : nullptr;

          if (!layer)
            LibMessage(IDS_LAYER_NOT_EXISTS);
        }
      }
    }
  }

  return layer;
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  bool byType = false;
  ksapi::IKompasDocumentPtr activeDoc = kompasApp->GetActiveDocument();

  switch (commandId)
  {
    // Подписаться на события приложения Компас
    case CMP_APP_ADVISE:
    {
      if (!EventContainer::FindEvents(EventType::Application))
      {
        // обработчик событий приложения КОМПАС
        if (EventContainer::NewEvent(EventType::Application)) // подписка на события приложения КОМПАС
          LibMessage(IDS_ADVISE_APP);
        else
          LibMessage(IDS_ADVISE_FAIL);
      }
      else
        LibMessage(IDS_ADVISE_EXISTS);

      break;
    }

    // Отписаться от событий приложения Компас
    case CMP_APP_UNADVISE:
    {
      if (EventContainer::FindEvents(EventType::Application))
      {
        EventContainer::UnAdviseByType(EventType::Application);
        LibMessage(IDS_UNADVISE);
      }
      else
        LibMessage(IDS_ADVISE_NOT_FOUND);

      break;
    }

    // Подписаться на события документа
    case CMP_DOC_ADVISE:
    {
      if (activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::Document, activeDoc))
        {
          if (EventContainer::NewEvent(EventType::Document, activeDoc))
            LibMessage(IDS_ADVISE_DOC);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Отписаться от событий текущего документа
    case CMP_DOC_UNADVISE:
    {
      if (activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Document, activeDoc);

        if (!EventContainer::FindEvents(EventType::Document, activeDoc))
          LibMessage(IDS_UNADVISE_DOC);
        else
          LibMessage(IDS_UNADVISE_DOC_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Подписаться на события 2D документа
    case CMP_DOC_2D_ADVISE:
    {
      if (ksapi::IKompasDocument2DPtr doc2D = activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::Document2D, doc2D))
        {
          if (EventContainer::NewEvent(EventType::Document2D, doc2D))
            LibMessage(IDS_ADVISE_DOC_2D);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_2D);

      break;
    }

    // Отписаться от событий текущего 2D документа
    case CMP_DOC_2D_UNADVISE:
    {
      if (ksapi::IKompasDocument2DPtr doc2D = activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Document2D, doc2D);

        if (!EventContainer::FindEvents(EventType::Document2D, doc2D))
          LibMessage(IDS_UNADVISE_DOC_2D);
        else
          LibMessage(IDS_UNADVISE_DOC_2D_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_2D);

      break;
    }

    // Подписаться на события редактирования 2D объектов ( указанием объекта )
    case CMP_OBJECT_2D_ADVISE:
    {
      AdviseObj2D();
      break;
    }

    // Отписаться от событий редактирования 2D Объектов
    case CMP_OBJECT_2D_UNADVISE:
    {
      UnAdviseObj2D();
      break;
    }

    // Подписаться на события для вида по номеру
    case CMP_ADVISE_VIEW_BY_NUM:
    {
      if (ksapi::IViewPtr view = GetViewByNumber())
      {
        if (!EventContainer::FindEvents(EventType::Object2D, activeDoc, -1, view))
        {
          if (EventContainer::NewEvent(EventType::Object2D, activeDoc, -1, view))
            LibMessage(FormatResourceString(IDS_OBJECT_2D_ADVISED_OJECT, view->GetName()));
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }

      break;
    }

    // Отписаться от событий для вида по номеру
    case CMP_UNADVISE_VIEW_BY_NUM:
    {
      if (ksapi::IViewPtr view = GetViewByNumber())
      {
        EventContainer::UnAdviseFromObject(EventType::Object2D, activeDoc, -1, view);
        if (!EventContainer::FindEvents(EventType::Object2D, activeDoc, -1, view))
          LibMessage(IDS_UNADVISE);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }

      break;
    }

    // Подписаться на события для слоя по номеру
    case CMP_ADVISE_LAYER_BY_NUM:
    {
      if (ksapi::ILayerPtr layer = GetLayerByNumber())
      {
        if (!EventContainer::FindEvents(EventType::Object2D, activeDoc, -1, layer))
        {
          if (EventContainer::NewEvent(EventType::Object2D, activeDoc, -1, layer))
            LibMessage(FormatResourceString(IDS_OBJECT_2D_ADVISED_OJECT, layer->GetName()));
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }

      break;

      break;
    }

    // Отписаться на слой по номеру
    case CMP_UNADVISE_LAYER_BY_NUM:
    {
      if (ksapi::ILayerPtr layer = GetLayerByNumber())
      {
        EventContainer::UnAdviseFromObject(EventType::Object2D, activeDoc, -1, layer);
        if (!EventContainer::FindEvents(EventType::Object2D, activeDoc, -1, layer))
          LibMessage(IDS_UNADVISE);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }
      break;
    }

    // Отписать события всех объектов 2D
    case CMP_OBJECT_2D_UNADVISE_ALL:
    {
      if (ksapi::IKompasDocument2DPtr doc2D = activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Object2D, doc2D);
        LibMessage(IDS_UNADVISE);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_2D);

      break;
    }

    // Подписаться на события селектирования
    case CMP_SELECTION_MNG_ADVISE:
    {
      if (activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::SelectionManager, activeDoc))
        {
          if (EventContainer::NewEvent(EventType::SelectionManager, activeDoc))
            LibMessage(IDS_ADVISE_SELECTION_MANAGER);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Отписаться от событий селектирования
    case CMP_SELECTION_MNG_UNADVISE:
    {
      if (activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::SelectionManager, activeDoc);

        if (!EventContainer::FindEvents(EventType::SelectionManager, activeDoc))
          LibMessage(IDS_UNADVISE);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Подписаться на события редактирования штампа
    case CMP_STAMP_ADVISE:
    {
      if (activeDoc)
      {
        switch (activeDoc->GetDocumentType())
        {
          case DocumentTypeEnum::ksDocumentDrawing:
          case DocumentTypeEnum::ksDocumentSpecification:
          case DocumentTypeEnum::ksDocumentTextual:
          {
            if (!EventContainer::FindEvents(EventType::Stamp, activeDoc))
            {
              if (EventContainer::NewEvent(EventType::Stamp, activeDoc))
                LibMessage(IDS_ADVISE_STAMP);
              else
                LibMessage(IDS_ADVISE_FAIL);
            }
            else
              LibMessage(IDS_ADVISE_EXISTS);
            break;
          }
          default:
            LibMessage(IDS_WRONG_DOCTYPE);
            break;
        }
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);
      break;
    }

    // Отписаться от событий редактирования штампа
    case CMP_STAMP_UNADVISE:
    {
      if (activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Stamp, activeDoc);

        if (!EventContainer::FindEvents(EventType::Stamp, activeDoc))
          LibMessage(IDS_UNADVISE_STAMP);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Подписаться на события 3D документа
    case CMP_DOC_3D_ADVISE:
    {
      if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::Document3D, doc3D))
        {
          if (EventContainer::NewEvent(EventType::Document3D, doc3D))
            LibMessage(IDS_ADVISE_DOC_3D);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_3D);

      break;
    }

    // Отписаться от событий 3D документа
    case CMP_DOC_3D_UNADVISE:
    {
      if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Document3D, doc3D);

        if (!EventContainer::FindEvents(EventType::Document3D, doc3D))
          LibMessage(IDS_UNADVISE_DOC_3D);
        else
          LibMessage(IDS_UNADVISE_DOC_3D_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_3D);
      break;
    }

    // Подписаться на события редактирования 3D объектов  (с запросом указания объекта)
    case CMP_OBJECT_3D_ADVISE_SELECT:
    {
      if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
      {
        ksapi::IModelObjectPtr obj;
        ksObj3dTypeEnum objType = o3d_unknown; // Все объекты
        if (SelectObj3DType(obj, objType))
        {
          if (EventContainer::NewEvent(EventType::Object3D, doc3D, objType, obj))
            LibMessage(FormatResourceString(IDS_OBJECT_3D_ADVISED_TYPE, static_cast<int32_t>(objType)));
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_3D);

      break;
    }

    // Отписаться на события редактирования 3D объектов (с запросом указания объекта)
    case CMP_OBJECT_3D_UNADVISE_SELECT:
    {
      if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
      {
        ksapi::IModelObjectPtr obj;
        ksObj3dTypeEnum objType = o3d_unknown;
        if (SelectObj3DType(obj, objType))
        {
          EventContainer::UnAdviseFromObject(EventType::Object3D, doc3D, objType, obj);
          if (!EventContainer::FindEvents(EventType::Object3D, doc3D, objType, obj))
            LibMessage(IDS_UNADVISE_OBJECT_3D);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_3D);

      break;
    }


    // Отписать события всех объектов 3D
    case CMP_OBJECT_3D_UNADVISE_ALL:
    {
      if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::Object3D, doc3D);
        LibMessage(IDS_UNADVISE_ALL_OBJ_3D);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_3D);

      break;
    }

    // Подписаться на события редактирования объектов спецификации
    case CMP_SPC_ADVISE_ALL:
    case CMP_SPC_ADVISE_BASE:
    case CMP_SPC_ADVISE_COMMENT:
      byType = true;
      [[fallthrough]];
    case CMP_SPC_ADVISE_CURRENT:
    {
      int objType = byType ? commandId - 300 : 0;
      if (activeDoc)
      {
        if (SelectSpcObjType(objType, byType))
        {
          if (!EventContainer::FindEvents(EventType::SpecificationObject, activeDoc, objType, nullptr))
          {
            if (EventContainer::NewEvent(EventType::SpecificationObject, activeDoc, objType, nullptr))
              LibMessage(IDS_ADVISE_SPC_OBJ);
            else
              LibMessage(IDS_ADVISE_FAIL);
          }
          else
            LibMessage(IDS_ADVISE_EXISTS);
        }
        else
          LibMessage(IDS_SPC_NO_OBJ);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Отписаться от события редактирования объектов спецификации
    case CMP_SPC_UNADVISE_ALL:
    case CMP_SPC_UNADVISE_BASE:
    case CMP_SPC_UNADVISE_COMMENT:
      byType = true;
      [[fallthrough]];
    case CMP_SPC_UNADVISE_CURRENT:
    {
      int objType = byType ? commandId - 400 : 0;
      if (activeDoc)
      {
        if (SelectSpcObjType(objType, byType))
        {
          EventContainer::UnAdviseFromObject(EventType::SpecificationObject, activeDoc, objType, nullptr);
          if (!EventContainer::FindEvents(EventType::SpecificationObject, activeDoc, objType, nullptr))
            LibMessage(IDS_UNADVISE_SPC_OBJ);
          else
            LibMessage(IDS_UNADVISE_FAIL);
        }
        else
          LibMessage(IDS_SPC_NO_OBJ);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }


    // Подписаться на события документа спецификации
    case CMP_SPC_DOC_ADVISE:
    {
      if (ksapi::ISpecificationDocumentPtr docSpc = activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::SpecificationDocument, docSpc))
        {
          if (EventContainer::NewEvent(EventType::SpecificationDocument, docSpc))
            LibMessage(IDS_ADVISE_SPC_DOC);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_SPC);

      break;
    }


    // Отписаться от событий документа спецификации
    case CMP_SPC_DOC_UNADVISE:
    {
      if (ksapi::ISpecificationDocumentPtr docSpc = activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::SpecificationDocument, docSpc);
        if (!EventContainer::FindEvents(EventType::SpecificationDocument, docSpc))
          LibMessage(IDS_UNADVISE_SPC_DOC);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC_SPC);

      break;
    }

    // Подписаться на события настройки спецификации
    case CMP_SPC_ADVISE:
    {
      if (activeDoc)
      {
        if (!EventContainer::FindEvents(EventType::SpecificationDescription, activeDoc))
        {
          if (EventContainer::NewEvent(EventType::SpecificationDescription, activeDoc))
            LibMessage(IDS_ADVISE_SPC_DESCR);
          else
            LibMessage(IDS_ADVISE_FAIL);
        }
        else
          LibMessage(IDS_ADVISE_EXISTS);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }

    // Отписаться от событий настройки спецификации
    case CMP_SPC_UNADVISE:
    {
      if (activeDoc)
      {
        EventContainer::UnAdviseByTypeAndDoc(EventType::SpecificationDescription, activeDoc);
        if (!EventContainer::FindEvents(EventType::SpecificationDescription, activeDoc))
          LibMessage(IDS_UNADVISE_SPC_DESCR);
        else
          LibMessage(IDS_UNADVISE_FAIL);
      }
      else
        LibMessage(IDS_NO_ACTIVE_DOC);

      break;
    }


    case CMP_SNAPSHOT: // Сделать снепшот
    {
#ifdef TESTS
      static int snapshotNumber = 0;
      CreateSnapshot(snapshotNumber++, BaseEvent::m_eventMessages);
      BaseEvent::m_eventMessages.clear();
#endif
      break;
    }


    case CMP_LIST_EVENTS: // Подписанные события
    {
      EventContainer::ListEvents();
      break;
    }

    // Отписать все
    case CMP_UNADVISE_ALL_EVENTS:
    {
      EventContainer::UnAdviseAll();
      LibMessage(IDS_UNADVISE);
      break;
    }

    case CMP_CONFIG: // Конфигурация
    {
      ReadIni();
      if (theApp.m_auto && !EventContainer::FindEvents(EventType::Application))
        EventContainer::NewEvent(EventType::Application); // обработчик событий приложения КОМПАС

      break;
    }

    // Подписаться/Отписаться на события 3D объектов по номеру типа
    default:
    {
      if (commandId >= (o3d_unknown + 500) && commandId <= (o3d_feature + 500))
      {
        // Подписаться
        if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
        {
          int32_t objType = commandId - 500;
          if (!EventContainer::FindEvents(EventType::Object3D, doc3D, objType, nullptr))
          {
            if (EventContainer::NewEvent(EventType::Object3D, doc3D, objType, nullptr))
              LibMessage(FormatResourceString(IDS_OBJECT_3D_ADVISED_TYPE, objType));
            else
              LibMessage(IDS_ADVISE_FAIL);
          }
          else
            LibMessage(IDS_ADVISE_EXISTS);
        }
        else
          LibMessage(IDS_NO_ACTIVE_DOC_3D);

        break;
      }
      else
      {
        if (commandId >= (o3d_unknown + 700) && commandId <= (o3d_feature + 700))
        {
          // Отписаться
          if (ksapi::IKompasDocument3DPtr doc3D = activeDoc)
          {
            int objType = commandId - 700;
            EventContainer::UnAdviseFromObject(EventType::Object3D, doc3D, objType, nullptr);
            if (!EventContainer::FindEvents(EventType::Object3D, doc3D, objType, nullptr))
              LibMessage(IDS_UNADVISE);
            else
              LibMessage(IDS_UNADVISE_FAIL);
          }
          else
            LibMessage(IDS_NO_ACTIVE_DOC_3D);

          break;
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
  ReadIni();

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
  WriteIni();
}
