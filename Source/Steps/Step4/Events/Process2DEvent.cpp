////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Process2DEvent.h"
#include "Step4_pch.h"
#include "../CPropMen.h"

extern std::wstring handlerOwner;

//------------------------------------------------------------------------------
/**
  Конструктор  
*/
//---
Process2DEvent::Process2DEvent(ksapi::IProcess2DPtr & process, PropertyManagerObject & obj, bool copyProc)
  : m_process(process)
  , m_obj(obj)
{
  AdviseEvents();
}

//------------------------------------------------------------------------------
/**
  Подписаться на события  
*/
//---
void Process2DEvent::AdviseEvents()
{
  if (m_process)
  {
    ksapi::IProcess2DEventsPtr events = m_process->Events();

    if (!events)
      return;

    events->AddExecuteCommandHandler(handlerOwner, [this](std::int32_t command) { return ExecuteCommand(command); });
    events->AddPlacementChangedHandler(handlerOwner, [this](double x, double y, double angle, bool dynamic)
                                       { return PlacementChanged(x, y, angle, dynamic); });
    events->AddEndProcessHandler(handlerOwner, [this]() { return EndProcess(); });
  }
}


//------------------------------------------------------------------------------
/**
  Выполнить команду меню.  
*/
//---
bool Process2DEvent::ExecuteCommand(std::int32_t command)
{
  return m_obj.ButtonClick(command);
}


//------------------------------------------------------------------------------
/**
  Изменено положения объекта  
*/
//---
bool Process2DEvent::PlacementChanged(double x, double y, double angle, bool dynamic)
{
  return m_obj.PlacementChanged(x, y, angle, dynamic);
}


//------------------------------------------------------------------------------
/**
  Окончание процесса.  
*/
//---
bool Process2DEvent::EndProcess()
{
  // Завершение процесса
  m_obj.EndProcess();
  // Отписка от получения событий
  UnadviseEvents();
  delete &m_obj;
  return true;
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий  
*/
//---
void Process2DEvent::UnadviseEvents()
{
  if (m_process)
    m_process->Events()->RemoveAllHandlers(handlerOwner);
}
