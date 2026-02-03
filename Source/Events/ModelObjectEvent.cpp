#include "ModelObjectEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;


//-------------------------------------------------------------------------------
//
// ---
std::vector<ksapi::IModelObjectPtr> ToModelObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  std::vector<ksapi::IModelObjectPtr> modelObjs(objects.begin(), objects.end());
  return modelObjs;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
ModelObjectEvent::ModelObjectEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Object3D, document)
{
  InitEventHandler();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
ModelObjectEvent::ModelObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName)
  : BaseEvent(EventType::Object3D, document)
  , m_handlerName(handlerName)
{
  InitEventHandler();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::InitEventHandler()
{
  if (ksapi::IKompasDocument3DPtr doc3D = m_doc)
    m_events = doc3D->ObjectEvents3D(m_handlerName);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
ModelObjectEvent::~ModelObjectEvent()
{
  UnAdvise();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  if (m_events)
  {
    m_events->AddBeginDeleteObjectHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                          { return OnBeginDeleteObject(objects); });
    m_events->AddDeleteObjectHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects) { OnDeleteObject(objects); });
    m_events->AddExcludedObjectHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects, bool excluded)
                                       { OnExcludedObject(objects, excluded); });
    m_events->AddHiddenObjectHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects, bool hidden)
                                     { OnHiddenObject(objects, hidden); });
    m_events->AddBeginPropertyChangedHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                             { return OnBeginPropertyChanged(objects); });
    m_events->AddPropertyChangedHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                        { OnPropertyChanged(objects); });
    m_events->AddBeginPlacementChangedHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                              { return OnBeginPlacementChanged(objects); });
    m_events->AddPlacementChangedHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                         { OnPlacementChanged(objects); });
    m_events->AddBeginProcessHandler([this](ksObj3dTypeEnum objectType, const ksapi::IModelObjectPtr & object)
                                     { return OnBeginProcess(objectType, object); });
    m_events->AddEndProcessHandler([this](ksObj3dTypeEnum objectType) { OnEndProcess(objectType); });
    m_events->AddCreateObjectHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects)
                                     { return OnCreateObject(objects); });
    m_events->AddUpdateObjectHandler([this](const ksapi::IModelObjectPtr & object) { OnUpdateObject(object); });
    m_events->AddBeginLoadStateChangeHandler(
      [this](const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState)
      { return OnBeginLoadStateChange(objects, loadState); });
    m_events->AddLoadStateChangeHandler([this](const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState)
                                        { OnLoadStateChange(objects, loadState); });

    ShowEventName(LoadResourceString(IDS_ADVISE_OBJECT_3D), {}, false);
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (m_events)
    {
      m_events->RemoveAllObjects();
      m_events->RemoveAllObjectsTypes();
      m_events->RemoveAllHandlers();
    }

    BaseEvent::UnAdvise();
    ShowEventName(LoadResourceString(IDS_UNADVISE_OBJECT_3D), {}, false);
  }
}

//-------------------------------------------------------------------------------
// Добавить объекты на события которых подписываемся.
// ---
void ModelObjectEvent::AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  BaseEvent::AddObjects(objects);

  if (m_events)
    m_events->AddObjects(ToModelObjects(objects));
}


//-------------------------------------------------------------------------------
// Удалить объекты на события которых подписываемся.
// ---
void ModelObjectEvent::RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  BaseEvent::RemoveObjects(objects);

  if (m_events)
    m_events->RemoveObjects(ToModelObjects(objects));
}


//-------------------------------------------------------------------------------
// Удалить все объекты на события которых подписываемся.
// ---
void ModelObjectEvent::RemoveAllObjects()
{
  BaseEvent::RemoveAllObjects();

  if (m_events)
    m_events->RemoveAllObjects();
}


//------------------------------------------------------------------------------
/**
  Добавить типы объектов на события которых подписываемся.  
*/
//---
void ModelObjectEvent::AddObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::AddObjectsTypes(types);

  if (m_events)
    m_events->AddObjectsTypes(types);
}


//------------------------------------------------------------------------------
/**
   Удалить типы объектов на события которых подписываемся. 
*/
//---
void ModelObjectEvent::RemoveObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::RemoveObjectsTypes(types);

  if (m_events)
    m_events->RemoveObjectsTypes(types);
}


//------------------------------------------------------------------------------
/**
   Удалить все типы объектов на события которых подписываемся. 
*/
//---
void ModelObjectEvent::RemoveAllObjectsTypes()
{
  BaseEvent::RemoveAllObjectsTypes();

  if (m_events)
    m_events->RemoveAllObjectsTypes();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IModelObjectPtr> & objects,
                                     bool showResult, const std::wstring & args /*= L""*/)
{
  if (!theApp.m_mes_OBJ_3DDOC)
    return;

  std::wstring objParams;
  for (const auto & obj : objects)
    objParams += GetObjectParam(obj) + L", ";

  if (!objParams.empty())
    objParams.erase(objParams.end() - 2, objParams.end());

  //#ifndef TESTS
  ChooseObjects(objects, true);
  //#endif
  BaseEvent::ShowEventName(eventName, m_doc, args + L" " + objParams);
  //#ifndef TESTS
  ChooseObjects(objects, false);
  //#endif


  if (ksapi::IKompasDocument2DPtr doc3D = m_doc; doc3D && showResult)
  {
    std::wstring message;
    for (const auto & obj : objects)
    {
      ksapi::IPartPtr iPart = obj;

      if (!iPart)
        continue;

      if (ksapi::IModelObjectEventsResultPtr iObj3DRes = doc3D->GetDrawingObjectEventsResult())
      {
        int type = iObj3DRes->GetNotifyType();
        if (type)
        {
          message += std::format(L"NotifyType={}", type);
          std::vector<ksapi::IModelObjectPtr> notifyObjects = iObj3DRes->GetNotifyObjects();
          message += std::format(L"\nFeatureCollection::Count={}", notifyObjects.size());

          if (ksapi::IPlacement3DPtr placement = iPart->GetPlacement())
          {
            double x = 0.0, y = 0.0, z = 0.0;
            placement->GetOrigin(x, y, z);
            message += std::format(L"\nx={:.2}, y={:.2}, z={:.2}", x, y, z);
          }
          message += L"\n";
        }
      }
    }

    ShowEventName(message, {}, false);
  }
}


#pragma region Обработчики событий объектов 3D документа
//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::OnBeginDeleteObject(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::BeginDeleteObject", objects, false);
  return !theApp.m_mes_OBJ_3DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_DELETE)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnDeleteObject(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::DeleteObject", objects, false);

  std::vector<ksapi::IKompasAPIObjectPtr> apiObjs(objects.begin(), objects.end());
  RemoveObjects(apiObjs);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnExcludedObject(const std::vector<ksapi::IModelObjectPtr> & objects, bool excluded)
{
  ShowEventName(L"Obj3DEvent::ExcludedObject", objects, false,
                excluded ? LoadResourceString(IDS_EXCLUDED_OBJ) : LoadResourceString(IDS_INCLUDED_OBJ));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnHiddenObject(const std::vector<ksapi::IModelObjectPtr> & objects, bool hidden)
{
  ShowEventName(L"Obj3DEvent::HiddenObject", objects, false,
                hidden ? LoadResourceString(IDS_HIDDEN_OBJ) : LoadResourceString(IDS_SHOWN_OBJ));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::OnBeginPropertyChanged(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::BeginPropertyChanged", objects, false);
  return !theApp.m_mes_OBJ_3DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_CHANGED)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnPropertyChanged(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::PropertyChanged", objects, false);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::OnBeginPlacementChanged(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::BeginPlacementChanged", objects, false);
  return !theApp.m_mes_OBJ_3DDOC || YesNoDialog(LoadResourceString(IDS_CHANGE_PLACEMENT)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnPlacementChanged(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::PlacementChanged", objects, true);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::OnBeginProcess(ksObj3dTypeEnum objectType, const ksapi::IModelObjectPtr & object)
{
  ShowEventName(L"Obj3DEvent::BeginProcess", {object}, false, std::format(L"objectType = {}; ", std::to_wstring(objectType)));
  return !theApp.m_mes_OBJ_3DDOC || YesNoDialog(LoadResourceString(IDS_BEGIN_PROCESS)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnEndProcess(ksObj3dTypeEnum objectType)
{
  ShowEventName(L"Obj3DEvent::EndProcess", {}, false, std::format(L"objectType {};", std::to_wstring(objectType)));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnCreateObject(const std::vector<ksapi::IModelObjectPtr> & objects)
{
  ShowEventName(L"Obj3DEvent::CreateObject", objects, false);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnUpdateObject(const ksapi::IModelObjectPtr & object)
{
  ShowEventName(L"Obj3DEvent::UpdateObject", {object}, false);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool ModelObjectEvent::OnBeginLoadStateChange(const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState)
{
  ShowEventName(L"Obj3DEvent::BeginLoadStateChange", {}, false, std::format(L"loadState {};", std::to_wstring(loadState)));
  return true;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::OnLoadStateChange(const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState)
{
  ShowEventName(L"Obj3DEvent::LoadStateChange", {}, false, std::format(L"loadState {};", std::to_wstring(loadState)));
}
#pragma endregion


//------------------------------------------------------------------------------
/**
  
*/
//---
void ModelObjectEvent::ChooseObjects(const std::vector<ksapi::IModelObjectPtr> & objects, bool choose)
{
  ksapi::IKompasDocument3DPtr doc3D = m_doc;
  std::vector<ksapi::IKompasAPIObjectPtr> apiObjs;
  apiObjs.reserve(objects.size());

  for (auto & obj : objects)
    apiObjs.push_back(obj);

  if (ksapi::IChooseManagerPtr selectionMngr = doc3D ? doc3D->GetChooseManager() : nullptr)
  {
    if (choose)
      selectionMngr->Choose(apiObjs);
    else
      selectionMngr->Unchoose(apiObjs);
  }
}
