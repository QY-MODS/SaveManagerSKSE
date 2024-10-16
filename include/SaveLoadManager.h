// https://github.com/digital-apple/CommonLibSSE/blob/a94a3c4ccb993770e89146874b8b8c87fa6e7889/include/RE/B/BGSSaveLoadManager.h

#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/BSFixedString.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTHashMap.h"
#include "RE/B/BSTMessageQueue.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/B/BSThread.h"
#include "RE/R/RaceSexMenuEvent.h"
#include "RE/R/Request.h"

namespace RE
{
	class BSSaveDataEvent;
	class BSSystemEvent;
	struct BGSSaveLoadManagerEvent;

	class SaveLoadFileEntry
	{
	public:
		enum class SaveType : std::uint32_t
		{
			kAutosave = 0x0,
			kSave = 0x1,
			kQuicksave = 0x2,
			kCustom = 0x3
		};

		bool PopulateFileEntryData();

		// members
		BSFixedString                             fileName;            // 00
		BSFixedString                             characterName;       // 08
		BSFixedString                             characterTitle;      // 10
		BSFixedString                             currentLocation;     // 18
		BSFixedString                             playTime;            // 20
		BSFixedString                             characterRace;       // 28
		std::int32_t                              saveVersion;         // 30
		std::uint32_t                             saveNumber;          // 34
		std::uint32_t                             characterLevel;      // 38
		float                                     currentExperience;   // 3C
		float                                     requiredExperience;  // 40
		std::uint32_t                             screenshotWidth;     // 44
		std::uint32_t                             screenshotHeight;    // 48
		std::uint32_t                             unk4C;               // 4C
		REX::W32::FILETIME                        fileTime;            // 50
		REX::W32::FILETIME                        saveTime;            // 58
		std::int32_t                              unk60;               // 60
		bool                                      loaded;              // 64
		bool                                      unk65;               // 65
		bool                                      unk66;               // 66
		std::uint8_t                              unk67;               // 67
		std::uint8_t                              unk68;               // 68
		bool                                      modded;              // 69
		std::uint16_t                             pad6A;               // 6A
		std::uint32_t                             characterID;         // 6C
		stl::enumeration<SaveType, std::uint32_t> saveType;            // 70
		std::uint32_t                             pad74;               // 74
	};
	static_assert(sizeof(SaveLoadFileEntry) == 0x78);

	class SaveLoadManager :
		public BSTEventSink<BSSaveDataEvent>,                     // 000
		public BSTEventSink<RaceSexMenuEvent::NameChangedEvent>,  // 008
		public BSTEventSource<BGSSaveLoadManagerEvent>,           // 018
		public BSTEventSink<BSSystemEvent>                        // 010
	{
	public:
		inline static constexpr auto RTTI = RTTI_BGSSaveLoadManager;
		inline static constexpr auto VTABLE = VTABLE_BGSSaveLoadManager;

		class Thread : public BSThread
		{
		public:
			inline static constexpr auto RTTI = RTTI_BGSSaveLoadManager__Thread;
			inline static constexpr auto VTABLE = VTABLE_BGSSaveLoadManager__Thread;

			~Thread() override;  // 00

			void Unk_01(void) override;  // 01
			void Unk_02(void) override;  // 02

			// members
			bool                                                                    isRunnning;                   // 50
			bool                                                                    isBusy;                       // 51
			std::uint16_t                                                           pad52;                        // 52
			std::uint32_t                                                           pad54;                        // 54
			BSEventFlag                                                             haveTask;                     // 58
			BSTCommonStaticMessageQueue<BSTSmartPointer<bgs::saveload::Request>, 8> asyncSaveLoadOperationQueue;  // 60
		};
		static_assert(sizeof(Thread) == 0xC0);

		struct SaveEntry
		{
			std::uint32_t                             characterID;     // 00
			std::uint32_t                             isModded;        // 04
			std::uint32_t                             manualSaveCount; // 08
			std::uint32_t                             autoSaveCount;   // 0C
			BSTHashMap<std::uint32_t, BSFixedString>  autoSaves;       // 18
			BSFixedString                             lastQuicksave;   // 40
		};
		static_assert(sizeof(SaveEntry) == 0x48);

		~SaveLoadManager() override;  // 00

		// override (BSTEventSink<BSSaveDataEvent>)
		BSEventNotifyControl ProcessEvent(const BSSaveDataEvent* a_event, BSTEventSource<BSSaveDataEvent>* a_eventSource) override;  // 01

		// override (BSTEventSink<RaceSexMenuEvent::NameChangedEvent>)
		BSEventNotifyControl ProcessEvent(const RaceSexMenuEvent::NameChangedEvent* a_event, BSTEventSource<RaceSexMenuEvent::NameChangedEvent>* a_eventSource) override;  // 01

		// override (BSTEventSink<BSSystemEvent>)
		BSEventNotifyControl ProcessEvent(const BSSystemEvent* a_event, BSTEventSource<BSSystemEvent>* a_eventSource) override;  // 01

		static SaveLoadManager* GetSingleton();

		void GenerateCharacterID();
		void Save(const char* a_fileName);
		void Load(const char* a_fileName);
		void Load(const char* a_fileName, bool a_checkForMods);
		bool LoadMostRecentSaveGame();
		bool PopulateSaveList();

		// members
		BSTHashMap<std::uint64_t, BSFixedString> characterIDNameMap;     // 078
		std::uint64_t                            unk0A0;                 // 0A0
		std::uint32_t                            unk0A8;                 // 0A8
		std::uint32_t                            unk0AC;                 // 0AC
		std::uint32_t                            unk0B0;                 // 0B0
		std::uint32_t                            unk0B4;                 // 0B4
		std::uint64_t                            unk0B8;                 // 0B8
		std::uint64_t                            unk0C0;                 // 0C0
		std::uint64_t                            unk0C8;                 // 0C8
		std::uint32_t                            currentCharacterID;     // 0D0
		std::uint32_t                            currentCharacterModded; // 0D4
		std::uint32_t                            displayCharacterID;     // 0D8
		std::uint32_t                            displayCharacterModded; // 0DC
		std::uint32_t                            unk0E0;                 // 0E0
		std::uint32_t                            unk0E4;                 // 0E4
		BSTArray<SaveEntry>                      loadedEntries;          // 0E8

		BSTArray<SaveLoadFileEntry*> saveGameList;  					 // 100
		std::uint8_t                    unk118;        					 // 118
		std::uint8_t                    pad119;        					 // 119
		std::uint16_t                   pad11A;       					 // 11A
		std::uint32_t                   saveCount;    					 // 11C
		std::uint8_t                    unk120;       					 // 120
		std::uint8_t                    pad121;							 // 121
		std::uint16_t                   pad122;							 // 122
		std::uint32_t                   pad124;							 // 124
		std::uint64_t                   tickCount;                		 // 128
		std::uint8_t                    unk130;                   		 // 130
		std::uint8_t                    pad131;                   		 // 131
		std::uint16_t                   pad132;                   		 // 132
		std::uint32_t                   pad134;                   		 // 134
		const char                      lastFileFullName[0x104];  		 // 138
		std::uint32_t                   pad23C;                   	 	 // 23C
		BSFixedString                   lastFileName;             	 	 // 240
		std::uint16_t                   lastFileNameLength;       	 	 // 248
		std::uint16_t                   maxFileNameLength;        	 	 // 24A
		std::uint32_t                   unk24C;                   	 	 // 24C
		std::int32_t                    unk250;                   	 	 // 250
		std::uint32_t                   pad254;                   	 	 // 254
		std::uint64_t                   unk258;                   	 	 // 258
		std::uint16_t                   unk260;                   	 	 // 260
		std::uint16_t                   unk262;                   	 	 // 262
		std::uint32_t                   unk264;                   	 	 // 264
		std::uint64_t                   unk268;                   	 	 // 268
		std::uint32_t                   unk270;                   	 	 // 270
		std::uint32_t                   unk274;                   	 	 // 274
		std::uint64_t                   unk278;                   	 	 // 278
		std::uint64_t                   unk280;                   	 	 // 280
		std::uint32_t                   unk288;                   	 	 // 288
		std::uint32_t                   unk28C;                   	 	 // 28C
		std::uint64_t                   unk290;                   	 	 // 290
		std::uint64_t                   unk298;                   	 	 // 298
		std::uint32_t                   unk2A0;                   	 	 // 2A0
		std::uint32_t                   unk2A4;                   	 	 // 2A4
		std::uint64_t                   unk2A8;                   	 	 // 2A8
#ifdef SKYRIM_SUPPORT_AE
		std::uint16_t   unk2B0;  // 2B0
		std::uint16_t   unk2B2;  // 2B2
		std::uint64_t   unk2B8;  // 2B8
		BSTArray<void*> unk2C0;  // 2C0
		BSTArray<void*> unk2D8;  // 2D8
		std::uint8_t    unk2F0;  // 2F0
#endif
		Thread thread;  // 2B0

		BSTCommonStaticMessageQueue<BSTSmartPointer<bgs::saveload::Request>, 8> unk370;  // 370

	protected:
		bool Save_Impl(std::int32_t a_deviceID, std::uint32_t a_outputStats, const char* a_fileName);
		bool Load_Impl(const char* a_fileName, std::int32_t a_deviceID, std::uint32_t a_outputStats, bool a_checkForMods);
	};
#ifdef SKYRIM_SUPPORT_AE
	static_assert(sizeof(SaveLoadManager) == 0x418);
#else
	static_assert(sizeof(SaveLoadManager) == 0x3D0);
#endif
}