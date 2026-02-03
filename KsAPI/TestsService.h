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

#include <string>

namespace ksapi
{

////////////////////////////////////////////////////////////////////////////////
//
/// Сервис для прогона тестов.
/*
  
*/
////////////////////////////////////////////////////////////////////////////////
struct ITestsService
{
  virtual ~ITestsService() = default;
  virtual std::wstring GetTestResultDir() const = 0;
  virtual std::wstring GetApplicationDir(const std::wstring & appId) const = 0;
  virtual bool IsTestMode() const = 0;
};

} // namespace ksapi


//------------------------------------------------------------------------------
/**
  Сервис для прогона тестов.
*/
//---
KSAPI_FUNC(ksapi::ITestsService &) GetTestsService();
