////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <KsAPI.h>

/// Сообщение загружается из ресурсов
std::int32_t LibMessage(std::int32_t strId, ksMessageTypeEnum dlgType = ksMessageError,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
/// Сообщение передается строкой
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType = ksMessageError,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
/// Cообщение об ошибке
std::int32_t ErrorMessage(const std::wstring & message);
/// Диалог Да|Нет|Отмена
std::int32_t YesNoMessage(const std::wstring & message);


////////////////////////////////////////////////////////////////////////////////
//
/// Класс для обслуживания панели свойств
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  std::int32_t m_flagMode;                        ///< Признак редактирования по дабл-клику
  ksapi::IProcessParamPtr m_procParam;            ///< Параметры процесса
  ksapi::IPropertyTabsPtr m_propTabs;             ///< Закладки
  ksapi::IPropertyControlsPtr m_curentCollection; ///< Контролы текущей закладки панели свойств
  ksapi::IPropertyGridPtr m_paramGrid;            ///< Грид для вывода дополнительных параметров элемента
  ksapi::IPropertySlideBoxPtr m_slideBox;         ///< Окно просмотра
  std::int32_t m_rowIndex;                        ///< Индекс строки

public:
  PropertyManagerObject();
  virtual ~PropertyManagerObject();

#pragma region Обработчики событий
  /// Нажатие кнопки.
  virtual bool ButtonClick(std::int32_t buttonID) { return false; }
  /// Установка состояния кнопки спецпанели.
  virtual bool ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) { return false; }
  /// Изменение положения.
  virtual bool PlacementChanged(double x, double y, double angle, bool dynamic) { return false; }
  /// Завершить процесс редактирования
  virtual void EndProcess() {}
#pragma endregion

  /// Инициализация параметров процесса
  bool InitProcessParam();

protected:
  /// Добавление закладки
  bool CreateTab(std::int32_t tabID, bool visible = true, bool active = false);
  /// Добавление едита вещественных значений
  ksapi::IPropertyEditPtr CreateEditReal(double minVal = 0, double maxVal = 0);
  /// Добавление комбобокса вещественных значений
  ksapi::IPropertyListPtr CreateList(double minVal = 0, double maxVal = 0);
  /// Добавление CheckBox-а
  virtual ksapi::IPropertyCheckBoxPtr CreateCheckBox(bool checked);
  /// Добавление SpinEdit
  virtual ksapi::IPropertySpinEditPtr CreateSpinList(double minVal = 0, double maxVal = 0);
  /// Добавление контрола
  ksapi::IPropertyControlPtr CreateControl(ControlTypeEnum conType);
  /// Получить указатель на Control по его идентификатору (для изменения параметров контрола)
  virtual ksapi::IPropertyControlPtr GetPropertyControl(long ctrlID);
  /// Изменить состояние контрола.
  void SetControlEnable(long ctrlID, bool enabled = true);
  /// Виртуальная функция наполнения контролов элемента
  virtual void ShowControls() {}
  /// Перерисовать фантом
  virtual void RedrawPhantom() {}
  /// Количество параметров выводимых в гриде
  virtual std::int32_t ParamCount() { return 0; }
  /// Заполнение грида параметров
  virtual void ShowParam() {}
  /// Добавление строкового значения
  virtual void AddStringToGrig(std::int32_t paramID, const std::wstring & value);
  /// Добавление числового значения
  virtual void AddDoubleToGrig(std::int32_t paramID, double value);
};
