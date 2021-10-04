// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-
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

#include <zxing/ZXing.h>
#include <zxing/oned/MultiFormatOneDReader.h>
#include <zxing/oned/MultiFormatUPCEANReader.h>
#include <zxing/oned/Code39Reader.h>
#include <zxing/oned/Code128Reader.h>
#include <zxing/oned/Code93Reader.h>
#include <zxing/oned/CodaBarReader.h>
#include <zxing/oned/ITFReader.h>
#include <zxing/oned/rss/RSS14Reader.h>
#include <zxing/oned/rss/expanded/RSSExpandedReader.h>
#include <zxing/ReaderException.h>
#include <zxing/NotFoundException.h>

using zxing::Result;
using zxing::oned::MultiFormatOneDReader;
using zxing::oned::rss::RSS14Reader;
using zxing::oned::rss::RSSExpandedReader;

// VC++
using zxing::BitArray;
using zxing::DecodeHints;

MultiFormatOneDReader::MultiFormatOneDReader(DecodeHints hints) : readers()
{
  if (hints.containsFormat(BarcodeFormat::EAN_13) ||
      hints.containsFormat(BarcodeFormat::EAN_8) ||
      hints.containsFormat(BarcodeFormat::UPC_A) ||
      hints.containsFormat(BarcodeFormat::UPC_E))
  {
    readers.push_back(QSharedPointer<OneDReader>(new MultiFormatUPCEANReader(hints)));
  }
  if (hints.containsFormat(BarcodeFormat::CODE_39))
  {
    readers.push_back(QSharedPointer<OneDReader>(new Code39Reader()));
  }
  if (hints.containsFormat(BarcodeFormat::CODE_93))
  {
    readers.push_back(QSharedPointer<OneDReader>(new Code93Reader()));
  }
  if (hints.containsFormat(BarcodeFormat::CODE_128))
  {
    readers.push_back(QSharedPointer<OneDReader>(new Code128Reader()));
  }
  if (hints.containsFormat(BarcodeFormat::ITF))
  {
    readers.push_back(QSharedPointer<OneDReader>(new ITFReader()));
  }
  if (hints.containsFormat(BarcodeFormat::CODABAR))
  {
    readers.push_back(QSharedPointer<OneDReader>(new CodaBarReader()));
  }

  if (hints.containsFormat(BarcodeFormat::RSS_14))
  {
    readers.push_back(QSharedPointer<OneDReader>(new RSS14Reader()));
  }

  if (hints.containsFormat(BarcodeFormat::RSS_EXPANDED))
  {
    readers.push_back(QSharedPointer<OneDReader>(new RSSExpandedReader()));
  }

  if (readers.size() == 0)
  {
    readers.push_back(QSharedPointer<OneDReader>(new MultiFormatUPCEANReader(hints)));
    readers.push_back(QSharedPointer<OneDReader>(new Code39Reader()));
    readers.push_back(QSharedPointer<OneDReader>(new CodaBarReader()));
    readers.push_back(QSharedPointer<OneDReader>(new Code93Reader()));
    readers.push_back(QSharedPointer<OneDReader>(new Code128Reader()));
    readers.push_back(QSharedPointer<OneDReader>(new ITFReader()));
    readers.push_back(QSharedPointer<OneDReader>(new RSS14Reader()));
    readers.push_back(QSharedPointer<OneDReader>(new RSSExpandedReader()));
  }
}

#include <typeinfo>

QSharedPointer<Result> MultiFormatOneDReader::decodeRow(int rowNumber, QSharedPointer<BitArray> row, zxing::DecodeHints hints)
{
  int size = int(readers.size());
  for (int i = 0; i < size; i++)
  {
    OneDReader *reader = readers[i].data();
    try
    {
      QSharedPointer<Result> result = reader->decodeRow(rowNumber, row, hints);
      return result;
    }
    catch (ReaderException const &re)
    {
      (void)re;
      // continue
    }
  }
  throw NotFoundException();
}
