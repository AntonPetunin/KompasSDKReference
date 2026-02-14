////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "BaseEvent.h"
#include "ApplicationEvent.h"
#include "DocumentEvent.h"
#include "Document2DEvent.h"
#include "Document3DEvent.h"
#include "LibFunctions.h"
#include "DocumentFrameEvent.h"
#include "DrawingObjectEvent.h"
#include "SelectMngEvent.h"
#include "StampEvent.h"
#include "ModelObjectEvent.h"
#include "SpecificationObjectEvent.h"
#include "SpecificationDocumentEvent.h"
#include "SpecificationDescriptionEvent.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D

EventsMap EventContainer::m_events = {};

#ifdef TESTS
EventMessages BaseEvent::m_eventMessages;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::NewEvent(EventType eventType)
{
  return NewEvent(eventType, nullptr);
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document)
{
  return NewEvent(eventType, document, -1, nullptr);
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType,
                                     const ksapi::IKompasAPIObjectPtr & object)
{
  return NewEvent(eventType, document, objectType, false, object);
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType, bool silent,
                                     const ksapi::IKompasAPIObjectPtr & object)
{
  BaseEvent * result = FindEvents(eventType, document, objectType, object);

  if (result)
    return result;

  std::unique_ptr<BaseEvent> newEvent;

  DocumentTypeEnum docType = document ? document->GetDocumentType() : DocumentTypeEnum::ksDocumentUnknown;

  switch (eventType)
  {
    case EventType::Application:
    {
      newEvent.reset(new ApplicationEvent);
      break;
    }
    case EventType::Document:
    {
      newEvent.reset(new DocumentEvent(document, silent));
      break;
    }
    case EventType::DocumentFrame:
    {
      newEvent.reset(new DocumentFrameEvent(document));
      break;
    }
    case EventType::Document2D:
    {
      // чтобы при закрытии документа отписаться
      NewEvent(EventType::Document, document, -1, true, nullptr);
      newEvent.reset(new Document2DEvent(document));
      break;
    }
    case EventType::Document3D:
    {
      // чтобы при закрытии документа отписаться
      NewEvent(EventType::Document, document, -1, true, nullptr);
      newEvent.reset(new Document3DEvent(document));
      break;
    }
    case EventType::Object2D:
    {
      // Если уже есть обработчик событий объектов 2D для текущего документа - новый не создаём
      auto drwObjEvents = FindEvents(eventType, document);
      if (!drwObjEvents)
      {
        // чтобы при закрытии документа отписаться
        NewEvent(EventType::Document, document, -1, true, nullptr);
        drwObjEvents = new DrawingObjectEvent(document);
        newEvent.reset(drwObjEvents);
      }

      if (drwObjEvents)
      {
        if (object)
          drwObjEvents->AddObjects({object});
        else
          drwObjEvents->AddObjectsTypes({objectType});
      }

      result = drwObjEvents;
      break;
    }
    case EventType::SelectionManager:
    {
      // чтобы при закрытии документа отписаться
      NewEvent(EventType::Document, document, -1, true, nullptr);
      newEvent.reset(new SelectMngEvent(document));
      break;
    }
    case EventType::Stamp:
    {
      if (docType == DocumentTypeEnum::ksDocumentDrawing || docType == DocumentTypeEnum::ksDocumentSpecification ||
          docType == DocumentTypeEnum::ksDocumentTextual)
      {
        // чтобы при закрытии документа отписаться
        NewEvent(EventType::Document, document, -1, true, nullptr);
        newEvent.reset(new StampEvent(document));
      }
      break;
    }
    case EventType::Object3D:
    {
      // Если уже есть обработчик событий объектов 3D для текущего документа - новый не создаём
      auto modelObjEvents = FindEvents(eventType, document);
      if (!modelObjEvents)
      {
        // чтобы при закрытии документа отписаться
        NewEvent(EventType::Document, document, -1, true, nullptr);
        modelObjEvents = new ModelObjectEvent(document);
        newEvent.reset(modelObjEvents);
      }

      if (modelObjEvents)
      {
        if (object)
          modelObjEvents->AddObjects({object});
        else
          modelObjEvents->AddObjectsTypes({objectType});
      }

      result = modelObjEvents;
      break;
    }
    case EventType::SpecificationObject:
    {
      // Если уже есть обработчик событий объектов спецификации для текущего документа - новый не создаём
      auto spcObjEvents = FindEvents(eventType, document);
      if (!spcObjEvents)
      {
        // чтобы при закрытии документа отписаться
        NewEvent(EventType::Document, document, -1, true, nullptr);
        spcObjEvents = new SpecificationObjectEvent(document);
        newEvent.reset(spcObjEvents);
      }

      if (spcObjEvents)
        spcObjEvents->AddObjectsTypes({objectType});

      result = spcObjEvents;
      break;
    }
    case EventType::SpecificationDocument:
    {
      // чтобы при закрытии документа отписаться
      NewEvent(EventType::Document, document, -1, true, nullptr);
      newEvent.reset(new SpecificationDocumentEvent(document));
      break;
    }
    case EventType::SpecificationDescription:
    {
      // чтобы при закрытии документа отписаться
      NewEvent(EventType::Document, document, -1, true, nullptr);
      newEvent.reset(new SpecificationDescriptionEvent(document));
      break;
    }

    default:
      break;
  }

  if (newEvent && newEvent->Advise())
    result = m_events.emplace(eventType, std::move(newEvent))->second.get();

  return result;
}


//-----------------------------------------------------------------------------
// отписать все события
// ---
void EventContainer::UnAdviseAll()
{
  for (const auto & [type, event] : m_events)
    event->UnAdvise();

  m_events.clear();
}


//-------------------------------------------------------------------------------
//
// ---
void EventContainer::UnAdviseByType(EventType eventType)
{
  UnAdviseByTypeAndDoc(eventType, nullptr);
}


//-------------------------------------------------------------------------------
//
// ---
void EventContainer::UnAdviseByDoc(ksapi::IKompasDocumentPtr document)
{
  UnAdviseByTypeAndDoc(EventType::Unkown, document);
}


//-------------------------------------------------------------------------------
//
// ---
void EventContainer::UnAdviseByTypeAndDoc(EventType eventType, ksapi::IKompasDocumentPtr document)
{
  for (EventsMap::iterator it = m_events.begin(); it != m_events.end();)
  {
    if ((eventType == EventType::Unkown || eventType == it->first) &&
        (!document || document && it->second->GetDocument() == document))
    {
      it->second->UnAdvise();
      it = m_events.erase(it);
    }
    else
    {
      it++;
    }
  }
}


//-------------------------------------------------------------------------------
//
// ---
void EventContainer::UnAdviseFromObject(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType,
                                        const ksapi::IKompasAPIObjectPtr & object)
{
  for (EventsMap::iterator it = m_events.begin(); it != m_events.end();)
  {
    if ((eventType == EventType::Unkown || eventType == it->first) &&
        (!document || document && it->second->GetDocument() == document) &&
        (objectType == -1 || it->second->IsObjectAdvised(objectType)) && (!object || it->second->IsObjectAdvised(object)))
    {
      // Случай когда не нужно отписывать весь обработчик, а нужно отписаться от конкретного объекта/типа
      if (objectType != -1 || object)
      {
        switch (eventType)
        {
          case EventType::Object2D:
          case EventType::Object3D:
          case EventType::SpecificationObject:
          {
            if (auto drawingObjEvents = it->second.get())
            {
              if (object)
                drawingObjEvents->RemoveObjects({object});
              else
                drawingObjEvents->RemoveObjectsTypes({objectType});
            }
            break;
          }
          default:
          {
            break;
          }
        }
        it++;
      }
      else // Отписываем весь обработчик
      {
        it->second->UnAdvise();
        it = m_events.erase(it);
      }
    }
    else
    {
      it++;
    }
  }
}


//-----------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::FindEvents(EventType eventType)
{
  return FindEvents(eventType, nullptr);
}


//-----------------------------------------------------------------------------
//
// ---
BaseEvent * EventContainer::FindEvents(EventType eventType, const ksapi::IKompasDocumentPtr & document)
{
  return FindEvents(eventType, document, -1, nullptr);
}


//-----------------------------------------------------------------------------
// Проверить наличие подписки в контейнере по типу, документу и типу объекта
// ---
BaseEvent * EventContainer::FindEvents(EventType eventType, const ksapi::IKompasDocumentPtr & document, int32_t objectType,
                                       const ksapi::IKompasAPIObjectPtr & object)
{
  BaseEvent * event = nullptr;

  for (auto it = m_events.find(eventType); it != m_events.end(); ++it)
  {
    auto doc = it->second->GetDocument();
    if (it->first == eventType && (!doc || doc == document) && (objectType == -1 || it->second->IsObjectAdvised(objectType)) &&
        (!object || it->second->IsObjectAdvised(object)))
    {
      event = it->second.get();
      break;
    }
  }

  return event;
}


//-----------------------------------------------------------------------------
//
// ---
void EventContainer::ListEvents()
{
  std::wstring str = LoadResourceString(IDS_ADVISE_LIST);
  for (const auto & [type, event] : m_events)
  {
    if (event->IsAdvised())
    {
      str += L"\n" + event->EventCaption();
      str += L"; " + event->GetListAdvisedObjects() + L"\n";
    }
  }
  LibMessage(str);
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent::BaseEvent(EventType eventType)
  : m_eventType(eventType) {};


//-------------------------------------------------------------------------------
//
// ---
BaseEvent::BaseEvent(EventType eventType, const ksapi::IKompasDocumentPtr & document)
  : m_eventType(eventType)
  , m_doc(document)
{
}


//-------------------------------------------------------------------------------
//
// ---
BaseEvent::~BaseEvent()
{
  m_doc = nullptr;
  UnAdvise(); // отпишемся от получения событий
}


//-------------------------------------------------------------------------------
//
// ---
bool BaseEvent::Advise()
{
  if (!m_advise)
    m_advise = true;

  return m_advise;
}


//-------------------------------------------------------------------------------
//
// ---
void BaseEvent::UnAdvise()
{
  if (m_advise)
    m_advise = false;
}


//-------------------------------------------------------------------------------
//
// ---
std::wstring BaseEvent::GetListAdvisedObjects()
{
  std::wstring list;

  if (!m_advisedTypes.empty())
  {
    list += LoadResourceString(IDS_ADVISED_TYPES);
    for (const auto & type : m_advisedTypes)
      list += std::to_wstring(type) + L", ";

    list.erase(list.end() - 2, list.end());
  }
  if (!m_advisedObjects.empty())
  {
    list += LoadResourceString(IDS_ADVISED_OBJECTS);
    for (const auto & obj : m_advisedObjects)
    {
      if (ksapi::IDrawingObjectPtr drwObj = obj)
        list += std::format(L"id={}, type={}", drwObj->GetId(), static_cast<int32_t>(drwObj->GetDrawingObjectType())) + L", ";
      else if (ksapi::IModelObjectPtr modelObj = obj)
        list +=
          std::format(L"name={}, type={}", modelObj->GetName(), static_cast<int32_t>(modelObj->GetModelObjectType())) + L", ";
    }

    list.erase(list.end() - 2, list.end());
  }

  return list;
}


//-------------------------------------------------------------------------------
//
// ---
void BaseEvent::ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document, const std::wstring & args)
{
  ShowEventName(document ? eventName + L"; " + GetDocumentParam(document) : eventName, args);
}


//-----------------------------------------------------------------------------
// Вывести имя события
// ---
void BaseEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  std::wstring mess = L"Events::";
  mess += eventName;
#ifdef TESTS
  BaseEvent::m_eventMessages.emplace_back(mess, args);
#else
  if (!args.empty())
    mess += L" - " + args;

  LibMessage(mess);
#endif
}


//-----------------------------------------------------------------------------
//
// ---
std::wstring BaseEvent::GetDocumentParam(ksapi::IKompasDocumentPtr doc)
{
  std::wstring res;
  DocumentTypeEnum type = doc->GetDocumentType();

  if (type != DocumentTypeEnum::ksDocumentUnknown)
  {
    auto filename = std::filesystem::path(doc->GetName()).filename().wstring();
#ifdef TESTS
    if (filename.find(L"tmp") != std::wstring::npos)
      filename = L"tmp";
#endif
    res = filename + LoadResourceString(IDS_TYPE_DOC) + std::to_wstring(type);

    PrintKompasError();
  }
  return res;
}


//-----------------------------------------------------------------------------
// Вывести параметры 2D объекта
// ---
void BaseEvent::ShowObjParam(ksapi::IDrawingObjectPtr obj)
{
  if (!obj)
    return;

  DrawingObjectTypeEnum type = obj->GetDrawingObjectType();

  if (type != DrawingObjectTypeEnum::ksAllObj)
  {
    ksapi::IKompasDocument2DPtr doc2D = m_doc;
    ksapi::ISelectionManagerPtr selectionMngr = doc2D ? doc2D->GetSelectionManager() : nullptr;
#ifndef TESTS
    if (selectionMngr)
      selectionMngr->Select({obj});
#endif
    ShowEventName(LoadResourceString(IDS_OBJECT_TYPE) + std::to_wstring(type));
#ifndef TESTS
    if (selectionMngr)
      selectionMngr->Unselect({obj});
#endif
  }
}

//-----------------------------------------------------------------------------
// Вывести параметры 2D объектов
// ---
void BaseEvent::ShowObjectsParam(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ksapi::IKompasDocument2DPtr doc2D = m_doc;
  ksapi::ISelectionManagerPtr selectionMngr = doc2D ? doc2D->GetSelectionManager() : nullptr;

  std::wstring message = LoadResourceString(IDS_OBJECT_TYPES);
  std::wstring types;
  for (const auto & obj : objects)
  {
    DrawingObjectTypeEnum type = obj->GetDrawingObjectType();

    if (type == DrawingObjectTypeEnum::ksAllObj)
      continue;
#ifndef TESTS
    if (selectionMngr)
      selectionMngr->Select({obj});
#endif
    types += std::to_wstring(type) + L", ";
#ifndef TESTS
    if (selectionMngr)
      selectionMngr->Unselect({obj});
#endif
  }

  if (!types.empty())
  {
    types.erase(types.end() - 2, types.end());
    ShowEventName(message + types);
  }
}


//-----------------------------------------------------------------------------
// Вывести параметры объекта
// ---
std::wstring BaseEvent::GetObjectParam(const ksapi::IKompasAPIObjectPtr & obj)
{
  std::wstring param;

  if (!obj)
    return param;


  if (ksapi::IPartPtr part = obj)
    param = std::format(L"name={} mark={}", part->GetName(), part->GetMarking());
  else if (ksapi::IModelObjectPtr modelObj = obj)
    param = std::format(L"type={} name={}", static_cast<int32_t>(modelObj->GetModelObjectType()), modelObj->GetName());
  else if (ksapi::IDrawingObjectPtr drwObj = obj)
    param = std::format(L"type={}", static_cast<int32_t>(drwObj->GetDrawingObjectType()));

  return param;
}


//------------------------------------------------------------------------------
/**
  Проверить подписку на объект по типу.  
*/
//---
bool BaseEvent::IsObjectAdvised(int32_t objectType)
{
  return m_advisedTypes.contains(objectType);
};


//------------------------------------------------------------------------------
/**
  Проверить подписку на объект. 
*/
//---
bool BaseEvent::IsObjectAdvised(const ksapi::IKompasAPIObjectPtr & object)
{
  return m_advisedObjects.contains(object);
};


//------------------------------------------------------------------------------
/**
  Добавить объекты на события которых подписываемся.
*/
//---
void BaseEvent::AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  m_advisedObjects.insert(objects.begin(), objects.end());
}


//------------------------------------------------------------------------------
/**
  Удалить объекты на события которых подписываемся.
*/
//---
void BaseEvent::RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  for (const auto & obj : objects)
    m_advisedObjects.erase(obj);
}


//------------------------------------------------------------------------------
/**
  Удалить все объекты на события которых подписываемся.
*/
//---
void BaseEvent::RemoveAllObjects()
{
  m_advisedObjects.clear();
}


//------------------------------------------------------------------------------
/**
  Добавить типы объектов на события которых подписываемся.  
*/
//---
void BaseEvent::AddObjectsTypes(const std::vector<int32_t> & types)
{
  m_advisedTypes.insert(types.begin(), types.end());
}


//------------------------------------------------------------------------------
/**
  Удалить типы объектов на события которых подписываемся. 
*/
//---
void BaseEvent::RemoveObjectsTypes(const std::vector<int32_t> & types)
{
  for (const int32_t type : types)
    m_advisedTypes.erase(type);
}


//------------------------------------------------------------------------------
/**
   Удалить все типы объектов на события которых подписываемся.
*/
//---
void BaseEvent::RemoveAllObjectsTypes()
{
  m_advisedTypes.clear();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void AdviseDoc(ksapi::IKompasDocumentPtr document)
{
  if (!document)
    document = kompasApp->GetActiveDocument();

  if (document)
  {
    DocumentTypeEnum docType = document->GetDocumentType();

    if (EventContainer::NewEvent(EventType::Document, document))
    {
      EventContainer::NewEvent(EventType::DocumentFrame, document);

      switch (docType)
      {
        case DocumentTypeEnum::ksDocumentDrawing:
          EventContainer::NewEvent(EventType::Stamp, document); //NewStampEvent(pDoc);
          [[fallthrough]];
        case DocumentTypeEnum::ksDocumentFragment:
        {
          EventContainer::NewEvent(EventType::Document2D, document);                       //NewDocument2DEvent(pDoc);
          EventContainer::NewEvent(EventType::SelectionManager, document);                 //NewSelectMngEvent(pDoc);
          EventContainer::NewEvent(EventType::Object2D, document, 0 /*ALL_OBJ*/, nullptr); //NewObj2DEvent(pDoc, ALL_OBJ);
          break;
        }
        case DocumentTypeEnum::ksDocumentPart:
        case DocumentTypeEnum::ksDocumentAssembly:
        case DocumentTypeEnum::ksDocumentTechnologyAssembly:
        {
          EventContainer::NewEvent(EventType::Document3D, document);       //NewDocument3DEvent(pDoc);
          EventContainer::NewEvent(EventType::SelectionManager, document); //NewSelectMngEvent(pDoc);
          EventContainer::NewEvent(EventType::Object3D, document, 0 /*o3d_unknown*/,
                                   nullptr); //NewObj3DEvent(pDoc, o3d_unknown, NULL); // На все
          break;
        }
        case DocumentTypeEnum::ksDocumentSpecification:
        {
          EventContainer::NewEvent(EventType::SpecificationDocument, document); //NewSpcDocEvent(pDoc);
          EventContainer::NewEvent(EventType::Stamp, document);                 //NewStampEvent(pDoc);
          break;
        }
        case DocumentTypeEnum::ksDocumentTextual:
          EventContainer::NewEvent(EventType::Stamp, document); //NewStampEvent(pDoc);
          break;
      }

      switch (docType)
      {
        case DocumentTypeEnum::ksDocumentDrawing:
        case DocumentTypeEnum::ksDocumentSpecification:
        case DocumentTypeEnum::ksDocumentAssembly:
        case DocumentTypeEnum::ksDocumentTechnologyAssembly:
        {
          EventContainer::NewEvent(EventType::SpecificationDescription, document); //NewSpecificationEvent(pDoc);
          EventContainer::NewEvent(EventType::SpecificationObject, document, ksSpecificationBaseObject /*SPC_BASE_OBJECT*/,
                                   nullptr); //NewSpcObjectEvent(pDoc, SPC_BASE_OBJECT /*На базовые объекты*/);
        }
      }
    }
  }
}
