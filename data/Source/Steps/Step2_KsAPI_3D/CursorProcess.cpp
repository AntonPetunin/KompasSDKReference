////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "CursorProcess.h"
#include <Step2_KsAPI_3D_pch.h>

extern ksapi::IApplication * kompasApp;
const std::wstring eventHandler = L"Step2_KsAPI_3D";

//------------------------------------------------------------------------------
/**
  Получить процесс.
*/
//---
ksapi::IProcessPtr CursorProcess::GetProcess()
{
  if (!m_process)
    InitProcess();

  return m_process;
}

//------------------------------------------------------------------------------
/**
  Запустить процесс выполняющий функцию.
*/
//---
void CursorProcess::Execute(PlacementChangedCall placementChanged)
{
  if (!m_process)
    InitProcess();

  Advice();

  m_placementChanged = placementChanged;

  m_process->SetCritical(true);
  m_process->Run(true, false);
}

//------------------------------------------------------------------------------
/**
  Инициализировать процесс.
*/
//---
void CursorProcess::InitProcess()
{
  ksapi::IKompasDocument3DPtr doc = kompasApp ? kompasApp->GetActiveDocument() : nullptr;
  m_process = doc ? doc->GetLibProcess(ksProcess3DPlacementAndEntity) : nullptr;
}

//------------------------------------------------------------------------------
/**
  Подписаться на события.
*/
//---
void CursorProcess::Advice()
{
  if (ksapi::IProcess3DPtr proc3d = m_process)
  {
    ksapi::IProcess3DEventsPtr events = proc3d->Events();

    if (!events)
      return;

    events->AddPlacementChangedHandler(eventHandler,
                                       [this](const ksapi::IKompasAPIObjectPtr & object) { return PlacementChanged(object); });
    events->AddEndProcessHandler(eventHandler, [this]() { return EndProcess(); });
  }
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Измененилось положение".
*/
//---
bool CursorProcess::PlacementChanged(const ksapi::IKompasAPIObjectPtr & object)
{
  if (m_placementChanged(object))
  {
    m_process->SetCritical(false);
    m_process->Stop();
  }
  return false;
}

//------------------------------------------------------------------------------
/**
  Обработчик события "Окончание процесс".
*/
//---
bool CursorProcess::EndProcess()
{
  bool res = false;

  if (ksapi::IProcess3DPtr proc3d = m_process)
  {
    if (ksapi::IProcess3DEventsPtr events = proc3d->Events())
      events->RemoveAllHandlers(eventHandler);

    res = true;
  }

  return res;
}
