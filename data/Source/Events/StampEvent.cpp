////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "StampEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

//------------------------------------------------------------------------------
/**
  
*/
//---
StampEvent::StampEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Stamp, document)
{
}


//------------------------------------------------------------------------------
/**
  
*/
//---
StampEvent::~StampEvent()
{
  UnAdvise();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool StampEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  if (ksapi::IStampPtr stamp = GetStamp())
  {
    ksapi::IStampEventsPtr events = stamp->Events();

    if (!events)
      return false;

    events->AddBeginEditStampHandler(handlerOwner, [this]() { return OnBeginEditStamp(); });
    events->AddEndEditStampHandler(handlerOwner, [this](bool wasChanged) { OnEndEditStamp(wasChanged); });
    events->AddStampCellDblClickHandler(handlerOwner, [this](int32_t cellId) { return OnStampCellDblClick(cellId); });
    events->AddStampCellBeginEditHandler(handlerOwner, [this](int32_t cellId) { return OnStampCellBeginEdit(cellId); });
    events->AddStampBeginClearCellsHandler(handlerOwner, [this](const std::vector<int32_t> & currentCellId)
                                           { return OnStampBeginClearCells(currentCellId); });

    ShowEventName(LoadResourceString(IDS_ADVISE_STAMP));
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  
*/
//---
void StampEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::IStampPtr stamp = GetStamp())
    {
      BaseEvent::UnAdvise();

      stamp->Events()->RemoveAllHandlers(libname);
      ShowEventName(LoadResourceString(IDS_UNADVISE_STAMP));
    }
  }
}


//------------------------------------------------------------------------------
/**
  Вывести сообщение от обработчика с проверкой.  
*/
//---
void StampEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  if (theApp.m_mes_STAMP)
    BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region Обработчики событий штампа
//------------------------------------------------------------------------------
/**
  Обработчик события "Начало работы со штампом".
*/
//---
bool StampEvent::OnBeginEditStamp()
{
  ShowEventName(L"StampEvent::BeginEditStamp");
  return !theApp.m_mes_STAMP || YesNoDialog(LoadResourceString(IDS_STAMP_BEGIN_EDIT)) == ksMessageButtonEnum::ksButtonYes;
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Завершение работы со штампом".
*/
//---
void StampEvent::OnEndEditStamp(bool wasChanged)
{
  ShowEventName(L"StampEvent::EndEditStamp", wasChanged ? L"True" : L"False");
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Двойной клик в ячейке штампа".
*/
//---
bool StampEvent::OnStampCellDblClick(int32_t cellId)
{
  ShowEventName(L"StampEvent::StampCellDblClick", std::to_wstring(cellId));

  std::wstring mess;
  bool res = ::YesNoDialog(FormatResourceString(IDS_STAMP_CELL_DBLCLICK, cellId)) == ksMessageButtonEnum::ksButtonYes;

#ifndef TESTS
  if (ksapi::IStampPtr stamp = res ? GetStamp() : nullptr)
  {
    if (ksapi::ITextPtr text = stamp->GetText(cellId))
    {
      text->SetStr(L"EventCom::StampEvent " + LoadResourceString(IDS_STAMP_CELL_DBLCLICK_STR));
      stamp->Update();
    }
  }
#endif


  return res;
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Начало редактирования в ячейке штампа".
*/
//---
bool StampEvent::OnStampCellBeginEdit(int32_t cellId)
{
  ShowEventName(L"StampEvent::StampCellBeginEdit", std::to_wstring(cellId));

  std::wstring mess;
  bool res = ::YesNoDialog(FormatResourceString(IDS_STAMP_CELL_EDIT, cellId)) == ksMessageButtonEnum::ksButtonYes;

#ifndef TESTS
  if (ksapi::IStampPtr stamp = res ? GetStamp() : nullptr)
  {
    if (ksapi::ITextPtr text = stamp->GetText(cellId))
    {
      text->SetStr(L"EventCom::StampEvent " + LoadResourceString(IDS_STAMP_CELL_EDIT_STR));
      stamp->Update();
    }
  }
#endif


  return res;
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Начало очистки ячеек штампа".
*/
//---
bool StampEvent::OnStampBeginClearCells(const std::vector<int32_t> & currentCellId)
{
  std::wstring args;

  for (const auto & i : currentCellId)
    args.append(std::to_wstring(i) + L", ");

  if (!args.empty())
    args.erase(args.end() - 2, args.end());

  ShowEventName(L"StampEvent::StampBeginClearCells", args);
  return true;
}
#pragma endregion


//------------------------------------------------------------------------------
/**
  Получить штамп первой страницы.
*/
//---
ksapi::IStampPtr StampEvent::GetStamp()
{
  if (ksapi::ILayoutSheetsPtr sheets = m_doc ? m_doc->GetLayoutSheets() : nullptr)
  {
    if (ksapi::ILayoutSheetPtr sheet = sheets->GetItem(0))
    {
      return sheet->GetStamp();
    }
  }
  return nullptr;
}
