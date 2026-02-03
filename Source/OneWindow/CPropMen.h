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
#include <Events/OneWindowEvents.h>


/// Сообщение загружается из ресурсов
int LibMessage(int strId, ksMessageTypeEnum dlgType = ksMessageError, ksMessageButtonSetEnum buttonsType = ksButtonSetOk,
               bool showModal = false);
/// Сообщение передается строкой
int LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType = ksMessageError,
               ksMessageButtonSetEnum buttonsType = ksButtonSetOk, bool showModal = false);
/// Диалог да/нет/отмена
int YesNoMessage(const std::wstring & message, bool question = true);


////////////////////////////////////////////////////////////////////////////////
//
/// Класс для обслуживания панели свойств
/**
  ctrlID используется как идентификатор контрола и как идентификатор для загрузки
  ресурсов контрола
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject
{
protected:
  std::int32_t m_flagMode;                        ///< Признак редактирования по дабл-клику
  ksapi::IProcessPtr m_process;                   ///< Процесс
  ksapi::IProcessParamPtr m_procParam;            ///< Параметры процесса
  ksapi::IPropertyTabsPtr m_propTabs;             ///< Закладки
  ksapi::IPropertyControlsPtr m_curentCollection; ///< Контролы текущей закладки панели свойств

  size_t m_procPropMngEvent;  ///< Хеш событий менеджера свойств
  OWEventCollection m_events; ///< Коллекция событий

public:
  /// Конструктор
  PropertyManagerObject();
  /// Деструктор
  virtual ~PropertyManagerObject();

  /// Нажатие кнопки.
  virtual bool OnButtonClick(std::int32_t buttonID) { return true; }
  /// Нажатие кнопки контрола
  virtual bool OnControlCommand(std::int32_t ctrlID, std::int32_t buttonID) { return true; }
  /// Установка состояния кнопки спецпанели.
  virtual bool OnButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) { return true; }
  /// Инициализация параметров процесса
  virtual bool InitProcessParam(std::int32_t toolBarID, SpecPropertyToolBarEnum toolBarType, std::int32_t firstTabID);
  /// Получить идентификатор первой закладки
  virtual std::int32_t GetFirstTabID() { return 0; }
  /// Получить тип панели инструментов
  virtual SpecPropertyToolBarEnum GetSpecToolBarType() { return pnEnterEscHelp; }
  /// Завершить процесс редактирования
  virtual void EndProcess();
  /// Добавление закладки
  bool CreateTab(std::int32_t tabID, bool visible = true, bool active = false);
  /// Добавление едита вещественных значений
  ksapi::IPropertyEditPtr CreateEditReal(double minVal = 0, double maxVal = 0);
  /// Добавление комбобокса вещественных значений
  ksapi::IPropertyListPtr CreateList(double minVal = 0, double maxVal = 0);
  /// Добавление комбобокса строковых значений
  ksapi::IPropertyListPtr CreateStringList(std::wstring matVal = L"");
  /// Добавление сепаратора
  ksapi::IPropertySeparatorPtr CreateSeparator(SeparatorTypeEnum type);
  /// Добавление CheckBox-а
  ksapi::IPropertyCheckBoxPtr CreateCheckBox(bool checked);
  /// Добавление SpinEdit
  ksapi::IPropertySpinEditPtr CreateSpinList(double minVal = 0, double maxVal = 0); // Голованов Е
  /// Добавление группы кнопок
  ksapi::IPropertyMultiButtonPtr CreateMultiButton(ButtonTypeEnum type);
  /// Получить указатель на Control по его идентификатору (для изменения параметров контрола)
  ksapi::IPropertyControlPtr GetPropertyControl(int ctrlID);
  /// Добавление кнопки в группу
  void AddButton(const ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, bool cheched = false, bool enable = true);
  /// Добавление кнопки в группу
  void AddButton2(const ksapi::IPropertyMultiButtonPtr & buttons, std::int32_t btnID, std::int32_t bmpID, bool cheched = false,
                  bool enable = true);
  /// Изменить состояние контрола
  void SetControlEnable(std::int32_t ctrlID, bool enabled = true);
  /// Накачка контрола общими параметрами
  void InitPropertyControl(ksapi::IPropertyControl * control, std::int32_t ctrlID, bool enable = true,
                           PropertyControlNameVisibility nameVisibility = ksNameAlwaysVisible, bool visible = true);
};
