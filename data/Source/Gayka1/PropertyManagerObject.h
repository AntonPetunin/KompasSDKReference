////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Содержит класс для работы с панелью свойств
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IHatchUtil.h"

/// Загрузить строку из ресурса
std::wstring LoadStr(int id);
/// Выдача библиотекой сообщения из ресурсов
int LibMessage(int strId, ksMessageTypeEnum dlgType = ksMessageWarning, ksMessageButtonSetEnum dlgButtons = ksButtonSetOk,
               bool showModal = false);
/// Выдача сообщения библиотекой
int LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType = ksMessageWarning,
               ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
/// Получение файла помощи
void OpenHelp(int Id);


////////////////////////////////////////////////////////////////////////////////
//
/// Класс для обслуживания панели свойств
/**
  ctrlID используется как идентивикатор контрола и как идентификатор для загрузки
  ресурсов контрола
*/
////////////////////////////////////////////////////////////////////////////////
class PropertyManagerObject : public IHatchUtil
{
protected:
  bool m_flagMode = false; ///< Признак редактирования по дабл-клику

public:
  /// Конструктор
  PropertyManagerObject() = default;
  /// Деструктор
  virtual ~PropertyManagerObject() = default;

  /// Получить признак редактирования по дабл-клику
  virtual bool GetFlagMode() { return m_flagMode; }
  /// Редактирование объекта спецификации
  virtual bool EditSpcObject() = 0;
  /// Процесс создания элемента
  virtual void Draw1(ksapi::IPhantom2D & phantom) = 0;
  /// Группа для отрисовки в окне
  virtual ksapi::IDrawingGroupPtr GetPreviewGroup() = 0;
  /// Получить номер команды
  virtual int GetCommand() = 0;
  /// Количество параметров выводимых в гриде
  virtual int ParamCount() { return 0; }

#pragma region IHatchUtil
  /// Получить угол штриховки
  virtual double GetHatchAngle() const override { return 0; }
  /// Установить угол штриховки
  virtual void SetHatchAngle(double hatch) override {}
  /// Получить шаг штриховки
  virtual double GetHatchStep() const override { return 0; }
  /// Установить шаг штриховки
  virtual void SetHatchStep(double step) override {}
  /// Если штриховка в одном из режимов есть, то нужно вернуть true
  virtual bool IsHatchEnable() const override { return false; }
  /// Если объект поддерживает штриховку, то нужно вернуть true
  virtual bool IsHatchObject() const override { return false; }
#pragma endregion
};
