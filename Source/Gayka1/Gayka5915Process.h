#pragma once
#include "ParameterObjectProcess.h"

class Gayka5915;

////////////////////////////////////////////////////////////////////////////////
//
/// Процесс создания гайки
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Gayka5915Process : public ParameterObjectProcess
{
private:
  Gayka5915 & m_gayka; ///< Гайка

public:
  /// Конструктор
  Gayka5915Process(Gayka5915 & gayka);

  /// Заполнение грида параметров
  void ShowParam();
  /// Нажатие на кнопку
  bool ButtonClick(std::int32_t buttonID);
  /// Вывод контролов
  void ShowControls();
  /// Изменить значение контрола
  bool ChangeControlValue(ksapi::IPropertyControl & ctrl);
};
