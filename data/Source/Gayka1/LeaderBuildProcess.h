#pragma once
#include "BaseProcess.h"
#include "ILeaderProcess.h"

class ParameterObject;
class ParameterObjectProcess;
enum class ProcessState;

using RunBaseProcessCallBack = std::function<bool(bool modal, bool postMessage)>;

////////////////////////////////////////////////////////////////////////////////
//
/// Подпроцесс создания линии выноски
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class LeaderBuildProcess final : public BaseProcess,
                                 public ILeaderProcess
{
private:
  ksapi::IProcess2DPtr m_leaderProcess2D;        ///< Процесс построения линии выноски
  ksapi::IProcessParamPtr m_positionLeaderParam; ///< Параметры процесса указания линии выноски

  ProcessState & m_processState;        ///< Признак запуска процесса Placement
  ParameterObject * m_leaderBaseObject; ///< Объект

  RunBaseProcessCallBack m_baseProcessRunFunc;

public:
  /// Конструктор
  LeaderBuildProcess(ProcessState & processState, ParameterObject * leaderBaseObj);
  /// Конструктор
  LeaderBuildProcess(const LeaderBuildProcess & other) = delete;
  /// Деструктор
  ~LeaderBuildProcess();

  /// Получить базовый объект процесса
  virtual ParameterObject * GetProcessObject() override;
  /// Установить функцию запуска базового процесса
  virtual void SetRunProcessCallBack(RunBaseProcessCallBack callback);

#pragma region ILeaderProcess
  /// Инициализация параметров процесса установки линии выноски
  virtual bool InitPosLeaderParam() override;
  /// Инициализация параметров процесса установки линии выноски
  virtual void InitLeaderProcess() override;
  /// Запустить процесс создания объекта спецификации
  virtual bool EditSpcObject() override;
  /// Отрисовать позиционную линию выноски
  virtual void DrawPositionLeader() override;
  /// Создать линию выноски
  virtual bool CreateLeader(ProcessState processState) override;
#pragma endregion

#pragma region HandlerFunctions
  /// Нажатие на кнопку
  virtual bool ButtonClick(std::int32_t buttonID) override;
  /// Изменено положения объекта
  virtual bool PlacementChanged(double x, double y, double angle, bool dynamic) override;
  /// Завершение процесса
  virtual bool EndProcess() override;
#pragma endregion

#pragma region BaseProcess
  /// Запустить процесс
  virtual bool Run(bool modal, bool postMessage) override;
  /// Отписаться от событий
  virtual void UnadviseEvents() override;
  /// Вывод контролов
  virtual void ShowControls() override;
#pragma endregion
};

/// Создать подпроцесс линии выноски
LeaderBuildProcess * CreateLeaderProcess(ProcessState & m_processState, ParameterObject * m_processObject);
