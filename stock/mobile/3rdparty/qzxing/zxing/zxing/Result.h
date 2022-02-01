#ifndef ZXING_RESULT_H
#define ZXING_RESULT_H

/*
 *  Result.h
 *  zxing
 *
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

#include <string>
#include <QSharedPointer>
#include <zxing/common/Str.h>
#include <zxing/common/Types.h>
#include <zxing/ResultPoint.h>
#include <zxing/ResultMetadata.h>
#include <zxing/BarcodeFormat.h>

namespace zxing {

class Result  {
private:
  QSharedPointer<String> text_;
  QSharedPointer<std::vector<zxing::byte>> rawBytes_;
  QSharedPointer<std::vector<QSharedPointer<ResultPoint>> > resultPoints_;
  BarcodeFormat format_;
  std::string charSet_;
  ResultMetadata metadata_;

public:
  Result(QSharedPointer<String> text,
         QSharedPointer<std::vector<zxing::byte>> rawBytes,
         QSharedPointer<std::vector<QSharedPointer<ResultPoint>> > resultPoints,
         BarcodeFormat format, const std::string &charSet = "",
         ResultMetadata metadata = ResultMetadata());
  ~Result();
  QSharedPointer<String> getText();
  QSharedPointer<std::vector<zxing::byte>> getRawBytes();
  QSharedPointer<std::vector<QSharedPointer<ResultPoint>> > const& getResultPoints() const;
  QSharedPointer<std::vector<QSharedPointer<ResultPoint>> >& getResultPoints();
  BarcodeFormat getBarcodeFormat() const;
  std::string getCharSet() const;
  ResultMetadata& getMetadata();

  friend std::ostream& operator<<(std::ostream &out, Result& result);
};

}
#endif // ZXING_RESULT_H
