#pragma once
namespace ap::sdk {
	class c_game_types
	{
	public:
		virtual ~c_game_types() {}
		virtual bool Initialize(bool force) = 0;
		virtual bool IsInitialized() const = 0;

		virtual bool SetGameTypeAndMode(const char* gameType, const char* gameMode) = 0;
		virtual bool GetGameTypeAndModeFromAlias(const char* alias, int& gameType, int& gameMode) = 0;

		virtual bool SetGameTypeAndMode(int gameType, int gameMode) = 0;

		virtual void SetAndParseExtendedServerInfo(void* pExtendedServerInfo) = 0;

		virtual bool CheckShouldSetDefaultGameModeAndType(const char* mapName) = 0;

		virtual int GetCurrentGameType() const = 0;
		virtual int GetCurrentGameMode() const = 0;

		virtual const char* GetCurrentMapName() = 0;

		virtual const char* GetCurrentGameTypeNameID() = 0;
		virtual const char* GetCurrentGameModeNameID() = 0;

	};

	enum game_type
	{
		INVALID = -1,
		CLASSIC_CASUAL,
		CLASSIC_COMPETITIVE,
		GUNGAME_PROGRESSIVE,
		GUNGAME_BOMB,
		DANGER_ZONE = 6,
		COUNT
	};
}