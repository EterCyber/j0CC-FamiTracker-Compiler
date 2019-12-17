#include "CustomExporters.h"

CCustomExporters::CCustomExporters()
: m_currentExporter( NULL )
{
	//who: added by Derek Andrews <derek.george.andrews@gmail.com>
	//why: Load all custom exporter plugins on startup.

	TCHAR pathToPlugins[MAX_PATH];
	GetModuleFileName(NULL, pathToPlugins, MAX_PATH);
	PathRemoveFileSpec(pathToPlugins);
	PathAppend(pathToPlugins, _T("\\Plugins"));
	
	FindCustomExporters(pathToPlugins);
}

CCustomExporters::~CCustomExporters( void )
{

}

CCustomExporters *CCustomExporters::GetObject()
{
	static CCustomExporters Object;
	return &Object;
}

void CCustomExporters::GetNames( CStringArray& names ) const
{
	names.RemoveAll();
	for( int i = 0; i < m_customExporters.GetCount(); ++i )
	{
		names.Add(m_customExporters[ i ].getName() );
	}
}

void CCustomExporters::SetCurrentExporter( CString name )
{
	for( int i = 0; i < m_customExporters.GetCount(); ++i )
	{
		if(m_customExporters[ i ].getName() == name )
		{
			m_currentExporter = &m_customExporters[ i ];
			break;
		}
	}
}

CCustomExporter& CCustomExporters::GetCurrentExporter( void ) const
{
	return *m_currentExporter;
}

void CCustomExporters::FindCustomExporters( CString PluginPath )
{
	CFileFind finder;

	CString path = PluginPath + _T("\\*.dll");
	BOOL bWorking = finder.FindFile( path );
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString fileName = finder.GetFileName();
		CString filePath = finder.GetFilePath();
		
		CCustomExporter customExporter;

		if( customExporter.load( filePath ) )
		{
			m_customExporters.Add( customExporter );
		}
	}
}
