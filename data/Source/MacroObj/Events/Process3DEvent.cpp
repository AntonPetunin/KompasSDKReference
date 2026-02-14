////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Реализация класса обработчика событий от процесса 3D
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Process3DEvent.h"
#include <MacroObj_pch.h>
#include <PropManager/CPropMen.h>


extern ksapi::IApplication * ksAppl;
extern std::wstring handlerOwner;

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Process3DEvent::Process3DEvent(ksapi::IProcess3DPtr & process, PropertyManagerObject & _obj, bool copyProc)
  : m_process(process)
  , m_obj(_obj)
{
  AdviseEvents();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
Process3DEvent::~Process3DEvent()
{
}


//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
bool Process3DEvent::ExecuteCommand(std::int32_t command)
{
  return m_obj.ButtonClick(command);
}


//------------------------------------------------------------------------------
/**
  Фильтрация объетов
*/
//---
bool Process3DEvent::FilterObjects(const ksapi::IKompasAPIObjectPtr & object)
{
  return m_obj.FilterProc(object);
}


//------------------------------------------------------------------------------
/**
  Измененилось положение
*/
//---
bool Process3DEvent::PlacementChanged(const ksapi::IKompasAPIObjectPtr & object)
{
  return m_obj.PlacementChanged(object);
}


//------------------------------------------------------------------------------
/**
  Окончание процесса
*/
//---
bool Process3DEvent::EndProcess()
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
  Запуск процесса
*/
//---
bool Process3DEvent::Run()
{
  return m_obj.ProcessRun();
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void Process3DEvent::AdviseEvents()
{
  if (!m_process)
    return;

  ksapi::IProcess3DEventsPtr events = m_process->Events();

  if (!events)
    return;

  events->AddExecuteCommandHandler(handlerOwner, [this](std::int32_t command) { return ExecuteCommand(command); });
  events->AddFilterObjectsHandler(handlerOwner,
                                  [this](const ksapi::IKompasAPIObjectPtr & object) { return FilterObjects(object); });
  events->AddPlacementChangedHandler(handlerOwner,
                                     [this](const ksapi::IKompasAPIObjectPtr & object) { return PlacementChanged(object); });
  events->AddEndProcessHandler(handlerOwner, [this]() { return EndProcess(); });
  events->AddRunHandler(handlerOwner, [this]() { return Run(); });
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void Process3DEvent::UnadviseEvents()
{
  if (!m_process)
    return;

  if (ksapi::IProcess3DEventsPtr events = m_process->Events())
    events->RemoveAllHandlers(handlerOwner);
}
