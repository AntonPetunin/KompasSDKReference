#pragma once

#include <Step2_KsAPI_3D_pch.h>

/// Результат диалога
enum DialogResult
{
  Positive = 6,
  Negative = 7,
  Cancel = 2
};

/// Загрузить строку из ресурса.
std::wstring LoadStr(std::int32_t strId);
/// Сообщение от библиотеки.
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
///  Диалог да/нет/отмена.
std::int32_t YesNoMessage(const std::wstring & message, bool question = true);
