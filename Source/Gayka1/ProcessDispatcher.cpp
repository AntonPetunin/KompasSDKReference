#include "ProcessDispatcher.h"
#include "BaseProcess.h"
#include "ParameterObject.h"
#include "ParameterObjectProcess.h"

extern ksapi::IApplication * application;

namespace
{
//------------------------------------------------------------------------------
/**
  Получить активный документ
*/
//---
ksapi::IKompasDocumentPtr GetActiveDocument()
{
  return application ? application->GetActiveDocument() : nullptr;
}
} // namespace


//------------------------------------------------------------------------------
/**
  Получить инициализированный процесс
*/
//---
BaseProcess * ProcessDispatcher::GetActive(ProcessType activeType)
{
  BaseProcess * process = nullptr;

  if (IKompasDocumentPtr doc = GetActiveDocument())
  {
    auto & documentProcesses = GetDocumentProcesses(*doc);

    if (documentProcesses.contains(activeType))
      process = documentProcesses.at(activeType);
  }

  return process;
}


//------------------------------------------------------------------------------
/**
  Добавить процесс создания объекта
*/
//---
BaseProcess * ProcessDispatcher::AddParameterObjectCreation(ParameterObject & parameterObj, unsigned int commandId)
{
  if (ParameterObjectProcess * process = CreateParameterObjectProcess(parameterObj))
  {
    if (IKompasDocumentPtr doc = GetActiveDocument())
    {
      auto & documentProcesses = GetDocumentProcesses(*doc);
      process->InitProcessParam(commandId + 4000, pnEnterEscHelp, commandId + 4000);
      process->InitPlacementProcess();
      documentProcesses.insert({ParameterObjectCreation, process});
      return process;
    }
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Добавить процесс создания спецификации с линией выноской
*/
//---
BaseProcess * ProcessDispatcher::AddLeaderForSpcCreation(ProcessState & processState, ParameterObject * processObject)
{
  if (LeaderBuildProcess * process = CreateLeaderProcess(processState, processObject))
  {
    if (IKompasDocumentPtr doc = GetActiveDocument())
    {
      auto & documentProcesses = GetDocumentProcesses(*doc);
      process->InitLeaderProcess();
      documentProcesses.insert({LeaderForSpcCreation, process});
      return process;
    }
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Запустить процесс активного документа
*/
//---
bool ProcessDispatcher::RunActive(ProcessType activeType, bool modal, bool postMessage)
{
  bool res = false;

  if (IKompasDocumentPtr doc = GetActiveDocument())
  {
    auto & documentProcesses = GetDocumentProcesses(*doc);

    if (documentProcesses.contains(activeType))
      res = documentProcesses.at(activeType)->Run(modal, postMessage);
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Приостановить процесс активного документа
*/
//---
void ProcessDispatcher::PauseActive(ProcessType activeType, const ksapi::IKompasDocumentPtr & document)
{
  if (IKompasDocumentPtr doc = document ? document : GetActiveDocument())
  {
    auto & documentProcesses = GetDocumentProcesses(*doc);

    if (documentProcesses.contains(activeType))
    {
      BaseProcess * process = documentProcesses.at(activeType);
      application->StopCurrentProcess(true);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Отписать процесс и удалить из коллекции активного документа
*/
//---
void ProcessDispatcher::ResetActive(ProcessType activeType, const ksapi::IKompasDocumentPtr & document)
{
  if (IKompasDocumentPtr doc = document ? document : GetActiveDocument())
  {
    auto & documentProcesses = GetDocumentProcesses(*doc);

    if (documentProcesses.contains(activeType))
    {
      BaseProcess * process = documentProcesses.at(activeType);
      process->UnadviseEvents();
      documentProcesses.erase(activeType);
    }
  }
}

//------------------------------------------------------------------------------
/**
  Отписать и удалить из коллекции все процессы документа
*/
//---
void ProcessDispatcher::ResetAll(ksapi::IKompasDocument * document)
{
  if (document)
  {
    auto & documentProcesses = GetDocumentProcesses(*document);

    for (const auto & process : documentProcesses)
    {
      if (process.second)
        process.second->UnadviseEvents();
    }

    m_activeProcesses.erase(document);
  }
}


//------------------------------------------------------------------------------
/**
  Получить процессы документа
*/
//---
DocumentProcesses & ProcessDispatcher::GetDocumentProcesses(ksapi::IKompasDocument & document)
{
  if (!m_activeProcesses.contains(&document))
    m_activeProcesses[&document] = {};

  return m_activeProcesses.at(&document);
}
