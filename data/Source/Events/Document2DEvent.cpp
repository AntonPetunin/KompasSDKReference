////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "Document2DEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Document2DEvent::Document2DEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Document2D, document)
{
}


//-------------------------------------------------------------------------------
//
// ---
Document2DEvent::~Document2DEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool Document2DEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  if (ksapi::IKompasDocument2DPtr doc2D = m_doc)
  {
    ksapi::IKompasDocument2DEventsPtr events = doc2D->Events2D();

    if (!events)
      return false;

    events->AddBeginRebuildHandler(handlerOwner, [this]() { return this->OnBeginRebuild(); });
    events->AddRebuildHandler(handlerOwner, [this]() { return this->OnRebuild(); });
    events->AddBeginInsertFragmentHandler(handlerOwner, [this]() { return this->OnBeginInsertFragment(); });
    events->AddBeginChoiceMaterialHandler(handlerOwner, [this]() { return this->OnBeginChoiceMaterial(); });
    events->AddChoicePropertyHandler(handlerOwner,
                                     [this](const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
                                     { return this->OnChoiceProperty(object, propertyTypeId); });
    events->AddBeginChoicePropertyHandler(handlerOwner,
                                          [this](const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
                                          { return this->OnBeginChoiceProperty(object, propertyTypeId); });
    events->AddBeginDeletePropertyHandler(handlerOwner,
                                          [this](const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
                                          { return this->OnBeginDeleteProperty(object, propertyTypeId); });
    events->AddDeletePropertyHandler(handlerOwner,
                                     [this](const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
                                     { return this->OnDeleteProperty(object, propertyTypeId); });

    ShowEventName(LoadResourceString(IDS_ADVISE_DOC_2D));
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void Document2DEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::IKompasDocument2DPtr doc2D = m_doc)
    {
      doc2D->Events2D()->RemoveAllHandlers(libname);

      BaseEvent::UnAdvise();
      ShowEventName(LoadResourceString(IDS_UNADVISE_DOC_2D));
    }
  }
}


//-------------------------------------------------------------------------------
//
// ---
void Document2DEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  if (theApp.m_mes_2DDOC)
    BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region Обработчики событий 2D документа
//-------------------------------------------------------------------------------
// Начало перестроения модели
// ---
bool Document2DEvent::OnBeginRebuild()
{
  ShowEventName(L"Document2DEvent::BeginRebuild");
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_DOC_REBUILD)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
// Модель перестроена
// ---
bool Document2DEvent::OnRebuild()
{
  ShowEventName(L"Document2DEvent::Rebuild");
  return true;
}


//-------------------------------------------------------------------------------
// Начало выбора материала
// ---
bool Document2DEvent::OnBeginChoiceMaterial()
{
  ShowEventName(L"Document2DEvent::BeginChoiceMaterial");
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_CHANGE_MATERIAL)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
// Начало вставки фрагмента (до диалога выбора имени)
// ---
bool Document2DEvent::OnBeginInsertFragment()
{
  ShowEventName(L"Document2DEvent::BeginInsertFragment");
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_INSERT_FRAGMENT)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
// Начало выбора свойства
// ---
bool Document2DEvent::OnBeginChoiceProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
{
  ShowEventName(L"Document2DEvent::BeginChoiceProperty",
                std::format(L"object = {}, propertyTypeId = {}", object ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyTypeId)));
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_CHOICE_PROPERTY)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
// Закончен выбор свойства
// ---
bool Document2DEvent::OnChoiceProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
{
  ShowEventName(L"Document2DEvent::ChoiceProperty",
                std::format(L"object = {}, propertyTypeId = {}", object ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyTypeId)));
  return true;
}


//-------------------------------------------------------------------------------
// Начало удаления значения свойства
// ---
bool Document2DEvent::OnBeginDeleteProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
{
  ShowEventName(L"Document2DEvent::BeginDeleteProperty",
                std::format(L"object = {}, propertyTypeId = {}", object ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyTypeId)));
  return !theApp.m_mes_2DDOC || YesNoDialog(LoadResourceString(IDS_DELETE_PROPERTY)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
// Завершение удаления свойства
// ---
bool Document2DEvent::OnDeleteProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId)
{
  ShowEventName(L"Document2DEvent::DeleteProperty",
                std::format(L"object = {}, propertyTypeId = {}", object ? L"Not NULL" : L"NULL",
                            static_cast<uint64_t>(propertyTypeId)));
  return true;
}
#pragma endregion
