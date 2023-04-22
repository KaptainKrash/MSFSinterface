#pragma once

#pragma region WASM attempt to incorporate WASM Client
//#define WIN32_LEAN_AND_MEAN			// used by WASMClient.cpp app
//#include "framework.h"
//#include <wchar.h>
//#include <map>
// #include "WASMIF.h"						CRASH AS SOON AS THIS IS ENABLED
//#include "Default.h"
#pragma endregion

#include <Windows.h>				// Originally used for SimConnect app
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "SimConnect.h"

#pragma region Instructions for SimConnect configuration in Visual Studio
// SimConnect requirements from SDK doc; Programming APIs / SimConnect SDK:
//		Add the SimConnect include folder ($(MSFS_SDK)\SimConnect SDK\include)
//			to C++ / General / Additional Includes.
//		Add SimConnect from the library directory ($(MSFS_SDK)\SimConnect SDK\lib)
//			to Linker / General / Additional Library Directories.
//		Link to the SimConnect.lib library
//		 by adding the following dependencies to Linker / Input / Additional dependencies:
//			SimConnect.lib
//			shlwapi.lib
//			user32.lib
//			Ws2_32.lib
//		Include SimConnect.dll from ($(MSFS_SDK)\SimConnect SDK\lib)
//			in development folder.
#pragma endregion

HANDLE hSimConnect = NULL;
HRESULT hr;

bool bMsfsConnected = false;
bool bWasmConnected = false;

#pragma region Static Enums
static enum GROUP_ID
{
	GROUP0
};

static enum EVENT_ID
{
	PARKING_BRAKE_TOGGLE,
	CABIN_LIGHTS_OFF,
	CABIN_LIGHTS_ON
};

static enum DATA_DEFINE_ID
{
	DEFINITION_1
};

static enum DATA_REQUEST_ID
{
	REQUEST_1
};
#pragma endregion

#pragma region opencloseSimconnect

void openSimConnect()
{
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Open", NULL, 0, 0, 0)))
	{
		SimConnect_MapClientEventToSimEvent(hSimConnect, PARKING_BRAKE_TOGGLE, "PARKING_BRAKES");
		SimConnect_MapClientEventToSimEvent(hSimConnect, CABIN_LIGHTS_OFF, "CABIN_LIGHTS_SET");
		SimConnect_MapClientEventToSimEvent(hSimConnect, CABIN_LIGHTS_ON, "CABIN_LIGHTS_SET");

		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, CABIN_LIGHTS_OFF);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, CABIN_LIGHTS_ON);

		SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
		bMsfsConnected = true;
	}
	else
	{
		bMsfsConnected = false;
	}
}

void closeSimConnect()
{
	SimConnect_RemoveClientEvent(hSimConnect, GROUP0, PARKING_BRAKE_TOGGLE);
	SimConnect_RemoveClientEvent(hSimConnect, GROUP0, CABIN_LIGHTS_OFF);
	SimConnect_RemoveClientEvent(hSimConnect, GROUP0, CABIN_LIGHTS_ON);
	SimConnect_ClearNotificationGroup(hSimConnect, GROUP0);

	SimConnect_Close(hSimConnect);
	bMsfsConnected = false;
}
#pragma endregion

#pragma region opencloseFsuipcWasm

void openFsuipcWasm()
{
	//WASMIF* WasmPtr = WASMIF::getInstance();

	//WasmPtr->setLogLevel(LOG_LEVEL_DEBUG);

	//WasmPtr->setSimConfigConnection(connectionNo); // to set the SimConnect connection number - only needed in certain circumstances, e.g. running on a client PC
	//WasmPtr->setLvarUpdateFrequency(freq); // sets the frequency for the WAPI to request lcar updates/values. Recommended NOT to use this, and configure frequency in the WASM
	//WasmPtr->registerUpdateCallback(availableCallbackFunction);  // register a function to be called when lvars are loaded/reloaded and available for use. Recommended to USE this function
	//WasmPtr->registerLvarUpdateCallback(lvarCallbackFunction); // register a function to be called when lvar values change. You also ned to flag lvars for this callback

	//WasmPtr->start();

	//WasmPtr->end(); // Terminate the WASM connection
	bWasmConnected = true;
}

void closeFsuipcWasm()
{
	//WasmPtr->end(); // Terminate the WASM connection
	bWasmConnected = false;
}
#pragma endregion

#pragma region Windows progressing

namespace KK2023Main
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
		public:
		MainForm(void)
		{
			InitializeComponent();
		}

		protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
		private: System::Windows::Forms::TextBox^ tbMsfsStatus;
		private: System::Windows::Forms::TextBox^ tbWasmStatus;
		private: System::Windows::Forms::Button^ bConnectMsfs;
		private: System::Windows::Forms::Button^ bConnectWasm;

		private: System::Windows::Forms::Button^ bCabinLightsOFF;
		private: System::Windows::Forms::Button^ bCabinLightsON;
		private: System::Windows::Forms::TextBox^ tCabinLightsValue;
		private: System::Windows::Forms::Label^ lblCabinLights;

		private: System::Windows::Forms::Label^ lblParkingBrake;
		private: System::Windows::Forms::TextBox^ tParkingBrakeValue;
		private: System::Windows::Forms::Button^ bParkingBrakeToggle;

		private: System::Windows::Forms::GroupBox^ gbADFmode;
		private: System::Windows::Forms::RadioButton^ rbADF0;
		private: System::Windows::Forms::RadioButton^ rbADF1;
		private: System::Windows::Forms::RadioButton^ rbADF2;
		private: System::Windows::Forms::RadioButton^ rbADF3;

		private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tbMsfsStatus = (gcnew System::Windows::Forms::TextBox());
			this->tbWasmStatus = (gcnew System::Windows::Forms::TextBox());
			this->bConnectMsfs = (gcnew System::Windows::Forms::Button());
			this->bConnectWasm = (gcnew System::Windows::Forms::Button());
			this->bCabinLightsOFF = (gcnew System::Windows::Forms::Button());
			this->bCabinLightsON = (gcnew System::Windows::Forms::Button());
			this->tCabinLightsValue = (gcnew System::Windows::Forms::TextBox());
			this->lblCabinLights = (gcnew System::Windows::Forms::Label());
			this->lblParkingBrake = (gcnew System::Windows::Forms::Label());
			this->tParkingBrakeValue = (gcnew System::Windows::Forms::TextBox());
			this->bParkingBrakeToggle = (gcnew System::Windows::Forms::Button());
			this->gbADFmode = (gcnew System::Windows::Forms::GroupBox());
			this->rbADF0 = (gcnew System::Windows::Forms::RadioButton());
			this->rbADF1 = (gcnew System::Windows::Forms::RadioButton());
			this->rbADF2 = (gcnew System::Windows::Forms::RadioButton());
			this->rbADF3 = (gcnew System::Windows::Forms::RadioButton());
			this->gbADFmode->SuspendLayout();
			this->SuspendLayout();
			// 
			// tbMsfsStatus
			// 
			this->tbMsfsStatus->Enabled = false;
			this->tbMsfsStatus->Location = System::Drawing::Point(176, 42);
			this->tbMsfsStatus->Name = L"tbMsfsStatus";
			this->tbMsfsStatus->Size = System::Drawing::Size(315, 20);
			this->tbMsfsStatus->TabIndex = 98;
			this->tbMsfsStatus->TabStop = false;
			this->tbMsfsStatus->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// tbWasmStatus
			// 
			this->tbWasmStatus->Enabled = false;
			this->tbWasmStatus->Location = System::Drawing::Point(176, 138);
			this->tbWasmStatus->Name = L"tbWasmStatus";
			this->tbWasmStatus->Size = System::Drawing::Size(315, 20);
			this->tbWasmStatus->TabIndex = 99;
			this->tbWasmStatus->TabStop = false;
			this->tbWasmStatus->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->tbWasmStatus->Visible = false;
			// 
			// bConnectMsfs
			// 
			this->bConnectMsfs->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->bConnectMsfs->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->bConnectMsfs->Location = System::Drawing::Point(34, 25);
			this->bConnectMsfs->Name = L"bConnectMsfs";
			this->bConnectMsfs->Size = System::Drawing::Size(116, 49);
			this->bConnectMsfs->TabIndex = 1;
			this->bConnectMsfs->Text = L"Connect to MSFS";
			this->bConnectMsfs->UseVisualStyleBackColor = true;
			this->bConnectMsfs->Click += gcnew System::EventHandler(this, &MainForm::bConnectMsfs_Click);
			// 
			// bConnectWasm
			// 
			this->bConnectWasm->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->bConnectWasm->Enabled = false;
			this->bConnectWasm->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->bConnectWasm->Location = System::Drawing::Point(34, 121);
			this->bConnectWasm->Name = L"bConnectWasm";
			this->bConnectWasm->Size = System::Drawing::Size(116, 49);
			this->bConnectWasm->TabIndex = 2;
			this->bConnectWasm->Text = L"Connect to WASM";
			this->bConnectWasm->UseVisualStyleBackColor = true;
			this->bConnectWasm->Visible = false;
			this->bConnectWasm->Click += gcnew System::EventHandler(this, &MainForm::bConnectWasm_Click);
			// 
			// bCabinLightsOFF
			// 
			this->bCabinLightsOFF->Location = System::Drawing::Point(416, 269);
			this->bCabinLightsOFF->Name = L"bCabinLightsOFF";
			this->bCabinLightsOFF->Size = System::Drawing::Size(75, 23);
			this->bCabinLightsOFF->TabIndex = 4;
			this->bCabinLightsOFF->Text = L"Turn OFF";
			this->bCabinLightsOFF->UseVisualStyleBackColor = true;
			this->bCabinLightsOFF->Click += gcnew System::EventHandler(this, &MainForm::bChange0_Click);
			// 
			// bCabinLightsON
			// 
			this->bCabinLightsON->Location = System::Drawing::Point(416, 305);
			this->bCabinLightsON->Name = L"bCabinLightsON";
			this->bCabinLightsON->Size = System::Drawing::Size(75, 23);
			this->bCabinLightsON->TabIndex = 5;
			this->bCabinLightsON->Text = L"Turn ON";
			this->bCabinLightsON->UseVisualStyleBackColor = true;
			this->bCabinLightsON->Click += gcnew System::EventHandler(this, &MainForm::bChange1_Click);
			// 
			// tCabinLightsValue
			// 
			this->tCabinLightsValue->Enabled = false;
			this->tCabinLightsValue->Location = System::Drawing::Point(416, 234);
			this->tCabinLightsValue->Name = L"tCabinLightsValue";
			this->tCabinLightsValue->Size = System::Drawing::Size(75, 20);
			this->tCabinLightsValue->TabIndex = 3;
			this->tCabinLightsValue->TabStop = false;
			// 
			// lblCabinLights
			// 
			this->lblCabinLights->AutoSize = true;
			this->lblCabinLights->Enabled = false;
			this->lblCabinLights->Location = System::Drawing::Point(413, 218);
			this->lblCabinLights->Name = L"lblCabinLights";
			this->lblCabinLights->Size = System::Drawing::Size(61, 13);
			this->lblCabinLights->TabIndex = 10;
			this->lblCabinLights->Text = L"Cabin lights";
			// 
			// lblParkingBrake
			// 
			this->lblParkingBrake->AutoSize = true;
			this->lblParkingBrake->Enabled = false;
			this->lblParkingBrake->Location = System::Drawing::Point(288, 218);
			this->lblParkingBrake->Name = L"lblParkingBrake";
			this->lblParkingBrake->Size = System::Drawing::Size(74, 13);
			this->lblParkingBrake->TabIndex = 100;
			this->lblParkingBrake->Text = L"Parking Brake";
			// 
			// tParkingBrakeValue
			// 
			this->tParkingBrakeValue->Enabled = false;
			this->tParkingBrakeValue->Location = System::Drawing::Point(291, 234);
			this->tParkingBrakeValue->Name = L"tParkingBrakeValue";
			this->tParkingBrakeValue->Size = System::Drawing::Size(75, 20);
			this->tParkingBrakeValue->TabIndex = 101;
			this->tParkingBrakeValue->TabStop = false;
			// 
			// bParkingBrakeToggle
			// 
			this->bParkingBrakeToggle->Location = System::Drawing::Point(291, 269);
			this->bParkingBrakeToggle->Name = L"bParkingBrakeToggle";
			this->bParkingBrakeToggle->Size = System::Drawing::Size(75, 23);
			this->bParkingBrakeToggle->TabIndex = 102;
			this->bParkingBrakeToggle->Text = L"Toggle";
			this->bParkingBrakeToggle->UseVisualStyleBackColor = true;
			this->bParkingBrakeToggle->Click += gcnew System::EventHandler(this, &MainForm::bParkingBrakeToggle_Click);
			// 
			// gbADFmode
			// 
			this->gbADFmode->Controls->Add(this->rbADF0);
			this->gbADFmode->Controls->Add(this->rbADF1);
			this->gbADFmode->Controls->Add(this->rbADF2);
			this->gbADFmode->Controls->Add(this->rbADF3);
			this->gbADFmode->Location = System::Drawing::Point(176, 207);
			this->gbADFmode->Name = L"gbADFmode";
			this->gbADFmode->Size = System::Drawing::Size(77, 141);
			this->gbADFmode->TabIndex = 108;
			this->gbADFmode->TabStop = false;
			this->gbADFmode->Text = L"ADF Mode";
			// 
			// rbADF0
			// 
			this->rbADF0->AutoSize = true;
			this->rbADF0->Location = System::Drawing::Point(11, 42);
			this->rbADF0->Name = L"rbADF0";
			this->rbADF0->Size = System::Drawing::Size(55, 17);
			this->rbADF0->TabIndex = 108;
			this->rbADF0->TabStop = true;
			this->rbADF0->Text = L"0 BFO";
			this->rbADF0->UseVisualStyleBackColor = true;
			this->rbADF0->CheckedChanged += gcnew System::EventHandler(this, &MainForm::rbADF0_CheckedChanged);
			// 
			// rbADF1
			// 
			this->rbADF1->AutoSize = true;
			this->rbADF1->Location = System::Drawing::Point(11, 65);
			this->rbADF1->Name = L"rbADF1";
			this->rbADF1->Size = System::Drawing::Size(56, 17);
			this->rbADF1->TabIndex = 109;
			this->rbADF1->TabStop = true;
			this->rbADF1->Text = L"1 REC";
			this->rbADF1->UseVisualStyleBackColor = true;
			this->rbADF1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::rbADF1_CheckedChanged);
			// 
			// rbADF2
			// 
			this->rbADF2->AutoSize = true;
			this->rbADF2->Location = System::Drawing::Point(11, 88);
			this->rbADF2->Name = L"rbADF2";
			this->rbADF2->Size = System::Drawing::Size(55, 17);
			this->rbADF2->TabIndex = 110;
			this->rbADF2->TabStop = true;
			this->rbADF2->Text = L"2 ADF";
			this->rbADF2->UseVisualStyleBackColor = true;
			this->rbADF2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::rbADF2_CheckedChanged);
			// 
			// rbADF3
			// 
			this->rbADF3->AutoSize = true;
			this->rbADF3->Location = System::Drawing::Point(12, 111);
			this->rbADF3->Name = L"rbADF3";
			this->rbADF3->Size = System::Drawing::Size(62, 17);
			this->rbADF3->TabIndex = 111;
			this->rbADF3->TabStop = true;
			this->rbADF3->Text = L"3 TEST";
			this->rbADF3->UseVisualStyleBackColor = true;
			this->rbADF3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::rbADF3_CheckedChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(522, 360);
			this->Controls->Add(this->gbADFmode);
			this->Controls->Add(this->bParkingBrakeToggle);
			this->Controls->Add(this->tParkingBrakeValue);
			this->Controls->Add(this->lblParkingBrake);
			this->Controls->Add(this->lblCabinLights);
			this->Controls->Add(this->tCabinLightsValue);
			this->Controls->Add(this->bCabinLightsON);
			this->Controls->Add(this->bCabinLightsOFF);
			this->Controls->Add(this->bConnectMsfs);
			this->Controls->Add(this->bConnectWasm);
			this->Controls->Add(this->tbMsfsStatus);
			this->Controls->Add(this->tbWasmStatus);
			this->Name = L"MainForm";
			this->Text = L"KK2023";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->gbADFmode->ResumeLayout(false);
			this->gbADFmode->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e)
		{
			this->tbMsfsStatus->BackColor = System::Drawing::Color::Yellow;
			this->tbMsfsStatus->Text = L"Please connect to MSFS";
			this->tbWasmStatus->BackColor = System::Drawing::Color::Yellow;
			this->tbWasmStatus->Text = L"Please connect to WASM";
		}
#pragma endregion

#pragma region MSFS Connection

		private: System::Void bConnectMsfs_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (bMsfsConnected == false)
			{
				openSimConnect();
				if (bMsfsConnected == true)
				{
					this->tbMsfsStatus->BackColor = System::Drawing::Color::LightGreen;
					this->tbMsfsStatus->Text = L"Connected to MSFS";
					this->bConnectMsfs->Text = L"Disconnect from MSFS";
				}
				else
				{
					this->tbMsfsStatus->BackColor = System::Drawing::Color::Red;
					this->tbMsfsStatus->Text = L"Connection to MSFS failed";
				}
			}
			else
			{
				closeSimConnect();
				this->tbMsfsStatus->BackColor = System::Drawing::Color::Yellow;
				this->tbMsfsStatus->Text = L"Disconnected from MSFS";
				this->bConnectMsfs->Text = L"Connect to MSFS";
			}
		}
#pragma endregion

#pragma region WASM Connection

		private: System::Void bConnectWasm_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (bWasmConnected == false)
			{
				openFsuipcWasm();
				if (bWasmConnected == true)
				{
					this->tbWasmStatus->BackColor = System::Drawing::Color::LightGreen;
					this->tbWasmStatus->Text = L"Connected to WASM";
					this->bConnectWasm->Text = L"Disconnect from WASM";
				}
				else
				{
					this->tbWasmStatus->BackColor = System::Drawing::Color::Red;
					this->tbWasmStatus->Text = L"Connection to WASM failed";
				}
			}
			else
			{
				closeFsuipcWasm();
				this->tbWasmStatus->BackColor = System::Drawing::Color::Yellow;
				this->tbWasmStatus->Text = L"Disconnected from WASM";
				this->bConnectWasm->Text = L"Connect to WASM";
			}
		}
			   //  if (!wasmPtr) wasmPtr = WASMIF::GetInstance();
			//	wasmPtr->setLogLevel((LOGLEVEL)logLevel);
			//	wasmPtr->setSimConfigConnection(defaults->GetSimConnectConnection());
			//	if (lvarUpdateFrequency)
			//	{
			//		wasmPtr->setLvarUpdateFrequency(lvarUpdateFrequency);
			//	}
			//	if (wasmPtr->start())
			//	{
			//		bWASMConnected = 1;
			//		this->tbWasmStatus->BackColor = System::Drawing::Color::LightGreen;
			//		this->tbWasmStatus->Text = L"Connected to WASM";
			//	}
			//	else
			//	{
			//		bWASMConnected = 0;
			//		this->tbWasmStatus->BackColor = System::Drawing::Color::Red;
			//		this->tbWasmStatus->Text = L"Connection to WASM failed";
			//	}
#pragma endregion

#pragma region SimConnect Events and SimVars

		private: System::Void bChange0_Click(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_TransmitClientEvent(hSimConnect, 0, CABIN_LIGHTS_OFF, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			this-> tCabinLightsValue->Text = L"OFF";
		}

		private: System::Void bChange1_Click(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_TransmitClientEvent(hSimConnect, 0, CABIN_LIGHTS_ON, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			this-> tCabinLightsValue->Text = L"ON";
		}

		private: System::Void bParkingBrakeToggle_Click(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_TransmitClientEvent(hSimConnect, 0, PARKING_BRAKE_TOGGLE, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			this->tParkingBrakeValue->Text = L"Toggle";
		}

#pragma endregion

#pragma region WASM Lvars
		private: System::Void rbADF0_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:XMLVAR_ADF_Mode", "number", SIMCONNECT_DATATYPE_FLOAT64, 0, SIMCONNECT_UNUSED);
			SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_1, 0, SIMCONNECT_DATA_SET_FLAG_DEFAULT, 0, 0, 0);
		}
		private: System::Void rbADF1_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:XMLVAR_ADF_Mode", "number", SIMCONNECT_DATATYPE_FLOAT64, 0, 1);
		}
		private: System::Void rbADF2_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:XMLVAR_ADF_Mode", "number", SIMCONNECT_DATATYPE_FLOAT64, 0, 2);
		}
		private: System::Void rbADF3_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:XMLVAR_ADF_Mode", "number", SIMCONNECT_DATATYPE_FLOAT64, 0, 3);
		}
#pragma endregion
	};
}
