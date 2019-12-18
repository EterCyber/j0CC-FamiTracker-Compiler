/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2015 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

/*
 *  Add new program settings to the SetupSettings function,
 *  three macros are provided for the type of setting you want to add.
 *  (SETTING_INT, SETTING_BOOL, SETTING_STRING)
 *
 */

#include "stdafx.h"
#include "FamiTracker.h"
#include "Settings.h"

// // // registry context

stOldSettingContext::stOldSettingContext()
{
}

stOldSettingContext::~stOldSettingContext()
{
}

// CSettings

CSettings *CSettings::GetObject()
{
  static CSettings Object;
  return &Object;
}

CSettings::CSettings() : m_iAddedSettings(0)
{
  memset(m_pSettings, 0, sizeof(CSettingBase*) * MAX_SETTINGS);
  SetupSettings();
  DefaultSettings();
  TRACE(_T("Settings: Added %i settings\n"), m_iAddedSettings);
}

CSettings::~CSettings()
{
  // Release all settings
  for (int i = 0; i < m_iAddedSettings; ++i) {
    SAFE_RELEASE(m_pSettings[i]);
  }
}

void CSettings::SetupSettings()
{
  //
  // This function defines all settings in the program that are stored in registry
  // All settings are loaded on program start and saved when closing the program
  //

  // The SETTING macros takes four arguments: 
  //
  //  1. Registry section
  //  2. Registry key name
  //  3. Default value
  //  4. A variable that contains the setting, loaded on program startup and saved on shutdown
  //

  const auto SETTING_INT = [&] (LPCTSTR Section, LPCTSTR Entry, int Default, int *Variable) {    // // //
    return AddSetting<int>((Section), (Entry), Default, Variable);
  };
  const auto SETTING_BOOL = [&] (LPCTSTR Section, LPCTSTR Entry, bool Default, bool *Variable) {
    return AddSetting<bool>((Section), (Entry), Default, Variable);
  };
  const auto SETTING_STRING = [&] (LPCTSTR Section, LPCTSTR Entry, CString Default, CString *Variable) {
    return AddSetting<CString>((Section), (Entry), Default, Variable);
  };

  stOldSettingContext s;    // // //
  /*
    location priorities:
    1. HKCU/SOFTWARE/0CC-FamiTracker
    2. HKCU/SOFTWARE/0CC-FamiTracker, original key (using UpdateDefault)
    3. HKCU/SOFTWARE/FamiTracker
    4. HKCU/SOFTWARE/FamiTracker, original key (using UpdateDefault)
    5. Default value
  */

  // General
  SETTING_INT(_T("General"), _T("Edit style"), EDIT_STYLE_FT2, &General.iEditStyle);
  SETTING_INT(_T("General"), _T("Page step size"), 4, &General.iPageStepSize);
  SETTING_BOOL(_T("General"), _T("Wrap cursor"), true, &General.bWrapCursor);
  SETTING_BOOL(_T("General"), _T("Wrap across frames"), true, &General.bWrapFrames);
  SETTING_BOOL(_T("General"), _T("Free cursor edit"),  false, &General.bFreeCursorEdit);
  SETTING_BOOL(_T("General"), _T("Wave preview"), true, &General.bWavePreview);
  SETTING_BOOL(_T("General"), _T("Key repeat"), true, &General.bKeyRepeat);
  SETTING_BOOL(_T("General"), _T("Hex row display"), true, &General.bRowInHex);
  SETTING_BOOL(_T("General"), _T("Frame preview"), true, &General.bFramePreview);
  SETTING_BOOL(_T("General"), _T("No DPCM reset"), false, &General.bNoDPCMReset);
  SETTING_BOOL(_T("General"), _T("No Step moving"), false, &General.bNoStepMove);
  SETTING_BOOL(_T("General"), _T("Delete pull up"), false, &General.bPullUpDelete);
  SETTING_BOOL(_T("General"), _T("Backups"), false, &General.bBackups);
  SETTING_BOOL(_T("General"), _T("Single instance"), false, &General.bSingleInstance);
  SETTING_BOOL(_T("General"), _T("Preview full row"), false, &General.bPreviewFullRow);
  SETTING_BOOL(_T("General"), _T("Double click selection"), false, &General.bDblClickSelect);
  // // //
  SETTING_BOOL(_T("General"), _T("Wrap pattern values"), false, &General.bWrapPatternValue);
  SETTING_BOOL(_T("General"), _T("Cut sub-volume"), false, &General.bCutVolume);
  SETTING_BOOL(_T("General"), _T("Use old FDS volume table"), false, &General.bFDSOldVolume);
  SETTING_BOOL(_T("General"), _T("Overflow paste mode"), false, &General.bOverflowPaste);
  SETTING_BOOL(_T("General"), _T("Show skipped rows"), false, &General.bShowSkippedRows);
  SETTING_BOOL(_T("General"), _T("Hexadecimal keypad"), false, &General.bHexKeypad);
  SETTING_BOOL(_T("General"), _T("Multi-frame selection"), false, &General.bMultiFrameSel);

  // // // Version / Compatibility info
  SETTING_INT(_T("Version"), _T("Module error level"), MODULE_ERROR_DEFAULT, &Version.iErrorLevel);

  // Keys
  SETTING_INT(_T("Keys"), _T("Note cut"),    0x31, &Keys.iKeyNoteCut);
  SETTING_INT(_T("Keys"), _T("Note release"), 0xDC, &Keys.iKeyNoteRelease);
  SETTING_INT(_T("Keys"), _T("Clear field"),  0xBD, &Keys.iKeyClear);
  SETTING_INT(_T("Keys"), _T("Repeat"),    0x00, &Keys.iKeyRepeat);
  SETTING_INT(_T("Keys"), _T("Echo buffer"),  0x00, &Keys.iKeyEchoBuffer);    // // //

  // Sound
  SETTING_INT(_T("Sound"), _T("Audio Device"), 0, &Sound.iDevice);
  SETTING_INT(_T("Sound"), _T("Sample rate"),  44100, &Sound.iSampleRate);
  SETTING_INT(_T("Sound"), _T("Sample size"), 16, &Sound.iSampleSize);
  SETTING_INT(_T("Sound"), _T("Buffer length"), 40, &Sound.iBufferLength);
  SETTING_INT(_T("Sound"), _T("Bass filter freq"), 30, &Sound.iBassFilter);
  SETTING_INT(_T("Sound"), _T("Treble filter freq"), 12000, &Sound.iTrebleFilter);
  SETTING_INT(_T("Sound"), _T("Treble filter damping"), 24, &Sound.iTrebleDamping);
  SETTING_INT(_T("Sound"), _T("Volume"), 100, &Sound.iMixVolume);

  // Midi
  SETTING_INT(_T("MIDI"), _T("Device"), 0, &Midi.iMidiDevice);
  SETTING_INT(_T("MIDI"), _T("Out Device"), 0, &Midi.iMidiOutDevice);
  SETTING_BOOL(_T("MIDI"), _T("Master sync"), false, &Midi.bMidiMasterSync);
  SETTING_BOOL(_T("MIDI"), _T("Key release"), false, &Midi.bMidiKeyRelease);
  SETTING_BOOL(_T("MIDI"), _T("Channel map"), false, &Midi.bMidiChannelMap);
  SETTING_BOOL(_T("MIDI"), _T("Velocity control"), false,  &Midi.bMidiVelocity);
  SETTING_BOOL(_T("MIDI"), _T("Auto Arpeggio"), false, &Midi.bMidiArpeggio);

  // Paths
  SETTING_STRING(_T("Paths"), _T("Instrument menu"), _T(""), &InstrumentMenuPath);

  // Mixing
  SETTING_INT(_T("Mixer"), _T("APU1"), 0, &ChipLevels.iLevelAPU1);
  SETTING_INT(_T("Mixer"), _T("APU2"), 0, &ChipLevels.iLevelAPU2);
  SETTING_INT(_T("Mixer"), _T("VRC6"), 0, &ChipLevels.iLevelVRC6);
  SETTING_INT(_T("Mixer"), _T("VRC7"), 0, &ChipLevels.iLevelVRC7);
  SETTING_INT(_T("Mixer"), _T("MMC5"), 0, &ChipLevels.iLevelMMC5);
  SETTING_INT(_T("Mixer"), _T("FDS"), 0, &ChipLevels.iLevelFDS);
  SETTING_INT(_T("Mixer"), _T("N163"), 0, &ChipLevels.iLevelN163);
  SETTING_INT(_T("Mixer"), _T("S5B"), 0, &ChipLevels.iLevelS5B);
}

template<class T>
CSettingBase *CSettings::AddSetting(LPCTSTR pSection, LPCTSTR pEntry, T tDefault, T *pVariable)
{
  ASSERT(m_iAddedSettings < MAX_SETTINGS);
  return m_pSettings[m_iAddedSettings++] = new CSettingType<T>(pSection, pEntry, tDefault, pVariable);    // // //
}

// CSettings member functions

void CSettings::DefaultSettings()
{
  for (int i = 0; i < m_iAddedSettings; ++i) {
    m_pSettings[i]->Default();
  }
}

// Settings types

template<class T>
void CSettingType<T>::Default()
{
  *m_pVariable = m_tDefaultValue;
}

void CSettingBase::UpdateDefault(LPCTSTR pSection, LPCTSTR pEntry)    // // //
{
  m_pSectionSecond = pSection;
  m_pEntrySecond = pEntry;
}
