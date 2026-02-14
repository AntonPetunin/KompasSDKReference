////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "CursorProcess.h"
#include "Step3_KsAPI_2D_pch.h"

extern ksapi::IApplication * ksAppl;
std::wstring handlerOwner = L"Step3_KsAPI_2D_lib";


//------------------------------------------------------------------------------
/**
  Получить объект процесса
*/
//---
ksapi::IProcessPtr & CursorProcess::Get()
{
  if (!m_process)
    InitProcess();

  return m_process;
}


//------------------------------------------------------------------------------
/**
  Запустить процесс выполняющий функцию
*/
//---
void CursorProcess::Execute(PlacementChangedCall placementChanged)
{
  if (!m_process)
    InitProcess();

  if (m_process)
  {
    Advice();
    m_placementChanged = placementChanged;
    m_process->SetCritical(true);
    m_process->Run(true, false);
  }
}


//------------------------------------------------------------------------------
/**
  Инициализировать процесс
*/
//---
void CursorProcess::InitProcess()
{
  ksapi::IKompasDocument2DPtr doc = ksAppl ? ksAppl->GetActiveDocument() : nullptr;
  m_process = doc ? doc->GetLibProcess(ksProcess2DCursor) : nullptr;
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void CursorProcess::Advice()
{
  if (ksapi::IProcess2DPtr proc2d = m_process)
  {
    ksapi::IProcess2DEventsPtr events = proc2d->Events();

    if (!events)
      return;

    events->AddPlacementChangedHandler(handlerOwner, [this](double x, double y, double angle, bool dynamic)
                                       { return PlacementChanged(x, y, angle, dynamic); });
    events->AddEndProcessHandler(handlerOwner, [this]() { return EndProcess(); });
  }
}


//------------------------------------------------------------------------------
/**
  Обработчик события "Измененилось положение"
*/
//---
bool CursorProcess::PlacementChanged(double x, double y, double angle, bool dynamic)
{
  m_placementChanged(x, y);
  m_process->SetCritical(false);
  return false;
}


//------------------------------------------------------------------------------
/**
  Обработчик события "Окончание процесс"
*/
//---
bool CursorProcess::EndProcess()
{
  bool res = false;

  if (ksapi::IProcess2DPtr proc2d = m_process)
  {
    if (ksapi::IProcess2DEventsPtr events = proc2d->Events())
      events->RemoveAllHandlers(handlerOwner);

    res = true;
  }

  return res;
}
