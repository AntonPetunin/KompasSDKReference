////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerObject - Класс для обслуживания панели свойств.
// PropertyManagerEvent  - Класс событий менеджера свойств.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include "BaseEvent.h"

#include <KsAPI.h>

////////////////////////////////////////////////////////////////////////////////
//
// Класс для обслуживания панели свойств.
//
// ctrlID используется как идентивикатор контрола и как идентификатор для загрузки
// ресурсов контрола
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  int m_flagMode;                          // Признак редактирования по дабл-клику.
  IProcessParamPtr m_procParam;            // Параметры процесса.
  IPropertyTabsPtr m_propTabs;             // Закладки.
  IPropertyControlsPtr m_curentCollection; // Контролы текущей закладки панели свойств.
  IPropertyGridPtr m_paramGrid;            // Грид для вывода дополнительных параметров элемента.
  IPropertySlideBoxPtr m_slideBox;         // Окно просмотра.
  int32_t m_rowIndex;                      // Индекс строки.
  IProcessParamPtr m_posLiaderParam;       // Параметры процесса указания линии выноски.

public:
  PropertyManagerObject();
  virtual ~PropertyManagerObject();

  // Панель свойств ##################################################################
  // События
  // Нажатие кнопки.
  virtual bool OnButtonClick(int32_t buttonID) = 0;
  // Нажатие кнопки контрола.
  virtual void OnControlCommand(int32_t ctrlId, int32_t buttonId) = 0;

  // Инициализация параметров процесса.
  virtual bool InitProcessParam();
  virtual int32_t GetFirstTabID() { return 0; }
  virtual SpecPropertyToolBarEnum GetSpecToolBarType() { return pnEnterEscHelp; }
  void EndProcess(); // Завершить процесс редактирования.

  // Добавление закладки.
  bool CreateTab(int32_t tabID, bool visible = true);

  // Добавление едита вещественных значений.
  IPropertyEditPtr CreateEditReal(double minVal = 0, double maxVal = 0);
  // Добавление комбобокса вещественных значений.
  IPropertyListPtr CreateList(double minVal = 0, double maxVal = 0);
  // Добавление комбобокса строковых значений.
  IPropertyListPtr CreateStringList(std::wstring matVal = L"");
  // Добавление сепаратора.
  IPropertySeparatorPtr CreateSeparator();
  // Добавление CheckBox-а.
  IPropertyCheckBoxPtr CreateCheckBox(bool checked);
  // Добавление SpinEdit.
  IPropertySpinEditPtr CreateSpinList(double minVal = 0, double maxVal = 0); // Голованов Е
  // Добавление группы кнопок.
  IPropertyMultiButtonPtr CreateMultiButton(ButtonTypeEnum type);

  // Получить указатель на Control по его идентификатору (для изменения параметров контрола).
  IPropertyControlPtr GetPropertyControl(int32_t ctrlID);

  // Добавление кнопки в группу.
  void AddButton(IPropertyMultiButtonPtr & buttons, int32_t btnID, bool cheched = false, bool enable = true);
  // Добавление кнопки в группу.
  void AddButton2(IPropertyMultiButtonPtr & buttons, int32_t btnID, int32_t bmpID, bool cheched = false, bool enable = true);

  // Изменить состояние контрола.
  void SetControlEnable(int32_t ctrlID, bool enabled = true);

  // Накачка контрола общими параметрами.
  void InitPropertyControl(IPropertyControlPtr & control, int32_t ctrlID, bool enable = true, bool visible = true);

  virtual void ShowControls() {}  // Виртуальная функция наполнения контролов элемента.
  virtual void RedrawPhantom() {} // Перерисовать фантом.

  virtual int ParamCount() { return 0; } // Количество параметров выводимых в гриде.
  virtual void ShowParam() {}            // Заполнение грида параметров.

  void AddStringToGrig(int32_t paramID, const std::wstring & value); // Добавление строкового значения.
  void AddDoubleToGrig(int32_t paramID, double value);               // Добавление числового  значения.
};


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обрабокчик событий от менеджера панели свойств.
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerEvent : public BaseEvent
{
protected:
  PropertyManagerObject & m_obj; // Указатель на подписанный объект.

public:
  PropertyManagerEvent(const IPropertyManagerPtr & manager, PropertyManagerObject & obj);
  virtual ~PropertyManagerEvent();

public:
  // Подписаться на события.
  void Advise();
  // Отписаться от событий.
  virtual void Unadvise() override;

  // Нажатие кнопки.
  bool ButtonClick(int32_t buttonId);
  // Нажатие кнопки контрола.
  void ControlCommand(const IPropertyControlPtr & control, int32_t buttonId);
  // Установка состояния кнопки спецпанели.
  bool ButtonUpdate(int32_t buttonId, bool & enable, int & checked);
  // Начало процесса.
  bool ProcessActivate();
  // Завершение процесса
  bool ProcessDeactivate();
  // Нажатие кнопки пользовательского меню.
  void UserMenuCommand(const IPropertyControlPtr & control, int32_t menuId);
};
