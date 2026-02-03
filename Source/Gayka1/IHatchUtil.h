#pragma once

////////////////////////////////////////////////////////////////////////////////
//
/// Функции штриховки
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
struct IHatchUtil
{
  /// Если объект поддерживает штриховку, то нужно вернуть true
  virtual bool IsHatchObject() const = 0;
  /// В текущем режиме штриховка есть
  virtual bool IsHatchEnable() const = 0;
  /// Получить угол штриховки
  virtual double GetHatchAngle() const = 0;
  /// Получить шаг штриховки
  virtual double GetHatchStep() const = 0;
  /// Установить угол штриховки
  virtual void SetHatchAngle(double hatch) = 0;
  /// Установить шаг штриховки
  virtual void SetHatchStep(double step) = 0;
};
