#ifndef DECODED_NUMERIC_H
#define DECODED_NUMERIC_H

/*
 * Copyright (C) 2010 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 2019-07-17 translation from Java into C++
 */

/*
 * These authors would like to acknowledge the Spanish Ministry of Industry,
 * Tourism and Trade, for the support in the project TSI020301-2008-2
 * "PIRAmIDE: Personalizable Interactions with Resources on AmI-enabled
 * Mobile Dynamic Environments", led by Treelogic
 * ( http://www.treelogic.com/ ):
 *
 *   http://www.piramidepse.com/
 */

#include "DecodedObject.h"

#include <zxing/FormatException.h>

namespace zxing {

namespace oned {

namespace rss {

class DecodedNumeric : public DecodedObject
{

public:
    static const int FNC1 = 10;
    DecodedNumeric(int newPosition, int firstDigit, int secondDigit);

    DecodedNumeric(const QSharedPointer<DecodedNumeric> other);

    int getFirstDigit() const;

    int getSecondDigit() const;

    int getValue() const;

    bool isFirstDigitFNC1() const;

    bool isSecondDigitFNC1() const;

private:
    int m_firstDigit;
    int m_secondDigit;

};

}
}
}

#endif
