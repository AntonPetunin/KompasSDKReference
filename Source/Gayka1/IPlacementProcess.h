#pragma once

////////////////////////////////////////////////////////////////////////////////
//
/// Процесс установки объекта
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct IPlacementProcess
{
  /// Инициализация параметров процесса
  virtual bool InitProcessParam(long toolBarID, SpecPropertyToolBarEnum toolBarType, long firstTabID = 0) = 0;
  /// Запуск процесса установки объекта
  virtual void InitPlacementProcess() = 0;
  /// Получить фантом
  virtual ksapi::IPhantom2DPtr GetPhantom2D() = 0;
};
