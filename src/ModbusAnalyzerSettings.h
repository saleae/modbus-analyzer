#ifndef MODBUS_ANALYZER_SETTINGS
#define MODBUS_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

#ifdef __GNUC__
#include <stdio.h>
#include <string.h>
#endif

namespace ModbusAnalyzerEnums // Note: There is another definition of enum Mode in the cpp file.
{
    // enum Mode { Normal, MpModeMsbZeroMeansAddress, MpModeMsbOneMeansAddress, ModbusRTUClient, ModbusRTUServer, ModbusASCIIClient,
    // ModbusASCIIServer };
    enum Mode
    {
        ModbusRTUClient,
        ModbusRTUServer,
        ModbusASCIIClient,
        ModbusASCIIServer,
        ModbusRTUBoth, // Client and Server
        ModbusASCIIBoth,
        Normal,
        MpModeMsbZeroMeansAddress,
        MpModeMsbOneMeansAddress
    };
    enum ParityAndStopbits
    {
        NoneTwo = AnalyzerEnums::Parity::None,
        EvenOne = AnalyzerEnums::Parity::Even,
        OddOne = AnalyzerEnums::Parity::Odd,
        NoneOne
    };
}

class ModbusAnalyzerSettings : public AnalyzerSettings
{
  public:
    ModbusAnalyzerSettings();
    virtual ~ModbusAnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings( const char* settings );
    virtual const char* SaveSettings();

    // User-modifiable settings - page 35.
    Channel mInputChannel;
    U32 mBitRate;
    U32 mBitsPerTransfer;
    AnalyzerEnums::ShiftOrder mShiftOrder;
    ModbusAnalyzerEnums::ParityAndStopbits mParity;
    bool mInverted;
    bool mAssumeDataStartsWithResponse;
    bool mUseAutobaud;
    ModbusAnalyzerEnums::Mode mModbusMode;

  protected:
    // AnalyzerSettingsInterfaces - page 36.
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
    std::auto_ptr<AnalyzerSettingInterfaceInteger> mBitRateInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList> mBitsPerTransferInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList> mShiftOrderInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList> mParityInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList> mInvertedInterface;
    std::auto_ptr<AnalyzerSettingInterfaceBool> mStartsWithResponseInterface;
    std::auto_ptr<AnalyzerSettingInterfaceBool> mUseAutobaudInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList> mModbusModeInterface;
};

#endif // MODBUS_ANALYZER_SETTINGS
