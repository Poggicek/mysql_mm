/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Sample Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * This sample plugin is public domain.
 */

#include <stdio.h>
#include "sample_mm.h"
#include "iserver.h"
#include "include/mysql_mm.h"

IMySQLClient *g_pMysqlClient;
IMySQLConnection* g_pConnection;

SamplePlugin g_SamplePlugin;

PLUGIN_EXPOSE(SamplePlugin, g_SamplePlugin);
bool SamplePlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	g_SMAPI->AddListener( this, this );

	META_CONPRINTF( "Starting plugin.\n" );

	return true;
}

bool SamplePlugin::Unload(char *error, size_t maxlen)
{
	if (g_pConnection)
		g_pConnection->Destroy();

	return true;
}

void SamplePlugin::AllPluginsLoaded()
{
	
	int ret;
	g_pMysqlClient = (IMySQLClient*)g_SMAPI->MetaFactory(MYSQLMM_INTERFACE, &ret, NULL);

	if (ret == META_IFACE_FAILED)
	{
		ConMsg("Failed to lookup mysql client. Aborting");
		return;
	}

	MySQLConnectionInfo info{ .host = "test", .user = "test", .pass = "test", .database = "test" };
	g_pConnection = g_pMysqlClient->CreateMySQLConnection(info);
	
	g_pConnection->Connect([](bool connect) {
		if (connect)
		{
			ConMsg("CONNECTED\n");

			g_pConnection->Query("SELECT * FROM test1", [](IMySQLQuery* test)
			{
				auto results = test->GetResultSet();
				ConMsg("Callback rows %i\n", results->GetRowCount());
				while (results->FetchRow())
				{
					ConMsg("ID: %i, str: %s\n", results->GetInt(0), results->GetString(1));
				}
			});
		}
		else
		{
			ConMsg("Failed to connect\n");

			// make sure to properly destroy the connection
			g_pConnection->Destroy();
			g_pConnection = nullptr;
		}
	});
	

}

void SamplePlugin::OnLevelInit( char const *pMapName,
									 char const *pMapEntities,
									 char const *pOldLevel,
									 char const *pLandmarkName,
									 bool loadGame,
									 bool background )
{
}

void SamplePlugin::OnLevelShutdown()
{
}

bool SamplePlugin::Pause(char *error, size_t maxlen)
{
	return true;
}

bool SamplePlugin::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *SamplePlugin::GetLicense()
{
	return "Public Domain";
}

const char *SamplePlugin::GetVersion()
{
	return "1.0.0.0";
}

const char *SamplePlugin::GetDate()
{
	return __DATE__;
}

const char *SamplePlugin::GetLogTag()
{
	return "SAMPLE";
}

const char *SamplePlugin::GetAuthor()
{
	return "AlliedModders LLC";
}

const char *SamplePlugin::GetDescription()
{
	return "Sample basic plugin";
}

const char *SamplePlugin::GetName()
{
	return "Sample Plugin";
}

const char *SamplePlugin::GetURL()
{
	return "http://www.sourcemm.net/";
}
