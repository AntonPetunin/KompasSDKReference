#include "SpecificationObjectEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;


//------------------------------------------------------------------------------
/**
  
*/
//---
std::vector<ksSpecificationObjectTypeEnum> ToSpcObjectTypes(const std::vector<int32_t> & types)
{
  std::vector<ksSpecificationObjectTypeEnum> spcTypes(types.size());
  std::transform(types.begin(), types.end(), spcTypes.begin(), [](int32_t type) { return ksSpecificationObjectTypeEnum(type); });

  return spcTypes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationObjectEvent::SpecificationObjectEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::SpecificationObject, document)
{
  InitEventHandler();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationObjectEvent::SpecificationObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName)
  : BaseEvent(EventType::SpecificationObject, document)
  , m_handlerName(handlerName)
{
  InitEventHandler();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::InitEventHandler()
{
  if (m_doc)
    m_events = m_doc->GetSpecificationObjectEvents(m_handlerName);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationObjectEvent::~SpecificationObjectEvent()
{
  UnAdvise();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  if (m_events)
  {
    m_events->AddBeginDeleteHandler([this](const ksapi::ISpecificationObjectPtr & object) { return OnBeginDelete(object); });
    m_events->AddDeleteHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnDelete(object); });
    m_events->AddCreateHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnCreate(object); });
    m_events->AddBeginCopyHandler([this](const ksapi::ISpecificationObjectPtr & object) { return OnBeginCopy(object); });
    m_events->AddCopyHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnCopy(object); });
    m_events->AddDocumentBeginAddHandler([this](const ksapi::ISpecificationObjectPtr & object)
                                         { return OnDocumentBeginAdd(object); });
    m_events->AddDocumentAddHandler([this](const ksapi::ISpecificationObjectPtr & object, const std::wstring & nameDoc)
                                    { OnDocumentAdd(object, nameDoc); });
    m_events->AddDocumentRemoveHandler([this](const ksapi::ISpecificationObjectPtr & object, const std::wstring & nameDoc)
                                       { OnDocumentRemove(object, nameDoc); });
    m_events->AddChangeCurrentHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnChangeCurrent(object); });
    m_events->AddBeginGeomChangeHandler([this](const ksapi::ISpecificationObjectPtr & object)
                                        { return OnBeginGeomChange(object); });
    m_events->AddGeomChangeHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnGeomChange(object); });
    m_events->AddBeginProcessHandler(
      [this](ksSpecificationObjectTypeEnum specificationObjectType, const ksapi::ISpecificationObjectPtr & object)
      { return OnBeginProcess(specificationObjectType, object); });
    m_events->AddEndProcessHandler([this](ksSpecificationObjectTypeEnum specificationObjectType)
                                   { return OnEndProcess(specificationObjectType); });
    m_events->AddCellDblClickHandler([this](const ksapi::ISpecificationObjectPtr & object, int32_t columnNum)
                                     { return OnCellDblClick(object, columnNum); });
    m_events->AddCellBeginEditHandler([this](const ksapi::ISpecificationObjectPtr & object, int32_t columnType)
                                      { return OnCellBeginEdit(object, columnType); });
    m_events->AddUpdateHandler([this](const ksapi::ISpecificationObjectPtr & object) { OnUpdate(object); });

    ShowEventName(LoadResourceString(IDS_ADVISE_SPC_OBJ), nullptr);
  }


  return res;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (m_events)
      m_events->RemoveAllHandlers();

    BaseEvent::UnAdvise();
    ShowEventName(LoadResourceString(IDS_UNADVISE_SPC_OBJ), nullptr);
  }
}


//------------------------------------------------------------------------------
/**
   Добавить типы объектов на события которых подписываемся.  
*/
//---
void SpecificationObjectEvent::AddObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::AddObjectsTypes(types);

  if (m_events)
    m_events->Add(ToSpcObjectTypes(types));
}


//------------------------------------------------------------------------------
/**
  Удалить типы объектов на события которых подписываемся.  
*/
//---
void SpecificationObjectEvent::RemoveObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::RemoveObjectsTypes(types);

  if (m_events)
    m_events->Remove(ToSpcObjectTypes(types));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::ShowEventName(const std::wstring & eventName, const ksapi::ISpecificationObjectPtr & object,
                                             const std::wstring & args /*= L""*/)
{
  if (!theApp.m_mes_OBJ_SPC)
    return;

  BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region Обработчики событий объектов спецификации
//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnBeginDelete(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::BeginDelete", object);
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_OBJECT_DELETE)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnDelete(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::Delete", object);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnCreate(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::Create", object);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnBeginCopy(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::BeginCopy", object);
  return true;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnCopy(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::Copy", object);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnDocumentBeginAdd(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::DocumentBeginAdd", object);
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_SPC_ADD_DOC)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnDocumentAdd(const ksapi::ISpecificationObjectPtr & object, const std::wstring & nameDoc)
{
  ShowEventName(L"SpcObjectEvent::DocumentAdd", object, nameDoc);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnDocumentRemove(const ksapi::ISpecificationObjectPtr & object, const std::wstring & nameDoc)
{
  ShowEventName(L"SpcObjectEvent::DocumentRemove", object, nameDoc);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnChangeCurrent(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::ChangeCurrent", object);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnBeginGeomChange(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::BeginGeomChange", object);
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_SPC_CHANGE_GEOM)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnGeomChange(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::GeomChange", object);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnBeginProcess(ksSpecificationObjectTypeEnum specificationObjectType,
                                              const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::BeginProcess", object,
                std::format(L"specificationObjectType={}", std::to_wstring(specificationObjectType)));
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_BEGIN_PROCESS)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnEndProcess(ksSpecificationObjectTypeEnum specificationObjectType)
{
  ShowEventName(L"SpcObjectEvent::EndProcess", nullptr,
                std::format(L"specificationObjectType={}", std::to_wstring(specificationObjectType)));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnCellDblClick(const ksapi::ISpecificationObjectPtr & object, int32_t columnNum)
{
  ShowEventName(L"SpcObjectEvent::CellDblClick", object, std::format(L"columnNum={}", std::to_wstring(columnNum)));
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_SPC_CELL_EDIT)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationObjectEvent::OnCellBeginEdit(const ksapi::ISpecificationObjectPtr & object, int32_t columnType)
{
  ShowEventName(L"SpcObjectEvent::CellBeginEdit", object, std::format(L"columnType={}", std::to_wstring(columnType)));
  return !theApp.m_mes_OBJ_SPC || YesNoDialog(LoadResourceString(IDS_SPC_OBJ_EDIT)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationObjectEvent::OnUpdate(const ksapi::ISpecificationObjectPtr & object)
{
  ShowEventName(L"SpcObjectEvent::Update", object);
}
#pragma endregion
