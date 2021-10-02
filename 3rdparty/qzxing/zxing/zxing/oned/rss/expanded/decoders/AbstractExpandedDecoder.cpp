#include "AbstractExpandedDecoder.h"

#include "AI01AndOtherAIs.h"
#include "AI013x0x1xDecoder.h"
#include "AnyAIDecoder.h"
#include "AI013103decoder.h"
#include "AI01320xDecoder.h"
#include "AI01392xDecoder.h"
#include "AI01393xDecoder.h"

namespace zxing {
namespace oned {
namespace rss {

AbstractExpandedDecoder::AbstractExpandedDecoder(QSharedPointer<BitArray> information)
    : m_information(information), m_generalDecoder(GeneralAppIdDecoder(information))
{

}

QSharedPointer<BitArray> AbstractExpandedDecoder::getInformation() const
{
    return m_information;
}

GeneralAppIdDecoder AbstractExpandedDecoder::getGeneralDecoder()
{
    return m_generalDecoder;
}

QSharedPointer<AbstractExpandedDecoder> AbstractExpandedDecoder::createDecoder(QSharedPointer<BitArray> information)
{
    if (information->get(1)) {
        return QSharedPointer<AbstractExpandedDecoder>(new AI01AndOtherAIs(information));
    }
    if (!information->get(2)) {
        return QSharedPointer<AbstractExpandedDecoder>(new AnyAIDecoder(information));
    }

    int fourBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 4);

    switch (fourBitEncodationMethod) {
    case 4: return QSharedPointer<AbstractExpandedDecoder>(new AI013103decoder(information));
    case 5: return QSharedPointer<AbstractExpandedDecoder>(new AI01320xDecoder(information));
    }

    int fiveBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 5);
    switch (fiveBitEncodationMethod) {
    case 12: return QSharedPointer<AbstractExpandedDecoder>(new AI01392xDecoder(information));
    case 13: return QSharedPointer<AbstractExpandedDecoder>(new AI01393xDecoder(information));
    }

    int sevenBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 7);
    switch (sevenBitEncodationMethod) {
    case 56: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("310"), String("11")));
    case 57: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("320"), String("11")));
    case 58: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("310"), String("13")));
    case 59: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("320"), String("13")));
    case 60: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("310"), String("15")));
    case 61: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("320"), String("15")));
    case 62: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("310"), String("17")));
    case 63: return QSharedPointer<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, String("320"), String("17")));
    }

    throw IllegalStateException();
}

}
}
}
