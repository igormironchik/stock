// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-
#ifndef ZXING_UPC_EAN_READER_H
#define ZXING_UPC_EAN_READER_H

/*
 *  Copyright 2010 ZXing authors All rights reserved.
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
 */

#include <zxing/oned/OneDReader.h>
#include <zxing/oned/UPCEANExtensionSupport.h>
#include <zxing/oned/EANManufacturerOrgSupport.h>
#include <zxing/common/BitArray.h>
#include <zxing/Result.h>

namespace zxing {
namespace oned {

class UPCEANReader : public OneDReader {
 private:
  std::string decodeRowStringBuffer;
  UPCEANExtensionSupport extensionReader;
  EANManufacturerOrgSupport eanManSupport;

  static const int MAX_AVG_VARIANCE;
  static const int MAX_INDIVIDUAL_VARIANCE;

  static Range findStartGuardPattern(QSharedPointer<BitArray> row);

  virtual Range decodeEnd(QSharedPointer<BitArray> row, int endStart);

  static bool checkStandardUPCEANChecksum(QSharedPointer<String> const& s);

  static Range findGuardPattern(QSharedPointer<BitArray> row,
                                int rowOffset,
                                bool whiteFirst,
                                std::vector<int> const& pattern,
                                std::vector<int>& counters);


public:
  static const std::vector<int> START_END_PATTERN;
  static const std::vector<int> MIDDLE_PATTERN;

  static const std::vector<int const*> L_PATTERNS;
  static const std::vector<int const*> L_AND_G_PATTERNS;

  static Range findGuardPattern(QSharedPointer<BitArray> row,
                                int rowOffset,
                                bool whiteFirst,
                                std::vector<int> const& pattern);

public:
  UPCEANReader();

  virtual int decodeMiddle(QSharedPointer<BitArray> row,
                           Range const& startRange,
                           std::string& resultString) = 0;

  virtual QSharedPointer<Result> decodeRow(int rowNumber, QSharedPointer<BitArray> row, zxing::DecodeHints hints);
  virtual QSharedPointer<Result> decodeRow(int rowNumber, QSharedPointer<BitArray> row, Range const& range, zxing::DecodeHints hints = DecodeHints());

  static int decodeDigit(QSharedPointer<BitArray> row,
                         std::vector<int>& counters,
                         int rowOffset,
                         std::vector<int const*> const& patterns);

  virtual bool checkChecksum(QSharedPointer<String> const& s);

  virtual BarcodeFormat getBarcodeFormat() = 0;
  virtual ~UPCEANReader();

  friend class MultiFormatUPCEANReader;
};

}
}

#endif // ZXING_UPC_EAN_READER_H

