////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "SelectMngEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;


//-------------------------------------------------------------------------------
//
// ---
SelectMngEvent::SelectMngEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::SelectionManager, document)
{
  docType = document->GetDocumentType();
}


//-------------------------------------------------------------------------------
//
// ---
SelectMngEvent::~SelectMngEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool SelectMngEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  ksapi::ISelectionManagerPtr selectMngr;

  if (ksapi::IKompasDocument2DPtr doc2D = m_doc)
    selectMngr = doc2D->GetSelectionManager();
  else if (ksapi::IKompasDocument3DPtr doc3D = m_doc)
    selectMngr = doc3D->GetSelectionManager();

  if (selectMngr)
  {
    selectMngr->Events()->AddSelectHandler(handlerOwner, [this](const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
                                           { OnSelect(objects); });
    selectMngr->Events()->AddUnselectHandler(handlerOwner, [this](const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
                                             { OnUnselect(objects); });
    selectMngr->Events()->AddUnselectAllHandler(handlerOwner, [this]() { OnUnselectAll(); });

    ShowEventName(LoadResourceString(IDS_ADVISE_SELECTION_MANAGER), {});
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void SelectMngEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    ksapi::IKompasDocumentEventsPtr events = m_doc->Events();

    if (!events)
      return;

    events->RemoveAllHandlers(libname);

    BaseEvent::UnAdvise();
    ShowEventName(LoadResourceString(IDS_UNADVISE_SELECTION_MANAGER), {});
  }
}


//-------------------------------------------------------------------------------
//
// ---
void SelectMngEvent::ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  if (theApp.m_mes_SELECT)
  {
    std::wstring objParams;
    for (const auto & obj : objects)
      objParams += GetObjectParam(obj) + L", ";

    if (!objParams.empty())
      objParams.erase(objParams.end() - 2, objParams.end());

    BaseEvent::ShowEventName(eventName, m_doc, objParams);
  }
}


#pragma region Обработчики событий документа
//-------------------------------------------------------------------------------
// Обработчик события "Объект селектирован".
// ---
void SelectMngEvent::OnSelect(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  ShowEventName(L"SelectMngEvent::Select", objects);
}


//-------------------------------------------------------------------------------
// Обработчик события "Объект расселектирован".
// ---
void SelectMngEvent::OnUnselect(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  ShowEventName(L"SelectMngEvent::Unselect", objects);
}

///
//-------------------------------------------------------------------------------
// Обработчик события "Все объекты расселектированы".
// ---
void SelectMngEvent::OnUnselectAll()
{
  ShowEventName(L"SelectMngEvent::UnselectAll", {});
}
#pragma endregion
