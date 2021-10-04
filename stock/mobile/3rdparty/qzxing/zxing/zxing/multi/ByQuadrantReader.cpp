/*
 *  Copyright 2011 ZXing authors All rights reserved.
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

#include <zxing/multi/ByQuadrantReader.h>
#include <zxing/ReaderException.h>

namespace zxing {
namespace multi {

ByQuadrantReader::ByQuadrantReader(Reader& delegate) : delegate_(delegate) {}

ByQuadrantReader::~ByQuadrantReader(){}

QSharedPointer<Result> ByQuadrantReader::decode(QSharedPointer<BinaryBitmap> image){
  return decode(image, DecodeHints::DEFAULT_HINT);
}

QSharedPointer<Result> ByQuadrantReader::decode(QSharedPointer<BinaryBitmap> image, DecodeHints hints){
  int width = image->getWidth();
  int height = image->getHeight();
  int halfWidth = width / 2;
  int halfHeight = height / 2;
  QSharedPointer<BinaryBitmap> topLeft = image->crop(0, 0, halfWidth, halfHeight);
  try {
    return delegate_.decode(topLeft, hints);
  } catch (ReaderException const& re) {
    (void)re;
    // continue
  }

  QSharedPointer<BinaryBitmap> topRight = image->crop(halfWidth, 0, halfWidth, halfHeight);
  try {
    return delegate_.decode(topRight, hints);
  } catch (ReaderException const& re) {
    (void)re;
    // continue
  }

  QSharedPointer<BinaryBitmap> bottomLeft = image->crop(0, halfHeight, halfWidth, halfHeight);
  try {
    return delegate_.decode(bottomLeft, hints);
  } catch (ReaderException const& re) {
    (void)re;
    // continue
  }

  QSharedPointer<BinaryBitmap> bottomRight = image->crop(halfWidth, halfHeight, halfWidth, halfHeight);
  try {
    return delegate_.decode(bottomRight, hints);
  } catch (ReaderException const& re) {
    (void)re;
    // continue
  }

  int quarterWidth = halfWidth / 2;
  int quarterHeight = halfHeight / 2;
  QSharedPointer<BinaryBitmap> center = image->crop(quarterWidth, quarterHeight, halfWidth, halfHeight);
  return delegate_.decode(center, hints);
}

} // End zxing::multi namespace
} // End zxing namespace
