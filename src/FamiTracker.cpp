/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2016 HertzDevil
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

#include "json/json.hpp"    // // //
#include "version.h"    // // //
#include "stdafx.h"
#include "FamiTracker.h"
#include "FamiTrackerDoc.h"
#include "TrackerChannel.h"
#include "MIDI.h"
#include "SoundGen.h"
#include "Compiler.h"
#include "Settings.h"
#include "ChannelMap.h"
#include "CustomExporters.h"
// #include "CommandLineExport.h"
#include "WinSDK/VersionHelpers.h"    // // //

#include "WinInet.h"    // // //
#pragma comment(lib, "wininet.lib")

// Single instance-stuff
const TCHAR FT_SHARED_MUTEX_NAME[]  = _T("FamiTrackerMutex");  // Name of global mutex
const TCHAR FT_SHARED_MEM_NAME[]  = _T("FamiTrackerWnd");    // Name of global memory area
const DWORD  SHARED_MEM_SIZE      = 256;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void ftm_convert_to_nsf(const TCHAR *ftm_filepath, const TCHAR *nsf_filepath)
{
  CFamiTrackerDoc *pExportDoc = new CFamiTrackerDoc();
  CCompiler *pComplie = nullptr;
  
  pExportDoc->OnOpenDocument(ftm_filepath);
  pComplie = new CCompiler(pExportDoc, NULL);

  pComplie->ExportNSF(nsf_filepath, pExportDoc->GetMachine());

  delete pComplie;
  delete pExportDoc;
}
