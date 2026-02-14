////////////////////////////////////////////////////////////////////////////////
///
/**
  \file 
  \brief Интерфейс параметров конвертации.
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <KsAPI.h>

/// Команды библиотечного конвертера.
enum LibConverterCommands
{
  libConv_Exp_m3d = 1,  ///< Экспорт в формат mym3d.
  libConv_Exp_a3d = 2,  ///< Экспорт в формат mya3d.
  libConv_Exp_frw = 3,  ///< Экспорт в формат myfrw.
  libConv_Exp_cdw = 4,  ///< Экспорт в формат mycdw.
  libConv_Exp_spw = 5,  ///< Экспорт в формат myspw.
  libConv_Exp_kdw = 6,  ///< Экспорт в формат mykdw.
  libConv_Exp_t3d = 7,  ///< Экспорт в формат mytrd.
  libConv_Imp_m3d = 8,  ///< Импорт из формата mym3d.
  libConv_Imp_a3d = 9,  ///< Импорт из формата mya3d.
  libConv_Imp_frw = 10, ///< Импорт из формата myfrw.
  libConv_Imp_cdw = 11, ///< Импорт из формата mycdw.
  libConv_Imp_spw = 12, ///< Импорт из формата myspw.
  libConv_Imp_kdw = 13, ///< Импорт из формата mykdw.
  libConv_Imp_t3d = 14  ///< Импорт из формата myt3d.
};


////////////////////////////////////////////////////////////////////////////////
//
/// Интерфейс библиотечных параметров конвертации.
/*
*/
////////////////////////////////////////////////////////////////////////////////
struct ILibConverterParam : ksapi::IAPIObject
{
  /// Получить признак сохранения стиля линий.
  virtual bool GetSaveLineStyle() = 0;
  /// Установить признак сохранения стиля линий.
  virtual void SetSaveLineStyle(bool newValue) = 0;
};

/// Сгенерированый идентификатор класса библиотечных параметров.
constexpr ksapi::IAPIObjectIID IID_ILibConverterParam{"129836E6-81A7-4EB8-AB20-FC3BEB983B80"};

/// Указатель на интерфейс. Поведение указателя аналогично поведению указателей KsAPI интерфейсов.
using ILibConverterParamPtr = ksapi::SmartPtr<ILibConverterParam, IID_ILibConverterParam>;
