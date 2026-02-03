////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stack>
#include <string>
#include <KsAPI.h>

using namespace ksapi;

////////////////////////////////////////////////////////////////////////////////
//
/// Диспетчер активного документа
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class SketchDispatcher
{
private:
  ksapi::IKompasDocument2DPtr m_sketch;                 ///< Активный документ
  std::stack<ksapi::IDrawingContainerPtr> m_containers; ///< Контейнеры объектов
  std::stack<ksapi::IDrawingGroupPtr> m_groups;         ///< Группы

public:
  /// Конструктор
  SketchDispatcher();

  /// Установить активный документ
  void SetSketch(const ksapi::IKompasDocument2DPtr & sketch);
  /// Создать окружность по центру и радиусу
  IDrawingObjectPtr CreateCircle(double xc, double yc, double rad, unsigned short style);
  /// Создать группу
  IDrawingGroupPtr CreateDrawingGroup(bool tmpGroup);
  /// Добавить объект в группу
  void AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject);
  /// Открыть группу
  IDrawingGroupPtr OpenDrawingGroup(IDrawingGroupPtr & createdGroup);
  /// Очистить группу
  void DeleteDrawingGroup(ksapi::IDrawingGroupPtr & group);
  /// Закрыть редактирование группы
  IDrawingGroupPtr CloseGroup();
  /// Создать
  IDrawingObjectPtr CreateDiamDimension(double xc, double yc, double rad, std::int32_t arrowType, std::int32_t sign);

private:
  /// Установить активный контейнер объектов
  void SetDocumentDrawingContainer();
  /// Получить активный контейнер объектов
  IDrawingContainerPtr GetContainer();
};
