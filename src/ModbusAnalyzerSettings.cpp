#include "ModbusAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>

#ifndef __GNUC__
#pragma warning( disable : 4800 ) // warning C4800: 'U32' : forcing value to bool 'true' or 'false' (performance warning)
#endif


ModbusAnalyzerSettings::ModbusAnalyzerSettings()
    : mInputChannel( UNDEFINED_CHANNEL ),
      mBitRate( 9600 ),
      mBitsPerTransfer( 8 ),
      mShiftOrder( AnalyzerEnums::LsbFirst ),
      mParity( ModbusAnalyzerEnums::ParityAndStopbits::EvenOne ),
      mInverted( false ),
      mUseAutobaud( false ),
      mAssumeDataStartsWithResponse( false ),
      mModbusMode( ModbusAnalyzerEnums::ModbusRTUClient )
{
    mParityInterface.reset( new AnalyzerSettingInterfaceNumberList() );
    mParityInterface->SetTitleAndTooltip( "Parity Bit", "Specify None, Even, or Odd Parity" );
    mParityInterface->AddNumber( ModbusAnalyzerEnums::ParityAndStopbits::EvenOne, "Even Parity Bit (default)", "" );
    mParityInterface->AddNumber( ModbusAnalyzerEnums::ParityAndStopbits::OddOne, "Odd Parity Bit", "" );
    mParityInterface->AddNumber( ModbusAnalyzerEnums::ParityAndStopbits::NoneTwo, "No Parity Bit, 2 stop bits", "" );
    mParityInterface->AddNumber( ModbusAnalyzerEnums::ParityAndStopbits::NoneOne, "No Parity Bit, 1 stop bit", "" );
    mParityInterface->SetNumber( mParity );


    mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mInputChannelInterface->SetTitleAndTooltip( "Modbus",
                                                "Modbus" ); // Todo: Change to reflect specific variant of Modbus later in program.
    mInputChannelInterface->SetChannel( mInputChannel );


    mModbusModeInterface.reset( new AnalyzerSettingInterfaceNumberList() );
    mModbusModeInterface->SetTitleAndTooltip( "Modbus Mode", "Specify which mode of Modbus this is" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusRTUClient, "RTU - Client", "Messages are transmitted in binary" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusRTUServer, "RTU - Server", "Messages are transmitted in binary" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusRTUBoth, "RTU - Client & Server", "Messages are transmitted in binary" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusASCIIClient, "ASCII - Client",
                                     "Messages are transmitted in ASCII-readable format" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusASCIIServer, "ASCII - Server",
                                     "Messages are transmitted in ASCII-readable format" );
    mModbusModeInterface->AddNumber( ModbusAnalyzerEnums::ModbusASCIIBoth, "ASCII - Client & Server",
                                     "Messages are transmitted in ASCII-readable format" );
    mModbusModeInterface->SetNumber( mModbusMode );


    mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/s)", "" );
    mBitRateInterface->SetMax( 6000000 );
    mBitRateInterface->SetMin( 1 );
    mBitRateInterface->SetInteger( mBitRate );


    mInvertedInterface.reset( new AnalyzerSettingInterfaceNumberList() );
    mInvertedInterface->SetTitleAndTooltip( "Signal Inversion", "Specify if the serial signal is inverted" );
    mInvertedInterface->AddNumber( false, "Non Inverted (Standard)", "" );
    mInvertedInterface->AddNumber( true, "Inverted", "" );
    mInvertedInterface->SetNumber( mInverted );

    mStartsWithResponseInterface.reset( new AnalyzerSettingInterfaceBool() );
    mStartsWithResponseInterface->SetTitleAndTooltip( "Starts with response", "Specify if the serial signal starts with a response, for client/server decoding" );
    mStartsWithResponseInterface->SetValue ( mAssumeDataStartsWithResponse );

    enum Mode
    {
        Normal,
        MpModeRightZeroMeansAddress,
        MpModeRightOneMeansAddress,
        MpModeLeftZeroMeansAddress,
        MpModeLeftOneMeansAddress
    }; // FIXME: unused?


    AddInterface( mInputChannelInterface.get() );
    AddInterface( mModbusModeInterface.get() );
    AddInterface( mBitRateInterface.get() );
    AddInterface( mInvertedInterface.get() );
    AddInterface( mStartsWithResponseInterface.get() );
    AddInterface( mParityInterface.get() );


    // AddExportOption( 0, "Export as text/csv file", "text (*.txt);;csv (*.csv)" );
    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    ClearChannels();
    AddChannel( mInputChannel, "Modbus", false );
}

ModbusAnalyzerSettings::~ModbusAnalyzerSettings()
{
}

bool ModbusAnalyzerSettings::SetSettingsFromInterfaces()
{
    /*
     * 	if( AnalyzerEnums::Parity( U32( mParityInterface->GetNumber() ) ) != AnalyzerEnums::None )
            if( ModbusAnalyzerEnums::Mode( U32( mModbusModeInterface->GetNumber() ) ) != ModbusAnalyzerEnums::Normal )
            {
                SetErrorText( "Sorry, but we don't support using parity at the same time as MP mode." );
                return false;
            }
    */
    mInputChannel = mInputChannelInterface->GetChannel();
    mBitRate = mBitRateInterface->GetInteger();
    // mBitsPerTransfer = U32( mBitsPerTransferInterface->GetNumber() );
    // mStopBits = mStopBitsInterface->GetNumber();
    mParity = ModbusAnalyzerEnums::ParityAndStopbits( U32( mParityInterface->GetNumber() ) );
    // mShiftOrder =  AnalyzerEnums::ShiftOrder( U32( mShiftOrderInterface->GetNumber() ) );
    mInverted = bool( U32( mInvertedInterface->GetNumber() ) );
    mAssumeDataStartsWithResponse = mStartsWithResponseInterface->GetValue();
    // mUseAutobaud = mUseAutobaudInterface->GetValue();
    mModbusMode = ModbusAnalyzerEnums::Mode( U32( mModbusModeInterface->GetNumber() ) );

    ClearChannels();
    AddChannel( mInputChannel, "Modbus", true );

    return true;
}

void ModbusAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel( mInputChannel );
    mBitRateInterface->SetInteger( mBitRate );
    // mBitsPerTransferInterface->SetNumber( mBitsPerTransfer );
    // mStopBitsInterface->SetNumber( mStopBits );
    mParityInterface->SetNumber( mParity );
    // mShiftOrderInterface->SetNumber( mShiftOrder );
    mInvertedInterface->SetNumber( mInverted );
    mStartsWithResponseInterface->SetValue( mAssumeDataStartsWithResponse );
    // mUseAutobaudInterface->SetValue( mUseAutobaud );
    mModbusModeInterface->SetNumber( mModbusMode );
}

void ModbusAnalyzerSettings::LoadSettings( const char* settings )
{
    // Example: $3 = 0x1647478 "22 serialization::archive 5 22 ModbusAnalyzerSettings 0 8 1324695933 0 9600"
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    const char* name_string; // the first thing in the archive is the name of the protocol analyzer that the data belongs to.
    text_archive >> &name_string;
    if( strcmp( name_string, "ModbusAnalyzerSettings" ) != 0 )
    {
        if( strcmp( name_string, "SaleaeAsyncModbusAnalyzer" ) != 0 )
        { // The old string; treat them the same for now.
            AnalyzerHelpers::Assert( "ModbusAnalyzerSettings: Provided with a settings string that doesn't belong to us;" );
        }
    }
    text_archive >> mInputChannel;
    text_archive >> mBitRate;
    // text_archive >> mBitsPerTransfer;
    // text_archive >> mStopBits;
    // text_archive >> *(U32*)&mParity;
    // text_archive >> *(U32*)&mShiftOrder;
    text_archive >> mInverted;

    // check to make sure loading it actual works befor assigning the result -- do this when adding settings to an anylzer which has been
    // previously released. bool use_autobaud; if( text_archive >> use_autobaud ) 	mUseAutobaud = use_autobaud;

    ModbusAnalyzerEnums::Mode mode;
    if( text_archive >> *( U32* )&mode )
        mModbusMode = mode;

    // added for 1.2.14
    ModbusAnalyzerEnums::ParityAndStopbits parity;
    if( text_archive >> *( U32* )&parity )
        mParity = parity;


    ClearChannels();
    AddChannel( mInputChannel, "Modbus", true );

    UpdateInterfacesFromSettings();
}

const char* ModbusAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << "ModbusAnalyzerSettings";
    text_archive << mInputChannel;
    text_archive << mBitRate;
    // text_archive << mBitsPerTransfer;
    // text_archive << mStopBits;
    // text_archive << mParity;
    // text_archive << mShiftOrder;
    text_archive << mInverted;
    text_archive << mAssumeDataStartsWithResponse;

    // text_archive << mUseAutobaud;

    text_archive << mModbusMode;

    // added for 1.2.14
    text_archive << mParity;

    return SetReturnString( text_archive.GetString() );
}
