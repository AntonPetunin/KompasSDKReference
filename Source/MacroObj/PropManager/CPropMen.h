////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "..\MacroObj.h"

//-------------------------------------------------------------------------------
// Функции для выдачи сообщений библиотеки
// ---
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk,
                        bool showModal = false); // Сообщение загружается из ресурсов
std::int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk,
                        bool showModal = false); // Сообщение передается строкой


////////////////////////////////////////////////////////////////////////////////
//
// Класс для обслуживания панели свойств
//
// ctrlID используется как идентификатор контрола и как идентификатор для загрузки
// ресурсов контрола
//
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  std::int32_t m_flagMode;    ///< Признак редактирования по дабл-клику
  std::int32_t m_rowIndex;    ///< Индекс строки
  std::int32_t m_objectIndex; ///< Индекс объекта

protected:
  ksapi::IProcessParamPtr m_procParam;            ///< Параметры процесса
  ksapi::IPropertyTabsPtr m_propTabs;             ///< Закладки
  ksapi::IPropertyControlsPtr m_curentCollection; ///< Контролы текущей закладки панели свойств
  ksapi::IPropertyGridPtr m_paramGrid;            ///< Грид для вывода дополнительных параметров элемента

public:
  PropertyManagerObject();
  virtual ~PropertyManagerObject();

  // Панель свойств ##################################################################
  // События
  // prButtonClick - Нажатие кнопки.
  virtual bool ButtonClick(std::int32_t buttonID) { return false; }
  // prChangeControlValue - Изменение значения контрола
  virtual bool OnChangeControlValue(std::int32_t ctrlID, double newVal) { return true; }
  // prControlCommand Нажатие кнопки контрола
  virtual bool ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID) { return false; }
  // prButtonUpdate        - Установка состояния кнопки спецпанели.
  virtual bool ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) { return false; }
  // Накачка контекстной панели параметрами
  virtual bool FillContextPanel(const ksapi::IProcessContextPanelPtr & contextPanel) { return false; }
  // Получение типа контекстного меню
  virtual ksProcessContextMenuType GetContextMenuType(std::int32_t screenX, std::int32_t screenY)
  {
    return ksProcessDefaultContextMenu;
  };

  // prProcessRun    - Начало процесса
  virtual bool ProcessRun() { return false; }
  // ksPlacementChanged     Изменение положения.
  virtual bool PlacementChanged(const ksapi::IKompasAPIObjectPtr & object) { return false; }
  // ksFilterProc         - Фильтрация объектов
  virtual bool FilterProc(const ksapi::IKompasAPIObjectPtr & object) { return false; }
  // Завершить процесс редактирования
  virtual void EndProcess();
  // Инициализация параметров процесса
  bool InitProcessParam();

protected:
  virtual std::int32_t GetFirstTabID() { return 0; }
  virtual SpecPropertyToolBarEnum GetSpecToolBarType() { return pnEnterEscHelp; }

  // Добавление закладки
  bool CreateTab(std::int32_t tabID, bool visible = true, bool active = false);

  // Добавление едита вещественных значений
  virtual ksapi::IPropertyEditPtr CreateEditReal(double minVal = 0, double maxVal = 0);
  // Добавление комбобокса вещественных значений
  virtual ksapi::IPropertyListPtr CreateList(double minVal = 0, double maxVal = 0);
  // Добавление CheckBox-а
  virtual ksapi::IPropertyCheckBoxPtr CreateCheckBox(bool checked);
  // Добавление SpinEdit
  virtual ksapi::IPropertySpinEditPtr CreateSpinList(double minVal = 0, double maxVal = 0); // Голованов Е
                                                                                            // Добавление контрола
  virtual ksapi::IPropertyControlPtr CreateControl(ControlTypeEnum conType);

  // Получить указатель на Control по его идентификатору (для изменения параметров контрола)
  virtual ksapi::IPropertyControlPtr GetPropertyControl(std::int32_t ctrlID);

  // Изменить состояние контрола.
  void SetControlEnable(std::int32_t ctrlID, bool enabled = true);

  // Накачка контрола общими параметрами
  void InitPropertyControl(const ksapi::IPropertyControlPtr & control, std::int32_t ctrlID, bool enable = true,
                           bool visible = true);

  virtual void ShowControls() {}  // Виртуальная функция наполнения контролов элемента
  virtual void RedrawPhantom() {} // Перерисовать фантом

  virtual std::int32_t ParamCount() { return 0; } // Количество параметров выводимых в гриде
  virtual void ShowParam() {}                     // Заполнение грида параметров

  virtual void AddStringToGrig(std::int32_t paramID, const std::wstring & value); // Добавление строкового значения
  virtual void AddDoubleToGrig(std::int32_t paramID, double value);               // Добавление числового  значения
};
