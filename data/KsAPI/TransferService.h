////////////////////////////////////////////////////////////////////////////////
//
/**
  \file  
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "KsAPI_def.h" // pragma: skip include style check

#include <cstdint>

#include "KsAPI_fwd.h" // pragma: skip include style check

class MbRefItem;
class MbAttributeContainer;
class MbName;

namespace ksapi
{

////////////////////////////////////////////////////////////////////////////////
//
/// Сервис для преобразования между интерфейсами.
/*
  
*/
////////////////////////////////////////////////////////////////////////////////
struct ITransferService
{
  virtual ~ITransferService() = default;
  /// Получить reference по объекту KsAPI для связи с API7/API5/LibTool.
  virtual int32_t GetObjectReference(IAPIObjectPtr apiObject) const = 0;
  /// Получить  объект KsAPI по reference для связи с API7/API5/LibTool.
  virtual IAPIObjectPtr TransferReference(int32_t objectReference, int32_t documentReference) = 0;
  /// Связать объект API с новым математическим объектом.
  virtual bool SetMathToAPI(IAPIObjectPtr & object, const MbRefItem * mathObj, int32_t iObj) = 0;
  /// Получить указатель на математический объект из объекта API.
  virtual MbRefItem * GetMathFromAPI(const IAPIObjectPtr & object, int32_t iObj) const = 0;
  /// Получить указатель на полигональный объект из объекта API.
  virtual MbRefItem * GetMeshFromAPI(const IAPIObjectPtr & object, int32_t iObj) const = 0;
  /// Получить контейнер атрибутов из объекта API.
  virtual bool GetAttributesFromAPI(const IAPIObjectPtr & object, MbAttributeContainer & attributes) = 0;
  /// Установить контейнер атрибутов объекту API.
  virtual bool SetAttributesToAPI(const IAPIObjectPtr & object, const MbAttributeContainer & attributes) = 0;
  /// Получить MbName из объекта API.
  virtual void GetNameFromApi(const IAPIObjectPtr & object, MbName & name) = 0;
};

} // namespace ksapi


//------------------------------------------------------------------------------
/**
  Сервис для преобразования между интерфейсами.
*/
//---
KSAPI_FUNC(ksapi::ITransferService &) GetTransferService();
