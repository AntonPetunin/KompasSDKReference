////////////////////////////////////////////////////////////////////////////////
//
/**
  \file  
  \brief Сервисные функции
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "KsAPI_def.h" // pragma: skip include style check

#include <cstdint>
#include <string>

#include "KsAPI_fwd.h" // pragma: skip include style check


namespace ksapi
{

/// Флаги проецирования.
enum ksElementsProjectionFlagsEnum : uint32_t
{
  ksElementsProjectionDismantled = 0x0000001,                ///< Проецировать сборку в разнесенном виде.
  ksElementsProjectionBodies = 0x0000002,                    ///< Проецировать тела.
  ksElementsProjectionSurfaces = 0x0000004,                  ///< Проецировать поверхности.
  ksElementsProjectionThreads = 0x0000008,                   ///< Проецировать обозначения резьбы.
  ksElementsProjectionUnfold = 0x0000010,                    ///< Проецировать в развертке.
  ksElementsProjectionWithoutHiddenComponents = 0x0000020,   ///< Не проецировать скрытые элементы.
  ksElementsProjectionWithoutStandardComponents = 0x0000040, ///< Не проецировать стандартные элементы.
  ksElementsProjectionDimensions = 0x0000080,                ///< Проецировать справочные размеры.
  ksElementsProjectionRoughs = 0x0000100,                    ///< Проецировать обозначение шероховатости.
  ksElementsProjectionLeaders = 0x0000200,                   ///< Проецировать линию выноски.
  ksElementsProjectionTolerances = 0x0000400,                ///< Проецировать допуск формы.
  ksElementsProjectionPositionLeaders = 0x0000800,           ///< Проецировать обозначение позиции.
  ksElementsProjectionBases = 0x0001000,                     ///< Проецировать обозначение базы.
  ksElementsProjectionMarkLeaders = 0x0002000,               ///< Проецировать обозначение маркировки.
  ksElementsProjectionBrandLeaders = 0x0004000,              ///< Проецировать обозначение клеймения.
  ksElementsProjectionWithHiddenBodies = 0x0008000,          ///< Проецировать со скрытыми телами.
  ksElementsProjectionCurves = 0x0010000,                    ///< Проецировать кривые.
  ksElementsProjectionPoints = 0x0020000,                    ///< Проецировать точки.
  ksElementsProjectionConsiderLayers = 0x0040000,            ///< Учитывать слои.
  ksElementsProjectionSketches = 0x0080000,                  ///< Проецировать эскизы.
  ksElementsProjectionAxes = 0x0100000,                      ///< Проецировать оси.
  ksElementsProjectionCreateAxes = 0x0200000,                ///< Создавать оси.
  ksElementsProjectionCreateCircularCentres = 0x0400000,     ///< Создавать круговые сетки центров.
  ksElementsProjectionCreateLinearCentres = 0x0800000,       ///< Создавать линейные сетки центров.
  ksElementsProjectionCreateCentresMarkers = 0x1000000,      ///< Создавать обозначения центров.
};

/// Флаги отрисовки проекционных линий.
enum ksProjectionLinesDrawingFlagsEnum : uint32_t
{
  ksProjectionLinesDrawingHiddenLinesProjection = 0x0001, ///< Проецировать невидимые линии.
  ksProjectionLinesDrawingHiddenLinesShow = 0x0002,       ///< Отрисовывать невидимые линии.
  ksProjectionLinesDrawingTangentEdgesShow = 0x0004,      ///< Отрисовывать линии перехода.
  ksProjectionLinesDrawingBendLinesShow = 0x0008,         ///< Отрисовывать линии сгиба.
  ksProjectionLinesDrawingCenterLinesShow = 0x0010,       ///< Отрисовывать осевые линии.
  ksProjectionLinesDrawingUseOcclusion = 0x0020,          ///< Черновое проецирование.
};


////////////////////////////////////////////////////////////////////////////////
//
/// Сервисные функции.
/*
  
*/
////////////////////////////////////////////////////////////////////////////////
struct IServiceFunctions
{
  virtual ~IServiceFunctions() = default;
  /// Создать проекцию модели.
  virtual IDrawingGroupPtr GetModelProjectionAsDrawingObjects(
    const std::vector<double> & projPlacement, const std::wstring & document3DFileName,
    const IKompasDocument2DPtr & targetDocument2D, const std::wstring & markEmbodyment,
    ksElementsProjectionFlagsEnum elemProjFlags, ksProjectionLinesDrawingFlagsEnum projLinesDrawFlags, int32_t visibleLinesStyle,
    int32_t hiddenLinesStyle, int32_t transitionLinesStyle, int32_t bendLinesStyle) = 0;
};

} // namespace ksapi


//------------------------------------------------------------------------------
/**
  Сервисные функции.
*/
//---
KSAPI_FUNC(ksapi::IServiceFunctions &) GetServiceFunctions();
