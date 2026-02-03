#pragma once

enum class ProcessState;

////////////////////////////////////////////////////////////////////////////////
//
/// Процесс создания линии выноски
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct ILeaderProcess
{
  /// Инициализация параметров процесса установки линии выноски
  virtual bool InitPosLeaderParam() = 0;
  /// Запуск процесса установки линии выноски
  virtual void InitLeaderProcess() = 0;
  /// Редактировать объект спецификации
  virtual bool EditSpcObject() = 0;
  /// Отрисовать позиционную линию выноски
  virtual void DrawPositionLeader() = 0;
  /// Создать линию выноски
  virtual bool CreateLeader(ProcessState processState) = 0;
};
