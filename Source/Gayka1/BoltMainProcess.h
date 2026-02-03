#pragma once
#include "ParameterObjectProcess.h"

class BoltMain;

////////////////////////////////////////////////////////////////////////////////
//
/// Процесс создания болта
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class BoltMainProcess : public ParameterObjectProcess
{
private:
  BoltMain & m_bolt; ///< Болт

public:
  /// Конструктор
  BoltMainProcess(BoltMain & bolt);

  /// Нажатие на кнопку
  virtual bool ButtonClick(std::int32_t buttonID) override;
  /// Вывод контролов
  virtual void ShowControls() override;
  /// Изменить значение контрола
  virtual bool ChangeControlValue(ksapi::IPropertyControl & ctrl) override;
  /// Заполнение грида параметров
  virtual void ShowParam() override;
};
