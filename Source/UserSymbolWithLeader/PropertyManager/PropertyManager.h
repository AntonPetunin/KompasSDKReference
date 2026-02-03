////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerObject - Класс для обслуживания панели свойств
// PropertyManagerEvent  - Класс событий менеджера свойств
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include <Events/ABaseEvent.h>


//----------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
// Класс для обслуживания панели свойств
//
// controlId используется как идентификатор контрола и как идентификатор для загрузки
// ресурсов контрола
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  ksapi::IProcess2DPtr m_process;      ///< Процесс
  ksapi::IProcessParamPtr m_procParam; ///< Параметры процесса
  int32_t m_rowIndex = 0;              ///< Индекс строки
  ksapi::IKompasDocument2DPtr m_doc;

public:
  PropertyManagerObject();
  virtual ~PropertyManagerObject() = default;

  // Панель свойств ##################################################################
  // События

  // Нажатие кнопки.
  virtual bool OnButtonClick(int32_t buttonId) { return true; }
  // Изменение значения контрола.
  virtual bool OnChangeControlValue(const IPropertyControlPtr & control) { return true; }
  // Нажатие кнопки контрола.
  virtual bool OnControlCommand(const IPropertyControlPtr & control, int32_t buttonId) { return true; }
  // Установка состояния кнопки спецпанели.
  virtual bool OnButtonUpdate(int32_t buttonId, bool & enable, int & checked) { return true; }
  // Вызов справки
  virtual bool OnCommandHelp(int32_t helpId) { return 0; };
  // Установка/снятие фокуса на поле ввода.
  virtual void OnEditFocus(const IPropertyControlPtr & control, bool setFocus) {}
  // Селектирование объектов в списке.
  virtual void OnSelectItem(const IPropertyControlPtr & control, int32_t index, bool select) {}
  // Получить тип текущего меню.
  virtual ksProcessContextMenuType GetContextMenuType(int32_t screenX, int32_t screenY) { return ksProcessDefaultContextMenu; }
  virtual void FillContextPanel(const IProcessContextPanelPtr & contextPanel) {}

  virtual void OnProcItemDblClick(const IPropertyControlPtr & control, int32_t itemIndex) {}
  virtual void OnCheckItem(const IPropertyControlPtr & control, int32_t index, bool check) {}
  virtual void ProcessActivate() {}

  virtual void RedrawPhantom() {} // Перерисовать фантом
  virtual bool Placement2DChange(double x, double y, double angle, bool dynamic) { return true; }
  virtual bool GetMouseEnterLeavePoint(const IPropertyControlPtr & control, int32_t buttonId, int32_t pointIndex,
                                       IMouseEnterLeaveParametersPtr & parameters)
  {
    return false;
  }

protected:
  // Инициализация параметров процесса
  virtual bool InitProcessParam();

  virtual void ShowControls() {} // Виртуальная функция наполнения контролов элемента

  virtual int32_t ParamCount() { return 0; } // Количество параметров, выводимых в гриде
  virtual void ShowParam() {}                // Заполнение грида параметров
};


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обработчик событий от менеджера панели свойств
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent : public ABaseEvent
{
protected:
  PropertyManagerObject & m_obj; // Указатель на подписанный объект

public:
  PropertyManagerEvent(const ksapi::IProcessParamPtr & manager, PropertyManagerObject & obj);
  PropertyManagerEvent(const ksapi::IPropertyManagerPtr & manager, PropertyManagerObject & obj);
  virtual ~PropertyManagerEvent();

  void Advise();                    ///< Подписаться на события
  virtual void Unadvise() override; ///< Отписаться от событий

public:
  // prButtonClick        - Нажатие кнопки.
  bool ButtonClick(int32_t buttonId);
  // prChangeControlValue - Событие изменения значения контрола.
  void ChangeControlValue(const ksapi::IPropertyControlPtr & control);
  // prControlCommand     - Нажатие кнопки контрола.
  void ControlCommand(const ksapi::IPropertyControlPtr & control, int32_t buttonId);
  // prButtonUpdate       - Обновление состояния кнопки.
  bool ButtonUpdate(int32_t buttonId, bool & enable, int & checked);
  // prProcessActivate    - Активизация процесса.
  void ProcessActivate();
  // prProcessDeactivate  - Деактивизация процесса.
  void ProcessDeactivate();
  //
  void EditFocus(const IPropertyControlPtr & control, bool setFocus);
  //
  void SelectItem(const IPropertyControlPtr & control, int32_t index, bool select);
  //
  void UserMenuCommand(const IPropertyControlPtr & control, int32_t menuId);
  // CLLBACK для получения типа контекстного меню
  ksProcessContextMenuType GetContextMenuType(int32_t screenX, int32_t screenY);
  // "CLLBACK для накачки контекстной панели
  void FillContextPanel(const IProcessContextPanelPtr & contextPanel);
  //
  void DoubleClick(const IPropertyControlPtr & control, int32_t itemIndex);
  /// Обработчик "Выбор элемента".
  void CheckItem(const IPropertyControlPtr & control, int32_t index, bool check);
};
