////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Класс для работы с хот точками
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gayka1_pch.h"

#include <KompasLibraryActions.h>

class ParameterObject;
using namespace ksapi;

////////////////////////////////////////////////////////////////////////////////
//
/// Оболочка для работы с Hot-точками
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class LibPropObj : public ILibraryPropertiesObject
{
protected:
  ParameterObject * m_obj; ///< Подконтрольный элемент
  int count;               ///< Счетчик

public:
  /// Конструктор
  LibPropObj(ParameterObject * _obj);
  /// Деструктор
  virtual ~LibPropObj();

  void AddRef() override;
  void Release() override;
  IAPIObject * QueryInterface(IAPIObjectIID id) override;

  /// Имя группы объектов по умолчанию макроэлементы
  std::wstring GetGroupName() override;
  /// Добавить свойство в список свойств
  bool GetProperty(int32_t index, const IPropertyParamPtr & propertyParam) override;
  /// Обновить параметры свойства
  bool UpdateProperty(const IPropertyParamPtr & propertyParam) override;
  /// Изменить свойство
  int32_t ApplyProperty(const IPropertyParamPtr & propertyParam) override;
  /// Событие запуска внешнего редактирования для пользовательского свойсва /*ksOPControlExternalEdit*/
  bool OnChoiceProperty(const IPropertyParamPtr & propertyParam) override { return false; }
  /// Запрос параметров точек для визуального определения места применения параметра
  bool GetMouseEnterLeavePoint(int32_t propertyId, int32_t pointIndex, const IMouseEnterLeaveParametersPtr & parameters) override
  {
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
//
/// Оболочка для работы с Hot-точками
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class HPObj : public LibPropObj,
              public ILibraryHotPointsObject
{
public:
  /// Конструктор
  HPObj(ParameterObject * _obj);
  /// Деструктор
  ~HPObj();

  void AddRef() override;
  void Release() override;
  IAPIObject * QueryInterface(IAPIObjectIID id) override;

  /// Подготовиться к редактированию методом таскания за хот-точку
  virtual bool BeginDrag(int32_t index) override;
  /// Завершить редактирование методом таскания за хот-точку.
  virtual bool EndDrag(int32_t index, bool success) override;
  /// Получить текущее описание хот-точки.
  virtual bool Get(int32_t index, const IObject2DHotPointPtr & hotPoint) override;
  /// Изменилось положение хот-точки.
  virtual bool Set(int32_t index, double x, double y) override;
  /// Получить текст для отображения курсором.
  virtual std::wstring GetCursorText(int32_t index) override;
  /// Выполнить команду из меню, присланного в GetMenu().
  virtual bool ExecuteMenuCommand(int32_t id) override;
  /// Получить popup-меню, ассоциированое с данным библиотечным элементом, если index = -1,
  /// или ассоциированое с данной hot-точкой, если index > -1.
  virtual IMenuPtr GetMenu(int32_t index) override;
  /// Получить состояние команды из меню.
  virtual void GetMenuCommandState(int32_t commanId, bool & enable, bool & checked) override;
  /// Выбрали хот-точку
  virtual bool Select(int32_t index) override;
  /// Сняли выбор с хот-точки
  virtual bool Unselect(int32_t index) override;
};

/// Создает соответствующий объект для работы с Hot точками
HPObj * LibObjInterfaceEntry(IKompasAPIObjectPtr currentObject);
