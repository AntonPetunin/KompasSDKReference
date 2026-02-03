////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "Document3DEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

//-------------------------------------------------------------------------------
//
// ---
Document3DEvent::Document3DEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Document3D, document)
{
}


//-------------------------------------------------------------------------------
//
// ---
Document3DEvent::~Document3DEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  if (ksapi::IKompasDocument3DPtr doc3D = m_doc)
  {
    ksapi::IKompasDocument3DEventsPtr events = doc3D->Events3D();

    if (!events)
      return false;

    events->AddBeginRebuildHandler(handlerOwner, [this]() { return this->OnBeginRebuild(); });
    events->AddRebuildHandler(handlerOwner, [this]() { return this->OnRebuild(); });
    events->AddBeginSetPartFromFileHandler(handlerOwner, [this]() { return this->OnBeginSetPartFromFile(); });
    events->AddBeginCreatePartFromFileHandler(handlerOwner, [this](bool isPart, const ksapi::IModelObjectPtr & object)
                                              { return this->OnBeginCreatePartFromFile(isPart, object); });
    events->AddBeginRollbackFeaturesHandler(handlerOwner, [this]() { return this->OnBeginRollbackFeatures(); });
    events->AddRollbackFeaturesHandler(handlerOwner, [this]() { return this->OnRollbackFeatures(); });
    events->AddBeginLoadCombinationChangeHandler(handlerOwner,
                                                 [this](int32_t index) { return this->OnBeginLoadCombinationChange(index); });
    events->AddLoadCombinationChangeHandler(handlerOwner, [this](int32_t index) { return this->OnLoadCombinationChange(index); });
    events->AddBeginChoiceMaterialHandler(handlerOwner, [this]() { return this->OnBeginChoiceMaterial(); });
    events->AddBeginChoiceMarkingHandler(handlerOwner, [this]() { return this->OnBeginChoiceMarking(); });
    events->AddCreateEmbodimentHandler(handlerOwner, [this](const std::wstring & marking) { this->OnCreateEmbodiment(marking); });
    events->AddDeleteEmbodimentHandler(handlerOwner, [this](const std::wstring & marking) { this->OnDeleteEmbodiment(marking); });
    events->AddChangeCurrentEmbodimentHandler(handlerOwner,
                                              [this](const std::wstring & marking) { this->OnChangeCurrentEmbodiment(marking); });
    events->AddBeginDeleteMaterialHandler(handlerOwner, [this]() { return this->OnBeginDeleteMaterial(); });
    events->AddDeleteMaterialHandler(handlerOwner, [this]() { return this->OnDeleteMaterial(); });
    events->AddBeginChoicePropertyHandler(handlerOwner,
                                          [this](const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
                                          { return this->OnBeginChoiceProperty(notifyObj, propertyId); });
    events->AddChoicePropertyHandler(handlerOwner, [this](const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
                                     { return OnChoiceProperty(notifyObj, propertyId); });
    events->AddBeginDeletePropertyHandler(handlerOwner,
                                          [this](const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
                                          { return this->OnBeginDeleteProperty(notifyObj, propertyId); });
    events->AddDeletePropertyHandler(handlerOwner, [this](const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
                                     { return this->OnDeleteProperty(notifyObj, propertyId); });

    ShowEventName(LoadResourceString(IDS_ADVISE_DOC_3D));
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void Document3DEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::IKompasDocument3DPtr doc3D = m_doc)
    {
      doc3D->Events3D()->RemoveAllHandlers(libname);

      BaseEvent::UnAdvise();
      ShowEventName(LoadResourceString(IDS_UNADVISE_DOC_3D));
    }
  }
}


//-------------------------------------------------------------------------------
//
// ---
void Document3DEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  if (theApp.m_mes_2DDOC)
    BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region Обработчики событий 3D документа
//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginRebuild()
{
  ShowEventName(L"Document3DEvent::BeginRebuild");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_DOC_REBUILD)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnRebuild()
{
  ShowEventName(L"Document3DEvent::Rebuild");
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginChoiceMaterial()
{
  ShowEventName(L"Document3DEvent::BeginChoiceMaterial");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_CHANGE_MATERIAL)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginChoiceMarking()
{
  ShowEventName(L"Document3DEvent::BeginChoiceMarking");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_CHANGE_MARKING)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginSetPartFromFile()
{
  ShowEventName(L"Document3DEvent::BeginSetPartFromFile");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_SETPART_FROMFILE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginCreatePartFromFile(bool isPart, const ksapi::IModelObjectPtr & object)
{
  ShowEventName(L"Document3DEvent::BeginCreatePartFromFile",
                std::format(L"isPart = {}, object = {}", isPart ? L"True" : L"False", object ? L"Not Null" : L"Null"));
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_CREATE_PART)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
void Document3DEvent::OnCreateEmbodiment(const std::wstring & marking)
{
  ShowEventName(L"Document3DEvent::CreateEmbodiment", marking);
}


//-------------------------------------------------------------------------------
//
// ---
void Document3DEvent::OnDeleteEmbodiment(const std::wstring & marking)
{
  ShowEventName(L"Document3DEvent::DeleteEmbodiment", marking);
}


//-------------------------------------------------------------------------------
//
// ---
void Document3DEvent::OnChangeCurrentEmbodiment(const std::wstring & marking)
{
  ShowEventName(L"Document3DEvent::ChangeCurrentEmbodiment", marking);
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginChoiceProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
{
  ShowEventName(L"Document3DEvent::BeginChoiceProperty",
                std::format(L"object = {}, propertyTypeId = {}", notifyObj ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyId)));
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_CHOICE_PROPERTY)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnChoiceProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propertyId)
{
  ShowEventName(L"Document3DEvent::ChoiceProperty",
                std::format(L"object = {}, propertyTypeId = {}", notifyObj ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyId)));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginRollbackFeatures()
{
  ShowEventName(L"Document3DEvent::BeginRollbackFeatures");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_ROLL_BACK)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnRollbackFeatures()
{
  ShowEventName(L"Document3DEvent::RollbackFeatures");
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginLoadCombinationChange(int32_t index)
{
  ShowEventName(L"Document3DEvent::BeginLoadCombinationChange", std::to_wstring(index));
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_LOAD_COMBINATION_CHANGE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnLoadCombinationChange(int32_t index)
{
  ShowEventName(L"Document3DEvent::LoadCombinationChange", std::to_wstring(index));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginDeleteMaterial()
{
  ShowEventName(L"Document3DEvent::BeginDeleteMaterial");
  return !theApp.m_mes_3DDOC || YesNoDialog(LoadResourceString(IDS_DELETE_MATERIAL)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnDeleteMaterial()
{
  ShowEventName(L"Document3DEvent::DeleteMaterial");
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnBeginDeleteProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propId)
{
  ShowEventName(L"Document3DEvent::BeginDeleteProperty",
                std::format(L"notifyObj = {}, propertyTypeId propId {}", notifyObj ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propId)));
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_DELETE_PROPERTY)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool Document3DEvent::OnDeleteProperty(const ksapi::IModelObjectPtr & notifyObj, ksapi::NumericId propId)
{
  ShowEventName(L"Document3DEvent::DeleteProperty",
                std::format(L"notifyObj = {}, propertyTypeId = {}", notifyObj ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propId)));
  return true;
}
#pragma endregion
