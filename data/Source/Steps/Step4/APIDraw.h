////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Resource.h"
#include "Doc2DDispatcher.h"
#include "CPropMen.h"


////////////////////////////////////////////////////////////////////////////////
//
/// Класс для отрисовки фигур
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class APIDraw : public PropertyManagerObject
{
  /// Типы фигур
  enum class DrawType
  {
    DrawRect = ID_BUTTON1,
    DrawCircle = ID_BUTTON2,
    DrawTreangle = ID_BUTTON3,
  };

private:
  ksapi::IDrawingGroupPtr m_phantomGroup;      ///< Фантомная группа
  ksapi::IProcessPtr m_process;                ///< Процесс
  ksapi::IMacroObjectPtr m_macroObj;           ///< Макро-объект
  DrawType m_phantomType = DrawType::DrawRect; ///< Тип отрисовываемого фантома
  Doc2DDispatcher dispatcher;                  ///< Диспетчер 2D-документа

public:
  /// Построение геометрии
  virtual void GetGroup();
  /// Запуск процесса
  virtual void ChoicePlacement();
  /// Отрисовка фантома
  virtual void DrawPhantom(double x, double y, double angle, bool dynamic);
  /// Изменение фантома
  virtual void RedrawPhantom() override;
  /// Отображение кнопок спецпанели
  virtual void ShowControls() override;
  /// Изменение положения
  virtual bool PlacementChanged(double x, double y, double angle, bool dynamic) override;
  /// Завершение процесса
  virtual void EndProcess() override;
  /// Нажатие кнопки
  virtual bool ButtonClick(std::int32_t buttonID) override;
  /// Установка состояния кнопки спецпанели
  virtual bool ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) override;

  /// Обновить состав меню процесса
  void UpdateProcessMenu();
};
