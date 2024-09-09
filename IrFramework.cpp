#define RECORD_GAP_MICROS 12000
#include "IRremote.hpp"
#include "IrFramework.hpp"
#include "ac_LG.hpp"

IrFramework::IrFramework(size_t ir_receiver_pin, size_t ir_sender_pin)
{
    IrReceiver.begin(ir_receiver_pin, DISABLE_LED_FEEDBACK);  
    IrSender.begin(ir_sender_pin);
    IrSender.enableIROut(38);
}

IrFramework::~IrFramework()
{
    IrReceiver.stop();
}

bool IrFramework::read_signal() {
    if (IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.resume();
        return true;
    }
    return false;
}

// TODO: Implement signal repeat
void IrFramework::send_protocol_signal(RecordedSignal record) {
    switch (record.protocol)
    {
    // case decode_type_t::APPLE:
    //     IrSender.sendApple(record.extra, record.command, 0);
    case decode_type_t::BANG_OLUFSEN:
        IrSender.sendBangOlufsenRaw(record.decodedRawData, record.numberOfBits);
        break;
    case decode_type_t::BOSEWAVE:
        IrSender.sendBoseWave(record.command);
        break;
    case decode_type_t::DENON:
        IrSender.sendDenon(record.address, record.command, 0);
        break;
    case decode_type_t::FAST:
        IrSender.sendFAST(record.command, 0);
        break;
    case decode_type_t::JVC:
        IrSender.sendJVC(record.decodedRawData, record.numberOfBits, false);
        break;
    case decode_type_t::KASEIKYO:
        IrSender.sendKaseikyo(record.address, record.command, 0, record.extra); // Maybe wrong record.extra?
        break;
    case decode_type_t::KASEIKYO_DENON:
        IrSender.sendKaseikyo_Denon(record.address, record.command, 0);
        break;
    case decode_type_t::KASEIKYO_JVC:
        IrSender.sendKaseikyo_JVC(record.address, record.command, 0);
        break;
    case decode_type_t::KASEIKYO_MITSUBISHI:
        IrSender.sendKaseikyo_Mitsubishi(record.address, record.command, 0);
        break;
    case decode_type_t::KASEIKYO_SHARP:
        IrSender.sendKaseikyo_Sharp(record.address, record.command, 0);
        break;
    case decode_type_t::LEGO_PF:
        IrSender.sendLegoPowerFunctions(record.decodedRawData);
        break;
    case decode_type_t::LG2:
        IrSender.sendLG2(record.address, record.command, 0);
        break;
    case decode_type_t::LG:
        IrSender.sendLG(record.address, record.command, 0);
        break;
    // case decode_type_t::MAGIQUEST:
    //     IrSender.sendMagiQuest(record)
    case decode_type_t::NEC2:
        IrSender.sendNEC2(record.address, record.command, 0);
        break;
    case decode_type_t::NEC:
        IrSender.sendNEC(record.address, record.command, 0);
        break;
    case decode_type_t::ONKYO:
        IrSender.sendOnkyo(record.address, record.command, 0);
        break;
    case decode_type_t::PANASONIC:
        IrSender.sendPanasonic(record.address,record.command, 0);
        break;
    case decode_type_t::RC5:
        IrSender.sendRC5(record.address, record.command, 0);
        break;
    case decode_type_t::RC6:
        IrSender.sendRC6Raw(record.decodedRawData, record.numberOfBits);
        break;
    case decode_type_t::SAMSUNG48:
        IrSender.sendSamsung48(record.address, record.command, 0);
        break;
    case decode_type_t::SAMSUNG:
        IrSender.sendSamsung(record.address, record.command, 0);
        break;
    case decode_type_t::SAMSUNGLG:
        IrSender.sendSamsungLG(record.address, record.command, 0);
        break;
    case decode_type_t::SHARP:
        IrSender.sendSharp(record.address, record.command, 0);
        break;
    case decode_type_t::SONY:
        IrSender.sendSony(record.address, record.command, 0, record.numberOfBits);
        break;
    case decode_type_t::WHYNTER:
        IrSender.sendWhynter(record.decodedRawData, record.numberOfBits);
        break;
    case decode_type_t::PULSE_WIDTH:    /* Sometimes the timing in IRData struct(RecordedSignal struct for us) is wrong. Send RawData with timing to avoid issues*/
    case decode_type_t::PULSE_DISTANCE:
    case decode_type_t::UNKNOWN:
    default:
        IrSender.sendRaw_P(record.decodedRawDataArray, record.raw_len, 38); // Assuming 38khz as frequency
        break;
    }
}



void IrFramework::send_lg_air_conditioner_signal(bool wall_type, char command, int param) {
    Aircondition_LG MyLG_Aircondition;
    MyLG_Aircondition.setType(LG_IS_WALL_TYPE);
    MyLG_Aircondition.sendCommandAndParameter(command, param);
}

RecordedSignal IrFramework::get_decoded_signal() {
    IRData signal = IrReceiver.decodedIRData;
    RecordedSignal wrapped_signal = RecordedSignal {
        .protocol = signal.protocol,
        .address = signal.address,
        .command = signal.command,
        .raw_len = signal.rawDataPtr->rawlen - 1,
        .numberOfBits = signal.numberOfBits,
        .flags = signal.flags,
        .extra = signal.extra,
        .decodedRawData = signal.decodedRawData
    };
    IrReceiver.compensateAndStoreIRResultInArray(wrapped_signal.decodedRawDataArray);
    return wrapped_signal;
}

String IrFramework::enum_to_str(size_t val) {
    switch (val) {
    case decode_type_t::UNKNOWN:
        return "Unknown";
    case decode_type_t::PULSE_WIDTH:
        return "Pulse width";
    case decode_type_t::PULSE_DISTANCE:
        return "Pulse distance";
    case decode_type_t::APPLE:
        return "Apple";
    case decode_type_t::DENON:
        return "Denon";
    case decode_type_t::JVC:
        return "JVC";
    case decode_type_t::LG:
        return "LG";
    case decode_type_t::LG2:
        return "LG2";
    case decode_type_t::NEC:
        return "NEC";
    case decode_type_t::NEC2:
        return "NEC2";
    case decode_type_t::ONKYO:
        return "Onkyo";
    case decode_type_t::PANASONIC:
        return "Panasonic";
    case decode_type_t::KASEIKYO:
        return "Kaseikyo";
    case decode_type_t::KASEIKYO_DENON:
        return "Kaseikyo Denon";
    case decode_type_t::KASEIKYO_SHARP:
        return "Kaseikyo Sharp";
    case decode_type_t::KASEIKYO_JVC:
        return "Kaseikyo JVC";
    case decode_type_t::KASEIKYO_MITSUBISHI:
        return "Kaseikyo Mitsubishi";
    case decode_type_t::RC5:
        return "RC5";
    case decode_type_t::RC6:
        return "RC6";
    case decode_type_t::SAMSUNG:
        return "Samsung"; /* 20*/
    case decode_type_t::SAMSUNGLG:
        return "SamsungLG";
    case decode_type_t::SAMSUNG48:
        return "Samsung48";
    case decode_type_t::SHARP:
        return "Sharp";
    case decode_type_t::SONY:
        return "Sony";
    case decode_type_t::BANG_OLUFSEN:
        return "Bang olufsen";
    case decode_type_t::BOSEWAVE:
        return "Bose";
    case decode_type_t::LEGO_PF:
        return "Lego";
    case decode_type_t::MAGIQUEST:
        return "Magiquest";
    case decode_type_t::WHYNTER:
        return "Whynter";
    case decode_type_t::FAST:
        return "Fast";
    default:
        Serial.println(val);
        return "Unknown, open an issue";
        break;
    }
}