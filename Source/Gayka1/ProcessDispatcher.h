////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class BaseProcess;
class ParameterObject;
enum class ProcessState;

/// Типы используемых процессов
enum ProcessType
{
  ParameterObjectCreation,
  LeaderForSpcCreation
};


using DocumentProcesses = std::unordered_map<ProcessType, BaseProcess *>;
using ActiveProcesses = std::unordered_map<ksapi::IKompasDocument *, DocumentProcesses>;


////////////////////////////////////////////////////////////////////////////////
//
/// Контроллер управления процессами
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class ProcessDispatcher
{
private:
  ActiveProcesses m_activeProcesses; ///< Текущие процессы (запущенные или только инициализированные)

public:
  /// Конструктор
  ProcessDispatcher() = default;

  /// Получить инициализированный процесс
  virtual BaseProcess * GetActive(ProcessType activeType);
  /// Добавить процесс создания объекта
  virtual BaseProcess * AddParameterObjectCreation(ParameterObject & parameterObj, unsigned int commandId);
  /// Добавить процесс создания спецификации с линией выноской
  virtual BaseProcess * AddLeaderForSpcCreation(ProcessState & processState, ParameterObject * processObject);
  /// Запустить процесс активного документа
  virtual bool RunActive(ProcessType activeType, bool modal, bool postMessage);
  /// Приостановить процесс документа
  virtual void PauseActive(ProcessType activeType, const ksapi::IKompasDocumentPtr & document = nullptr);
  /// Отписать процесс и удалить из коллекции документа
  virtual void ResetActive(ProcessType activeType, const ksapi::IKompasDocumentPtr & document = nullptr);
  /// Отписать и удалить из коллекции все процессы документа
  virtual void ResetAll(ksapi::IKompasDocument * document);

private:
  /// Получить процессы документа
  DocumentProcesses & GetDocumentProcesses(ksapi::IKompasDocument & document);
};
