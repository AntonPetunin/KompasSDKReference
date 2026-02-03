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
#include <string>
#include <variant>
#include <vector>

namespace ksapi
{
using WindowHandle = void *;
using ResourceModule = void *;

/// Цвет в формате RGB/RGBA (R - младший байт, например 0x00FF0000 - синий цвет).
using Color = uint32_t;
using NumericId = uint64_t;

////////////////////////////////////////////////////////////////////////////////
//
/// Значение свойста.
/*
  
*/
////////////////////////////////////////////////////////////////////////////////
class KSAPI_CLASS PropertyValue final
{
public:
  /// Тип значения свойства.
  enum PropertyValueType
  {
    prop_empty,
    prop_bool,
    prop_int,
    prop_double,
    prop_string
  };

  /// Конструктор.
  PropertyValue();
  /// Деструктор.
  ~PropertyValue();

  /// Конструктор копирования.
  PropertyValue(const PropertyValue & other);
  /// Конструктор перемещения.
  PropertyValue(PropertyValue && other) noexcept;

  /// Оператор копирования.
  PropertyValue & operator=(const PropertyValue &);
  /// Оператор перемещения.
  PropertyValue & operator=(PropertyValue &&) noexcept;

  /// Получить тип свойства.
  PropertyValueType GetValueType() const;

  /// Конструктор от булева значения.
  PropertyValue(bool value);
  /// Установить булево значение.
  void SetBool(bool value);
  /// Получить булево значение.
  bool GetBool() const;
  /// Привести текущее значение к булеву.
  bool ToBool() const;

  /// Конструктор от целочисленного значения.
  PropertyValue(int32_t value);
  /// Установить целочисленное значение.
  void SetInt(int32_t value);
  /// Получить целочисленное значение.
  int32_t GetInt() const;
  /// Привести текущее значение к целочисленному.
  int32_t ToInt() const;

  /// Конструктор от вещественного значения.
  PropertyValue(double value);
  /// Установить вещественное значение.
  void SetDouble(double value);
  /// Получить вещественное значение.
  double GetDouble() const;
  /// Привести текущее значение к вещественному.
  double ToDouble() const;

  /// Конструктор от строки.
  PropertyValue(const std::wstring & value);
  /// Конструктор от указателя на строку.
  PropertyValue(const wchar_t * value);
  /// Установить строковое значение.
  void SetString(const std::wstring & value);
  /// Получить строковое значение.
  std::wstring GetString() const;
  /// Привести текущее значение к строковому.
  std::wstring ToString() const;

private:
  struct Private;
  Private * m_d;
};


////////////////////////////////////////////////////////////////////////////////
//
/// Значение атрибута.
/*
  
*/
////////////////////////////////////////////////////////////////////////////////
class KSAPI_CLASS AttributeValue final
{
public:
  /// Тип значения атрибута.
  enum AttributeValueType
  {
    Empty,    ///< Пустой атрибут.
    Value,    ///< Атрибут представлен одним значением.
    ValueList ///< Атрибут представлен набором значений.
  };

  /// Конструктор.
  AttributeValue();
  /// Деструктор.
  ~AttributeValue();

  /// Конструктор копирования.
  AttributeValue(const AttributeValue & other);
  /// Конструктор перемещения.
  AttributeValue(AttributeValue && other) noexcept;

  /// Оператор копирования.
  AttributeValue & operator=(const AttributeValue &);
  /// Оператор перемещения.
  AttributeValue & operator=(AttributeValue &&) noexcept;

  /// Оператор присваивания одного значения.
  AttributeValue & operator=(const PropertyValue & value);
  /// Оператор присваивания набора значений.
  AttributeValue & operator=(const std::vector<PropertyValue> & values);

  /// Получить тип значения.
  AttributeValueType GetValueType() const;

  /// Конструктор от одного значения.
  AttributeValue(const PropertyValue & value);
  /// Установить значение.
  void SetProperty(const PropertyValue & value);
  /// Получить значение.
  PropertyValue GetProperty() const;

  /// Конструктор от набора значений.
  AttributeValue(const std::vector<PropertyValue> & values);
  /// Установить набор значений.
  void SetPropertyList(const std::vector<PropertyValue> & values);
  /// Получить набор значений.
  std::vector<PropertyValue> GetPropertyList() const;

private:
  struct Private;
  Private * m_d = nullptr;
};


////////////////////////////////////////////////////////////////////////////////
//
/// Тип данных пользовательского хранилища.
/*
    
*/
////////////////////////////////////////////////////////////////////////////////
class KSAPI_CLASS StorageValue final
{
public:
  /// Типы для пользовательского хранилища.
  enum StorageValueType
  {
    Empty,  ///< VT_EMPTY, VT_NULL
    Int8,   ///< VT_I1
    Uint8,  ///< VT_UI1
    Int16,  ///< VT_I2
    Uint16, ///< VT_UI2
    Int32,  ///< VT_INT
    Uint32, ///< VT_UINT
    Long,   ///< VT_I4
    Ulong,  ///< VT_UI4
    Int64,  ///< VT_I8
    Uint64, ///< VT_UI8
    Float,  ///< VT_R4
    Double, ///< VT_R8
    Bool,   ///< VT_BOOL
    String, ///< VT_BSTR
    Date,   ///< VT_DATE

    VectorInt8,     ///< VT_I1 | VT_ARRAY
    VectorUint8,    ///< VT_UI1 | VT_ARRAY
    VectorInt16,    ///< VT_I2 | VT_ARRAY
    VectorUint16,   ///< VT_UI2 | VT_ARRAY
    VectorInt32,    ///< VT_INT | VT_ARRAY
    VectorUint32,   ///< VT_UINT | VT_ARRAY
    VectorLong,     ///< VT_I4 | VT_ARRAY
    VectorUlong,    ///< VT_UI4 | VT_ARRAY
    VectorInt64,    ///< VT_I8 | VT_ARRAY
    VectorUint64,   ///< VT_UI8 | VT_ARRAY
    VectorFloat,    ///< VT_R4 | VT_ARRAY
    VectorDouble,   ///< VT_R8 | VT_ARRAY
    VectorBool,     ///< VT_BOOL | VT_ARRAY
    VectorString,   ///< VT_BSTR | VT_ARRAY, одномерный
    VectorString2d, ///< VT_BSTR | VT_ARRAY, двумерный
    VectorDate      ///< VT_DATE | VT_ARRAY
  };

  /// Конструктор.
  StorageValue();
  /// Деструктор.
  ~StorageValue();

  /// Конструктор копирования.
  StorageValue(const StorageValue & other);
  /// Конструктор перемещения.
  StorageValue(StorageValue && other) noexcept;

  /// Оператор копирования.
  StorageValue & operator=(const StorageValue &);
  /// Оператор перемещения.
  StorageValue & operator=(StorageValue &&) noexcept;

  /// Получить тип свойства.
  StorageValueType GetValueType() const;

  /// Очистить.
  void Clear();

  /// Конструктор от однобайтового целочисленного значения (VT_I1).
  StorageValue(int8_t value);
  /// Установить однобайтовое целочисленное значение (VT_I1).
  StorageValue & SetInt8(int8_t value);
  /// Получить однобайтовое целочисленное значение (VT_I1).
  int8_t GetInt8() const;
  /// Привести текущее значение к однобайтовому целочисленному (VT_I1).
  int8_t ToInt8() const;

  /// Конструктор от однобайтового беззнакового целочисленного значения (VT_UI1).
  StorageValue(uint8_t value);
  /// Установить однобайтовое беззнаковое целочисленное значение (VT_UI1).
  StorageValue & SetUint8(uint8_t value);
  /// Получить однобайтовое беззнаковое целочисленное значение (VT_UI1).
  uint8_t GetUint8() const;
  /// Привести текущее значение к однобайтовому беззнаковому целочисленному (VT_UI1).
  uint8_t ToUint8() const;

  /// Конструктор от двухбайтового целочисленного значения (VT_I2).
  StorageValue(int16_t value);
  /// Установить двухбайтовое целочисленное значение (VT_I2).
  StorageValue & SetInt16(int16_t value);
  /// Получить двухбайтовое целочисленное значение (VT_I2).
  int16_t GetInt16() const;
  /// Привести текущее значение к двухбайтовому целочисленному (VT_I2).
  int16_t ToInt16() const;

  /// Конструктор от двухбайтового беззнакового целочисленного значения (VT_UI1).
  StorageValue(uint16_t value);
  /// Установить двухбайтовое беззнаковое целочисленное значение (VT_UI1).
  StorageValue & SetUint16(uint16_t value);
  /// Получить двухбайтовое беззнаковое целочисленное значение (VT_UI1).
  uint16_t GetUint16() const;
  /// Привести текущее значение к двухбайтовому беззнаковому целочисленному (VT_UI1).
  uint16_t ToUint16() const;

  /// Конструктор от четырёхбайтового целочисленного значения (VT_INT).
  StorageValue(int32_t value);
  /// Установить четырёхбайтовое целочисленное значение (VT_INT).
  StorageValue & SetInt32(int32_t value);
  /// Получить четырёхбайтовое целочисленное значение (VT_INT).
  int32_t GetInt32() const;
  /// Привести текущее значение к четырёхбайтовому целочисленному (VT_INT).
  int32_t ToInt32() const;

  /// Конструктор от четырёхбайтового беззнакового целочисленного значения (VT_UINT).
  StorageValue(uint32_t value);
  /// Установить четырёхбайтовое беззнаковое целочисленное значение (VT_UINT).
  StorageValue & SetUint32(uint32_t value);
  /// Получить четырёхбайтовое беззнаковое целочисленное значение (VT_UINT).
  uint32_t GetUint32() const;
  /// Привести текущее значение к четырёхбайтовому беззнаковому целочисленному (VT_UINT).
  uint32_t ToUint32() const;

  /// Установить четырёхбайтовое целочисленное значение (VT_I4).
  StorageValue & SetLong(int32_t value);
  /// Получить четырёхбайтовое целочисленное значение (VT_I4).
  int32_t GetLong() const;
  /// Привести текущее значение к четырёхбайтовому целочисленному (VT_I4).
  int32_t ToLong() const;

  /// Установить четырёхбайтовое беззнаковое целочисленное значение (VT_UI4).
  StorageValue & SetUlong(uint32_t value);
  /// Получить четырёхбайтовое беззнаковое целочисленное значение (VT_UI4).
  uint32_t GetUlong() const;
  /// Привести текущее значение к четырёхбайтовому беззнаковому целочисленному (VT_UI4).
  uint32_t ToUlong() const;

  /// Конструктор от восьмибайтового целочисленного значения (VT_I8).
  StorageValue(int64_t value);
  /// Установить восьмибайтовое целочисленное значение (VT_I8).
  StorageValue & SetInt64(int64_t value);
  /// Получить восьмибайтовое целочисленное значение (VT_I8).
  int64_t GetInt64() const;
  /// Привести текущее значение к восьмибайтовому целочисленному (VT_I8).
  int64_t ToInt64() const;

  /// Конструктор от восьмибайтового беззнакового целочисленного значения (VT_I8).
  StorageValue(uint64_t value);
  /// Установить восьмибайтовое беззнаковое целочисленное значение (VT_I8).
  StorageValue & SetUint64(uint64_t value);
  /// Получить восьмибайтовое беззнаковое целочисленное значение (VT_I8).
  uint64_t GetUint64() const;
  /// Привести текущее значение к восьмибайтовому беззнаковому целочисленному (VT_I8).
  uint64_t ToUint64() const;

  /// Конструктор от четырёхбайтового вещественного значения (VT_R4).
  StorageValue(float value);
  /// Установить четырёхбайтовое вещественное значение (VT_R4).
  StorageValue & SetFloat(float value);
  /// Получить четырёхбайтовое вещественное значение (VT_R4).
  float GetFloat() const;
  /// Привести текущее значение к четырёхбайтовому вещественному (VT_R4).
  float ToFloat() const;

  /// Конструктор от восьмибайтового вещественного значения (VT_R8).
  StorageValue(double value);
  /// Установить восьмибайтовое вещественное значение (VT_R8).
  StorageValue & SetDouble(double value);
  /// Получить восьмибайтовое вещественное значение (VT_R8).
  double GetDouble() const;
  /// Привести текущее значение к восьмибайтовому вещественному (VT_R8).
  double ToDouble() const;

  /// Конструктор от булева значения (VT_BOOL).
  StorageValue(bool value);
  /// Установить булево значение (VT_BOOL).
  StorageValue & SetBool(bool value);
  /// Получить булево значение (VT_BOOL).
  bool GetBool() const;
  /// Привести текущее значение к булеву (VT_BOOL).
  bool ToBool() const;

  /// Конструктор от строкового значения (VT_BSTR).
  StorageValue(const std::wstring & value);
  /// Конструктор от указателя на строку (VT_BSTR).
  StorageValue(const wchar_t * value);
  /// Установить строковое значение (VT_BSTR).
  StorageValue & SetString(const std::wstring & value);
  /// Получить строковое значение (VT_BSTR).
  std::wstring GetString() const;
  /// Привести текущее значение к строковому (VT_BSTR).
  std::wstring ToString() const;

  /// Установить дату (VT_DATE).
  StorageValue & SetDate(double date);
  /// Получить дату (VT_DATE).
  double GetDate() const;

  /// Конструктор от массива однобайтовых целочисленных значений (VT_I1 | VT_ARRAY).
  StorageValue(const std::vector<int8_t> & array);
  /// Установить массив однобайтовых целочисленных значений (VT_I1 | VT_ARRAY).
  StorageValue & SetVectorInt8(const std::vector<int8_t> & array);
  /// Получить массив однобайтовых целочисленных значений (VT_I1 | VT_ARRAY).
  std::vector<int8_t> GetVectorInt8() const;

  /// Конструктор от массива однобайтовых целочисленных беззнаковых значений (VT_UI1 | VT_ARRAY).
  StorageValue(const std::vector<uint8_t> & array);
  /// Установить массив однобайтовых целочисленных беззнаковых значений (VT_UI1 | VT_ARRAY).
  StorageValue & SetVectorUint8(const std::vector<uint8_t> & array);
  /// Получить массив однобайтовых целочисленных беззнаковых значений (VT_UI1 | VT_ARRAY).
  std::vector<uint8_t> GetVectorUint8() const;

  /// Конструктор от массива двухбайтовых целочисленных значений (VT_I2 | VT_ARRAY).
  StorageValue(const std::vector<int16_t> & array);
  /// Установить массив двухбайтовых целочисленных значений (VT_I2 | VT_ARRAY).
  StorageValue & SetVectorInt16(const std::vector<int16_t> & array);
  /// Получить массив двухбайтовых целочисленных значений (VT_I2 | VT_ARRAY).
  std::vector<int16_t> GetVectorInt16() const;


  /// Конструктор от массива двухбайтовых целочисленных беззнаковых значений (VT_UI2 | VT_ARRAY).
  StorageValue(const std::vector<uint16_t> & array);
  /// Установить массив двухбайтовых целочисленных беззнаковых значений (VT_UI2 | VT_ARRAY).
  StorageValue & SetVectorUint16(const std::vector<uint16_t> & array);
  /// Получить массив двухбайтовых целочисленных беззнаковых значений (VT_UI2 | VT_ARRAY).
  std::vector<uint16_t> GetVectorUint16() const;

  /// Конструктор от массива четырёхбайтовых целочисленных значений (VT_INT | VT_ARRAY).
  StorageValue(const std::vector<int32_t> & array);
  /// Установить массив четырёхбайтовых целочисленных значений (VT_INT | VT_ARRAY).
  StorageValue & SetVectorInt32(const std::vector<int32_t> & array);
  /// Получить массив четырёхбайтовых целочисленных значений (VT_INT | VT_ARRAY).
  std::vector<int32_t> GetVectorInt32() const;

  /// Конструктор от массива четырёхбайтовых целочисленных беззнаковых значений (VT_UINT | VT_ARRAY).
  StorageValue(const std::vector<uint32_t> & array);
  /// Установить массив четырёхбайтовых целочисленных беззнаковых значений (VT_UINT | VT_ARRAY).
  StorageValue & SetVectorUint32(const std::vector<uint32_t> & array);
  /// Получить массив четырёхбайтовых целочисленных беззнаковых значений (VT_UINT | VT_ARRAY).
  std::vector<uint32_t> GetVectorUint32() const;

  /// Установить массив четырёхбайтовых целочисленных значений (VT_I4 | VT_ARRAY).
  StorageValue & SetVectorLong(const std::vector<int32_t> & array);
  /// Получить массив четырёхбайтовых целочисленных значений (VT_I4 | VT_ARRAY).
  std::vector<int32_t> GetVectorLong() const;

  /// Установить массив четырёхбайтовых целочисленных беззнаковых значений (VT_UI4 | VT_ARRAY).
  StorageValue & SetVectorUlong(const std::vector<uint32_t> & array);
  /// Получить массив четырёхбайтовых целочисленных беззнаковых значений (VT_UI4 | VT_ARRAY).
  std::vector<uint32_t> GetVectorUlong() const;

  /// Конструктор от массива восьмибайтовых целочисленных значений (VT_I8 | VT_ARRAY).
  StorageValue(const std::vector<int64_t> & array);
  /// Установить массив восьмибайтовых целочисленных значений (VT_I8 | VT_ARRAY).
  StorageValue & SetVectorInt64(const std::vector<int64_t> & array);
  /// Получить массив восьмибайтовых целочисленных значений (VT_I8 | VT_ARRAY).
  std::vector<int64_t> GetVectorInt64() const;

  /// Конструктор от массива восьмибайтовых целочисленных беззнаковых значений (VT_UI8 | VT_ARRAY).
  StorageValue(const std::vector<uint64_t> & array);
  /// Установить массив восьмибайтовых целочисленных беззнаковых значений (VT_UI8 | VT_ARRAY).
  StorageValue & SetVectorUint64(const std::vector<uint64_t> & array);
  /// Получить массив восьмибайтовых целочисленных беззнаковых значений (VT_UI8 | VT_ARRAY).
  std::vector<uint64_t> GetVectorUint64() const;

  /// Конструктор от массива четырёхбайтовых вещественных значений (VT_R4 | VT_ARRAY).
  StorageValue(const std::vector<float> & array);
  /// Установить массив четырёхбайтовых вещественных значений (VT_R4 | VT_ARRAY).
  StorageValue & SetVectorFloat(const std::vector<float> & array);
  /// Получить массив четырёхбайтовых вещественных значений (VT_R4 | VT_ARRAY).
  std::vector<float> GetVectorFloat() const;

  /// Конструктор от массива восьмибайтовых вещественных значений (VT_R8 | VT_ARRAY).
  StorageValue(const std::vector<double> & array);
  /// Установить массив восьмибайтовых вещественных значений (VT_R8 | VT_ARRAY).
  StorageValue & SetVectorDouble(const std::vector<double> & array);
  /// Получить массив восьмибайтовых вещественных значений (VT_R8 | VT_ARRAY).
  std::vector<double> GetVectorDouble() const;

  /// Конструктор от массива булевых значений (VT_BOOL | VT_ARRAY).
  StorageValue(const std::vector<bool> & array);
  /// Установить массив булевых значений (VT_BOOL | VT_ARRAY).
  StorageValue & SetVectorBool(const std::vector<bool> & array);
  /// Получить массив булевых значений (VT_BOOL | VT_ARRAY).
  std::vector<bool> GetVectorBool() const;

  /// Конструктор от массива строковых значений (VT_BSTR | VT_ARRAY, одномерный).
  StorageValue(const std::vector<std::wstring> & array);
  /// Установить массив строковых значений (VT_BSTR | VT_ARRAY, одномерный).
  StorageValue & SetVectorString(const std::vector<std::wstring> & array);
  /// Получить массив строковых значений (VT_BSTR | VT_ARRAY, одномерный).
  std::vector<std::wstring> GetVectorString() const;

  /**
    \brief Конструктор от двумерного массива строковых значений.
    \details Вложенные массивы должны иметь одинаковый размер.
    \param[in] array - массив строковых значений (VT_BSTR | VT_ARRAY, двумерный).
    \return Успешность получения.
  */
  StorageValue(const std::vector<std::vector<std::wstring>> & array);
  /**
    \brief Установить двумерный массив строковых значений.
    \details Вложенные массивы должны иметь одинаковый размер.
    \param[in] array - массив строковых значений (VT_BSTR | VT_ARRAY, двумерный).
  */
  StorageValue & SetVectorString2d(const std::vector<std::vector<std::wstring>> & array);
  /// Получить двумерный массив строковых значений (VT_BSTR | VT_ARRAY, двумерный).
  std::vector<std::vector<std::wstring>> GetVectorString2d() const;

  /// Установить массив дат (VT_DATE | VT_ARRAY, одномерный).
  StorageValue & SetVectorDate(const std::vector<double> & array);
  /// Получить массив дат (VT_DATE | VT_ARRAY, одномерный).
  std::vector<double> GetVectorDate() const;

private:
  struct Private;
  Private * m_d = nullptr;
};

} // namespace ksapi
