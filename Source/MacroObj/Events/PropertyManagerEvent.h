////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Объявление класса обработчика событий от менеджера панели свойств
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class PropertyManagerObject;

////////////////////////////////////////////////////////////////////////////////
//
/// Обработчик событий от менеджера панели свойств
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent
{
private:
  PropertyManagerObject & m_obj;            ///< Указатель на подписанный объект
  ksapi::IProcessParamPtr & m_processParam; ///< Параметры процесса

public:
  /// Конструктор
  PropertyManagerEvent(ksapi::IProcessParamPtr & processParam, PropertyManagerObject & obj);
  /// Деструктор
  virtual ~PropertyManagerEvent();

private:
  /// Нажатие кнопки спецпанели
  bool ButtonClick(std::int32_t buttonID);
  /// Изменение значения контрола
  bool ChangeControlValue(const ksapi::IPropertyControlPtr & ctrl);
  /// Нажатие кнопки контрола
  bool ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID);
  /// Установка состояния кнопки спецпанели
  bool ButtonUpdate(std::int32_t buttonId, bool & enable, std::int32_t & checked);
  /// CALLBACK для накачки контекстной панели
  bool FillContextPanel(const ksapi::IProcessContextPanelPtr & contextPanel);
  /// CALLBACK для получения типа контекстного меню
  ksProcessContextMenuType GetContextMenuType(std::int32_t screenX, std::int32_t screenY);

private:
  /// Подписаться на события
  void AdviseEvents();
};
